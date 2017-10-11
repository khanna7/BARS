/*
 * CD4ViralTests.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include <random>

#include "gtest/gtest.h"

#include "repast_hpc/Random.h"

#include "Parameters.h"
#include "RInstance.h"
#include "utils.h"
#include "Diagnoser.h"
#include "StatsBuilder.h"
#include "DayRangeCalculator.h"
#include "RangeWithProbability.h"
#include "AdherenceCategory.h"
#include "GeometricDistribution.h"
#include "TestingConfigurator.h"
#include "Person.h"

using namespace TransModel;
using namespace Rcpp;

/*
 Sanity check that this is working as I thought it would.
 TEST(GeometricDistTests, GeometricDistTest) {
 repast::Random::initialize(1);
 GeometricDistribution dist(1 / 500.0, 0);
 double sum = 0;
 for (int i = 0; i < 2000; ++i) {
 sum += dist.next();
 }
 std::cout << (sum / 2000) << std::endl;

 }
 */

TEST(MiscTests, TestProbDist) {
	/*
	 * prep.prop.never.adherent <- 0.211
prep.prop.always.adherent <- 0.619
prep.prop.part.plus.adherent <- 0.10
prep.prop.part.neg.adherent <- 0.07
	 */
	double always = 0.619;
	double never = 0.211;
	double partial_plus = 0.10;
	double partial_minus = 0.07;

	//std::cout << "prep: " << always << "," << never << "," << partial_plus << ", " << partial_minus << std::endl;

	ProbDistCreator<AdherenceData> creator;
	creator.addItem(always, AdherenceData(0, AdherenceCategory::ALWAYS));
	creator.addItem(never, AdherenceData(0, AdherenceCategory::NEVER));
	creator.addItem(partial_plus, AdherenceData(0, AdherenceCategory::PARTIAL_PLUS));
	creator.addItem(partial_minus, AdherenceData(0, AdherenceCategory::PARTIAL_MINUS));

	std::map<AdherenceCategory, int> map;
	map.emplace(AdherenceCategory::ALWAYS, 0);
	map.emplace(AdherenceCategory::NEVER, 0);
	map.emplace(AdherenceCategory::PARTIAL_PLUS, 0);
	map.emplace(AdherenceCategory::PARTIAL_MINUS, 0);

	ProbDist<AdherenceData> dist = creator.createProbDist();

	for (int i = 0; i < 100000; ++i) {
		AdherenceData data = dist.draw(repast::Random::instance()->nextDouble());
		int val = map[data.category];
		map[data.category] = ++val;
	}

	int always_count = map[AdherenceCategory::ALWAYS];
	int never_count = map[AdherenceCategory::NEVER];
	int pp_count = map[AdherenceCategory::PARTIAL_PLUS];
	int pm_count = map[AdherenceCategory::PARTIAL_MINUS];

	double sum = always_count + never_count + pp_count + pm_count;
	std::cout << "always: " << always_count / sum << std::endl;
	std::cout << "never: " << never_count / sum << std::endl;
	std::cout << "pp: " << pp_count / sum << std::endl;
	std::cout << "pm: " << pm_count / sum << std::endl;
}

TEST(MiscTests, TestRangeWithProb) {
	RangeWithProbabilityCreator creator;
	creator.addBin(12, 15, 0.3);
	creator.addBin(16, 20, 0.8);
	creator.addBin(21, 30, 0.1);

	RangeWithProbability rp = creator.createRangeWithProbability();
	ASSERT_TRUE(rp.run(13, 0.2));
	ASSERT_FALSE(rp.run(12, 0.9));
	ASSERT_TRUE(rp.run(16, 0.2));
	ASSERT_FALSE(rp.run(19, 0.9));
	ASSERT_TRUE(rp.run(25, 0.05));
	ASSERT_FALSE(rp.run(29, 0.9));

	creator.clear();
	creator.addBin("asm.12_15", 0.3);
	creator.addBin("asm.16_20", 0.8);
	creator.addBin("asm.21_30", 0.1);

	rp = creator.createRangeWithProbability();
	ASSERT_TRUE(rp.run(13, 0.2));
	ASSERT_FALSE(rp.run(12, 0.9));
	ASSERT_TRUE(rp.run(16, 0.2));
	ASSERT_FALSE(rp.run(19, 0.9));
	ASSERT_TRUE(rp.run(25, 0.05));
	ASSERT_FALSE(rp.run(29, 0.9));
}

TEST(MiscTests, TestCreator) {
	DayRangeCalculatorCreator creator;

	try {
		creator.createCalculator();
		FAIL();
	} catch (...) {
	}

	creator.clear();
	try {
		creator.addBin(".16,1-7");
		creator.createCalculator();
		FAIL();
	} catch (...) {
	}

	creator.clear();
	creator.addBin(".16,1-7");
	creator.addBin(".34,7-30");
	creator.addBin(".5,5-5");
	creator.createCalculator();

}

void test_calc(std::shared_ptr<DayRangeCalculator> calc, double min, double max) {
	for (int i = 0; i < 1000; ++i) {
		double val = calc->calculateLag(1.0f);
		ASSERT_GE(val, min);
		ASSERT_LE(val, max);

		val = calc->calculateLag(2.0f);
		ASSERT_GE(val, min / 2.0);
		ASSERT_LE(val, max / 2.0);
	}
}

void test_calc(std::shared_ptr<DayRangeCalculator> calc, double min, double max, double min2, double max2) {
	int c1 = 0, c2 = 0;
	for (int i = 0; i < 1000; ++i) {
		double val = calc->calculateLag(1.0f);
		if (val <= max && val >= min) {
			++c1;
		} else if (val <= max2 && val >= min2) {
			++c2;
		}
	}

	ASSERT_EQ(1000, c1 + c2);
	ASSERT_GE(c1, 450);
	ASSERT_GE(c2, 450);
	ASSERT_LE(c1, 550);
	ASSERT_LE(c2, 550);
}

TEST(MiscTests, TestCreateFromR) {
	//RInstance::rptr
	repast::Properties props("../test_data/test.props");
	Parameters::initialize(props);
	init_parameters("../test_data/parameters.R", "../test_data/params_derived.R", "", Parameters::instance(),
			RInstance::rptr);

	Parameters* params = Parameters::instance();
	ASSERT_EQ(1.11, params->getDoubleParameter(B3_FEMALE));
	ASSERT_EQ(1, params->getIntParameter(ACUTE_LENGTH_MIN));
	ASSERT_EQ(3.5f, params->getFloatParameter(STOP_AT));
	ASSERT_NEAR(1 / 180.0, params->getDoubleParameter("prep.daily.stop.prob"), 0.001);

	// make sure that R vectors of size > 1 are ignored
	ASSERT_FALSE(params->contains("vector.param"));

	RInstance::rptr->parseEvalQ("rm(list=ls())");
	// prep.mean.days.usage so now 1/90
	std::string param_string("prep.mean.days.usage=90");
	init_parameters("../test_data/parameters.R", "../test_data/params_derived.R", param_string, Parameters::instance(),
			RInstance::rptr);
	ASSERT_NEAR(1 / 90.0, params->getDoubleParameter("prep.daily.stop.prob"), 0.001);
}

TEST(MiscTests, TestParameterParsing) {
	std::string params("run=1,num.foo=3.5,x.y.z=4.2");
	repast::Properties props("../test_data/test.props");
	Parameters::initialize(props);
	TransModel::param_string_to_R_vars(params, Parameters::instance(), RInstance::rptr);
	ASSERT_EQ(1, as<int>((*RInstance::rptr)["run"]));
	ASSERT_EQ(3.5, as<double>((*RInstance::rptr)["num.foo"]));
	ASSERT_EQ(4.2, as<double>((*RInstance::rptr)["x.y.z"]));
}

TEST(MiscTests, TestDiagnosis) {
	StatsBuilder builder("/dev");
	builder.countsWriter("null");
	builder.partnershipEventWriter("null");
	builder.infectionEventWriter("null");
	builder.biomarkerWriter("null");
	builder.deathEventWriter("null");
	builder.personDataRecorder("null");
	builder.testingEventWriter("null");
	builder.prepEventWriter("null");
	builder.artEventWriter("null");
	builder.createStatsSingleton(0);

	// window is 5, test count 0, prob of test is 0
	Diagnoser diagnoser(5, 0, 0);

	ASSERT_EQ(0, diagnoser.testCount());
	ASSERT_EQ(-1, diagnoser.lastTestAt());

	InfectionParameters infection_params;
	infection_params.infection_status = false;
	for (int i = 1; i < 100; ++i) {
		ASSERT_TRUE(diagnoser.test(i, infection_params) == Result::NO_TEST);
		ASSERT_EQ(0, diagnoser.testCount());
	}

	diagnoser = Diagnoser(5, 0, 1);

	// next test at 5 so increase test count, but
	// diagnosis is false because status is false
	ASSERT_TRUE(diagnoser.test(5, infection_params) == Result::NEGATIVE);
	ASSERT_EQ(5, diagnoser.lastTestAt());
	ASSERT_EQ(1, diagnoser.testCount());

	infection_params.infection_status = true;
	infection_params.time_of_infection = 8;
	// false because window not passed
	ASSERT_TRUE(diagnoser.test(8, infection_params) == Result::NEGATIVE);
	ASSERT_EQ(8, diagnoser.lastTestAt());
	ASSERT_EQ(2, diagnoser.testCount());

	// time for next test and window has passed
	ASSERT_EQ(Result::POSITIVE, diagnoser.test(13, infection_params));
	ASSERT_EQ(3, diagnoser.testCount());
}

TEST(MiscTests, testTestingFunc) {
	repast::Properties props("../test_data/testing_ranges.props");
	Parameters::initialize(props);
	// testing.prob.1 = 1-3,0.4
	// testing.prob.2 = 4-6,0.3
	// testing.prob.3 = 7-8,0.3
	ProbDist<TestingDist> dist = create_prob_dist("testing.prob.lt");
	std::default_random_engine generator;

	// 730 -- days in 2 years
	double exp_min = 1 / 730.0;
	double exp_max = 3 / 730.0;
	std::uniform_real_distribution<double> r(0.0,0.4);
	for (int i = 0; i < 1000; ++i) {
		double val = dist.draw(r(generator)).next(1);
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
	}

	exp_min = 4 / 730.0;
	exp_max = 6 / 730.0;
	std::uniform_real_distribution<double> r1(0.4,0.7);
	for (int i = 0; i < 1000; ++i) {
		double val = dist.draw(r1(generator)).next(1);
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
	}

	exp_min = 7 / 730.0;
	exp_max = 8 / 730.0;
	std::uniform_real_distribution<double> r2(0.7,1.0);
	for (int i = 0; i < 1000; ++i) {
		double val = dist.draw(r2(generator)).next(1);
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
	}

	Diagnoser diagnoser(0, 0);
	PersonPtr person = std::make_shared<Person>(1, 27, true, 1, 1, diagnoser);
	TestingConfigurator config = create_testing_configurator();

	exp_min = 1 / 730.0;
	exp_max = 3 / 730.0;
	std::uniform_real_distribution<double> g(0.0,0.2);
	for (int i = 0; i < 1000; ++i) {
		double draw = g(generator);
		config.configurePerson(person, 1, draw);
		double val = person->diagnoser().testingProbability();
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
		ASSERT_TRUE(!person->isTestable());
	}

	exp_min = 4 / 730.0;
	exp_max = 6 / 730.0;
	std::uniform_real_distribution<double> g1(0.2, 0.3);
	for (int i = 0; i < 1000; ++i) {
		double draw = g1(generator);
		config.configurePerson(person, 1, draw);
		double val = person->diagnoser().testingProbability();
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
		ASSERT_TRUE(!person->isTestable());
	}

	exp_min = 7 / 730.0;
	exp_max = 8 / 730.0;
	std::uniform_real_distribution<double> g2(0.3, 1.0);
	for (int i = 0; i < 1000; ++i) {
		double draw = g2(generator);
		config.configurePerson(person, 1, draw);
		double val = person->diagnoser().testingProbability();
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
		ASSERT_TRUE(!person->isTestable());
	}

	exp_min = 1 / 730.0;
	exp_max = 3 / 730.0;
	person->setAge(25);
	std::uniform_real_distribution<double> g3(0.0, 0.4);
	for (int i = 0; i < 1000; ++i) {
		double draw = g3(generator);
		config.configurePerson(person, 1, draw);
		double val = person->diagnoser().testingProbability();
		ASSERT_GE(val, exp_min);
		ASSERT_LE(val, exp_max);
		ASSERT_TRUE(person->isTestable());
	}
}
