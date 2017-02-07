/*
 * TransmissionRunner.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: nick
 */

#include "TransmissionRunner.h"

#include "Person.h"

namespace TransModel {

TransmissionRunner::TransmissionRunner(float circumcision_multiplier, float prep_multiplier, float condom_multiplier,
		std::vector<float>& given_dur_inf_by_age) :
		circumcision_multiplier_(circumcision_multiplier), prep_multiplier_(prep_multiplier), condom_multiplier_(condom_multiplier),
		dur_inf_by_age(given_dur_inf_by_age) {
}

TransmissionRunner::~TransmissionRunner() {
}

bool TransmissionRunner::determineInfection(PersonPtr& infector, PersonPtr& infectee, bool condom_used, int edge_type) {
	float infectivity = infector->infectivity();

	if (condom_used) {
		infectivity *= condom_multiplier_;
	}

	if (infectee->isOnPrep()) {
		infectivity *= prep_multiplier_;
	}

	int infectee_role = edge_type == STEADY_NETWORK_TYPE ? infectee->steady_role() : infectee->casual_role();
	int infector_role = edge_type == STEADY_NETWORK_TYPE ? infector->steady_role() : infector->casual_role();

	if (infectee->isCircumcised() && (infectee_role == INSERTIVE || infectee_role == VERSATILE)
			&& infector_role == RECEPTIVE && !condom_used) {
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
	float dur_inf = durInfByAge(infectee->age());
	infectee->infect(dur_inf, time);
}

} /* namespace TransModel */
