/*
 * TestingConfigurator.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: nick
 */

#include <vector>
#include <exception>

#include "boost/algorithm/string.hpp"
#include "Parameters.h"

#include "TestingConfigurator.h"

using namespace std;

namespace TransModel {

repast::NumberGenerator* create_gen(float rate) {
	BinomialGen coverage(repast::Random::instance()->engine(), boost::random::binomial_distribution<>(1, rate));
	return new repast::DefaultNumberGenerator<BinomialGen>(coverage);
}

TestingConfigurator::TestingConfigurator(ProbDist<TestingDist>& lt_dist, ProbDist<TestingDist>& gte_dist,
		float age_threshold, float lt_non_tester_rate, float gte_non_tester_rate) : lt_dist_{lt_dist}, gte_dist_{gte_dist},
				threshold{age_threshold}, lt_gen{create_gen(lt_non_tester_rate)}, gte_gen{create_gen(gte_non_tester_rate)}
{
}

TestingConfigurator::~TestingConfigurator() {}

void TestingConfigurator::configurePerson(std::shared_ptr<Person> p, double size_of_timestep) {
	configurePerson(p, size_of_timestep, repast::Random::instance()->nextDouble());
}

void TestingConfigurator::configurePerson(std::shared_ptr<Person> p, double size_of_timestep, double draw) {
	ProbDist<TestingDist>* dist = nullptr;
	repast::NumberGenerator* gen = nullptr;
	if (p->age() < threshold) {
		dist = &lt_dist_;
		gen = lt_gen;
	} else {
		dist = &gte_dist_;
		gen = gte_gen;
	}

	bool testable = ((int) gen->next()) == 0;
	double test_prob = 0;
	if (testable) {
		test_prob = dist->draw(draw).next(size_of_timestep);
	}
	p->updateDiagnoser(test_prob, testable);
}

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

ProbDist<TestingDist> create_prob_dist(const std::string& key_prefix) {
	ProbDistCreator<TestingDist> creator;
		vector<string> lag_keys;
		Parameters::instance()->getKeys(key_prefix, lag_keys);
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

TestingConfigurator create_testing_configurator() {
	ProbDist<TestingDist> lt_dist = create_prob_dist(TESTING_PROB_PREFIX_LT);
	ProbDist<TestingDist> gte_dist = create_prob_dist(TESTING_PROB_PREFIX_GTE);

	double lt_non_tester_rate = Parameters::instance()->getDoubleParameter(NON_TESTERS_PROP_LT);
	double gte_non_tester_rate = Parameters::instance()->getDoubleParameter(NON_TESTERS_PROP_GTE);
	float age_threshold = Parameters::instance()->getFloatParameter(AGE_THRESHOLD);

	return TestingConfigurator(lt_dist, gte_dist, age_threshold, lt_non_tester_rate, gte_non_tester_rate);
}

} /* namespace TransModel */
