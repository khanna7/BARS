
#include <vector>
#include <string>

#include "boost/algorithm/string.hpp"

#include "Range.h"

namespace TransModel {

Range<double> parse_range(const std::string& range_string) {
    std::vector<std::string> tokens;
    boost::split(tokens, range_string, boost::is_any_of("-"));
    if (tokens.size() != 2) {
        throw std::invalid_argument("Bad range definition " + range_string);
    }

    boost::algorithm::trim(tokens[0]);
    boost::algorithm::trim(tokens[1]);
    try {
        float min = std::stod(tokens[0]);
        float max = std::stod(tokens[1]);
        return Range<double>(min, max);
    } catch (const std::invalid_argument& ex) {
        throw std::invalid_argument("Bad range definition " + range_string);
    }
}


}
