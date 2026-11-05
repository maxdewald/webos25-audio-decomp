/* AdaptivePlayer: raw ARM checked at Ghidra 0xe797c..0xe8154.
 * Object size 0x64; +4 holds std::shared_ptr<ChangeManager>. Pipeline ownership is
 * boost::shared_ptr, not std::shared_ptr. The constructor replaces the base pipeline.
 * No ADEC or mixer index is computed here; resource delivery belongs to Pipeline.
 */
#include "libpf.h"
#include "shared_ptr.h"
#include <memory>
#include <stdexcept>

namespace mediapipeline {
class ChangeManager;
class Pipeline { public: virtual ~Pipeline(); };
class AdaptivePipeline : public Pipeline { public: ~AdaptivePipeline(); };
}

using mediapipeline::Pipeline;
using mediapipeline::AdaptivePipeline;
using mediapipeline::ChangeManager;

struct AdaptiveLog { PmLogContext ctx; char name[16]; };
extern AdaptiveLog adaptive_log __asm__("_ZN13mediapipeline14AdaptivePlayer20adaptiveplayer_pflogE");
extern AdaptiveLog pipeline_log __asm__("_ZN13mediapipeline8Pipeline5pflogE");
extern void *adaptive_vtable[] __asm__("_ZTVN13mediapipeline14AdaptivePlayerE");

extern "C" {
void generic_construct(void *) __asm__("_ZN13mediapipeline13GenericPlayerC2Ev");
void generic_destruct(void *) __asm__("_ZN13mediapipeline13GenericPlayerD2Ev");
boost::shared_ptr<Pipeline> abstract_pipeline(const void *)
    __asm__("_ZNK13mediapipeline14AbstractPlayer11getPipelineEv");
void abstract_set_pipeline(void *, boost::shared_ptr<Pipeline>)
    __asm__("_ZN13mediapipeline14AbstractPlayer11setPipelineEN5boost10shared_ptrINS_8PipelineEEE");
void adaptive_pipeline_construct(AdaptivePipeline *, std::shared_ptr<ChangeManager>)
    __asm__("_ZN13mediapipeline16AdaptivePipelineC1ESt10shared_ptrINS_13ChangeManagerEE");
bool adaptive_set_uri(AdaptivePipeline *, const char *)
    __asm__("_ZN13mediapipeline16AdaptivePipeline9setNewURIEPKc");
char *string_create(std::string *, size_t *, size_t)
    __asm__("_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj");
}

void *AdaptivePlayer_destruct(void *) __asm__("_ZN13mediapipeline14AdaptivePlayerD1Ev");
/* @0xe797c */
void *AdaptivePlayer_destruct(void *self)
{
    FIELD(void **, self, 0) = adaptive_vtable + 2;
    PF_LOG_DEBUG(adaptive_log, "adaptiveplayer.cpp", "~AdaptivePlayer", 0x25, "AdaptivePlayer");
    generic_destruct(self);
    return self;
}

void *AdaptivePlayer_delete(void *) __asm__("_ZN13mediapipeline14AdaptivePlayerD0Ev");
/* @0xe7a58 */
void *AdaptivePlayer_delete(void *self)
{
    AdaptivePlayer_destruct(self);
    ::operator delete(self, size_t(0x64));
    return self;
}

static void adaptive_string_construct(std::string *, const char *, const char *)
    __attribute__((used, noinline));
/* @0xe7a6c: outlined libstdc++ string range constructor (24-byte ARM string). */
static void adaptive_string_construct(std::string *out, const char *begin, const char *end)
{
    if (!begin && end)
        std::__throw_logic_error("basic_string::_M_construct null not valid");
    size_t length = end - begin;
    char *data;
    if (length > 15) {
        data = string_create(out, &length, 0);
        FIELD(char *, out, 0) = data;
        FIELD(size_t, out, 8) = length;
    } else {
        data = FIELD(char *, out, 0);
    }
    if (length == 1)
        *data = *begin;
    else if (length)
        memcpy(data, begin, length);
    FIELD(size_t, out, 4) = length;
    data[length] = '\0';
}

bool AdaptivePlayer_setNewURI(void *, const char *)
    __asm__("_ZN13mediapipeline14AdaptivePlayer9setNewURIEPKc");
/* @0xe7ae0: temporary shared_ptr is released before the direct tail call. Ghidra
 * incorrectly inlines AdaptivePipeline::setNewURI into this 304-byte wrapper. */
bool AdaptivePlayer_setNewURI(void *self, const char *uri)
{
    PF_LOG_DEBUG(adaptive_log, "adaptiveplayer.cpp", "setNewURI", 0x29, "AdaptivePlayer");
    Pipeline *pipeline;
    {
        auto owned = abstract_pipeline(self);
        pipeline = owned.get();
    }
    return adaptive_set_uri(static_cast<AdaptivePipeline *>(pipeline), uri);
}

void *AdaptivePlayer_construct(void *) __asm__("_ZN13mediapipeline14AdaptivePlayerC1Ev");
/* @0xe7d84: getPipeline returns two words through a hidden return pointer. */
void *AdaptivePlayer_construct(void *self)
{
    generic_construct(self);
    FIELD(void **, self, 0) = adaptive_vtable + 2;
    try {
        auto pipeline = abstract_pipeline(self);
        void *storage = ::operator new(0x7b0);
        auto *replacement = static_cast<AdaptivePipeline *>(storage);
        try {
            adaptive_pipeline_construct(replacement,
                FIELD(std::shared_ptr<ChangeManager>, self, 4));
        } catch (...) {
            ::operator delete(storage);
            throw;
        }
        pipeline.reset(replacement);
        abstract_set_pipeline(self, pipeline);
        adaptive_log = pipeline_log;
        PF_LOG_DEBUG(adaptive_log, "adaptiveplayer.cpp", "AdaptivePlayer", 0x1f, "AdaptivePlayer");
        PF_LOG_DEBUG(adaptive_log, "adaptiveplayer.cpp", "AdaptivePlayer", 0x20,
                     "> create adaptive pipeline");
    } catch (...) {
        generic_destruct(self);
        throw;
    }
    return self;
}

bool compareAudioUsingTrackOrder(const void *, const void *)
    __asm__("_Z27compareAudioUsingTrackOrderN13mediapipeline14AudioTrackInfoES0_");
/* @0xe8140 */
bool compareAudioUsingTrackOrder(const void *left, const void *right)
{
    return FIELD(int, left, 0x154) < FIELD(int, right, 0x154);
}

bool compareAudioUsingValidTrackOrder(const void *, const void *)
    __asm__("_Z32compareAudioUsingValidTrackOrderN13mediapipeline14AudioTrackInfoES0_");
/* @0xe8154: unordered (-1) tracks follow tracks with an explicit order. */
bool compareAudioUsingValidTrackOrder(const void *left, const void *right)
{
    int a = FIELD(int, left, 0x154);
    int b = FIELD(int, right, 0x154);
    return a != -1 && (b == -1 || a < b);
}
