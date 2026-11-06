/* Host-only mock harness for resource_delivery.cpp and airplayer.cpp. */
#include <any>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct GstStructure {
    std::string name;
    std::map<std::string, std::any> values;
    std::map<std::string, int> types;
};
struct GstElement {
    std::map<std::string, GstElement *> children;
    std::shared_ptr<GstStructure> resource;
    bool locked = true;
    int syncs = 0;
};
struct GstEvent { int type; GstStructure *structure; };
using GstEventType = int;
constexpr int G_TYPE_STRING = 0x40, G_TYPE_INT = 0x18, G_TYPE_FLOAT = 0x38;
constexpr int G_TYPE_BOOLEAN = 0x14;
constexpr int TRUE = 1, FALSE = 0, GST_STATE_READY = 2, GST_STATE_PAUSED = 3;
constexpr int GST_STATE_CHANGE_FAILURE = 0, PF_EVENT_ACQUIRED_RESOURCE = 0x10e01;
struct PfLog {};
#define PF_LOG_DEBUG(...) ((void)0)
#define PF_LOG_LEVEL(...) ((void)0)
#define PF_LOG_ERROR(...) ((void)0)
#define GST_BIN(p) (p)
#define G_OBJECT(p) (p)

/* Host pointers are 64-bit, whereas shipped layouts are ILP32. Keying slots by
 * (object, offset) tests the logic without silently overlapping adjacent fields. */
static std::map<std::pair<const void *, unsigned>, std::any> slots;
template<class T> T &field(const void *self, unsigned offset)
{
    auto &value = slots[{self, offset}];
    if (!value.has_value()) value = T{};
    return std::any_cast<T &>(value);
}
#define FIELD(T, self, offset) field<T>(self, offset)

static int calls, fail_structure, state_result = 1, sends;
static int src_result, middle_result, sink_result, pause_result = 1;
static int delayed_es, delayed_uri;
static std::vector<std::string> steps;
static std::shared_ptr<GstStructure> sent;
static std::vector<std::unique_ptr<GstStructure>> owned;

static GstStructure *gst_structure_new(const char *name, const char *key, ...)
{
    ++calls;
    if (fail_structure) return nullptr;
    auto structure = std::make_unique<GstStructure>();
    structure->name = name;
    va_list args;
    va_start(args, key);
    while (key) {
        int type = va_arg(args, int);
        structure->types[key] = type;
        if (type == G_TYPE_STRING) structure->values[key] = std::string(va_arg(args, const char *));
        else if (type == G_TYPE_FLOAT) structure->values[key] = va_arg(args, double);
        else {
            assert(type == G_TYPE_INT || type == G_TYPE_BOOLEAN);
            structure->values[key] = va_arg(args, int);
        }
        key = va_arg(args, const char *);
    }
    va_end(args);
    auto *result = structure.get();
    owned.push_back(std::move(structure));
    return result;
}
static GstStructure *gst_structure_copy(GstStructure *structure)
{
    owned.push_back(std::make_unique<GstStructure>(*structure));
    return owned.back().get();
}
static void gst_structure_free(GstStructure *) {}
static GstEvent *gst_event_new_custom(int type, GstStructure *structure)
{
    return new GstEvent{type, structure};
}
static bool gst_element_send_event(GstElement *, GstEvent *event)
{
    assert(event->type == PF_EVENT_ACQUIRED_RESOURCE);
    sent = std::make_shared<GstStructure>(*event->structure);
    delete event;
    ++sends;
    return false; // The original setters intentionally ignore event-send failure.
}
static GstElement *gst_bin_get_by_name(GstElement *bin, const char *name)
{
    return bin->children.at(name);
}
static void g_object_set(GstElement *element, const char *name, GstStructure *structure, void *)
{
    assert(std::string(name) == "resource-info");
    element->resource = std::make_shared<GstStructure>(*structure);
}
static void gst_object_unref(GstElement *) {}
static void gst_element_set_locked_state(GstElement *element, int locked) { element->locked = locked; }
static void gst_element_sync_state_with_parent(GstElement *element) { ++element->syncs; }
static int gst_element_set_state(GstElement *, int state)
{
    steps.push_back(state == GST_STATE_READY ? "ready" : "paused");
    return state_result;
}

// @match src/libpf/resource_delivery.cpp (?s)(?<=#include "libpf.h"\n).*
// @match src/libpf/airplayer.cpp (?s)(?<=#include "libpf.h"\n).*

namespace mediapipeline {
PfLog Pipeline::pflog;
void EsSWDecPlayer::delayedPauseSinkElements() { ++delayed_es; }
void UriSWDecPlayer::delayedPauseSinkElements() { ++delayed_uri; }
int CustomPipeline::addSrcElement(MEDIA_CUSTOM_SRC_TYPE_T type)
{
    assert(int(type) == 7); steps.push_back("src"); return src_result;
}
int CustomPipeline::addMiddleElement_ES() { steps.push_back("middle"); return middle_result; }
int CustomPipeline::addSinkElement_ES() { steps.push_back("sink"); return sink_result; }
bool CustomPipeline::internalPause() { steps.push_back("pause"); return pause_result; }
}

static void check_ports(const GstStructure &resource, int audio, int mixer)
{
    assert(resource.name == "acquired-resource");
    assert(std::any_cast<int>(resource.values.at("audio-port")) == audio);
    assert(std::any_cast<int>(resource.values.at("mixer-port")) == mixer);
    assert(std::any_cast<std::string>(resource.values.at("core-type")) == "core");
}
static void reset()
{
    slots.clear(); owned.clear(); sent.reset(); steps.clear();
    calls = fail_structure = sends = src_result = middle_result = sink_result = 0;
    state_result = pause_result = 1;
    delayed_es = delayed_uri = 0;
}

int main()
{
    using namespace mediapipeline;
    Pipeline pipeline;
    GstElement bin, sink, decoder;
    for (int audio : {-1, 0, 1, 2}) {
        reset();
        assert(!pipeline.setResource("core", 3, audio, 5, 1920, 1080, 60, true));
        assert(calls == 0 && field<int>(&pipeline, 0x534) == 3);
        field<GstElement *>(&pipeline, 0xd8) = &bin;
        field<std::string>(&pipeline, 0x24) = "regular-app";
        assert(pipeline.setResource("core", 3, audio, 5, 1920, 1080, 60, true));
        check_ports(*sent, audio, 5);
        assert(sent->types.at("max-frame-rate") == G_TYPE_FLOAT);
        assert(std::any_cast<double>(sent->values.at("max-frame-rate")) == 60);
        assert(std::any_cast<int>(sent->values.at("hfr")) == 1);
        field<std::string>(&pipeline, 0x24) = "com.webos.app.mediadiscovery";
        assert(pipeline.setResource("core", 3, audio, 5, 0, 0, 0, false));
        assert(sent->values.size() == 4);
    }
    DvrPipeline dvr;
    for (bool dimensions : {false, true}) {
        reset(); field<GstElement *>(&dvr, 0xd8) = &bin;
        field<int>(&dvr, 0x3e8) = dimensions ? 1920 : 0;
        field<int>(&dvr, 0x3ec) = dimensions ? 1080 : 0;
        field<float>(&dvr, 0x3f0) = 24;
        assert(dvr.setResource("core", 0, 1, -1, 99, 88, 77, false));
        check_ports(*sent, 1, -1);
        assert(sent->values.count("max-width") == dimensions);
        assert(field<GstStructure *>(&dvr, 0xc80) != sent.get());
        field<unsigned char>(&dvr, 0xd5c) = 1;
        assert(!dvr.setResource("core", 0, 0, 0, 0, 0, 0, false));
        assert(sends == 1);
    }
    CustomPipeline custom;
    reset(); field<GstElement *>(&custom, 0xd8) = &bin;
    assert(custom.setResource("core", 0, -1, 4, 0, 0, 0, false));
    check_ports(*sent, -1, 4);
    field<int>(&custom, 0x888) = 7;
    assert(custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    assert((steps == std::vector<std::string>{"src", "middle", "ready", "pause", "sink"}));
    steps.clear(); field<int>(&custom, 0x538) = 1;
    assert(custom.setResource("core", 0, 1, -1, 0, 0, 0, false) && steps.empty());
    field<int>(&custom, 0x538) = 0; src_result = 1;
    assert(!custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    assert((steps == std::vector<std::string>{"src"}));
    steps.clear(); src_result = 0; middle_result = 1;
    assert(!custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    assert((steps == std::vector<std::string>{"src", "middle"}));
    steps.clear(); middle_result = 0; pause_result = 0;
    assert(!custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    assert((steps == std::vector<std::string>{"src", "middle", "ready", "pause"}));
    steps.clear(); pause_result = 1; sink_result = 1;
    assert(!custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    assert(steps.back() == "sink");
    fail_structure = 1;
    assert(!custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    fail_structure = sink_result = 0;
    assert(custom.setResource("core", 0, 1, -1, 0, 0, 0, false));
    field<GstElement *>(&custom, 0x678) = &sink;
    field<GstElement *>(&custom, 0x660) = &decoder;
    custom.setResourcePropertyToSink();
    check_ports(*sink.resource, 1, -1);
    assert(!sink.locked && !decoder.locked && sink.syncs && decoder.syncs);

    EsSWDecPlayer es; UriSWDecPlayer uri; char info;
    for (bool video_only : {false, true}) {
        reset(); bin.children["audio-sink"] = &sink;
        field<GstElement *>(&es, 0x8c) = &bin;
        field<char *>(&es, 0x90) = &info;
        field<unsigned char>(&info, 0x88) = video_only;
        assert(es.setResource("core", 0, 1, -1, 0, 0, 0, false));
        assert(delayed_es == 1 && calls == !video_only && sends == 0);
        assert(field<unsigned char>(&es, 0xa6) == 1);
        field<GstElement *>(&uri, 0x78) = &bin;
        field<char *>(&uri, 0x7c) = &info;
        field<unsigned char>(&info, 0x8c) = video_only;
        assert(uri.setResource("core", 0, -1, 3, 0, 0, 0, false));
        assert(delayed_uri == 1 && calls == 2 * !video_only && sends == 0);
        if (!video_only) check_ports(*sink.resource, -1, 3);
    }
    AirPlayer air;
    for (bool decoder_present : {false, true}) {
        reset(); bin.children["sink"] = &sink;
        if (decoder_present) bin.children["decoder"] = &decoder;
        else bin.children["decoder"] = nullptr;
        field<GstElement *>(&air, 0x8c) = &bin;
        field<char *>(&air, 0x90) = &info;
        field<float>(&info, 0xa4) = 120;
        field<int>(&info, 0x9c) = 1920;
        field<int>(&info, 0xa0) = 1080;
        assert(air.setResource("core", 4, 1, -1, 1, 2, 3, true));
        check_ports(*sink.resource, 1, -1);
        check_ports(*sent, 1, -1);
        assert(std::any_cast<double>(sent->values.at("max-frame-rate")) == 60);
        assert(std::any_cast<int>(sent->values.at("max-width")) == 1920);
        assert(std::any_cast<int>(sent->values.at("active")) == 1);
        assert(sent->values.count("hfr") == 0);
        if (decoder_present) check_ports(*decoder.resource, 1, -1);
    }
    reset();
    assert(!air.setResource("core", 0, 0, -1, 0, 0, 0, false) && calls == 0);

    DirectMediaPlayer direct;
    reset(); field<GstElement *>(&direct, 0x8c) = &bin;
    field<char *>(&direct, 0x90) = &info;
    field<GstElement *>(&direct, 0xb0) = &sink;
    field<GstElement *>(&direct, 0xa8) = &decoder;
    assert(direct.setResource("core", 4, 1, -1, 1920, 1080, 30, true));
    check_ports(*sink.resource, 1, -1);
    assert(std::any_cast<int>(sink.resource->values.at("active")) == 1);
    state_result = GST_STATE_CHANGE_FAILURE;
    assert(!direct.setResource("core", 4, 0, -1, 1920, 1080, 30, false));
    std::cout << "PASS: resource delivery guards, unchanged ports, metadata, ownership and deferred construction\n";
}
