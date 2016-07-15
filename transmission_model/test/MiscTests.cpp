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

using namespace TransModel;
using namespace Rcpp;

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
	ASSERT_EQ(40.5f, params->getDoubleParameter("mid.age"));

	// make sure that R vectors of size > 1 are ignored
	ASSERT_FALSE(params->contains("vector.param"));

	RInstance::rptr->parseEvalQ("rm(list=ls())");
	// override min and max age, so that derived mid.age should now be 6
	std::string param_string("min.age=2,max.age=10");
	init_parameters("../test_data/parameters.R", "../test_data/params_derived.R", param_string, Parameters::instance(),
			RInstance::rptr);
	ASSERT_EQ(6.0, params->getDoubleParameter("mid.age"));
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
