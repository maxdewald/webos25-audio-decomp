// @sources src/libpf/audio_metadata.cpp
// @cflags -std=c++14 -Isrc/libpf -isystem lg-opensource/public/boost -isystem lg-opensource/public/rapidjson/include
// @pkg glib-2.0
/* Host tests for public-dependency-assisted libpf reconstructions. */
/* options.cpp gets only libpf.h's PmLog part, so GStreamer is not needed. */
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/syscall.h>
#ifndef SYS_gettid
#define SYS_gettid SYS_getpid
#endif
static long libpf_test_syscall(long) { return static_cast<long>(getpid()); }
#define syscall libpf_test_syscall
// @match src/libpf/libpf.h (?s)/\* PmLogLib.*?(?=/\* Custom event)
#define LIBPF_H
#include "../../src/libpf/options.cpp"
#include "options.h"
#include "audio_metadata.h"
#include "shared_ptr.h"
#include <glib.h>
#include <boost/property_tree/exceptions.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

extern "C" {
static std::vector<std::string> messages;
int PmLogGetContext(const char *name, PmLogContext *ctx)
{
    assert(std::string(name) == "playerfactory.default");
    static PmLogContextStruct context{7}; *ctx = &context; return 0;
}
int PmLogGetContextLevel(PmLogContext ctx, int *level) { *level = ctx->level; return 0; }
int PmLogString_(PmLogContext, int, const char *, const char *, const char *message)
{
    messages.push_back(message); return 0;
}
}

static void testOptions()
{
    mediapipeline::utils::Options options;
    assert(options.load(R"({"name":"audio","yes":true,"no":false,"zero":0,"count":7,"nested":{"rate":48.0},"wide":9223372036854775807,"bad":"7tail","null":null})"));
    assert(options.getString("name") == "audio");
    assert(options.getBoolean("yes") && !options.getBoolean("no"));
    assert(options.getInt("count") == 7);
    bool boolean = true;
    assert(options.getBoolean("no", &boolean) && !boolean);
    int integer = 123;
    assert(!options.getInt("bad", &integer) && integer == 123);
    assert(!options.getInt("missing", &integer) && integer == 123);
    assert(options.getInt("zero", &integer) && integer == 0);
    double number = -1;
    assert(options.getDouble("nested.rate", &number) && number == 48);
    float small = -1;
    assert(options.getFloat("nested.rate", &small) && small == 48);
    int64_t wide = 0;
    assert(options.getGint64("wide", &wide) && wide == std::numeric_limits<int64_t>::max());
    std::string text = "untouched";
    assert(!options.getString("missing", &text) && text == "untouched");
    assert(options.getString("name", &text) && text == "audio");
    assert(options.checkKeyExistance("no") && options.checkKeyExistance("zero"));
    assert(options.checkKeyExistance("null") && !options.checkKeyExistance("missing"));
    bool threw = false;
    try { options.getInt("bad"); } catch (const boost::property_tree::ptree_bad_data &) { threw = true; }
    assert(threw);
    messages.clear();
    assert(!options.load("{broken"));
    assert(options.getString("name") == "audio");
    assert(messages.size() == 2 && messages[0].find("Error parsing: [] on line: [1]") != std::string::npos);
    assert(options.loadJSON(R"({"new":9})"));
    assert(!options.checkKeyExistance("name") && options.getInt("new") == 9);
    assert(!options.loadJSON("["));
}

template<class Info> static void testStream(void (*parse)(Info &, const rapidjson::Value &))
{
    Info out{99, SAMPLE_RATE_44_1, nullptr, 0};
    rapidjson::Document json;
    json.Parse("{}"); parse(out, json);
    assert(out.channels == 99 && out.sampleRate == SAMPLE_RATE_44_1);
    json.Parse(R"({"channels":2,"sampleRate":48,"streamHeader":"AQIDBA=="})");
    parse(out, json);
    assert(out.channels == 2 && out.sampleRate == SAMPLE_RATE_48 && out.headerSize == 4);
    assert(out.header[0] == 1 && out.header[3] == 4);
    auto *header = out.header;
    json.Parse(R"({"sampleRate":96,"streamHeader":""})"); parse(out, json);
    assert(out.sampleRate == SAMPLE_RATE_48 && out.header == header && out.headerSize == 4);
    json.Parse(R"({"sampleRate":44.1,"streamHeader":4})"); parse(out, json);
    assert(out.sampleRate == SAMPLE_RATE_44_1 && out.header == header);
    json.Parse(R"({"streamHeader":null})"); parse(out, json);
    assert(out.header == header && out.headerSize == 4);
    json.Parse(R"({"streamHeader":""})"); parse(out, json);
    assert(out.header == header && out.headerSize == 4);
    g_free(header);
    out.header = nullptr;
    json.Parse(R"({"streamHeader":"AQIDBAUGBwgJCgsMDQ4PEA=="})"); parse(out, json);
    assert(out.headerSize == 16 && out.header[0] == 1 && out.header[15] == 16);
    g_free(out.header);
}

static void testCodecs()
{
    using namespace mediapipeline;
    testStream(setOPUSinfo); testStream(setIAMFinfo); testStream(setVORBISinfo);
    MEDIA_AUDIO_AC3PLUS_INFO_T info{9, SAMPLE_RATE_48};
    rapidjson::Document json;
    json.Parse(R"({"channels":6,"frequency":22.05})"); setAC3PLUSinfo(info, json);
    assert(info.channels == 6 && info.sampleRate == SAMPLE_RATE_22_05);
    json.Parse(R"({"frequency":96})"); setAC3PLUSinfo(info, json);
    assert(info.sampleRate == SAMPLE_RATE_22_05);
    const unsigned hz[] = {0,48000,44100,32000,24000,16000,12000,8000,22050};
    for (unsigned i = 0; i < 9; ++i) assert(transSampleRate(MEDIA_AUDIO_SAMPLERATE(i)) == hz[i]);
    assert(transSampleRate(MEDIA_AUDIO_SAMPLERATE(99)) == 0);
}

static void testOwnership()
{
    struct Tracked { int *count; explicit Tracked(int *count) : count(count) {} ~Tracked() { ++*count; } };
    int destroyed = 0;
    boost::weak_ptr<Tracked> weak;
    {
        boost::shared_ptr<Tracked> first(new Tracked(&destroyed));
        weak = first;
        auto second = first;
        assert(first.use_count() == 2);
        first.reset(new Tracked(&destroyed));
        assert(!weak.expired() && destroyed == 0);
        second.reset();
        assert(weak.expired() && destroyed == 1);
    }
    assert(destroyed == 2);
}
int main()
{
    testOptions(); testCodecs(); testOwnership();
    std::cout << "PASS: Options conversions/parse errors, codec metadata/base64, real Boost ownership\n";
}
