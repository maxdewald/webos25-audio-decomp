// SPDX-License-Identifier: Apache-2.0
/* Copyright (c) 2008-2018 LG Electronics, Inc.
 * Source-assisted from pinned calculator/video_resource_table.cpp (Apache-2.0).
 * TV "max" marker and permissive unknown-token handling verified at 0x1661c;
 * the marker string is at 0x17ea0. */
#include "calculator.h"
#include <cstdlib>
#include <sstream>

namespace mrc {
/* @0x1661c: TV tolerates unknown tokens; "max" marks an unconditional row. */
VideoInfo::VideoInfo(std::string videoInfoString)
    : width(0), height(0), frameRate(0), isDefault(false)
{
    std::string token;
    std::istringstream input(videoInfoString);
    while (std::getline(input, token, ' ')) {
        std::string::size_type delimiter;
        if ((delimiter = token.find('*')) != std::string::npos) {
            width = std::atoi(token.substr(0, delimiter).c_str());
            height = std::atoi(token.substr(delimiter + 1).c_str());
        } else if (token.find("2k") != std::string::npos || token.find("2K") != std::string::npos) {
            width = 2048;
            height = 1080;
        } else if (token.find("4k") != std::string::npos || token.find("4K") != std::string::npos) {
            width = 4096;
            height = 2160;
        } else if ((delimiter = token.find('p')) != std::string::npos ||
                   (delimiter = token.find('P')) != std::string::npos) {
            frameRate = std::atoi(token.substr(0, delimiter).c_str());
        } else if ((delimiter = token.find('i')) != std::string::npos ||
                   (delimiter = token.find('I')) != std::string::npos) {
            frameRate = std::atoi(token.substr(0, delimiter).c_str()) / 2;
        } else if (token.find("max") != std::string::npos) {
            isDefault = true;
        }
    }
}

}
