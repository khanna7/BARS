/*
 * TestingConfigurator.h
 *
 *  Created on: Oct 11, 2017
 *      Author: nick
 */

#ifndef SRC_TESTINGCONFIGURATOR_H_
#define SRC_TESTINGCONFIGURATOR_H_

#include <memory>

#include "repast_hpc/Random.h"

#include "ProbDist.h"
#include "Person.h"

namespace TransModel {

class TestingDist {

private:
	int min_, max_;

public:
	TestingDist(int min, int max);
	double next(double size_of_timestep);
};


/**
 * Configures the testing parameters for a person.
 */
class TestingConfigurator {
private:
	ProbDist<TestingDist> lt_dist_, gte_dist_;
	float threshold;
	repast::NumberGenerator* lt_gen, *gte_gen;

public:
	TestingConfigurator(ProbDist<TestingDist>& lt_dist, ProbDist<TestingDist>& gte_dist,
			float age_threshold, float lt_non_tester_rate, float gte_non_tester_rate);
	virtual ~TestingConfigurator();

	void configurePerson(std::shared_ptr<Person> p, double size_of_timestep);
	void configurePerson(std::shared_ptr<Person> p, double size_of_timestep, double rnd_draw);
};

ProbDist<TestingDist> create_prob_dist(const std::string& key_prefix);

TestingConfigurator create_testing_configurator();

} /* namespace TransModel */

#endif /* SRC_TESTINGCONFIGURATOR_H_ */
