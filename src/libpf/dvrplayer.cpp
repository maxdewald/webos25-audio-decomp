/*
 * mediapipeline::DvrPlayer (dvrplayer.cpp). RECONSTRUCTION from libpf-1.0.so.1.0.0.
 * Thin player facade: every *Spi call fetches the boost::shared_ptr<Pipeline> from
 * AbstractPlayer and forwards to the DvrPipeline it created in the constructor.
 * No audio-port / adec / mixer handling happens here (DvrPipeline does that).
 *
 * Layout used here:
 *   DvrPlayer (0x68 bytes, AbstractPlayer is 0x64)
 *     +0x04  std::shared_ptr<ChangeManager>  (AbstractPlayer) handed to DvrPipeline
 *     +0x28  unsigned  m_transportType        (AbstractPlayer) set by DvrPlayer(unsigned)
 *     +0x64  int       ready-to-feed flag: 0 while seek/flush/rate change runs, 1 after
 *   Pipeline
 *     +0x13c pointer checked before seek/flush (presumably the GstElement pipeline)
 *   Pipeline vtable slots (byte offsets):
 *     0x14 preload(const string&)  0x1c load(string)  0x28 unload()  0x30 play(int)
 *     0x34 pause(State)  0x38 seek(ull)  0x40 flush()  0x44 flush(int, ll)
 *     0x6c setPlaybackRate(float, int, ull)  0x70 setPlaybackRate(float, int)
 *     0x8c FeedStream(uchar*, uint, ull, MEDIA_DATA_CHANNEL_T, uint)  0x94 PushEOS()
 *     0xcc setSubtitleEnable(bool)  0xd0 setSubtitleTrack(uint)  0xd8 isSeekable()
 *     0xfc informationMonitorStartSpi(uint)  0x108 setTimeToDecode(ll)
 *     0x114 getCurrentState()  0x12c enterBufferingState()  0x130 exitBufferingState(bool)
 */
#include "libpf.h"

#pragma GCC diagnostic ignored "-Wformat-zero-length"

#include <memory>
#include <utility>
#include <typeinfo>

/* __FILE__ is the build path; the log macro passes its basename (path + 0x61). */
#define DVR_FILE                                                                           \
    ("/usr/src/debug/lib32-player-factory-libs/1.0.0-637.ptl4tv.30-r41/git/player-factory/" \
     "internal/dvr/dvrplayer.cpp" + 0x61)

#include "shared_ptr.h"

enum MEDIA_CUSTOM_SRC_TYPE_T {};
enum MEDIA_DATA_CHANNEL_T {};
struct MEDIA_CUSTOM_CONTENT_INFO;
typedef int MEDIA_STATUS_T;

/* Vtable slot `off` of a polymorphic object, as a typed function pointer. */
template <typename F> static inline F vfunc(const void *obj, unsigned off)
{
    return *(F *)(*(const char *const *)obj + off);
}

/* libstdc++ std::string::_M_create (private member, called by its mangled name). */
extern "C" char *_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj(
    std::string *self, size_t *capacity, size_t oldCapacity);

static void string_construct_range(std::string *s, const char *beg, const char *end)
    __attribute__((used));
static void string_construct_range_c(std::string *s, const char *beg, const char *end)
    __attribute__((used));

namespace mediapipeline {

/* Pipeline::pflog / DvrPlayer::dvrplayer_pflog: context + 16-byte name (0x14 bytes). */
struct PfLog20 {
    PmLogContext ctx;
    char name[16];
};

class ChangeManager;

class Pipeline {
public:
    enum State {};
    virtual ~Pipeline();
    bool isReadyToPlay();
    bool setPlaybackRate(float rate);
    bool getAudioLanguagesList(char **list, int *count, int *len);
    bool setAudioLanguage(const char *lang);
    bool setAudioTrack(int track);
    bool getCurAudioLanguage(char **lang);
    bool getCurAudioTrack(int *track);
    bool getTotalVideoAngle(int *total);
    bool setVideoAngle(int angle);
    bool getCurrentVideoAngle(int *angle);
    bool getBufferQueueTimeStamp(unsigned long long *start, unsigned long long *end);
    long long position(int format);
    void setTransportType(unsigned int type);
    int load_getInfo(std::string options, char **info, MEDIA_CUSTOM_CONTENT_INFO *content);
    void registerTxCustomCallback(void (*cb)(void *, int, long long, const char *), void *data);
    void setRealTimeStream(int realTime);
    static PfLog20 pflog;
};

class DvrPipeline : public Pipeline {
public:
    DvrPipeline(std::shared_ptr<ChangeManager> changeManager, unsigned int transportType);
    ~DvrPipeline();
    int internalPlay();
    int internalPause();
    MEDIA_STATUS_T internalPushEOS();
    bool preload(MEDIA_CUSTOM_SRC_TYPE_T type, MEDIA_CUSTOM_CONTENT_INFO *info);
    bool load(MEDIA_CUSTOM_SRC_TYPE_T type, MEDIA_CUSTOM_CONTENT_INFO *info);
};

class AbstractPlayer {
public:
    AbstractPlayer();
    virtual ~AbstractPlayer();
    boost::shared_ptr<Pipeline> getPipeline() const;
    void setPipeline(boost::shared_ptr<Pipeline> pipeline);

private:
    char opaque_[0x60];
};

class DvrPlayer : public AbstractPlayer {
public:
    DvrPlayer();
    DvrPlayer(unsigned int transportType);
    ~DvrPlayer();
    void registerCallbackFunctionSpi(void (*cb)(int, long long, const char *));
    bool playSpi(int rate);
    bool pauseSpi();
    bool preload(const std::string &options);
    bool seekSpi(unsigned long long position);
    bool flushSpi();
    bool flushSpi(int type, long long time);
    bool isReadyToPlay();
    bool setPlaybackRateSpi(float rate);
    bool setPlaybackRateSpi(float rate, int audioOn);
    bool setPlaybackRateSpi(float rate, int audioOn, unsigned long long pts);
    bool unloadSpi();
    MEDIA_STATUS_T feedSpi(unsigned char *data, unsigned int size, unsigned long long pts,
                           MEDIA_DATA_CHANNEL_T channel, unsigned int flags);
    MEDIA_STATUS_T pushEOSSpi();
    bool isSeekableSpi() const;
    bool setTimeToDecodeSpi(long long time);
    bool getAudioLanguagesListSpi(char **list, int *count, int *len);
    bool setAudioLanguageSpi(const char *lang);
    bool setAudioTrackSpi(int track);
    bool getCurAudioLanguageSpi(char **lang);
    bool getCurAudioTrackSpi(int *track);
    bool getTotalVideoAngleSpi(int *total);
    bool setVideoAngleSpi(int angle);
    bool getCurrentVideoAngleSpi(int *angle);
    bool setSubtitleEnable(bool enable);
    bool setSubtitleTrack(unsigned int track);
    bool getBufferQueueTimeStampSpi(unsigned long long *start, unsigned long long *end);
    long long positionSpi();
    int loadSpi(std::string options);
    bool internalPlay();
    bool internalPause();
    MEDIA_STATUS_T internalPushEOS();
    int loadSpi_getInfo(std::string options, char **info, MEDIA_CUSTOM_CONTENT_INFO *content);
    bool preload(MEDIA_CUSTOM_SRC_TYPE_T type, MEDIA_CUSTOM_CONTENT_INFO *info);
    bool loadSpi(MEDIA_CUSTOM_SRC_TYPE_T type, const char *uri, const char *option,
                 unsigned long long size, MEDIA_CUSTOM_CONTENT_INFO *info);
    long long durationSpi() const;
    bool isAudioAvailableSpi() const;
    bool isVideoAvailableSpi() const;
    float playbackRateSpi() const;
    void registerTxCustomCallback(void (*cb)(void *, int, long long, const char *), void *data);
    void setRealTimeStream(int realTime);
    void enterBufferingState();
    void exitBufferingState(bool resume);
    static PfLog20 dvrplayer_pflog;

private:
    char fields_[4];
};

PfLog20 DvrPlayer::dvrplayer_pflog;

#define DP_LOG             DvrPlayer::dvrplayer_pflog
#define DP_CHANGE_MGR(s)   FIELD(std::shared_ptr<ChangeManager>, s, 0x04)
#define DP_TRANSPORT(s)    FIELD(unsigned int, s, 0x28)
#define DP_READY(s)        FIELD(int, s, 0x64)
#define PIPE_ELEMENT(p)    FIELD(void *, p, 0x13c)

/* @0x1173a4: stub, DP players must register through their own player. */
void DvrPlayer::registerCallbackFunctionSpi(void (*cb)(int, long long, const char *))
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "registerCallbackFunctionSpi", 0x18b, "DvrPlayer");
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "registerCallbackFunctionSpi", 0x18d,
                 "registerCallbackFunctionSpi ---> wrong ---> must get this function at DP "
                 "Player not HERE");
}

/* @0x11750c */
DvrPlayer::~DvrPlayer()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "~DvrPlayer", 0x39, "DvrPlayer");
}

/* @0x1175e8 = _ZN13mediapipeline9DvrPlayerD0Ev: deleting destructor (operator delete 0x68),
 * emitted by GCC from ~DvrPlayer above. */

} // namespace mediapipeline

/* @0x1175fc: std::string::_M_construct(beg, end) instance used by the by-value string copies. */
static void string_construct_range(std::string *s, const char *beg, const char *end)
{
    if (beg == NULL && end != NULL)
        std::__throw_logic_error("basic_string::_M_construct null not valid");
    size_t n = end - beg;
    size_t len = n;
    char *dst;
    if (n < 0x10) {
        dst = FIELD(char *, s, 0);
        if (n == 1) {
            *dst = *beg;
            FIELD(size_t, s, 4) = 1;
            FIELD(char *, s, 0)[1] = 0;
            return;
        }
        if (n == 0) {
            FIELD(size_t, s, 4) = 0;
            *dst = 0;
            return;
        }
    } else {
        dst = _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj(s, &len, 0);
        FIELD(char *, s, 0) = dst;
        FIELD(size_t, s, 8) = len;
    }
    memcpy(dst, beg, n);
    FIELD(size_t, s, 4) = len;
    FIELD(char *, s, 0)[len] = 0;
}

/* @0x117670: identical second instance (other iterator type). */
static void string_construct_range_c(std::string *s, const char *beg, const char *end)
{
    if (beg == NULL && end != NULL)
        std::__throw_logic_error("basic_string::_M_construct null not valid");
    size_t n = end - beg;
    size_t len = n;
    char *dst;
    if (n < 0x10) {
        dst = FIELD(char *, s, 0);
        if (n == 1) {
            *dst = *beg;
            FIELD(size_t, s, 4) = 1;
            FIELD(char *, s, 0)[1] = 0;
            return;
        }
        if (n == 0) {
            FIELD(size_t, s, 4) = 0;
            *dst = 0;
            return;
        }
    } else {
        dst = _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj(s, &len, 0);
        FIELD(char *, s, 0) = dst;
        FIELD(size_t, s, 8) = len;
    }
    memcpy(dst, beg, n);
    FIELD(size_t, s, 4) = len;
    FIELD(char *, s, 0)[len] = 0;
}

namespace mediapipeline {

/* @0x1176e4 */
bool DvrPlayer::playSpi(int rate)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "playSpi", 0x3d, "");
    DP_READY(this) = 1;
    boost::shared_ptr<Pipeline> p = getPipeline();
    vfunc<bool (*)(Pipeline *, int)>(p.get(), 0x30)(p.get(), rate);
    return true;
}

/* @0x1177e4: pause(State 5). */
bool DvrPlayer::pauseSpi()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "pauseSpi", 0x47, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(Pipeline *, Pipeline::State)>(p.get(), 0x34)(p.get(),
                                                                        (Pipeline::State)5);
}

/* @0x1178d4 */
bool DvrPlayer::preload(const std::string &options)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "preload", 0x70, "");
    if (options.empty()) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "preload", 0x73, "ERROR!!!!! option string empty!!!!!");
        return false;
    }
    boost::shared_ptr<Pipeline> p = getPipeline();
    bool ret = vfunc<bool (*)(Pipeline *, const std::string &)>(p.get(), 0x14)(p.get(), options);
    if (ret)
        DP_READY(this) = 1;
    return ret;
}

/* @0x117a8c: refused when the pipeline element is missing or state == 4. */
bool DvrPlayer::seekSpi(unsigned long long position)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "seekSpi", 0xca, "position:%llu", position);
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!PIPE_ELEMENT(p.get()) ||
        vfunc<int (*)(const Pipeline *)>(p.get(), 0x114)(p.get()) == 4) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "seekSpi", 0xd0, "not paused or playing state");
        return false;
    }
    DP_READY(this) = 0;
    if (!vfunc<bool (*)(Pipeline *, unsigned long long)>(p.get(), 0x38)(p.get(), position)) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "seekSpi", 0xd5, "fail gstreamer seek");
        return false;
    }
    DP_READY(this) = 1;
    return true;
}

/* @0x117d0c */
bool DvrPlayer::flushSpi()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "flushSpi", 0xde, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!PIPE_ELEMENT(p.get()) ||
        vfunc<int (*)(const Pipeline *)>(p.get(), 0x114)(p.get()) == 4) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "flushSpi", 0xe4, "not paused or playing state");
        return false;
    }
    DP_READY(this) = 0;
    if (!vfunc<bool (*)(Pipeline *)>(p.get(), 0x40)(p.get())) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "flushSpi", 0xe9, "fail gstreamer flush");
        return false;
    }
    DP_READY(this) = 1;
    return true;
}

/* @0x117f7c: only type 0 clears the ready flag. GCC speculatively devirtualized slot 0x44
 * against the inline Pipeline::flush(int, long long) default (returns false, 0x81794). */
bool DvrPlayer::flushSpi(int type, long long time)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "flushSpi", 0xf2, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!PIPE_ELEMENT(p.get()) ||
        vfunc<int (*)(const Pipeline *)>(p.get(), 0x114)(p.get()) == 4) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "flushSpi", 0xf8, "not paused or playing state");
        return false;
    }
    if (type == 0)
        DP_READY(this) = 0;
    if (!vfunc<bool (*)(Pipeline *, int, long long)>(p.get(), 0x44)(p.get(), type, time)) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "flushSpi", 0xff, "fail gstreamer flush");
        return false;
    }
    DP_READY(this) = 1;
    return true;
}

/* @0x118200 */
bool DvrPlayer::isReadyToPlay()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "isReadyToPlay", 0x107, "DvrPlayer");
    return getPipeline()->isReadyToPlay();
}

/* @0x1182f0 */
bool DvrPlayer::setPlaybackRateSpi(float rate)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setPlaybackRateSpi", 0x10d, "");
    return getPipeline()->setPlaybackRate(rate);
}

/* @0x1183f0 */
bool DvrPlayer::setPlaybackRateSpi(float rate, int audioOn)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setPlaybackRateSpi", 0x113, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(Pipeline *, float, int)>(p.get(), 0x70)(p.get(), rate, audioOn);
}

/* @0x118500: the ready flag is restored even when the seek fails. */
bool DvrPlayer::setPlaybackRateSpi(float rate, int audioOn, unsigned long long pts)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setPlaybackRateSpi", 0x11a, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    bool ret;
    DP_READY(this) = 0;
    if (!vfunc<bool (*)(Pipeline *, float, int, unsigned long long)>(p.get(), 0x6c)(
            p.get(), rate, audioOn, pts)) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "setPlaybackRateSpi", 0x11f, "fail gst_element_seek");
        ret = false;
    } else {
        ret = true;
    }
    DP_READY(this) = 1;
    return ret;
}

/* @0x1186d8 */
bool DvrPlayer::unloadSpi()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "unloadSpi", 0x128, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(Pipeline *)>(p.get(), 0x28)(p.get());
}

/* @0x1187c8 */
MEDIA_STATUS_T DvrPlayer::feedSpi(unsigned char *data, unsigned int size, unsigned long long pts,
                                  MEDIA_DATA_CHANNEL_T channel, unsigned int flags)
{
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!DP_READY(this)) {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "feedSpi", 0x147, "not ready to feed stream");
        return -1;
    }
    return vfunc<MEDIA_STATUS_T (*)(Pipeline *, unsigned char *, unsigned int,
                                    unsigned long long, MEDIA_DATA_CHANNEL_T, unsigned int)>(
        p.get(), 0x8c)(p.get(), data, size, pts, channel, flags);
}

/* @0x1188e4 */
MEDIA_STATUS_T DvrPlayer::pushEOSSpi()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "pushEOSSpi", 0x152, "MEDIA_STATUS_T DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<MEDIA_STATUS_T (*)(Pipeline *)>(p.get(), 0x94)(p.get());
}

/* @0x1189d8 */
bool DvrPlayer::isSeekableSpi() const
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "isSeekableSpi", 0x16a, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(const Pipeline *)>(p.get(), 0xd8)(p.get());
}

/* @0x118acc: only applied in state 6. */
bool DvrPlayer::setTimeToDecodeSpi(long long time)
{
    boost::shared_ptr<Pipeline> p = getPipeline();
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setTimeToDecodeSpi", 0x17b, "DvrPlayer");
    if (vfunc<int (*)(const Pipeline *)>(p.get(), 0x114)(p.get()) == 6) {
        vfunc<bool (*)(Pipeline *, long long)>(p.get(), 0x108)(p.get(), time);
        return true;
    }
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setTimeToDecodeSpi", 0x181, "[ERROR]state: %d",
                 vfunc<int (*)(const Pipeline *)>(p.get(), 0x114)(p.get()));
    return false;
}

/* @0x118cac */
bool DvrPlayer::getAudioLanguagesListSpi(char **list, int *count, int *len)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "getAudioLanguagesListSpi", 0x19e, "DvrPlayer");
    return getPipeline()->getAudioLanguagesList(list, count, len);
}

/* @0x118dc0 */
bool DvrPlayer::setAudioLanguageSpi(const char *lang)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setAudioLanguageSpi", 0x1a5, "DvrPlayer");
    return getPipeline()->setAudioLanguage(lang);
}

/* @0x118ec0 */
bool DvrPlayer::setAudioTrackSpi(int track)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setAudioTrackSpi", 0x1ac, "DvrPlayer");
    return getPipeline()->setAudioTrack(track);
}

/* @0x118fc0 */
bool DvrPlayer::getCurAudioLanguageSpi(char **lang)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "getCurAudioLanguageSpi", 0x1b3, "DvrPlayer");
    return getPipeline()->getCurAudioLanguage(lang);
}

/* @0x1190c0 */
bool DvrPlayer::getCurAudioTrackSpi(int *track)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "getCurAudioTrackSpi", 0x1ba, "DvrPlayer");
    return getPipeline()->getCurAudioTrack(track);
}

/* @0x1191c0 */
bool DvrPlayer::getTotalVideoAngleSpi(int *total)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "getTotalVideoAngleSpi", 0x1c1, "DvrPlayer");
    return getPipeline()->getTotalVideoAngle(total);
}

/* @0x1192c0 */
bool DvrPlayer::setVideoAngleSpi(int angle)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setVideoAngleSpi", 0x1c8, "DvrPlayer");
    return getPipeline()->setVideoAngle(angle);
}

/* @0x1193c0 */
bool DvrPlayer::getCurrentVideoAngleSpi(int *angle)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "getCurrentVideoAngleSpi", 0x1cf, "DvrPlayer");
    return getPipeline()->getCurrentVideoAngle(angle);
}

/* @0x1194c0 */
bool DvrPlayer::setSubtitleEnable(bool enable)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setSubtitleEnable", 0x1d7, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(Pipeline *, bool)>(p.get(), 0xcc)(p.get(), enable);
}

/* @0x1195c4 */
bool DvrPlayer::setSubtitleTrack(unsigned int track)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "setSubtitleTrack", 0x1de, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return vfunc<bool (*)(Pipeline *, unsigned int)>(p.get(), 0xd0)(p.get(), track);
}

/* @0x1196c8 */
bool DvrPlayer::getBufferQueueTimeStampSpi(unsigned long long *start, unsigned long long *end)
{
    return getPipeline()->getBufferQueueTimeStamp(start, end);
}

/* @0x119764: -1 without a pipeline; position(1). */
long long DvrPlayer::positionSpi()
{
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!p)
        return -1;
    return p->position(1);
}

/* @0x119808: starts the DvrPipeline information monitor (200 ms) after a successful load. */
int DvrPlayer::loadSpi(std::string options)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "loadSpi", 0x82, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    int ret = vfunc<int (*)(Pipeline *, std::string)>(p.get(), 0x1c)(p.get(), options);
    if (ret == 1) {
        ret = vfunc<int (*)(Pipeline *, unsigned int)>(p.get(), 0xfc)(p.get(), 200);
        DP_READY(this) = 1;
    } else {
        PF_LOG_ERROR(DP_LOG, DVR_FILE, "loadSpi", 0x8d,
                     "pipeline load return false. skip start information timer.");
    }
    return ret;
}

/* @0x119b5c */
bool DvrPlayer::internalPlay()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "internalPlay", 0x4d, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return dynamic_cast<DvrPipeline *>(p.get())->internalPlay();
}

/* @0x119c74 */
bool DvrPlayer::internalPause()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "internalPause", 0x5a, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return dynamic_cast<DvrPipeline *>(p.get())->internalPause();
}

/* @0x119d8c */
MEDIA_STATUS_T DvrPlayer::internalPushEOS()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "internalPushEOS", 0x67, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return dynamic_cast<DvrPipeline *>(p.get())->internalPushEOS();
}

/* @0x119ea0 */
int DvrPlayer::loadSpi_getInfo(std::string options, char **info,
                               MEDIA_CUSTOM_CONTENT_INFO *content)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "loadSpi_getInfo", 0x99, "DvrPlayer");
    boost::shared_ptr<Pipeline> p = getPipeline();
    p->setTransportType(DP_TRANSPORT(this));
    int ret = p->load_getInfo(options, info, content);
    if (ret == 1)
        DP_READY(this) = 1;
    return ret;
}

/* @0x119fe8 */
bool DvrPlayer::preload(MEDIA_CUSTOM_SRC_TYPE_T type, MEDIA_CUSTOM_CONTENT_INFO *info)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "preload", 0xad, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    return dynamic_cast<DvrPipeline *>(p.get())->preload(type, info);
}

/* @0x11a10c: uri, option and size are unused. */
bool DvrPlayer::loadSpi(MEDIA_CUSTOM_SRC_TYPE_T type, const char *uri, const char *option,
                        unsigned long long size, MEDIA_CUSTOM_CONTENT_INFO *info)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "loadSpi", 0xbc, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    if (!dynamic_cast<DvrPipeline *>(p.get())->load(type, info))
        return false;
    return vfunc<int (*)(Pipeline *, unsigned int)>(p.get(), 0xfc)(p.get(), 200);
}

/* @0x11a238 */
long long DvrPlayer::durationSpi() const
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "durationSpi", 0x130, "DvrPlayer");
    return 0;
}

/* @0x11a300 */
bool DvrPlayer::isAudioAvailableSpi() const
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "isAudioAvailableSpi", 0x15c, "DvrPlayer");
    return false;
}

/* @0x11a3c4 */
bool DvrPlayer::isVideoAvailableSpi() const
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "isVideoAvailableSpi", 0x163, "DvrPlayer");
    return false;
}

/* @0x11a488 */
float DvrPlayer::playbackRateSpi() const
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "playbackRateSpi", 0x170, "DvrPlayer");
    return 0;
}

/* @0x11a54c */
void DvrPlayer::registerTxCustomCallback(void (*cb)(void *, int, long long, const char *),
                                         void *data)
{
    boost::shared_ptr<Pipeline> p = getPipeline();
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "registerTxCustomCallback", 0x195, "DvrPlayer");
    p->registerTxCustomCallback(cb, data);
}

/* @0x11a650 */
void DvrPlayer::setRealTimeStream(int realTime)
{
    getPipeline()->setRealTimeStream(realTime);
}

/* @0x11a6e4 */
void DvrPlayer::enterBufferingState()
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "enterBufferingState", 0x1f5, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    vfunc<void (*)(Pipeline *)>(p.get(), 0x12c)(p.get());
}

/* @0x11a7d0 */
void DvrPlayer::exitBufferingState(bool resume)
{
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "exitBufferingState", 0x1fc, "");
    boost::shared_ptr<Pipeline> p = getPipeline();
    vfunc<void (*)(Pipeline *, bool)>(p.get(), 0x130)(p.get(), resume);
}

/* @0x11a8d0: replaces the AbstractPlayer pipeline with a DvrPipeline (transport type 0)
 * and adopts the Pipeline logger. */
DvrPlayer::DvrPlayer() : AbstractPlayer()
{
    boost::shared_ptr<Pipeline> pipeline = getPipeline();
    pipeline.reset(new DvrPipeline(DP_CHANGE_MGR(this), 0));
    setPipeline(pipeline);
    dvrplayer_pflog = Pipeline::pflog;
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "DvrPlayer", 0x21, "DvrPlayer");
    DP_READY(this) = 0;
}

/* @0x11ab48 */
DvrPlayer::DvrPlayer(unsigned int transportType) : AbstractPlayer()
{
    boost::shared_ptr<Pipeline> pipeline = getPipeline();
    pipeline.reset(new DvrPipeline(DP_CHANGE_MGR(this), transportType));
    setPipeline(pipeline);
    dvrplayer_pflog = Pipeline::pflog;
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "DvrPlayer", 0x2f, "DvrPlayer");
    DP_TRANSPORT(this) = transportType;
    DP_READY(this) = 0;
    PF_LOG_DEBUG(DP_LOG, DVR_FILE, "DvrPlayer", 0x35, "m_transportType : %d",
                 DP_TRANSPORT(this));
}

} // namespace mediapipeline
