/*
 * Stage.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: nick
 */

#include <cmath>

#include "Stage.h"

namespace TransModel {

Stage::Stage(float baseline_infectivity, float multiplier, const Range<float>& range) : baseline_infectivity_(baseline_infectivity),
		multiplier_(multiplier), range_(range) {
}

Stage::~Stage() {
}

bool Stage::in(float time_since_infection) {
	return range_.within(time_since_infection);
}

ChronicStage::ChronicStage(float baseline_infectivity, const Range<float>& range) : Stage(baseline_infectivity, 1, range) {}
ChronicStage::~ChronicStage() {}

float ChronicStage::calculateInfectivity(const InfectionParameters& params) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		infectivity = baseline_infectivity_;
	} else if (params.viral_load > 2) {
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2);
	}
	return infectivity;
}

AcuteStage::AcuteStage(float baseline_infectivity, float multiplier, const Range<float>& range) : Stage(baseline_infectivity, multiplier, range) {}
AcuteStage::~AcuteStage() {}

float AcuteStage::calculateInfectivity(const InfectionParameters& params) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		infectivity = baseline_infectivity_ * multiplier_;
	} else if (params.viral_load > 2) {
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2) * multiplier_;
	}
	return infectivity;
}

LateStage::LateStage(float baseline_infectivity, float multiplier, const Range<float>& range) : Stage(baseline_infectivity, multiplier, range) {}
LateStage::~LateStage() {}

float LateStage::calculateInfectivity(const InfectionParameters& params) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		float mult = params.art_status ? 1 : multiplier_;
		infectivity = baseline_infectivity_ * mult;
	} else if (params.viral_load > 2) {
		float mult = params.art_status ? 1 : multiplier_;
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2) * mult;
	}
	return infectivity;
}

} /* namespace TransModel */
