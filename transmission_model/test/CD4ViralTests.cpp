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
#include "Stage.h"

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
	InfectionParameters infection_params;
	infection_params.infection_status = true;
	infection_params.art_status = true;
	infection_params.time_since_infection = time_since_inf;
	infection_params.time_since_art_init = time_since_art_init;
	infection_params.cd4_count = cd4_count;

	double cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_EQ(cd4_count, cd4);

	cd4_count = cd4_at_infection_male - 1;
	infection_params.time_since_art_init = cd4_recovery_time + 1;
	infection_params.cd4_count = cd4_count;
	// time since art init > cd4 recovery time so return cd4_count
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_EQ(cd4_count, cd4);

	infection_params.time_since_art_init = cd4_recovery_time - 1;
	float expected = cd4_count + (per_day_cd4_recovery * size_of_timestep);
	cd4 = calc.calculateCD4(age, infection_params);
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
	InfectionParameters infection_params;
	infection_params.infection_status = true;
	infection_params.art_status = false;
	infection_params.time_since_infection = time_since_inf;
	infection_params.time_since_art_init = time_since_art_init;
	infection_params.cd4_count = cd4_count;

	float b6Age = 0;
	float expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	float cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 16;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 30;
	b6Age = -0.1f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 39;
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 40;
	b6Age = -0.34f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 49;
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 50;
	b6Age = -0.63f;
	expected = calcNoArtExpected(b6Age, time_since_inf, size_of_timestep);
	cd4 = calc.calculateCD4(age, infection_params);
	ASSERT_DOUBLE_EQ(expected, cd4);

	age = 65;
	cd4 = calc.calculateCD4(age, infection_params);
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
	InfectionParameters inf_params;
	inf_params.infection_status = true;
	inf_params.art_status = false;
	inf_params.time_since_infection = 5;
	inf_params.dur_inf_by_age = 20;
	inf_params.viral_load = 10;

	SharedViralLoadParameters shared_params = { 10, 20, 30, 0, 110, 200, 300, 0 };
	ViralLoadCalculator calc(shared_params);

	// time since infection is <= time to peek viral load so
	// is the mean of 0 - peak viral load which is 110
	float expected = 55;
	float actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 10;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 15;
	expected = shared_params.peak_viral_load
			- ((shared_params.peak_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_peak_load)
					/ (shared_params.time_infection_to_set_point - shared_params.time_infection_to_peak_load)) * 0.5;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 20;
	expected = shared_params.peak_viral_load
			- ((shared_params.peak_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_peak_load)
					/ (shared_params.time_infection_to_set_point - shared_params.time_infection_to_peak_load)) * 0.5;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = 21;
	expected = shared_params.set_point_viral_load;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = shared_params.time_infection_to_late_stage;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_infection = shared_params.time_infection_to_late_stage + 1;
	inf_params.viral_load = shared_params.late_stage_viral_load - 1;
	expected = shared_params.set_point_viral_load
			+ ((shared_params.late_stage_viral_load - shared_params.set_point_viral_load)
					* (inf_params.time_since_infection - shared_params.time_infection_to_late_stage)
					/ ((inf_params.dur_inf_by_age - 1) - shared_params.time_infection_to_late_stage)) * 0.5;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.viral_load = shared_params.late_stage_viral_load + 1;
	expected = shared_params.late_stage_viral_load;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);
}

TEST(TestViralLoadTests, TestVLART) {
	InfectionParameters inf_params;
	inf_params.infection_status = true;
	inf_params.art_status = true;
	inf_params.time_since_infection = 5;
	inf_params.dur_inf_by_age = 20;
	inf_params.viral_load = 10;

	SharedViralLoadParameters shared_params = { 10, 20, 30, 0, 110, 200, 300, 1 };
	ViralLoadCalculator calc(shared_params);

	inf_params.time_since_art_init = shared_params.time_to_full_supp - 1;
	float expected = inf_params.viral_load - inf_params.vl_art_traj_slope;
	float actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_art_init = shared_params.time_to_full_supp;
	expected = shared_params.undetectable_viral_load;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);

	inf_params.time_since_art_init = shared_params.time_to_full_supp + 1;
	actual = calc.calculateViralLoad(inf_params);
	ASSERT_EQ(expected, actual);
}

TEST(InfectivityTests, TestRange) {
	Range<float> range(3, 6);
	ASSERT_FALSE(range.within(2));
	ASSERT_TRUE(range.within(3));
	ASSERT_TRUE(range.within(4));
	ASSERT_TRUE(range.within(3.5));
	ASSERT_TRUE(range.within(6));
	ASSERT_FALSE(range.within(6.1));
}

TEST(InfectivityTests, TestChronic) {
	Range<float> range(3, 6);
	float baseline = 2.5f;
	ChronicStage stage(baseline, range);
	ASSERT_TRUE(stage.in(4));
	ASSERT_FALSE(stage.in(1));

	InfectionParameters inf_params;
	inf_params.viral_load = 1;
	inf_params.art_status = false;
	float exp = 0;
	float actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.viral_load = 2;
	exp = 1 - std::pow((1 - baseline), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.viral_load = 2.5;
	inf_params.art_status = true;
	exp = baseline * std::pow(2.89, 2.5 - 2);
	exp = 1 - std::pow((1 - exp), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

}

TEST(InfectivityTests, TestAcute) {
	Range<float> range(3, 6);
	float baseline = 2.5f;
	float multiplier = 0.25;
	AcuteStage stage(baseline, multiplier, range);
	ASSERT_TRUE(stage.in(4));
	ASSERT_FALSE(stage.in(1));

	InfectionParameters inf_params;
	inf_params.viral_load = 1;
	inf_params.art_status = false;

	float exp = 0;
	float actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	exp = baseline * multiplier;
	exp = 1 - std::pow((1 - exp), 3);
	inf_params.viral_load = 2;
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	exp = baseline * std::pow(2.89, 2.5 - 2) * multiplier;
	exp = 1 - std::pow((1 - exp), 3);
	inf_params.viral_load = 2.5f;
	inf_params.art_status = true;
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);
}

TEST(InfectivityTests, TestLate) {
	Range<float> range(3, 6);
	float baseline = 2.5f;
	float multiplier = 0.25;
	LateStage stage(baseline, multiplier, range);
	ASSERT_TRUE(stage.in(4));
	ASSERT_FALSE(stage.in(1));

	InfectionParameters inf_params;

	float exp = 0;
	inf_params.viral_load = 1;
	inf_params.art_status = false;
	float actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.viral_load = 2;
	exp = baseline * multiplier;
	exp = 1 - std::pow((1 - exp), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.art_status = true;
	exp = baseline;
	exp = 1 - std::pow((1 - exp), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.viral_load = 2.5;
	inf_params.art_status = false;
	exp = baseline * std::pow(2.89, 2.5 - 2) * multiplier;
	exp = 1 - std::pow((1 - exp), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);

	inf_params.art_status = true;
	exp = baseline * std::pow(2.89, 2.5 - 2);
	exp = 1 - std::pow((1 - exp), 3);
	actual = stage.calculateInfectivity(inf_params, 3);
	ASSERT_EQ(exp, actual);
}

