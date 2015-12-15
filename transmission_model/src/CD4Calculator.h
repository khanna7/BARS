/*
 * CD4Calculator.h
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#ifndef SRC_CD4CALCULATOR_H_
#define SRC_CD4CALCULATOR_H_

#include "DiseaseParameters.h"

namespace TransModel {

/**
 * Calculates the current CD4.
 */
class CD4Calculator {

private:
	float size_of_timestep_, cd4_recovery_time_,
	cd4_at_infection_male_, per_day_cd4_recovery_;

	float getB6AgeValue(float age);

public:
	CD4Calculator(float size_of_timestep, float cd4_recovery_time,
			float cd4_at_infection_male, float per_day_cd4_recovery);

	virtual ~CD4Calculator();

	/**
	 * Assumes infected individual.
	 */
	double calculateCD4(float age, double cd4_count, InfectionParameters& infection_params);
};

} /* namespace TransModel */

#endif /* SRC_CD4CALCULATOR_H_ */
