#include <string>

namespace mediapipeline {

/* REA Hopper ev_5277d1c38ad9089665271930c66c9b624580f0349ecc77c7f5f602b214add366,
 * shipped libpf raw 0x78720 (Ghidra 0x88720).
 * Extracted from Pipeline::handleGstMediaInfoAudio, non-MPEG path
 * 0x88ae0-0x88b5c. The caller stores profile separately at track +0x48;
 * this block uses only MIME, not profile, tags or pbutils descriptions.
 * No prefix validation or dtsh/dtsl/true-hd spelling normalization occurs. */
std::string nonMpegTrackCodec(const std::string &mimeType)
{
    if (mimeType == "audio/AMR")
        return mimeType.substr(6);
    if (mimeType.empty() || mimeType.size() < 9)
        return "";
    return mimeType.substr(8);
}

/* Extracted from Pipeline::handleGstMediaInfoAudio (0x88720), 0x88a40-0x88a86.
 * A lowercase dts substring replaces the MIME-derived codec verbatim. This is
 * not a profile classifier or a reconstruction of the complete handler.
 * These helpers intentionally carry no function-address coverage annotation. */
void updateDtsTrackCodec(std::string &codec, const std::string &codecName)
{
    if (codecName.find("dts") != std::string::npos)
        codec = codecName;
}

}