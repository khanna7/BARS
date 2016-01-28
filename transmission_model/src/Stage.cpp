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

float ChronicStage::calculateInfectivity(const InfectionParameters& params, float sex_acts_per_timstep) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		infectivity = 1 - std::pow(1 - baseline_infectivity_, sex_acts_per_timstep);
	} else if (params.viral_load > 2) {
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2);
		infectivity = 1 - std::pow(1 - infectivity, sex_acts_per_timstep);
	}
	return infectivity;
}

AcuteStage::AcuteStage(float baseline_infectivity, float multiplier, const Range<float>& range) : Stage(baseline_infectivity, multiplier, range) {}
AcuteStage::~AcuteStage() {}

float AcuteStage::calculateInfectivity(const InfectionParameters& params, float sex_acts_per_timstep) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		infectivity = baseline_infectivity_ * multiplier_;
		infectivity = 1 - std::pow(1 - infectivity, sex_acts_per_timstep);
	} else if (params.viral_load > 2) {
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2) * multiplier_;
		infectivity = 1 - std::pow(1 - infectivity, sex_acts_per_timstep);
	}
	return infectivity;
}

LateStage::LateStage(float baseline_infectivity, float multiplier, const Range<float>& range) : Stage(baseline_infectivity, multiplier, range) {}
LateStage::~LateStage() {}

float LateStage::calculateInfectivity(const InfectionParameters& params, float sex_acts_per_timstep) {
	// viral_load < 2 return 0
	float infectivity = 0;
	if (params.viral_load == 2) {
		float mult = params.art_status ? 1 : multiplier_;
		infectivity = baseline_infectivity_ * mult;
		infectivity = 1 - std::pow(1 - infectivity, sex_acts_per_timstep);
	} else if (params.viral_load > 2) {
		float mult = params.art_status ? 1 : multiplier_;
		infectivity = baseline_infectivity_ * std::pow(2.89f, params.viral_load - 2) * mult;
		infectivity = 1 - std::pow(1 - infectivity, sex_acts_per_timstep);
	}
	return infectivity;
}

} /* namespace TransModel */
