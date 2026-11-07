#include <cassert>
#include <string>

// @match src/libpf/track_caps.cpp std::string nonMpegTrackCodec[\s\S]*?\n}
// @extract src/libpf/track_caps.cpp updateDtsTrackCodec

int main()
{
    assert(nonMpegTrackCodec("audio/x-dts") == "dts");
    assert(nonMpegTrackCodec("audio/x-dtsh") == "dtsh");
    assert(nonMpegTrackCodec("audio/x-dtsl") == "dtsl");
    assert(nonMpegTrackCodec("audio/x-dtse") == "dtse");
    assert(nonMpegTrackCodec("audio/x-dtsx") == "dtsx");
    assert(nonMpegTrackCodec("audio/x-true-hd") == "true-hd");
    assert(nonMpegTrackCodec("audio/AMR") == "AMR");
    assert(nonMpegTrackCodec("").empty());
    assert(nonMpegTrackCodec("audio/x-").empty());
    assert(nonMpegTrackCodec("audio/x").empty());
    assert(nonMpegTrackCodec("12345678dts") == "dts");

    const struct {
        const char *mime;
        const char *codecName;
        const char *expected;
    } cases[] = {
        {"audio/x-dtsh", "", "dtsh"},
        {"audio/x-dtsh", "DTSHD", "dtsh"},
        {"audio/x-dtsh", "DTS-HD MA", "dtsh"},
        {"audio/x-dtsh", "dts_hd", "dts_hd"},
        {"audio/x-dtsh", "dts_hd_ma", "dts_hd_ma"},
        {"audio/x-dtsl", "DTS audio lossless", "dtsl"},
        {"audio/x-dtse", "dts_express", "dts_express"},
        {"audio/x-true-hd", "Dolby TrueHD", "true-hd"},
        {"audio/x-true-hd", "truehd", "true-hd"},
        {"audio/x-true-hd", "dts_hd_ma", "dts_hd_ma"},
        {"audio/x-dts", "not-dts-a-codec", "not-dts-a-codec"},
        {"audio/x-dts", "dTs_hd_ma", "dts"},
        {"", "dts_hd_ma", "dts_hd_ma"}
    };
    for (const auto &entry : cases) {
        std::string result = nonMpegTrackCodec(entry.mime);
        updateDtsTrackCodec(result, entry.codecName);
        assert(result == entry.expected);
    }

    std::string codec = nonMpegTrackCodec("audio/x-dtsh");
    updateDtsTrackCodec(codec, "DTSHD");
    assert(codec == "dtsh");
    updateDtsTrackCodec(codec, "");
    assert(codec == "dtsh");
    updateDtsTrackCodec(codec, "dts_hd_ma");
    assert(codec == "dts_hd_ma");
    updateDtsTrackCodec(codec, "DTSHD");
    assert(codec == "dts_hd_ma");
    updateDtsTrackCodec(codec, "prefix-dts-suffix");
    assert(codec == "prefix-dts-suffix");

    codec = "true-hd";
    updateDtsTrackCodec(codec, "Dolby TrueHD");
    assert(codec == "true-hd");
    updateDtsTrackCodec(codec, "");
    assert(codec == "true-hd");
}