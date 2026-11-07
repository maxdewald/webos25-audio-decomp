#include <algorithm>
#include <any>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using GType = unsigned long;
using guint64 = uint64_t;
using gchar = char;
constexpr GType G_TYPE_STRING = 0x40, G_TYPE_INT = 0x18;
constexpr GType GST_TYPE_BITMASK = 0x100, GST_TYPE_ARRAY = 0x104, GST_TYPE_BUFFER = 0x108;
constexpr guint64 GST_ELEMENT_FACTORY_TYPE_DECODER = 1;
constexpr int TRUE = 1, FALSE = 0, GST_FORMAT_TIME = 3;
constexpr int GST_MAP_READ = 1, GST_BUFFER_FLAG_HEADER = 0x400;

struct GstBuffer { std::vector<unsigned char> data; int refs = 1; int flags = 0; };
struct GstStructure {
    std::string name;
    std::map<std::string, std::any> values;
    std::map<std::string, GType> types;
    std::vector<GstBuffer *> headers;
};
struct GstCaps { GstStructure structure; int refs = 1; };
struct GstElement {
    std::string name;
    std::map<std::string, std::any> properties;
    GstCaps *caps = nullptr;
    bool in_bin = false;
    int state = 0;
};
struct GstElementFactory { bool decoder; int refs = 1; };
struct GstPad { int refs = 1; };
struct GstMapInfo { unsigned char *data; };
struct GValue { GType type; GstBuffer *buffer; std::vector<GstBuffer *> *array; };
#define G_VALUE_INIT {}
#define GST_BIN(element) (element)
#define G_OBJECT(element) (element)
#define GST_BUFFER_FLAG_SET(buffer, flag) ((buffer)->flags |= (flag))

struct LogContext { int level = 7; };
struct PfLog { LogContext *ctx = nullptr; char name[32] = "direct"; };
static std::vector<std::string> events, logs;
static int PmLogGetContextLevel(LogContext *, int *level) { *level = 7; return 0; }
static void PmLogString_(LogContext *, int, const char *, const char *, const char *message)
{
    logs.emplace_back(message);
    events.emplace_back("log:" + std::string(message));
}
#define PF_CODE_POINT(buffer, name, file, func, line) \
    snprintf(buffer, 0x100, "[%s]<%s:%s(%d)>", name, file, func, line)

static std::map<std::pair<const void *, unsigned>, std::any> slots;
template<class Type> static Type &field(const void *object, unsigned offset)
{
    auto &value = slots[{object, offset}];
    if (!value.has_value()) value = Type{};
    return std::any_cast<Type &>(value);
}
#define FIELD(Type, object, offset) field<Type>(object, offset)

static std::vector<std::unique_ptr<GstCaps>> caps_owned;
static std::vector<std::unique_ptr<GstBuffer>> buffers_owned;
static std::vector<std::unique_ptr<GstElement>> elements_owned;
static GstElementFactory decoder_factory{true}, sink_factory{false};
static GstPad decoder_pad;
static GstCaps *source_caps, *selected_caps;
static bool fail_decoder, fail_sink, fail_link, missing_pad;
static int null_decoder_sets, strings_live;
static std::vector<GstElement *> added, linked;

static GstCaps *gst_caps_new_empty_simple(const char *name)
{
    caps_owned.push_back(std::make_unique<GstCaps>());
    caps_owned.back()->structure.name = name;
    return caps_owned.back().get();
}
static GstCaps *gst_caps_new_simple(const char *name, const char *key, ...)
{
    GstCaps *caps = gst_caps_new_empty_simple(name);
    va_list args;
    va_start(args, key);
    while (key) {
        GType type = va_arg(args, GType);
        caps->structure.types[key] = type;
        if (type == G_TYPE_STRING)
            caps->structure.values[key] = std::string(va_arg(args, const char *));
        else if (type == GST_TYPE_BITMASK)
            caps->structure.values[key] = va_arg(args, guint64);
        else {
            assert(type == G_TYPE_INT);
            caps->structure.values[key] = va_arg(args, int);
        }
        key = va_arg(args, const char *);
    }
    va_end(args);
    source_caps = caps;
    return caps;
}
static guint64 gst_audio_channel_get_fallback_mask(int channels)
{
    return (guint64(1) << channels) - 1;
}
static void gst_buffer_unref(GstBuffer *buffer) { assert(buffer->refs > 0); --buffer->refs; }
static void gst_caps_unref(GstCaps *caps)
{
    assert(caps && caps->refs > 0);
    if (--caps->refs == 0)
        for (GstBuffer *header : caps->structure.headers) gst_buffer_unref(header);
}
static gchar *gst_caps_to_string(GstCaps *caps)
{
    ++strings_live;
    return strdup(caps ? caps->structure.name.c_str() : "NULL");
}
static void g_free(void *text) { assert(strings_live > 0); --strings_live; free(text); }
static GstCaps *gst_caps_make_writable(GstCaps *caps) { assert(caps->refs == 1); return caps; }
static GstStructure *gst_caps_get_structure(GstCaps *caps, int index)
{
    assert(index == 0); return &caps->structure;
}
static GstBuffer *gst_buffer_new_allocate(void *, size_t size, void *)
{
    buffers_owned.push_back(std::make_unique<GstBuffer>());
    buffers_owned.back()->data.resize(size);
    return buffers_owned.back().get();
}
static bool gst_buffer_map(GstBuffer *buffer, GstMapInfo *map, int flags)
{
    assert(flags == GST_MAP_READ);
    map->data = buffer->data.data();
    events.emplace_back("map");
    return true;
}
static void gst_buffer_unmap(GstBuffer *, GstMapInfo *) { events.emplace_back("unmap"); }
static void g_value_init(GValue *value, GType type)
{
    assert(value->type == 0);
    value->type = type;
    if (type == GST_TYPE_ARRAY) value->array = new std::vector<GstBuffer *>;
}
static void g_value_set_boxed(GValue *value, GstBuffer *buffer)
{
    assert(value->type == GST_TYPE_BUFFER);
    value->buffer = buffer; ++buffer->refs;
}
static void gst_value_array_append_value(GValue *array, GValue *value)
{
    assert(array->type == GST_TYPE_ARRAY && value->type == GST_TYPE_BUFFER);
    array->array->push_back(value->buffer); ++value->buffer->refs;
}
static void g_value_unset(GValue *value)
{
    if (value->buffer) gst_buffer_unref(value->buffer);
    if (value->array) {
        for (GstBuffer *buffer : *value->array) gst_buffer_unref(buffer);
        delete value->array;
    }
    *value = {};
}
static void gst_structure_set_value(GstStructure *structure, const char *key, GValue *value)
{
    assert(std::string(key) == "streamheader" && value->type == GST_TYPE_ARRAY);
    structure->headers = *value->array;
    for (GstBuffer *buffer : structure->headers) ++buffer->refs;
}
static GstElement *new_element(const char *name)
{
    elements_owned.push_back(std::make_unique<GstElement>());
    elements_owned.back()->name = name;
    return elements_owned.back().get();
}
static GstElement *gst_element_factory_make(const char *factory, const char *name)
{
    assert(std::string(factory) == "appsrc" && std::string(name) == "audio-src");
    return new_element(name);
}
static GstElementFactory *findElementFactory(guint64 type, GstCaps *caps)
{
    assert(strings_live == 0);
    if (type == GST_ELEMENT_FACTORY_TYPE_DECODER) {
        assert(caps == source_caps);
        events.emplace_back("decoder-factory");
        return &decoder_factory;
    }
    assert(type == 0xe000000000004ULL);
    selected_caps = caps;
    events.emplace_back("sink-factory");
    return &sink_factory;
}
static GstElement *gst_element_factory_create(GstElementFactory *factory, const char *name)
{
    assert(!name && factory->refs == 1);
    if (factory->decoder ? fail_decoder : fail_sink) return nullptr;
    return new_element(factory->decoder ? "decoder" : "sink");
}
static void gst_object_unref(GstElementFactory *factory) { assert(factory->refs-- == 1); }
static void gst_object_unref(GstPad *pad) { assert(pad->refs-- == 1); }
static GstPad *gst_element_get_static_pad(GstElement *, const char *name)
{
    assert(std::string(name) == "src");
    return missing_pad ? nullptr : &decoder_pad;
}
static GstCaps *gst_pad_query_caps(GstPad *, void *)
{
    assert(caps_owned.back()->refs == 0);
    return gst_caps_new_empty_simple("audio/decoder-output");
}
static void gst_bin_add_many(GstElement *, GstElement *first, ...)
{
    va_list args; va_start(args, first);
    for (GstElement *element = first; element; element = va_arg(args, GstElement *)) {
        assert(element->state == 0 && !element->in_bin);
        element->in_bin = true; added.push_back(element);
    }
    va_end(args);
    events.emplace_back("add");
}
static bool gst_element_link_many(GstElement *first, ...)
{
    va_list args; va_start(args, first);
    for (GstElement *element = first; element; element = va_arg(args, GstElement *))
        linked.push_back(element);
    va_end(args);
    events.emplace_back("link");
    return !fail_link && linked.back()->name == "sink";
}
static void g_object_set(GstElement *element, const char *key, ...)
{
    if (!element) {
        assert(std::string(key) == "app-type");
        ++null_decoder_sets; events.emplace_back("null-decoder"); return;
    }
    assert(element->state == 0);
    va_list args; va_start(args, key);
    while (key) {
        std::string property(key);
        events.emplace_back(element->name + ":" + property);
        if (property == "caps") {
            element->caps = va_arg(args, GstCaps *);
            if (element->caps) ++element->caps->refs;
        } else if (property == "app-type") {
            element->properties[property] = std::string(va_arg(args, const char *));
        } else {
            element->properties[property] = va_arg(args, int);
        }
        key = va_arg(args, const char *);
    }
    va_end(args);
}

// @match src/libpf/directmediaplayer.cpp (?s)namespace mediapipeline \{.*?(?=/\* @0x14bf68)
// @extract src/libpf/directmediaplayer.cpp DirectMediaPlayer::createAudioPath
}

struct Fixture {
    mediapipeline::DirectMediaPlayer player;
    GstElement pipeline;
    char info;
    Fixture()
    {
        slots.clear(); events.clear(); logs.clear();
        caps_owned.clear(); buffers_owned.clear(); elements_owned.clear();
        source_caps = selected_caps = nullptr;
        decoder_factory.refs = sink_factory.refs = decoder_pad.refs = 1;
        fail_decoder = fail_sink = fail_link = missing_pad = false;
        null_decoder_sets = strings_live = 0;
        added.clear(); linked.clear();
        field<GstElement *>(&player, 0x8c) = &pipeline;
        field<char *>(&player, 0x90) = &info;
        field<std::string>(&info, 8) = "PCM";
        field<std::string>(&info, 0x60) = "S16LE";
        field<std::string>(&info, 0x90) = "interleaved";
        field<int>(&info, 0xac) = 1;
        field<int>(&info, 0xb0) = 2;
        field<std::string>(&player, 0xb4) = "regular-app";
    }
    ~Fixture()
    {
        for (auto &element : elements_owned)
            if (element->caps) gst_caps_unref(element->caps);
        for (auto &caps : caps_owned) assert(caps->refs == 0);
        for (auto &buffer : buffers_owned) assert(buffer->refs == 0);
        assert(strings_live == 0);
    }
    GstElement *source() { return field<GstElement *>(&player, 0xa0); }
    GstElement *sink() { return field<GstElement *>(&player, 0xb0); }
    void success()
    {
        assert(player.createAudioPath());
        assert(source()->caps == source_caps && source_caps->refs == 1);
        assert(selected_caps->refs == 0);
        assert(std::any_cast<int>(source()->properties.at("format")) == GST_FORMAT_TIME);
        assert(std::any_cast<int>(source()->properties.at("emit-signals")) == FALSE);
        assert(std::any_cast<std::string>(sink()->properties.at("app-type")) == "DIRECTMEDIA");
        assert(std::any_cast<int>(sink()->properties.at("sync")) == FALSE);
        for (const char *key : {"nodelay", "disable-gstsync", "game-mode"})
            assert(std::any_cast<int>(sink()->properties.at(key)) == TRUE);
        assert(source()->state == 0 && sink()->state == 0 && pipeline.state == 0);
    }
};
static bool logged(const char *text)
{
    return std::any_of(logs.begin(), logs.end(), [text](const std::string &message) {
        return message.find(text) != std::string::npos;
    });
}
static size_t step(const std::string &text)
{
    auto found = std::find(events.begin(), events.end(), text);
    assert(found != events.end());
    return size_t(found - events.begin());
}

int main()
{
    const int rates[] = {48000, 44100, 32000, 24000, 16000, 12000, 8000, 22050};
    for (int index : {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        Fixture fixture;
        field<int>(&fixture.info, 0xac) = index;
        fixture.success();
        auto &caps = source_caps->structure;
        assert(caps.name == "audio/x-raw" && caps.values.size() == 5);
        assert(std::any_cast<int>(caps.values.at("rate")) ==
               (index >= 1 && index <= 8 ? rates[index - 1] : 44100));
        assert(std::any_cast<std::string>(caps.values.at("format")) == "S16LE");
        assert(std::any_cast<std::string>(caps.values.at("layout")) == "interleaved");
        assert(std::any_cast<int>(caps.values.at("channels")) == 2);
        assert(caps.types.at("channel-mask") == GST_TYPE_BITMASK);
        assert(std::any_cast<guint64>(caps.values.at("channel-mask")) == 3);
        assert(null_decoder_sets == 1 && added == linked && added.size() == 2);
        assert(!fixture.sink()->properties.count("pcm-bypass"));
        assert(!fixture.sink()->properties.count("pcm-multi-ch"));
        assert(logged("check src caps:") && logged("audio sink caps:"));
    }
    for (int channels : {5, 6, 8}) {
        for (const auto &application : std::vector<std::pair<std::string, bool>>{
                 {"com.google.stadia", true}, {"com.google.stadia.game", true},
                 {"regular-app", false}, {"com.google.stadi", false}}) {
            Fixture fixture;
            field<int>(&fixture.info, 0xb0) = channels;
            field<std::string>(&fixture.player, 0xb4) = application.first;
            fixture.success();
            bool stadia = application.second;
            assert(fixture.sink()->properties.count("pcm-bypass") == (channels > 5 && stadia));
            assert(fixture.sink()->properties.count("pcm-multi-ch") == (channels > 5 && !stadia));
            if (channels > 5) {
                std::string flag = stadia ? "pcm-bypass" : "pcm-multi-ch";
                assert(step("link") < step("audio-src:caps"));
                assert(step("audio-src:caps") < step("null-decoder"));
                assert(step("null-decoder") < step("sink:game-mode"));
                assert(step("sink:game-mode") < step("sink:" + flag));
            }
        }
    }
    for (bool header_present : {false, true}) {
        Fixture fixture;
        const unsigned char header[] = {0x4f, 0x70, 0x75, 0x73, 0, 0xff};
        field<std::string>(&fixture.info, 8) = "OPUS";
        field<int>(&fixture.info, 0xf4) = 8;
        field<float>(&fixture.info, 0xf8) = 48.0f;
        field<const void *>(&fixture.info, 0xfc) = header;
        field<size_t>(&fixture.info, 0x100) = header_present ? sizeof(header) : 0;
        field<bool>(&fixture.info, 4) = true;
        fixture.success();
        auto &caps = source_caps->structure;
        assert(caps.name == "audio/x-opus");
        assert(std::any_cast<int>(caps.values.at("rate")) == 48000);
        assert(std::any_cast<int>(caps.values.at("channels")) == 8);
        assert(caps.headers.size() == size_t(header_present));
        if (header_present) {
            GstBuffer *buffer = caps.headers.front();
            assert(buffer->data == std::vector<unsigned char>(header, header + sizeof(header)));
            assert(buffer->flags == GST_BUFFER_FLAG_HEADER && buffer->refs == 1);
            assert(step("map") < step("unmap"));
        }
        assert(selected_caps->structure.name == "audio/decoder-output");
        assert(decoder_pad.refs == 0 && null_decoder_sets == 0);
        assert(added == linked && added.size() == 3);
        assert(!fixture.sink()->properties.count("pcm-multi-ch"));
    }
    {
        Fixture fixture;
        field<std::string>(&fixture.info, 8) = "AAC";
        field<std::string>(&fixture.info, 0xb4) = "adts";
        field<int>(&fixture.info, 0xd0) = 6;
        field<float>(&fixture.info, 0xd4) = 44.1f;
        fixture.success();
        auto &caps = source_caps->structure;
        assert(caps.name == "audio/mpeg" && caps.values.size() == 4);
        assert(std::any_cast<int>(caps.values.at("mpegversion")) == 4);
        assert(std::any_cast<std::string>(caps.values.at("stream-format")) == "adts");
        assert(std::any_cast<int>(caps.values.at("rate")) == 44100);
        assert(std::any_cast<int>(caps.values.at("channels")) == 6);
        assert(!fixture.sink()->properties.count("pcm-multi-ch"));
    }
    for (bool decoder_needed : {false, true}) {
        Fixture fixture;
        field<bool>(&fixture.info, 4) = decoder_needed;
        fail_link = true;
        fixture.success();
        assert(logged("error in linking audio elements"));
        assert(source_caps->refs == 1 && selected_caps->refs == 0);
    }
    {
        Fixture fixture;
        field<bool>(&fixture.info, 4) = true;
        fail_decoder = true;
        fixture.success();
        assert(null_decoder_sets == 1 && added.size() == 1 && linked.size() == 1);
        assert(selected_caps->structure.name == "audio/x-raw");
        assert(logged("error in linking audio elements"));
    }
    {
        Fixture fixture;
        field<bool>(&fixture.info, 4) = true;
        missing_pad = true;
        fixture.success();
        assert(selected_caps->structure.name == "audio/x-raw" && decoder_pad.refs == 1);
    }
    for (bool decoder_needed : {false, true}) {
        Fixture fixture;
        field<bool>(&fixture.info, 4) = decoder_needed;
        fail_sink = true;
        assert(!fixture.player.createAudioPath());
        assert(logged("cannot create audiosink"));
        assert(source_caps->refs == 1 && selected_caps->refs == 0);
        assert(added.empty() && linked.empty() && null_decoder_sets == 0);
        assert(!fixture.source()->in_bin && !fixture.source()->caps);
        gst_caps_unref(source_caps);
    }
    std::cout << "DirectMedia caps, pre-READY flags, header ownership and failure contracts passed\n";
}