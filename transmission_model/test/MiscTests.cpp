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

using namespace TransModel;
using namespace Rcpp;

TEST(ParametersTests, TestCreateFromR) {
	//RInstance::rptr
	repast::Properties props("../test_data/test.props");
	Parameters::initialize(props);
	add_from_R("../test_data/parameters.R", Parameters::instance(), RInstance::rptr);

	Parameters* params = Parameters::instance();
	ASSERT_EQ(1.11, params->getDoubleParameter(B3_FEMALE));
	ASSERT_EQ(1, params->getIntParameter(ACUTE_LENGTH_MIN));
	ASSERT_EQ(3.5f, params->getFloatParameter(STOP_AT));

	// make sure that R vectors of size > 1 are ignored
	ASSERT_FALSE(params->contains("vector.param"));
}

struct MockGen {

	double next() {
		return 3;
	}
};

TEST(DiagnoserTests, TestDiagnosis) {

	std::shared_ptr<MockGen> gen = std::make_shared<MockGen>();
	// window is 2, next_test at 5
	Diagnoser<MockGen> diagnoser(2, 5, 0, gen);

	ASSERT_EQ(5, diagnoser.timeUntilNextTest(0));
	ASSERT_EQ(0, diagnoser.testCount());

	InfectionParameters infection_params;
	infection_params.infection_status = false;
	for (int i = 1; i < 5; ++i) {
		ASSERT_FALSE(diagnoser.test(i, infection_params));
		ASSERT_EQ(0, diagnoser.testCount());
	}

	// next test at 5 so increase test count, but
	// diagnosis is false because status is false
	ASSERT_FALSE(diagnoser.test(5, infection_params));
	ASSERT_EQ(1, diagnoser.testCount());

	// next test should be at 8 (5 + 3).
	for (int i = 1; i < 3; ++i) {
		ASSERT_FALSE(diagnoser.test(5 + i, infection_params));
		ASSERT_EQ(1, diagnoser.testCount());
	}

	infection_params.infection_status = true;
	infection_params.time_of_infection = 8;
	// false because window not passed
	ASSERT_FALSE(diagnoser.test(8, infection_params));
	ASSERT_EQ(2, diagnoser.testCount());

	// window passed but not time for next test
	ASSERT_FALSE(diagnoser.test(10, infection_params));
	ASSERT_EQ(2, diagnoser.testCount());

	// time for next test and window has passed
	ASSERT_TRUE(diagnoser.test(11, infection_params));
	ASSERT_EQ(3, diagnoser.testCount());
}
