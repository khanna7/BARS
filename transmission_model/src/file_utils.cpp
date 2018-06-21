/*
 * file_utils.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: nick
 */

#include <sstream>

#include "boost/filesystem.hpp"

#include "file_utils.h"

namespace fs = boost::filesystem;

namespace TransModel {

std::string unique_file_name(const std::string& fname) {

    fs::path filepath(fname);
    int i = 1;
    std::string stem = filepath.stem().string();
    while (fs::exists(filepath)) {    // This will increment i until it hits a unique name
        i++;
        std::stringstream ss;
        ss << stem << "_" << i << filepath.extension().string();
        fs::path newName(filepath.parent_path() / ss.str());
        filepath = newName;
    }
    return filepath.string();
}

}
