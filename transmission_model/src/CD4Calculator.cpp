/*
 * CD4Calculator.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include <cmath>
#include "CD4Calculator.h"

namespace TransModel {

// constants from Aditya's cd4-update.R file
const float B1_REF = 23.53f;
const float B2_AFRICAN = -0.76f;
const float B4_CD4_REF = -1.49f;
const float B5_AFRICAN = 0.34f;
const float B6_AGE_1529 = 0.0f;
const float B6_AGE_3039 = -0.1f;
const float B6_AGE_4049 = -0.34f;
const float B6_AGE_50 = -0.63f;

const float B1_PLUS_B2 = B1_REF + B2_AFRICAN;
const float B4_PLUS_B5 = B4_CD4_REF + B5_AFRICAN;

CD4Calculator::CD4Calculator(float size_of_timestep, float cd4_recovery_time,
		float cd4_at_infection_male, float per_day_cd4_recovery) : size_of_timestep_(size_of_timestep),
				cd4_recovery_time_(cd4_recovery_time), cd4_at_infection_male_(cd4_at_infection_male),
				per_day_cd4_recovery_(per_day_cd4_recovery){
}

CD4Calculator::~CD4Calculator() {

}

float CD4Calculator::getB6AgeValue(float age) {
	if (age < 30) {
		return B6_AGE_1529;
	} else if (age < 40) {
		return B6_AGE_3039;
	} else if (age < 50) {
		return B6_AGE_4049;
	}
	return B6_AGE_50;
}

float CD4Calculator::calculateCD4(float age, const InfectionParameters& infection_params) {
	float cd4 = infection_params.cd4_count;
	if (infection_params.art_status) {
		// with anti-retroviral treatment
		if (infection_params.time_since_art_init <= cd4_recovery_time_ && infection_params.cd4_count <= cd4_at_infection_male_) {
			cd4 = infection_params.cd4_count + (per_day_cd4_recovery_ * size_of_timestep_);
		}
	} else {
		// no anti-retroviral treatment
		float b6_val = getB6AgeValue(age);
		cd4 = std::pow(B1_PLUS_B2 + (infection_params.time_since_infection / 365.0 * size_of_timestep_) *
				(B4_PLUS_B5 + b6_val), 2);
	}

	return cd4;
}

} /* namespace TransModel */
