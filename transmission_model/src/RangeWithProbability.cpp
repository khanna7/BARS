/*
 * RangeWithProbability.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: nick
 */

#include <exception>

#include "boost/algorithm/string.hpp"

#include "RangeWithProbability.h"

namespace TransModel {

using namespace std;

RangeWithProbability::RangeWithProbability(std::vector<RangeBin> range_bins) :
        bins { range_bins } {
}

RangeWithProbability::~RangeWithProbability() {
}

bool RangeWithProbability::run(float rangeValue, double draw) {
    return draw <= lookup(rangeValue);
}

bool RangeWithProbability::run(float rangeValue, double increase, double draw) {
    double prob = lookup(rangeValue);
    return draw <= (prob + (prob * increase));
}

double RangeWithProbability::lookup(float rangeValue) {
    for (auto& bin : bins) {
        if (bin.min <= rangeValue && bin.max > rangeValue) {
            return bin.prob;
        }
    }
    throw std::domain_error("Error in RangeWithProbabilty::run: rangeValue " + std::to_string(rangeValue) + " is not within any bin range");
}


RangeWithProbabilityCreator::RangeWithProbabilityCreator() {
}
RangeWithProbabilityCreator::~RangeWithProbabilityCreator() {
}

void RangeWithProbabilityCreator::parseRangeString(const std::string &bin_definition, double *min, double *max) {
    std::vector<string> tokens;
    boost::split(tokens, bin_definition, boost::is_any_of("."));

    if (tokens.size() != 2) {
        throw std::invalid_argument("Bad bin definition in RangeWithProbability: " + bin_definition);
    }

    std::string range = tokens[1];
    tokens.clear();
    boost::split(tokens, range, boost::is_any_of("_"));
    if (tokens.size() == 1) {
        *min = stod(tokens[0]);
        *max = *min;
    } else if (tokens.size() == 2) {
        *min = stod(tokens[0]);
        *max = stod(tokens[1]);
    } else {
        throw std::invalid_argument("Bad bin definition in RangeWithProbability: " + bin_definition);
    }

    if (*min < 0 || *max < 0 || *max < *min) {
        throw std::invalid_argument(
                "Bad bin definition in RangeWithProbability: " + bin_definition
                        + ",  min and max must be > 0 and max must be > min.");
    }
    return;
}

void RangeWithProbabilityCreator::addBin(const std::string& bin_definition, double bin_probability) {
// asm.15-19
    double min = 0.0, max = 0.0;
    parseRangeString(bin_definition, &min, &max);
    addBin(min, max, bin_probability);
}

std::ostream& operator<< (std::ostream& os, const RangeWithProbability& rp) {
    os << "RangeWithProbability[";
    bool not_first = false;
    for (auto& bin : rp.bins) {
        if (not_first) {
            os << ",";
        }
        os << bin.min << "-" << bin.max << ": " << bin.prob;
        not_first = true;
    }
    os << "]";
    return os;
}

void RangeWithProbabilityCreator::addBin(float min, float max, double prob) {
    bins.push_back( { min, max, prob });
}

RangeWithProbability RangeWithProbabilityCreator::createRangeWithProbability() {
    if (bins.size() == 0) {
        throw std::domain_error("Cannot create RangeWithProbability without any bins.");
    }

    return RangeWithProbability(bins);
}

} /* namespace TransModel */
