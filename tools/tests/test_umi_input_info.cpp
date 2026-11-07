#include <cassert>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

namespace pbnjson {
struct JValue {
    bool present = false;
    bool boolean = false;
    bool hasKey(const char *) const { return present; }
    JValue operator[](const char *) const { return *this; }
    int asBool(bool &output) const { output = boolean; return 0; }
};
}

namespace umi_input_info {
struct Audio {
    bool mixer = false;
    std::string effectiveMode = "pcm";
    std::string appliedMode;
    int applyResult = 1;
};

static std::string getSoundOutputDigitalMode(const Audio *self)
{
    return self->effectiveMode;
}

static int setSoundOutputDigital(Audio *self, const std::string &mode)
{
    self->appliedMode = mode;
    return self->applyResult;
}

// @extract src/libumi/input_info.cpp setMixerSettings
// @match src/libumi/input_info.cpp struct MetadataValue \{[\s\S]*?\n\};
// @match src/libumi/input_info.cpp struct MetadataState \{[\s\S]*?\n\};
// @match src/libumi/input_info.cpp struct MetadataEffects \{[\s\S]*?\n\};
// @match src/libumi/input_info.cpp static const MetadataValue &metadataField\([\s\S]*?\n\}
// @match src/libumi/input_info.cpp static std::string metadataString\([\s\S]*?\n\}
// @extract src/libumi/input_info.cpp promoteAudioMetadata updateAudioCategoryInfoProjection setAudioInputInfoProjection
}

using namespace umi_input_info;

static MetadataValue stringValue(const std::string &text)
{
    return {MetadataValue::String, text};
}

static MetadataValue objectValue(std::map<std::string, MetadataValue> fields)
{
    return {MetadataValue::Object, "", std::move(fields)};
}

static MetadataValue inputValue(const MetadataValue &pipeline, const MetadataValue &category)
{
    return objectValue({{"pipelineInfo", pipeline}, {"categoryInfo", category}});
}

struct AppliedMetadata {
    std::vector<std::map<std::string, std::string>> digital;
    std::vector<std::string> immersive;
    std::string sequence;
    std::string source;
    std::string attribute;
    std::string delayPipeline;
    int delayResult = 0;
    int gainResult = 0;
    int encoderResult = 0;
    int logs = 0;
};

static void metadataTests()
{
    MetadataState state;
    state.activeState = true;
    state.digitalOutputEligible = true;
    state.pipelines.push_back({{"pipelineId", "music"}, {"resource", "decoder"}});
    AppliedMetadata applied;
    MetadataEffects effects;
    effects.context = &applied;
    effects.reapplyDigital = [](void *context, const MetadataState &current) {
        static_cast<AppliedMetadata *>(context)->digital.push_back(current.current);
    };
    effects.updateImmersive = [](void *context, const std::string &immersive) {
        static_cast<AppliedMetadata *>(context)->immersive.push_back(immersive);
    };
    auto pipeline = objectValue({{"pipelineId", stringValue("music")},
                                 {"resource", stringValue("decoder")}});
    auto category = objectValue({{"codec", stringValue("dts_hd_ma")},
                                 {"channel", {MetadataValue::Scalar, "8"}}});
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("codec") == "dts_hd_ma" && state.current.at("channel") == "8");
    assert(applied.digital.size() == 1 && applied.digital.back().at("channel") == "8");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(applied.digital.size() == 2);
    state.activeState = false;
    category.fields["codec"] = stringValue("pcm");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.pipelines.front().at("codec") == "pcm");
    assert(state.current.at("codec") == "dts_hd_ma" && applied.digital.size() == 2);
    state.activeState = true;
    pipeline.fields["resource"] = stringValue("wrong");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 0);
    assert(state.current.at("codec") == "dts_hd_ma" && applied.digital.size() == 2);
    pipeline.fields["resource"] = stringValue("decoder");
    pipeline.fields["pipelinePurpose"] = {MetadataValue::Array};
    pipeline.fields["pipelinePurpose"].elements.push_back(stringValue("RECORDING"));
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.recording.at("codec") == "pcm" && state.current.at("codec") == "dts_hd_ma");
    assert(applied.digital.size() == 2);
    pipeline.fields["pipelinePurpose"].elements.push_back(stringValue("playback"));
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("codec") == "pcm" && applied.digital.size() == 3);
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, {}), effects) == 0);
    assert(applied.digital.size() == 3);
    pipeline.fields.erase("pipelinePurpose");
    MetadataState noCodec;
    noCodec.activeState = noCodec.digitalOutputEligible = true;
    noCodec.pipelines.push_back({{"pipelineId", "music"}, {"resource", "decoder"}});
    assert(setAudioInputInfoProjection(noCodec, inputValue(pipeline, objectValue({})), effects) == 1);
    assert(noCodec.current.count("codec") == 0 && applied.digital.size() == 3);

    effects.isFreerun = [](void *, const MetadataValue &) { return true; };
    auto rejected = pipeline;
    rejected.fields["resource"] = stringValue("wrong");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    assert(state.pipelines.front().at("audioFreerunMode") == "true");
    assert(state.current.at("audioFreerunMode") == "false");
    assert(applied.digital.size() == 3);
    rejected.fields.erase("resource");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    rejected.fields["resource"] = {MetadataValue::Scalar, "0"};
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    rejected = pipeline;
    rejected.fields["pipelineId"] = stringValue("musical");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    rejected.fields["pipelineId"] = stringValue("");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    rejected.fields.erase("pipelineId");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    assert(applied.digital.size() == 3);

    auto partial = objectValue({{"channel", {MetadataValue::Scalar, "6"}}});
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, partial), effects) == 1);
    assert(state.current.at("codec") == "pcm" && state.current.at("channel") == "6");
    assert(applied.digital.size() == 4);
    state.digitalOutputEligible = false;
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("channel") == "8" && applied.digital.size() == 4);
    state.digitalOutputEligible = true;
    pipeline.fields["pipelinePurpose"] = {MetadataValue::Array};
    category.fields["codec"] = stringValue("aac");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("codec") == "pcm" && state.pipelines.front().at("codec") == "aac");
    assert(applied.digital.size() == 4);
    pipeline.fields["pipelinePurpose"] = stringValue("invalid array type");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("codec") == "aac" && applied.digital.size() == 5);
    pipeline.fields["pipelinePurpose"] = {MetadataValue::Array};
    pipeline.fields["pipelinePurpose"].elements = {stringValue("playback"), stringValue("playback")};
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(applied.digital.size() == 7);
    state.activeState = false;
    pipeline.fields["pipelinePurpose"].elements = {stringValue("RECORDING")};
    category.fields["codec"] = stringValue("ac3");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.recording.at("codec") == "ac3" && state.current.at("codec") == "aac");
    assert(applied.digital.size() == 7);

    state.activeState = true;
    pipeline.fields.erase("pipelinePurpose");
    MetadataValue params{MetadataValue::Array};
    params.elements = {stringValue("first"), {MetadataValue::Scalar, "3"}};
    category.fields["params"] = params;
    category.fields["ignored"] = params;
    category.fields["immersive"] = stringValue("true");
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("audioParams0") == "first" && state.current.at("audioParams1").empty());
    assert(!state.current.count("params") && !state.current.count("ignored"));
    assert(applied.immersive.back() == "true");
    category.fields["params"].elements.clear();
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(state.current.at("audioParams0") == "first");

    effects.prepareDelayPipeline = [](void *context, const std::string &identifier) {
        static_cast<AppliedMetadata *>(context)->delayPipeline = identifier;
    };
    effects.inputLog = [](void *context) { ++static_cast<AppliedMetadata *>(context)->logs; };
    effects.registerDelay = [](void *context) {
        auto &result = *static_cast<AppliedMetadata *>(context);
        result.sequence += 'D';
        return result.delayResult;
    };
    effects.finishDelayUpdate = [](void *context, const MetadataValue &, const MetadataValue &,
                                   const std::string &source, const std::string &attribute, int result) {
        auto &applied = *static_cast<AppliedMetadata *>(context);
        applied.sequence += 'A';
        applied.source = source;
        applied.attribute = attribute;
        return result;
    };
    effects.registerGain = [](void *context) {
        auto &result = *static_cast<AppliedMetadata *>(context);
        result.sequence += 'G';
        return result.gainResult;
    };
    effects.registerEncoderGain = [](void *context) {
        auto &result = *static_cast<AppliedMetadata *>(context);
        result.sequence += 'E';
        return result.encoderResult;
    };
    effects.notifyInput = [](void *context) { static_cast<AppliedMetadata *>(context)->sequence += 'N'; };
    pipeline.fields["sourceType"] = stringValue("MEDIA");
    pipeline.fields["sourceAttribute"] = stringValue("movie");
    applied.delayResult = 2;
    applied.gainResult = 3;
    applied.encoderResult = 7;
    auto digitalBeforeFailure = applied.digital.size();
    rejected = pipeline;
    rejected.fields["resource"] = stringValue("wrong");
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 7);
    assert(applied.sequence == "DAGEN" && applied.digital.size() == digitalBeforeFailure);
    assert(applied.source == "MEDIA" && applied.attribute == "movie");
    assert(applied.delayPipeline == "music" && state.cachedSource == "MEDIA");
    assert(applied.logs == 1);
    applied.sequence.clear();
    applied.gainResult = applied.encoderResult = 0;
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 2);
    assert(applied.logs == 1 && applied.sequence == "DAGEN");
    applied.sequence.clear();
    applied.delayResult = 0;
    assert(setAudioInputInfoProjection(state, inputValue(rejected, category), effects) == 0);
    assert(applied.sequence == "DGEN");
    applied.sequence.clear();
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, category), effects) == 1);
    assert(applied.sequence == "DGEN");
    applied.sequence.clear();
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, {}), effects) == 0);
    assert(applied.sequence.empty());
    applied.delayResult = 9;
    assert(setAudioInputInfoProjection(state, inputValue({}, category), effects) == 9);
    assert(applied.source.empty() && applied.attribute.empty() && state.cachedSource.empty());
    applied.sequence.clear();
    assert(setAudioInputInfoProjection(state, inputValue(pipeline, partial), effects) == 9);
    assert(applied.source.empty() && applied.attribute.empty());

    MetadataState unmatched;
    unmatched.activeState = unmatched.digitalOutputEligible = true;
    unmatched.pipelines.push_back({{"pipelineId", "music"}});
    assert(setAudioInputInfoProjection(unmatched, inputValue(pipeline, category), {}) == 0);
    assert(unmatched.current.empty() && !unmatched.pipelines.front().count("audioFreerunMode"));
    unmatched.pipelines.front()["resource"] = "decoder";
    auto unknown = pipeline;
    unknown.fields["pipelineId"] = stringValue("unknown");
    assert(setAudioInputInfoProjection(unmatched, inputValue(unknown, category), {}) == 0);
    assert(unmatched.pipelines.size() == 1 && unmatched.current.empty());
    unmatched.pipelines.front()["resource"] = "";
    pipeline.fields["resource"] = stringValue("");
    category.fields["codec"] = {MetadataValue::Scalar, "17"};
    category.fields["channel"] = stringValue("not-a-number");
    assert(setAudioInputInfoProjection(unmatched, inputValue(pipeline, category), {}) == 1);
    assert(unmatched.current.at("codec") == "17");
    assert(unmatched.current.at("channel") == "not-a-number");
}

int main()
{
    umi_input_info::Audio audio;
    assert(umi_input_info::setMixerSettings(&audio, {false, true}) == 0);
    assert(!audio.mixer && audio.appliedMode.empty());
    assert(umi_input_info::setMixerSettings(&audio, {true, false}) == 1);
    assert(audio.appliedMode.empty());
    audio.applyResult = 0;
    assert(umi_input_info::setMixerSettings(&audio, {true, true}) == 0);
    assert(audio.mixer && audio.appliedMode == "pcm");
    audio.appliedMode.clear();
    assert(umi_input_info::setMixerSettings(&audio, {true, true}) == 1);
    assert(audio.appliedMode.empty());
    audio.effectiveMode = "passThrough";
    audio.applyResult = 7;
    assert(umi_input_info::setMixerSettings(&audio, {true, false}) == 7);
    assert(!audio.mixer && audio.appliedMode == "passThrough");
    metadataTests();
    std::puts("input-info: mixer, metadata promotion and conditional digital reapply passed");
}