/*
 * CD4ViralTests.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "PersonCreator.h"
#include "RInstance.h"
#include "TransmissionRunner.h"

using namespace TransModel;
using namespace Rcpp;

TEST(CreatorTests, TestInfectedPersonCreationNoART) {
	std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["n0"]);
	List val = as<List>(rnet["val"]);
	List p_list = as<List>(val[28]);
	// make sure I have the right one
	ASSERT_NEAR(51.174, as<double>(p_list["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner, 0.5, 1);
	PersonPtr person = creator(p_list, 1);
	ASSERT_FALSE(person->isCircumcised());
	ASSERT_TRUE(person->isInfected());
	ASSERT_EQ(0, person->role());
	ASSERT_NEAR(51.174f, person->age(), 0.00001);

	InfectionParameters params = person->infectionParameters();
	ASSERT_FALSE(params.art_covered);
	ASSERT_FALSE(params.art_status);
	ASSERT_FLOAT_EQ(2968, params.time_since_infection);
	ASSERT_FLOAT_EQ(-2968, params.time_of_infection);
	ASSERT_FLOAT_EQ(51.174, params.age_at_infection);
	ASSERT_FLOAT_EQ(68.82225, params.cd4_count);
	ASSERT_TRUE(isnan(params.time_since_art_init));
	ASSERT_TRUE(isnan(params.time_of_art_init));
	ASSERT_FLOAT_EQ(4.526072f, params.viral_load);
	ASSERT_TRUE(isnan(params.vl_art_traj_slope));
	ASSERT_TRUE(isnan(params.vl_at_art_init));
	ASSERT_TRUE(isnan(params.cd4_at_art_init));
}

TEST(CreatorTests, TestUninfectedPersonCreation) {
	std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["n0"]);
	List val = as<List>(rnet["val"]);
	List p_list = as<List>(val[0]);
	// make sure I have the right one
	ASSERT_NEAR(39.38478, as<double>(p_list["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner, 0.5, 1);
	PersonPtr person = creator(p_list, 1);
	ASSERT_TRUE(person->isCircumcised());
	ASSERT_FALSE(person->isInfected());
	ASSERT_EQ(1, person->role());
	ASSERT_NEAR(39.38478f, person->age(), 0.00001);

	InfectionParameters params = person->infectionParameters();
	ASSERT_FALSE(params.art_covered);
	ASSERT_FALSE(params.art_status);
	ASSERT_TRUE(isnan(params.time_since_infection));
	ASSERT_TRUE(isnan(params.time_of_infection));
	ASSERT_TRUE(isnan(params.age_at_infection));
	ASSERT_FLOAT_EQ(518.0f, params.cd4_count);
	ASSERT_TRUE(isnan(params.time_since_infection));
	ASSERT_TRUE(isnan(params.time_since_art_init));
	ASSERT_TRUE(isnan(params.time_of_art_init));
	ASSERT_FLOAT_EQ(0, params.viral_load);
	ASSERT_TRUE(isnan(params.vl_art_traj_slope));
	ASSERT_TRUE(isnan(params.vl_at_art_init));
	ASSERT_TRUE(isnan(params.cd4_at_art_init));
}

TEST(CreatorTests, TestInfectedPersonCreationART) {
	std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["n0"]);
	List val = as<List>(rnet["val"]);
	List p_list = as<List>(val[12]);
	// make sure I have the right one
	ASSERT_NEAR(43.70214, as<double>(p_list["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner, 0.5, 1);
	PersonPtr person = creator(p_list, 1);
	ASSERT_FALSE(person->isCircumcised());
	ASSERT_TRUE(person->isInfected());
	ASSERT_EQ(1, person->role());
	ASSERT_NEAR(43.70214f, person->age(), 0.00001);

	InfectionParameters params = person->infectionParameters();
	ASSERT_TRUE(params.art_covered);
	ASSERT_TRUE(params.art_status);
	ASSERT_FLOAT_EQ(2143, params.time_since_infection);
	ASSERT_FLOAT_EQ(-2143, params.time_of_infection);
	ASSERT_FLOAT_EQ(43.70214f, params.age_at_infection);
	ASSERT_FLOAT_EQ(196.6126f, params.cd4_count);
	ASSERT_EQ(0, params.time_since_art_init);
	ASSERT_EQ(0, params.time_of_art_init);
	ASSERT_FLOAT_EQ(4.279501f, params.viral_load);
	ASSERT_NEAR(0.02150442f, params.vl_art_traj_slope, 0.000001);
	ASSERT_FLOAT_EQ(4.279501f, params.vl_at_art_init);
	ASSERT_FLOAT_EQ(0, params.cd4_at_art_init);
}

TEST(CreatorTests, TestCreatorFromSavedNet) {
	std::string cmd = "nw.test <- readRDS(file=\"../test_data/network.RDS\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["nw.test"]);
	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner, 0.5, 1);
	List val = as<List>(rnet["val"]);

	// will throw exception if there's an issue
	for (auto& sexp : val) {
		List v = as<List>(sexp);
		creator(v, 1);
	}
}

TEST(CreatorTests, TestDiagnosis) {
	std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["n0"]);
	List val = as<List>(rnet["val"]);
	List p_list = as<List>(val[0]);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);

	// detection window = 10
	PersonCreator creator(runner, 0.5, 10);
	double tick = 2;
	PersonPtr person = creator(p_list, tick);

	float next_test_at = tick + as<double>(p_list["time.until.next.test"]);
	ASSERT_EQ(528 + tick, next_test_at);
	person->infect(true, 50, tick);
	// person should diagnose true at next_text_at, detection window is 10 which is long
	// before next_test_at
	ASSERT_FALSE(person->diagnose(100));
	ASSERT_FALSE(person->diagnose(next_test_at - 1));
	ASSERT_TRUE(person->diagnose(next_test_at));

	p_list["time.until.next.test"] = 5;
	PersonPtr p2 = creator(p_list, tick);
	p2->infect(true, 50, tick);
	ASSERT_EQ(4, p2->timeUntilNextTest(3));
	// test should be false at next_test_at (7)
	// because detection window not yet reached
	ASSERT_FALSE(p2->diagnose(7));
	next_test_at = p2->timeUntilNextTest(7);
	// do the next test at the max of the next testing data
	// or the detection window to insure a positive test
	double test_at = std::max(7 + next_test_at, 12.0f);
	ASSERT_TRUE(p2->diagnose(test_at));
}
