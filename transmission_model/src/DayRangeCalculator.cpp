/*
 * ARTInitLagCalculator.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#include <exception>

#include "boost/tokenizer.hpp"

#include "DayRangeCalculator.h"

namespace TransModel {

using namespace std;

const double EPSILON = .00001;

void tokenize(const string& str, const string& separator, vector<string>& tokens) {
	boost::char_separator<char> sep{separator.c_str()};
	boost::tokenizer<boost::char_separator<char>> tok(str, sep);
	for (const auto& t : tok) {
		tokens.push_back(t);
	}
}


DayRangeBin::DayRangeBin(double prob, double min, double max) : prob_(prob), gen(repast::Random::instance()->createUniIntGenerator(min, max)) {}
DayRangeBin::~DayRangeBin() {}

double DayRangeBin::calculateLag(float size_of_timestep) {
	return gen.next() / size_of_timestep;
}


DayRangeCalculatorCreator::DayRangeCalculatorCreator() : bins{} {}

DayRangeCalculatorCreator::~DayRangeCalculatorCreator() {

}

void DayRangeCalculatorCreator::addBin(const std::string& bin_definition) {
	vector<string> tokens;
	tokenize(bin_definition, ",", tokens);

	if (tokens.size() != 2) {
		throw std::invalid_argument("Bad bin definition: " + bin_definition);
	}

	double prob = stod(tokens[0]);

	tokens.clear();
	tokenize(tokens[1], "-", tokens);
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
		bins.push_back(DayRangeBin(prob, min, max));
	}
}

std::shared_ptr<DayRangeCalculator> DayRangeCalculatorCreator::createCalculator() {
	double sum = 0;
	for (auto& bin : bins) {
		sum += bin.probability();
		bin.setProbability(sum);
	}

	if (sum < 1.0 - EPSILON || sum > 1.0 + EPSILON) {
		std::cout << sum << std::endl;
		throw std::domain_error("Invalid value used to initialize DayRangeCalculator. Sum of values must equal 1.");
	}
	bins[bins.size() -1].setProbability(1.0);

	return shared_ptr<DayRangeCalculator>(new DayRangeCalculator(bins));
}

DayRangeCalculator::DayRangeCalculator(std::vector<DayRangeBin>& bins) : bins_(bins) {

}

DayRangeCalculator::~DayRangeCalculator() {}

double DayRangeCalculator::calculateLag(float size_of_timestep) {
	double prob = repast::Random::instance()->nextDouble();
	for (auto& bin : bins_) {
		if (bin.probability() <= prob) {
			return bin.calculateLag(size_of_timestep);
		}
	}
	return bins_[bins_.size() - 1].calculateLag(size_of_timestep);
}


} /* namespace TransModel */
