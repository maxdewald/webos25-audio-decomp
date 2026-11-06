/* Metadata wire formats reconstructed from the shipped Thumb functions.
 * Both internal track layouts are checked in metadata.hpp against ELF reads.
 * Missing tokens default individually; malformed numeric tokens throw, as stoi/stof
 * do in the original. JSON parses use default-valued property_tree conversions. */
#include "metadata.hpp"
#include <sstream>
#include <cstdlib>
#include <vector>

namespace {
std::vector<std::string> tokens(const std::string &text)
{
    std::vector<std::string> fields;
    std::istringstream stream(text);
    std::string field;
    while (std::getline(stream, field, ' ')) fields.push_back(field);
    return fields;
}
std::string textAt(const std::vector<std::string> &fields, unsigned index, const char *fallback = "(null)")
{
    return index < fields.size() ? fields[index] : fallback;
}
int intAt(const std::vector<std::string> &fields, unsigned index)
{
    return index < fields.size() ? std::stoi(fields[index]) : 0;
}
float floatAt(const std::vector<std::string> &fields, unsigned index)
{
    return index < fields.size() ? std::stof(fields[index]) : 0.0f;
}
/* fromJsonString calls get_child("ratings"), so a valid JSON object without
 * ratings still throws bad_path. Each rating's scalar data is copied. */
std::string ratingsJson(const smp::Ptree &track)
{
    smp::Ptree ratings, message;
    for (const auto &entry : track.get_child("ratings")) {
        smp::Ptree value;
        value.put_value(entry.second.data());
        ratings.push_back({"", value});
    }
    message.add_child("ratings", ratings);
    return smp::util::generateJsonString(message);
}
}
namespace smp_internal {
/* @0x71de0: legacy audio text starts with MIME, but reports the separate token-10
 * codec. MIME audio/x-raw is not itself reclassified here. ac4Text preserves tail
 * words; token 20 also sets audioEmergency (an observable wire-format overlap). */
void AudioTrack::fromString(const std::string &text)
{
    auto fields = tokens(text);
    mimeType = textAt(fields, 0, "");
    profile = textAt(fields, 1);
    level = textAt(fields, 2);
    sampleRate = floatAt(fields, 3);
    channels = intAt(fields, 4);
    language = textAt(fields, 5);
    pid = intAt(fields, 6);
    ctag = intAt(fields, 7);
    audioDescription = intAt(fields, 8) != 0;
    audioType = intAt(fields, 9);
    codec = textAt(fields, 10);
    trackId = intAt(fields, 11);
    role = textAt(fields, 12);
    adaptationSetId = intAt(fields, 13);
    immersive = textAt(fields, 14, "none");
    selected = fields.size() > 15 && std::strtol(fields[15].c_str(), nullptr, 10) != 0;
    periodStart = intAt(fields, 16);
    representationId = textAt(fields, 17);
    preselectionId = textAt(fields, 18);
    ac4Text = textAt(fields, 19);
    audioEmergency = intAt(fields, 20) != 0;
    for (unsigned index = 20; index < fields.size(); ++index) ac4Text += " " + fields[index];
}
/* @0x7148c: legacy video token order differs from its in-memory member order. */
void VideoTrack::fromString(const std::string &text)
{
    auto fields = tokens(text);
    angleNumber = intAt(fields, 0);
    mimeType = textAt(fields, 1, "");
    profile = textAt(fields, 2);
    level = textAt(fields, 3);
    width = intAt(fields, 4);
    height = intAt(fields, 5);
    aspectRatio = textAt(fields, 6);
    frameRate = floatAt(fields, 7);
    bitRate = static_cast<int>(floatAt(fields, 8));
    pid = intAt(fields, 9);
    ctag = intAt(fields, 10);
    codec = textAt(fields, 11);
    trackId = intAt(fields, 12);
    role = textAt(fields, 13);
    adaptationSetId = intAt(fields, 14);
    orgCodec = textAt(fields, 15);
    selected = fields.size() > 16 && std::strtol(fields[16].c_str(), nullptr, 10) != 0;
    periodStart = intAt(fields, 17);
    representationId = textAt(fields, 18);
}
/* @0x742ac: audio JSON requires audioTrackInfo and ratings. Missing scalar defaults
 * are zero/(null), MIME empty, immersive none. No DTS subtype is inferred. */
bool AudioTrack::fromJsonString(const std::string &text)
{
    Ptree message;
    if (!smp::util::parseJson(text.c_str(), &message)) return false;
    auto child = message.get_child_optional("audioTrackInfo");
    if (!child) return false;
    const auto &track = *child;
    mimeType = track.get<std::string>("mimeType", "");
    profile = track.get<std::string>("profile", "(null)");
    level = track.get<std::string>("level", "(null)");
    sampleRate = track.get<float>("sampleRate", 0.0f);
    channels = track.get<int>("channels", 0);
    language = track.get<std::string>("language", "(null)");
    pid = track.get<int>("pid", 0);
    ctag = track.get<int>("ctag", 0);
    audioDescription = track.get<int>("audioDescription", 0) != 0;
    audioType = track.get<int>("audioType", 0);
    codec = track.get<std::string>("codec", "(null)");
    trackId = track.get<int>("trackId", 0);
    role = track.get<std::string>("role", "(null)");
    adaptationSetId = track.get<int>("adaptationSetId", 0);
    immersive = track.get<std::string>("immersive", "none");
    selected = track.get<bool>("selected", false);
    periodStart = track.get<int>("periodStart", 0);
    representationId = track.get<std::string>("representationId", "(null)");
    preselectionId = track.get<std::string>("preselectionId", "(null)");
    ac4Text = track.get<std::string>("ac4Text", "(null)");
    audioEmergency = track.get<int>("audioEmergency", 0) != 0;
    ratings = ratingsJson(track);
    selectionPriority = track.get<int>("selectionPriority", 0);
    return true;
}
/* @0x76110: video JSON has no progressive parse in this function; an existing
 * progressive value survives. Ratings scalar data is reserialized to a string. */
bool VideoTrack::fromJsonString(const std::string &text)
{
    Ptree message;
    if (!smp::util::parseJson(text.c_str(), &message)) return false;
    auto child = message.get_child_optional("videoTrackInfo");
    if (!child) return false;
    const auto &track = *child;
    angleNumber = track.get<int>("angleNumber", 0);
    mimeType = track.get<std::string>("mimeType", "");
    profile = track.get<std::string>("profile", "(null)");
    level = track.get<std::string>("level", "(null)");
    width = track.get<int>("width", 0);
    height = track.get<int>("height", 0);
    aspectRatio = track.get<std::string>("aspectRatio", "(null)");
    frameRate = track.get<float>("frameRate", 0.0f);
    bitRate = static_cast<int>(track.get<float>("bitRate", 0.0f));
    pid = track.get<int>("pid", 0);
    ctag = track.get<int>("ctag", 0);
    codec = track.get<std::string>("codec", "(null)");
    trackId = track.get<int>("trackId", 0);
    role = track.get<std::string>("role", "(null)");
    adaptationSetId = track.get<int>("adaptationSetId", 0);
    orgCodec = track.get<std::string>("orgCodec", "(null)");
    selected = track.get<bool>("selected", false);
    periodStart = track.get<int>("periodStart", 0);
    representationId = track.get<std::string>("representationId", "(null)");
    ratings = ratingsJson(track);
    selectionPriority = track.get<int>("selectionPriority", 0);
    return true;
}
/* @0x737d8: internal audio includes channels/audioType/bitRate/periodStart even
 * when zero; MIME is not exported. Ratings remains a string-valued field. */
Ptree AudioTrack::makeAudioTrackPtree() const
{
    using namespace smp::util;
    Ptree tree;
    putString(&tree, "profile", profile);
    putString(&tree, "level", level);
    putFloat(&tree, "sampleRate", sampleRate);
    tree.put("channels", channels);
    putString(&tree, "language", language);
    putInt(&tree, "pid", pid);
    putInt(&tree, "ctag", ctag);
    putBool(&tree, "audioDescription", audioDescription);
    tree.put("audioType", audioType);
    putString(&tree, "codec", codec);
    putInt(&tree, "trackId", trackId);
    tree.put("bitRate", bitRate);
    putString(&tree, "role", role);
    putInt(&tree, "adaptationSetId", adaptationSetId);
    putString(&tree, "immersive", immersive);
    putBool(&tree, "selected", selected);
    tree.put("periodStart", periodStart);
    putString(&tree, "representationId", representationId);
    putString(&tree, "preselectionId", preselectionId);
    putString(&tree, "ac4Text", ac4Text);
    putString(&tree, "ratings", ratings);
    putInt(&tree, "selectionPriority", selectionPriority);
    putBool(&tree, "audioEmergency", audioEmergency);
    return tree;
}
/* @0x73544: internal video always emits angleNumber/periodStart, otherwise
 * uses presence filtering. The reserved +0x80 string is not exported. */
Ptree VideoTrack::makeVideoTrackPtree() const
{
    using namespace smp::util;
    Ptree tree;
    tree.put("angleNumber", angleNumber);
    putInt(&tree, "bitRate", bitRate);
    putInt(&tree, "ctag", ctag);
    putInt(&tree, "pid", pid);
    putString(&tree, "codec", codec);
    putInt(&tree, "trackId", trackId);
    putInt(&tree, "width", width);
    putInt(&tree, "height", height);
    putFloat(&tree, "frameRate", frameRate);
    putString(&tree, "aspectRatio", aspectRatio);
    putString(&tree, "level", level);
    putString(&tree, "profile", profile);
    putString(&tree, "role", role);
    putInt(&tree, "adaptationSetId", adaptationSetId);
    putBool(&tree, "progressive", progressive);
    putString(&tree, "orgCodec", orgCodec);
    putBool(&tree, "selected", selected);
    tree.put("periodStart", periodStart);
    putString(&tree, "representationId", representationId);
    putString(&tree, "ratings", ratings);
    putInt(&tree, "selectionPriority", selectionPriority);
    return tree;
}
} // smp_internal
