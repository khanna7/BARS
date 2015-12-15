/*
 * ViralLoadCalculator.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nick
 */

#ifndef SRC_VIRALLOADCALCULATOR_H_
#define SRC_VIRALLOADCALCULATOR_H_

#include "DiseaseParameters.h"

namespace TransModel {

class ViralLoadCalculator {

private:
	SharedViralLoadParameters vl_params;
	double calculateViralLoadNoART(InfectionParameters& infection_params, IndividualViralLoadParameters& viral_load_params);
	double calculateViralLoadART(InfectionParameters& infection_params, IndividualViralLoadParameters& viral_load_params);

public:
	ViralLoadCalculator(SharedViralLoadParameters& params);
	virtual ~ViralLoadCalculator();

	double calculateViralLoad(InfectionParameters& infection_params, IndividualViralLoadParameters& viral_load_params);
};

} /* namespace TransModel */

#endif /* SRC_VIRALLOADCALCULATOR_H_ */
