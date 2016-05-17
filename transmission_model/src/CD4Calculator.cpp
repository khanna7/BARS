/*
 * CD4Calculator.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include <cmath>
#include "CD4Calculator.h"

namespace TransModel {

CD4Calculator::CD4Calculator(float size_of_timestep, float cd4_recovery_time,
		float cd4_at_infection_male, float per_day_cd4_recovery, BValues& b_values) : size_of_timestep_(size_of_timestep),
				cd4_recovery_time_(cd4_recovery_time), cd4_at_infection_male_(cd4_at_infection_male),
				per_day_cd4_recovery_(per_day_cd4_recovery), b_values_(b_values) {
}

CD4Calculator::~CD4Calculator() {

}

float CD4Calculator::getB6AgeValue(float age) {
	if (age < 30) {
		return b_values_.b6_age_15to29;
	} else if (age < 40) {
		return b_values_.b6_age_30to39;
	} else if (age < 50) {
		return b_values_.b6_age_40to49;
	}
	return b_values_.b6_age_50ormore;
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
		cd4 = std::pow((b_values_.b1_ref + b_values_.b2_african) + (infection_params.time_since_infection / 365.0 * size_of_timestep_) *
				((b_values_.b4_cd4_ref + b_values_.b5_african) + b6_val), 2);
	}

	return cd4;
}

} /* namespace TransModel */
