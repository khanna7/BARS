/*
 * DiseaseParameters.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: nick
 */

#include "DiseaseParameters.h"

namespace TransModel {

bool SharedViralLoadParameters::lessEqTimeToPeak(float time_since_infection) {
    return time_since_infection <= time_infection_to_peak_load;
}

bool SharedViralLoadParameters::greaterTimeToLateStage(float time_since_infection) {
    return time_since_infection > time_infection_to_late_stage;
}

bool SharedViralLoadParameters::withinPeakToSetPoint(float time_since_infection) {
    return (time_infection_to_peak_load + 1) <= time_since_infection && time_since_infection <= time_infection_to_set_point;
}
bool SharedViralLoadParameters::withinSetPointToLateStage(float time_since_infection) {
    return (time_infection_to_set_point + 1) <= time_since_infection && time_since_infection <= time_infection_to_late_stage;
}


} /* namespace TransModel */
