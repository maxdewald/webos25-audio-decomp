/* Options uses Boost 1.78 property_tree, not a JSON representation invented from
 * decompiler output. ARM layout: weak_ptr base +0, ptree +8, total 0x24 bytes.
 * Getter signatures and two-pass optional conversions checked at 0x6ef7c..0x6f96c.
 * The boolean/string/integer existence checks preserve the shipped short circuit.
 */
#include "options.h"
#include "libpf.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace mediapipeline { namespace utils {
namespace {
struct OptionsLog { PmLogContext ctx; char name[16]; };
OptionsLog options_log __asm__("libpf_options_log_192900");
}

#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(Options) == 0x24, "Options ARM ABI");
#endif

/* @0x6eec0 = _ZN13mediapipeline5utils7OptionsC1Ev */
Options::Options()
{
    memset(options_log.name, 0, sizeof(options_log.name));
    PmLogGetContext("playerfactory.default", &options_log.ctx);
}

/* @0x6ee68 = _ZN13mediapipeline5utils7OptionsD1Ev */
Options::~Options() {}

/* @0x6ef7c = _ZN13mediapipeline5utils7Options9getStringENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
std::string Options::getString(std::string key)
{
    return tree_.get<std::string>(key);
}

/* @0x6f014 = _ZN13mediapipeline5utils7Options9getStringENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPS7_ */
bool Options::getString(std::string key, std::string *value)
{
    if (!tree_.get_optional<std::string>(key))
        return false;
    *value = tree_.get<std::string>(key);
    return true;
}

/* @0x6f188 = _ZN13mediapipeline5utils7Options10getBooleanENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
bool Options::getBoolean(std::string key)
{
    return tree_.get<bool>(key);
}

/* @0x6f214 = _ZN13mediapipeline5utils7Options10getBooleanENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPb */
bool Options::getBoolean(std::string key, bool *value)
{
    if (!tree_.get_optional<bool>(key))
        return false;
    *value = tree_.get<bool>(key);
    return true;
}

/* @0x6f2e0 = _ZN13mediapipeline5utils7Options17checkKeyExistanceENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
bool Options::checkKeyExistance(std::string key)
{
    return bool(tree_.get_optional<bool>(key)) || bool(tree_.get_optional<std::string>(key)) ||
           bool(tree_.get_optional<int>(key));
}

/* @0x6f3dc = _ZN13mediapipeline5utils7Options9getDoubleENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPd */
bool Options::getDouble(std::string key, double *value)
{
    if (!tree_.get_optional<double>(key))
        return false;
    *value = tree_.get<double>(key);
    return true;
}

/* @0x6f54c = _ZN13mediapipeline5utils7Options9getGint64ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPx */
bool Options::getGint64(std::string key, int64_t *value)
{
    if (!tree_.get_optional<int64_t>(key))
        return false;
    *value = tree_.get<int64_t>(key);
    return true;
}

/* @0x6f6bc = _ZN13mediapipeline5utils7Options8getFloatENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPf */
bool Options::getFloat(std::string key, float *value)
{
    if (!tree_.get_optional<float>(key))
        return false;
    *value = tree_.get<float>(key);
    return true;
}

/* @0x6f828 = _ZN13mediapipeline5utils7Options6getIntENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
int Options::getInt(std::string key)
{
    return tree_.get<int>(key);
}

/* @0x6f8b4 = _ZN13mediapipeline5utils7Options6getIntENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPi */
bool Options::getInt(std::string key, int *value)
{
    if (!tree_.get_optional<int>(key))
        return false;
    *value = tree_.get<int>(key);
    return true;
}

/* @0x6f97c = _ZN13mediapipeline5utils7Options8loadJSONENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
bool Options::loadJSON(std::string json)
{
    try {
        std::stringstream json_stream(json);
        boost::property_tree::read_json(json_stream, tree_);
        for (const auto &option : tree_)
            PF_LOG_DEBUG(options_log, "options.cpp", "loadJSON", 0x3a, "%s %s",
                         option.first.c_str(), option.second.get_value<std::string>().c_str());
        return true;
    } catch (const boost::property_tree::json_parser_error &error) {
        PF_LOG_ERROR(options_log, "options.cpp", "loadJSON", 0x3f,
                     "Error parsing: [%s] on line: [%lu]", error.filename().c_str(), error.line());
        PF_LOG_ERROR(options_log, "options.cpp", "loadJSON", 0x41, "%s", error.message().c_str());
        return false;
    }
}

/* @0x6fd58 = _ZN13mediapipeline5utils7Options4loadENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
bool Options::load(std::string json)
{
    try {
        std::stringstream stream(json);
        boost::property_tree::read_json(stream, tree_);
        return true;
    } catch (const boost::property_tree::json_parser_error &error) {
        PF_LOG_ERROR(options_log, "options.cpp", "load", 0x28,
                     "Error parsing: [%s] on line: [%lu]", error.filename().c_str(), error.line());
        PF_LOG_ERROR(options_log, "options.cpp", "load", 0x2a, "%s", error.message().c_str());
        return false;
    }
}
}}
