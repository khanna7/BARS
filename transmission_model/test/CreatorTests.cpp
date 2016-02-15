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
	List val11 = as<List>(val[11]);
	// make sure I have the right one
	ASSERT_NEAR(21.81211, as<double>(val11["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner);
	PersonPtr person = creator(val11);
	ASSERT_TRUE(person->isCircumcised());
	ASSERT_TRUE(person->isInfected());
	ASSERT_NEAR(21.81211f, person->age(), 0.00001);

	InfectionParameters params = person->infectionParameters();
	ASSERT_FALSE(params.art_covered);
	ASSERT_FALSE(params.art_status);
	ASSERT_FLOAT_EQ(1848, params.time_since_infection);
	ASSERT_FLOAT_EQ(-1848, params.time_of_infection);
	ASSERT_FLOAT_EQ(21.81211, params.age_at_infection);
	ASSERT_FLOAT_EQ(287.2189f, params.cd4_count);
	ASSERT_FLOAT_EQ(1848, params.time_since_infection);
	ASSERT_TRUE(isnan(params.time_since_art_init));
	ASSERT_TRUE(isnan(params.time_of_art_init));
	ASSERT_FLOAT_EQ(4.2f, params.viral_load);
	ASSERT_TRUE(isnan(params.vl_art_traj_slope));
	ASSERT_TRUE(isnan(params.vl_at_art_init));
	ASSERT_TRUE(isnan(params.cd4_at_art_init));
}

TEST(CreatorTests, TestUninfectedPersonCreation) {
	std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
	RInstance::rptr->parseEvalQ(cmd);
	List rnet = as<List>((*RInstance::rptr)["n0"]);
	List val = as<List>(rnet["val"]);
	List val11 = as<List>(val[0]);
	// make sure I have the right one
	ASSERT_NEAR(43.8509, as<double>(val11["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner);
	PersonPtr person = creator(val11);
	ASSERT_TRUE(person->isCircumcised());
	ASSERT_FALSE(person->isInfected());
	ASSERT_NEAR(43.8509f, person->age(), 0.00001);

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
	List val11 = as<List>(val[22]);
	// make sure I have the right one
	ASSERT_NEAR(37.97986, as<double>(val11["age"]), 0.00001);

	std::vector<float> dur_inf { 10, 20, 30, 40 };
	std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, dur_inf);
	PersonCreator creator(runner);
	PersonPtr person = creator(val11);
	ASSERT_FALSE(person->isCircumcised());
	ASSERT_TRUE(person->isInfected());
	ASSERT_NEAR(37.97986f, person->age(), 0.00001);

	InfectionParameters params = person->infectionParameters();
	ASSERT_TRUE(params.art_covered);
	ASSERT_TRUE(params.art_status);
	ASSERT_FLOAT_EQ(543, params.time_since_infection);
	ASSERT_FLOAT_EQ(-543, params.time_of_infection);
	ASSERT_FLOAT_EQ(37.97986f, params.age_at_infection);
	ASSERT_FLOAT_EQ(437.2453f, params.cd4_count);
	ASSERT_EQ(0, params.time_since_art_init);
	ASSERT_EQ(0, params.time_of_art_init);
	ASSERT_FLOAT_EQ(4.2f, params.viral_load);
	ASSERT_NEAR(0.02084192f, params.vl_art_traj_slope, 0.000001);
	ASSERT_FLOAT_EQ(4.2f, params.vl_at_art_init);
	ASSERT_FLOAT_EQ(0, params.cd4_at_art_init);

}
