#ifndef LIBPF_OPTIONS_H
#define LIBPF_OPTIONS_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <string>

namespace mediapipeline { namespace utils {
class Options : public boost::enable_shared_from_this<Options> {
public:
    Options();
    ~Options();
    std::string getString(std::string key);
    bool getString(std::string key, std::string *value);
    bool getBoolean(std::string key);
    bool getBoolean(std::string key, bool *value);
    bool checkKeyExistance(std::string key);
    bool getDouble(std::string key, double *value);
    bool getGint64(std::string key, int64_t *value);
    bool getFloat(std::string key, float *value);
    int getInt(std::string key);
    bool getInt(std::string key, int *value);
    bool loadJSON(std::string json);
    bool load(std::string json);

private:
    boost::property_tree::ptree tree_;
};
}}
#endif
