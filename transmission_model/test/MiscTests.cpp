/*
 * CD4ViralTests.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "repast_hpc/Random.h"

#include "Parameters.h"
#include "RInstance.h"
#include "utils.h"
#include "Diagnoser.h"
#include "StatsBuilder.h"
#include "DayRangeCalculator.h"

using namespace TransModel;
using namespace Rcpp;

TEST(DayRangeCalcTests, TestCreator) {
	DayRangeCalculatorCreator creator;

	try {
		creator.createCalculator();
		FAIL();
	} catch (...) {}

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

TEST(DayRangeCalcTests, TestLagCalculator) {
	repast::Random::initialize(1);
	DayRangeCalculatorCreator creator;

	creator.addBin("1,1-7");
	creator.addBin("0,7-30");
	creator.addBin("0,5-5");
	std::shared_ptr<DayRangeCalculator> calc = creator.createCalculator();
	test_calc(calc, 1, 7);
	creator.clear();

	creator.addBin("0,1-7");
	creator.addBin("1,7-30");
	creator.addBin("0,5-5");
	calc = creator.createCalculator();
	test_calc(calc, 7, 30);
	creator.clear();

	creator.addBin("0,1-7");
	creator.addBin("0,7-30");
	creator.addBin("1,5-5");
	calc = creator.createCalculator();
	test_calc(calc, 5, 5);
	creator.clear();

	creator.addBin("0,1-7");
	creator.addBin("0.5,7-30");
	creator.addBin("0.5,5-5");
	calc = creator.createCalculator();
	test_calc(calc, 5, 5, 7, 30);
}

TEST(ParametersTests, TestCreateFromR) {
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

TEST(ParametersTests, TestParameterParsing) {
	std::string params("run=1,num.foo=3.5,x.y.z=4.2");
	repast::Properties props("../test_data/test.props");
	Parameters::initialize(props);
	TransModel::param_string_to_R_vars(params, Parameters::instance(), RInstance::rptr);
	ASSERT_EQ(1, as<int>((*RInstance::rptr)["run"]));
	ASSERT_EQ(3.5, as<double>((*RInstance::rptr)["num.foo"]));
	ASSERT_EQ(4.2, as<double>((*RInstance::rptr)["x.y.z"]));
}

struct MockGen {

	double next() {
		return 3;
	}
};

TEST(DiagnoserTests, TestDiagnosis) {
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
	builder.createStatsSingleton();

	std::shared_ptr<MockGen> gen = std::make_shared<MockGen>();
	// window is 2, next_test at 5
	Diagnoser<MockGen> diagnoser(2, 5, 0, gen);

	ASSERT_EQ(5, diagnoser.timeUntilNextTest(0));
	ASSERT_EQ(0, diagnoser.testCount());
	ASSERT_EQ(-1, diagnoser.lastTestAt());

	InfectionParameters infection_params;
	infection_params.infection_status = false;
	for (int i = 1; i < 5; ++i) {
		ASSERT_TRUE(diagnoser.test(i, infection_params) == Result::NO_TEST);
		ASSERT_EQ(0, diagnoser.testCount());
	}

	// next test at 5 so increase test count, but
	// diagnosis is false because status is false
	ASSERT_TRUE(diagnoser.test(5, infection_params) == Result::NEGATIVE);
	ASSERT_EQ(5, diagnoser.lastTestAt());
	ASSERT_EQ(1, diagnoser.testCount());

	// next test should be at 8 (5 + 3).
	for (int i = 1; i < 3; ++i) {
		ASSERT_TRUE(diagnoser.test(5 + i, infection_params) == Result::NO_TEST);
		ASSERT_EQ(1, diagnoser.testCount());
	}

	infection_params.infection_status = true;
	infection_params.time_of_infection = 8;
	// false because window not passed
	ASSERT_TRUE(diagnoser.test(8, infection_params) == Result::NEGATIVE);
	ASSERT_EQ(8, diagnoser.lastTestAt());
	ASSERT_EQ(2, diagnoser.testCount());

	// window passed but not time for next test
	ASSERT_EQ(Result::NO_TEST, diagnoser.test(10, infection_params));
	ASSERT_EQ(2, diagnoser.testCount());

	// time for next test and window has passed
	ASSERT_EQ(Result::POSITIVE, diagnoser.test(11, infection_params));
	ASSERT_EQ(3, diagnoser.testCount());
}
