#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <pbnjson.hpp>

namespace umi_input_info {

struct Audio {
    unsigned char reserved[0x7c];
    bool mixer;
};

static_assert(offsetof(Audio, mixer) == 0x7c, "mixer setting offset");

extern std::string getSoundOutputDigitalMode(const Audio *)
    __asm__("_ZNK10umiTVAudio25getSoundOutputDigitalModeB5cxx11Ev");
extern int setSoundOutputDigital(Audio *, const std::string &)
    __asm__("_ZN10umiTVAudio21setSoundOutputDigitalERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE");

int setMixerSettings(Audio *, pbnjson::JValue)
    __asm__("_ZN10umiTVAudio16setMixerSettingsEN7pbnjson6JValueE");
/* @0x526f4: only changed value.mix writes +0x7c; reapply the effective mode.
 * Missing mix returns 0; unchanged mix returns hasKey's nonzero result.
 * Byte assignment precedes reapply and is not rolled back on failure: the next
 * identical request skips the failed attempt, not just redundant successful
 * writes. Explicit setSoundOutputDigital and capability updates are separate
 * reapply edges; no client identity or holder lifecycle is tracked here.
 * [GAP] The original ignores asBool's error and leaves its local uninitialized
 * on conversion failure; only a boolean mix has a recovered defined value. */
int setMixerSettings(Audio *self, pbnjson::JValue value)
{
    int result = value.hasKey("mix");
    if (result) {
        bool mix;
        value["mix"].asBool(mix);
        if (self->mixer != mix) {
            self->mixer = mix;
            result = setSoundOutputDigital(self, getSoundOutputDigitalMode(self));
        }
    }
    return result;
}

/* Coverage-excluded projection of Ghidra 0x4db20 / raw 0x3db20 and
 * Ghidra 0x4ee40 / raw 0x3ee40. Not the original umiTVAudio ABI.
 * [GAP] JSON adaptation, logging, ARM container layout and AutoAVSync virtual
 * calls remain external. Scalar/Object text must be the original JSON stringify;
 * String text is unquoted. Missing/non-string asString projects to empty.
 * activeState belongs to the outer dispatcher (+0x84), not these handlers. */
struct MetadataValue {
    enum Type { Missing, String, Scalar, Object, Array } type = Missing;
    std::string text;
    std::map<std::string, MetadataValue> fields;
    std::vector<MetadataValue> elements;
};

struct MetadataState {
    std::vector<std::map<std::string, std::string>> pipelines;
    std::map<std::string, std::string> current;
    std::map<std::string, std::string> recording;
    bool activeState = false;
    bool digitalOutputEligible = false;
    std::string cachedCodec;
    std::string cachedSource;
};

/* [GAP] prepareDelayPipeline is the codec/pipeline prelude after raw 0x3f4e4;
 * finishDelayUpdate owns AutoAVSync only. Immersive notification is also an ABI boundary.
 * reapplyDigital represents setSoundOutputDigital(effective mode), whose
 * downstream setAllSoundOutputDigital is not called directly by either handler. */
struct MetadataEffects {
    void *context = nullptr;
    bool (*isFreerun)(void *, const MetadataValue &) = nullptr;
    void (*updateImmersive)(void *, const std::string &) = nullptr;
    void (*reapplyDigital)(void *, const MetadataState &) = nullptr;
    void (*prepareDelayPipeline)(void *, const std::string &) = nullptr;
    void (*inputLog)(void *) = nullptr;
    int (*registerDelay)(void *) = nullptr;
    int (*finishDelayUpdate)(void *, const MetadataValue &, const MetadataValue &,
                            const std::string &, const std::string &, int) = nullptr;
    int (*registerGain)(void *) = nullptr;
    int (*registerEncoderGain)(void *) = nullptr;
    void (*notifyInput)(void *) = nullptr;
};

static const MetadataValue &metadataField(const MetadataValue &value, const std::string &key)
{
    static const MetadataValue missing;
    auto found = value.fields.find(key);
    return value.type == MetadataValue::Object && found != value.fields.end()
        ? found->second : missing;
}

static std::string metadataString(const MetadataValue &value)
{
    return value.type == MetadataValue::String ? value.text : std::string();
}

static void promoteAudioMetadata(MetadataState &state,
                                 const std::map<std::string, std::string> &record,
                                 const MetadataEffects &effects)
{
    state.current = record;
    auto immersive = state.current.find("immersive");
    if (immersive != state.current.end() && effects.updateImmersive)
        effects.updateImmersive(effects.context, immersive->second);
    if (state.current.count("codec") && state.digitalOutputEligible && effects.reapplyDigital)
        effects.reapplyDigital(effects.context, state);
}

/* Ghidra 0x4db20: first exact pipeline/resource match wins; freerun is written
 * BEFORE resource equality, even on rejection. Ordinary fields overwrite;
 * only params arrays flatten to audioParams0, audioParams1, ... . RECORDING
 * promotes parser (+0x214)+0x30 without activeState or codec notification;
 * other purpose elements promote +0x18 once PER element while active. */
static int updateAudioCategoryInfoProjection(MetadataState &state,
                                             const MetadataValue &pipeline,
                                             const MetadataValue &category,
                                             const MetadataEffects &effects)
{
    std::string pipelineId = metadataString(metadataField(pipeline, "pipelineId"));
    if (pipelineId.empty())
        return 0;
    for (auto &record : state.pipelines) {
        auto identifier = record.find("pipelineId");
        if (identifier == record.end() || identifier->second != pipelineId)
            continue;
        const auto &resource = metadataField(pipeline, "resource");
        auto storedResource = record.find("resource");
        if (resource.type != MetadataValue::String || storedResource == record.end())
            continue;
        record["audioFreerunMode"] = effects.isFreerun && effects.isFreerun(effects.context, pipeline)
            ? "true" : "false";
        if (storedResource->second != resource.text)
            continue;
        for (const auto &field : category.fields) {
            const auto &value = field.second;
            if (value.type == MetadataValue::Array) {
                if (field.first == "params") {
                    for (std::size_t index = 0; index < value.elements.size(); ++index)
                        record["audioParams" + std::to_string(index)] = metadataString(value.elements[index]);
                }
            } else {
                record[field.first] = value.text;
            }
        }
        const auto &purpose = metadataField(pipeline, "pipelinePurpose");
        if (purpose.type == MetadataValue::Array) {
            for (const auto &element : purpose.elements) {
                if (metadataString(element) == "RECORDING")
                    state.recording = record;
                else if (state.activeState)
                    promoteAudioMetadata(state, record, effects);
            }
        } else if (state.activeState) {
            promoteAudioMetadata(state, record, effects);
        }
        return 1;
    }
    return 0;
}

/* Ghidra 0x4ee40: invalid category returns before ALL middleware. Invalid
 * pipeline skips merge but still reaches the middleware tail. sourceType and
 * sourceAttribute are read only with object pipeline/category, nonempty string
 * pipelineId AND presence (not type) of category.codec. Cached +0x160/+0x178
 * compares effective current codec and sourceType; sourceAttribute goes to the
 * downstream delay path, not that cache. No direct digital-output call here. */
static int setAudioInputInfoProjection(MetadataState &state, const MetadataValue &input,
                                      const MetadataEffects &effects)
{
    const auto &pipeline = metadataField(input, "pipelineInfo");
    const auto &category = metadataField(input, "categoryInfo");
    std::string sourceType;
    std::string sourceAttribute;
    std::string pipelineId = metadataString(metadataField(pipeline, "pipelineId"));
    if (pipeline.type == MetadataValue::Object && category.type == MetadataValue::Object &&
        !pipelineId.empty() && category.fields.count("codec")) {
        if (effects.prepareDelayPipeline)
            effects.prepareDelayPipeline(effects.context, pipelineId);
        sourceType = metadataString(metadataField(pipeline, "sourceType"));
        sourceAttribute = metadataString(metadataField(pipeline, "sourceAttribute"));
    }
    if (category.type != MetadataValue::Object)
        return 0;
    int result = 0;
    if (pipeline.type == MetadataValue::Object)
        result = updateAudioCategoryInfoProjection(state, pipeline, category, effects);
    auto codec = state.current.find("codec");
    std::string currentCodec = codec == state.current.end() ? "" : codec->second;
    if (state.cachedCodec != currentCodec || state.cachedSource != sourceType) {
        state.cachedCodec = currentCodec;
        state.cachedSource = sourceType;
        if (effects.inputLog)
            effects.inputLog(effects.context);
    }
    int delayResult = effects.registerDelay ? effects.registerDelay(effects.context) : 0;
    if (delayResult) {
        result = effects.finishDelayUpdate
            ? effects.finishDelayUpdate(effects.context, pipeline, category, sourceType,
                                       sourceAttribute, delayResult)
            : delayResult;
    }
    int gainResult = effects.registerGain ? effects.registerGain(effects.context) : 0;
    if (gainResult)
        result = gainResult;
    int encoderResult = effects.registerEncoderGain ? effects.registerEncoderGain(effects.context) : 0;
    if (encoderResult)
        result = encoderResult;
    if (effects.notifyInput)
        effects.notifyInput(effects.context);
    return result;
}

}