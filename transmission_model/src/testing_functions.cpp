/*
 * testing_functions.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: nick
 */

#include <vector>
#include <exception>

#include "boost/algorithm/string.hpp"

#include "repast_hpc/Random.h"

#include "testing_functions.h"
#include "Parameters.h"

using namespace std;

namespace TransModel {

const double TWO_YEARS = 730;

TestingDist::TestingDist(int min, int max) : min_(min), max_(max) {

}

double TestingDist::next(double size_of_timestep) {
	double val = repast::Random::instance()->createUniIntGenerator(min_, max_).next();
	return val / TWO_YEARS * size_of_timestep;
}

// range is min-max of tests within the last two years
TestingDist create_dist(std::string& range) {
	vector<string> tokens;
	boost::split(tokens, range, boost::is_any_of("-"));

	if (tokens.size() != 2) {
		throw std::invalid_argument("Bad testing.prob definition: " + range);
	}

	try {
		int min = stoi(tokens[0]);
		int max = stoi(tokens[1]);
		return TestingDist(min, max);
	} catch (invalid_argument& e) {
		throw std::invalid_argument("Bad testing.prob definition: " + range);
	}
}

ProbDist<TestingDist> create_testing_dist() {
	ProbDistCreator<TestingDist> creator;
	vector<string> lag_keys;
	Parameters::instance()->getKeys(TESTING_PROB_PREFIX, lag_keys);
	for (auto& key : lag_keys) {
		string val = Parameters::instance()->getStringParameter(key);
		vector<string> tokens;
		boost::split(tokens, val, boost::is_any_of(","));

		if (tokens.size() != 2) {
			throw std::invalid_argument("Bad testing.prob definition: " + val);
		}

		TestingDist dist = create_dist(tokens[0]);
		double frac = stod(tokens[1]);
		creator.addItem(frac, dist);
	}
	return creator.createProbDist();
}

}


