/*
 * CD4ViralTests.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include <cmath>
#include "gtest/gtest.h"

#include "CD4Calculator.h"
#include "ViralLoadCalculator.h"

using namespace TransModel;

TEST(CD4Tests, TestART) {
	float size_of_timestep = 1.5f;
	float cd4_recovery_time = 35.0f;
	float cd4_at_infection_male = 50.0f;
	float per_day_cd4_recovery = 2.5f;

	CD4Calculator calc(size_of_timestep, cd4_recovery_time, cd4_at_infection_male, per_day_cd4_recovery);
	int age = 30;
	float time_since_inf = 10.0;
	float time_since_art_init = cd4_recovery_time - 1;
	float cd4_count = cd4_at_infection_male + 1;

	// cd4 count > cd4_at_infection so just return cd4 count
	InfectionParameters infection_params = { true, true, time_since_inf, time_since_art_init, -1, -1 };
	double cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_EQ(cd4_count, cd4);

	cd4_count = cd4_at_infection_male - 1;
	infection_params.time_since_art_init = cd4_recovery_time + 1;
	// time since art init > cd4 recovery time so return cd4_count
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_EQ(cd4_count, cd4);

	infection_params.time_since_art_init = cd4_recovery_time - 1;
	float expected = cd4_count + (per_day_cd4_recovery * size_of_timestep);
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_EQ(expected, cd4);
}

double calcNoArtExpected(float b6Age, float time_since_inf, float size_of_timestep) {
	float a = 23.53 + -0.76;
	float b = -1.49 + 0.34;
	return std::pow(a + (time_since_inf / 365.0 * size_of_timestep) * (b + b6Age), 2);
}

TEST(CD4Tests, TestNoART) {
	float size_of_timestep = 1.5f;
	float cd4_recovery_time = 35.0f;
	float cd4_at_infection_male = 50.0f;
	float per_day_cd4_recovery = 2.5f;

	CD4Calculator calc(size_of_timestep, cd4_recovery_time, cd4_at_infection_male, per_day_cd4_recovery);

	float age = 29;
	float time_since_inf = 10.0;
	float time_since_art_init = cd4_recovery_time - 1;
	float cd4_count = cd4_at_infection_male - 1;
	InfectionParameters infection_params = { true, false, time_since_inf, time_since_art_init, -1, -1 };

	float b6Age = 0;
	double expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	double cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 16;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 30;
	b6Age = -0.1f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 39;
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 40;
	b6Age = -0.34f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 49;
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 50;
	b6Age = -0.63f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 65;
	cd4 = calc.calculateCD4(age, cd4_count, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);
}

TEST(ViralLoadTests, TestVLParameters) {

	SharedViralLoadParameters vp;
	vp.time_infection_to_peak_load = 10;
	vp.time_infection_to_set_point = 15;
	vp.time_infection_to_late_stage = 30;

	ASSERT_TRUE(vp.lessEqTimeToPeak(5));
	ASSERT_TRUE(vp.lessEqTimeToPeak(10));
	ASSERT_FALSE(vp.lessEqTimeToPeak(10.5f));

	ASSERT_TRUE(vp.greaterTimeToLateStage(30.1f));
	ASSERT_FALSE(vp.greaterTimeToLateStage(30));

	ASSERT_FALSE(vp.withinPeakToSetPoint(10));
	ASSERT_TRUE(vp.withinPeakToSetPoint(11));
	ASSERT_TRUE(vp.withinPeakToSetPoint(15));
	ASSERT_FALSE(vp.withinPeakToSetPoint(16));

	ASSERT_FALSE(vp.withinSetPointToLateStage(15));
	ASSERT_TRUE(vp.withinSetPointToLateStage(25));
	ASSERT_TRUE(vp.withinSetPointToLateStage(30));
	ASSERT_FALSE(vp.withinSetPointToLateStage(31));
}

TEST(TestViralLoadTests, TestVLUntreated) {
	InfectionParameters inf_params = { true, false, 5, -1, -1, 20 };
	IndividualViralLoadParameters viral_params = { 10, -1 };
	SharedViralLoadParameters shared_params = { 10, 20, 30, 0, 110, 200, 300, 0 };
	ViralLoadCalculator calc(shared_params);

	// time since infection is <= time to peek viral load so
	// is the mean of 0 - peak viral load which is 110
	double expected = 55;
	double actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 10;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 15;
	expected = shared_params.peak_viral_load
			- ((shared_params.peak_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_peak_load)
					/ (shared_params.time_infection_to_set_point - shared_params.time_infection_to_peak_load)) * 0.5;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 20;
	expected = shared_params.peak_viral_load
			- ((shared_params.peak_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_peak_load)
					/ (shared_params.time_infection_to_set_point - shared_params.time_infection_to_peak_load)) * 0.5;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 21;
	expected = shared_params.set_point_viral_load;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = shared_params.time_infection_to_late_stage;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = shared_params.time_infection_to_late_stage + 1;
	viral_params.viral_load = shared_params.late_stage_viral_load - 1;
	expected = shared_params.set_point_viral_load
			+ ((shared_params.late_stage_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_late_stage)
					/ ((inf_params.dur_inf_by_age - 1) - shared_params.time_infection_to_late_stage)) * 0.5;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	viral_params.viral_load = shared_params.late_stage_viral_load + 1;
	expected = shared_params.late_stage_viral_load;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);
}

TEST(TestViralLoadTests, TestVLART) {
	InfectionParameters inf_params = { true, true, 5, -1, -1, 20 };
	IndividualViralLoadParameters viral_params = { 10, -1 };
	SharedViralLoadParameters shared_params = { 10, 20, 30, 0, 110, 200, 300, 1 };
	ViralLoadCalculator calc(shared_params);

	inf_params.time_since_art_init = shared_params.time_to_full_supp - 1;
	double expected = viral_params.viral_load - viral_params.vl_art_traj_slope;
	double actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_art_init = shared_params.time_to_full_supp;
	expected = shared_params.undetectable_viral_load;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_art_init = shared_params.time_to_full_supp + 1;
	actual = calc.calculateViralLoad(inf_params, viral_params);
	ASSERT_EQ(expected, actual);
}


