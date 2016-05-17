/*
 * VLArtSlopeCalculator.h
 *
 *  Created on: Feb 16, 2016
 *      Author: nick
 */

#ifndef SRC_VIRALLOADSLOPECALCULATOR_H_
#define SRC_VIRALLOADSLOPECALCULATOR_H_

#include "DiseaseParameters.h"

namespace TransModel {

class ViralLoadSlopeCalculator {
private:
	float undetectable_vl_, time_to_full_supp_;

public:
	ViralLoadSlopeCalculator(float undetectable_vl_, float time_to_full_supp_);
	virtual ~ViralLoadSlopeCalculator();

	/**
	 * Calculates the viral load on art trajectory slope for the given infection parameters.
	 * This assumes the associated person is on ART.
	 */
	float calculateSlope(const InfectionParameters& params);
};

} /* namespace TransModel */

#endif /* SRC_VIRALLOADSLOPECALCULATOR_H_ */
