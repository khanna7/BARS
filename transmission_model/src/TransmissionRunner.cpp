/*
 * TransmissionRunner.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: nick
 */

#include "TransmissionRunner.h"

namespace TransModel {

const int VERSATILE = 0;
const int INSERTIVE = 1;
const int RECEPTIVE = 2;

TransmissionRunner::TransmissionRunner(float circumcision_multiplier, float prep_multiplier,
		std::vector<float>& given_dur_inf_by_age) :
		circumcision_multiplier_(circumcision_multiplier), prep_multiplier_(prep_multiplier), dur_inf_by_age(
				given_dur_inf_by_age) {
}

TransmissionRunner::~TransmissionRunner() {
}

bool TransmissionRunner::determineInfection(PersonPtr& infector, PersonPtr& infectee) {
	float infectivity = infector->infectivity();

	if (infectee->onPrep()) {
		infectivity *= prep_multiplier_;
	}

	if (infectee->isCircumcised() && (infectee->role() == INSERTIVE || infectee->role() == VERSATILE)
			&& infector->role() == RECEPTIVE) {
		infectivity *= circumcision_multiplier_;
	}

	return infectivity >= repast::Random::instance()->nextDouble();
}

float TransmissionRunner::durInfByAge(float age) {
	float dur_inf = 0;
	if (age <= 24) {
		dur_inf = dur_inf_by_age[0];
	} else if (age <= 34) {
		dur_inf = dur_inf_by_age[1];
	} else if (age <= 44) {
		dur_inf = dur_inf_by_age[2];
	} else {
		dur_inf = dur_inf_by_age[3];
	}
	return dur_inf;
}

void TransmissionRunner::infect(PersonPtr infectee, float time) {
	int draw = (int) repast::Random::instance()->getGenerator(ART_COVERAGE_BINOMIAL)->next();
	float dur_inf = durInfByAge(infectee->age());
	infectee->infect(draw == 1, dur_inf, time);
}

} /* namespace TransModel */
