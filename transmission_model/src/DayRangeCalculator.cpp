/*
 * DayRangeCalculator.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#include <exception>

#include "boost/algorithm/string.hpp"

#include "DayRangeCalculator.h"
#include "utils.h"

namespace TransModel {

using namespace std;

// increment of 1, so dist never produces 0
DayRangeBin::DayRangeBin(double min, double max) : gen(1 / (min + (max - min) / 2), 1) {}
DayRangeBin::~DayRangeBin() {}

double DayRangeBin::calculateLag(float size_of_timestep) {

	return gen.next() / size_of_timestep;
}


DayRangeCalculatorCreator::DayRangeCalculatorCreator() : pd_creator{} {}

DayRangeCalculatorCreator::~DayRangeCalculatorCreator() {

}

void DayRangeCalculatorCreator::addBin(const std::string& bin_definition) {
	vector<string> tokens;
	boost::split(tokens, bin_definition, boost::is_any_of(":"));

	if (tokens.size() != 2) {
		throw std::invalid_argument("Bad bin definition: " + bin_definition);
	}

	double prob = stod(tokens[0]);
	std::string range = tokens[1];

	//std::cout << tokens[0] << ", " << tokens[1] << std::endl;

	tokens.clear();
	boost::split(tokens, range, boost::is_any_of("-"));
	double min = 0, max = 0;
	if (tokens.size() == 1) {
		min = stod(tokens[0]);
		max = min;
	} else if (tokens.size() == 2) {
		min = stod(tokens[0]);
		max = stod(tokens[1]);
	} else {
		throw std::invalid_argument("Bad bin definition: " + bin_definition);
	}

	if (min < 0 || max < 0 || max < min) {
		throw std::invalid_argument("Bad bin definition: " + bin_definition + ", probability, min and max must be > 0 and max must be > min.");
	}

	// std::cout << "adding bin " << prob << ", " << min << " - " << max << std::endl;

	if (prob != 0) {
		pd_creator.addItem(prob, make_shared<DayRangeBin>(min, max));
	}
}

std::shared_ptr<DayRangeCalculator> DayRangeCalculatorCreator::createCalculator() {
	return shared_ptr<DayRangeCalculator>(new DayRangeCalculator(pd_creator.createProbDist()));
}

DayRangeCalculator::DayRangeCalculator(ProbDist<std::shared_ptr<DayRangeBin>> prob_dist) : dist(prob_dist) {

}

DayRangeCalculator::~DayRangeCalculator() {}

double DayRangeCalculator::calculateLag(float size_of_timestep) {
	double prob = repast::Random::instance()->nextDouble();
	return dist.draw(prob)->calculateLag(size_of_timestep);
}


} /* namespace TransModel */
