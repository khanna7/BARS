/*
 * TransmissionRunner.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: nick
 */

#include "TransmissionRunner.h"

#include "Person.h"

namespace TransModel {

TransmissionRunner::TransmissionRunner(float circumcision_multiplier, float condom_multiplier, float infective_insertive_multiplier,
        float duration) :
        circumcision_multiplier_(circumcision_multiplier), condom_multiplier_(condom_multiplier),
        infective_insertive_multiplier_(infective_insertive_multiplier),
        duration_(duration) {
}

TransmissionRunner::~TransmissionRunner() {
}

bool TransmissionRunner::determineInfection(PersonPtr& infector, PersonPtr& infectee, bool condom_used, int edge_type) {
    float infectivity = infector->infectivity();

    if (condom_used) {
        infectivity *= condom_multiplier_;
    }

    if (infectee->isOnPrep(true)) {
        infectivity *= (1 - infectee->prepParameters().prepEffectiveness());
    }

    int infectee_role = edge_type == STEADY_NETWORK_TYPE ? infectee->steady_role() : infectee->casual_role();
    int infector_role = edge_type == STEADY_NETWORK_TYPE ? infector->steady_role() : infector->casual_role();

    if (infectee->isCircumcised() && (infectee_role == INSERTIVE || infectee_role == VERSATILE)
            && infector_role == RECEPTIVE && !condom_used) {
        infectivity *= circumcision_multiplier_;
    }

    if ((infector_role == INSERTIVE || infector_role == VERSATILE) && infectee_role == RECEPTIVE) {
        infectivity *= infective_insertive_multiplier_;
    }

    return infectivity >= repast::Random::instance()->nextDouble();
}

float TransmissionRunner::durationOfInfection() {
    return duration_;
}

void TransmissionRunner::infect(PersonPtr infectee, float time) {
    float dur_inf = durationOfInfection();
    infectee->infect(dur_inf, time);
}

} /* namespace TransModel */
