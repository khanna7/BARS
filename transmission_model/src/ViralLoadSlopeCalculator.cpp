/*
 * VLArtSlopeCalculator.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: nick
 */

#include "ViralLoadSlopeCalculator.h"

namespace TransModel {

ViralLoadSlopeCalculator::ViralLoadSlopeCalculator(float undetectable_vl, float time_to_full_supp) : undetectable_vl_(undetectable_vl),
		time_to_full_supp_(time_to_full_supp) {
}

ViralLoadSlopeCalculator::~ViralLoadSlopeCalculator() {}

float ViralLoadSlopeCalculator::calculateSlope(const InfectionParameters& params) {
	return std::abs((undetectable_vl_ - params.viral_load) / time_to_full_supp_);
}

} /* namespace TransModel */
