/*
 * ViralLoadCalculator.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: nick
 */

#include "ViralLoadCalculator.h"
#include <cmath>

namespace TransModel {


ViralLoadCalculator::ViralLoadCalculator(SharedViralLoadParameters& params) : vl_params(params) {
}

ViralLoadCalculator::~ViralLoadCalculator() {
}

float ViralLoadCalculator::calculateViralLoadART(const InfectionParameters& infection_params) {
    double viral_load = vl_params.undetectable_viral_load;
    if (infection_params.time_since_art_init < vl_params.time_to_full_supp) {
        viral_load = infection_params.viral_load - infection_params.vl_art_traj_slope;
    }
    return viral_load;
}

float ViralLoadCalculator::calculateViralLoad(const InfectionParameters& infection_params) {
    if (infection_params.art_status && !infection_params.art_forced_off) {
        // AK: (art.status[i] == 1 && (art.type[i] == 1 || art.type[i] == 4)
        return calculateViralLoadART(infection_params);
    } else {
        // AK: (is.na(art.status[i]) && is.na(art.type[i])) || (art.status[i] == 0 && is.na(art.type[i])
        return calculateViralLoadNoART(infection_params);
    }
}

float ViralLoadCalculator::calculateViralLoadNoART(const InfectionParameters& infection_params) {
    if (!std::isnan(infection_params.time_since_art_cessation)) {
        if (infection_params.time_since_art_cessation < vl_params.time_to_full_supp) {
            float target = getViralLoadByDiseaseProgression(infection_params);
            float time_to_target = vl_params.time_to_full_supp - infection_params.time_since_art_cessation;
            return infection_params.viral_load + (target - infection_params.viral_load) / time_to_target;
        }
    }
    return getViralLoadByDiseaseProgression(infection_params);
};

float ViralLoadCalculator::getViralLoadByDiseaseProgression(const InfectionParameters& infection_params) {
    if (vl_params.lessEqTimeToPeak(infection_params.time_since_infection)) {
        return vl_params.peak_viral_load / 2.0;
    }

    if (vl_params.withinPeakToSetPoint(infection_params.time_since_infection)) {
        return vl_params.peak_viral_load - ((vl_params.peak_viral_load - vl_params.set_point_viral_load) *
                (infection_params.time_since_infection - vl_params.time_infection_to_peak_load) /
                (vl_params.time_infection_to_set_point - vl_params.time_infection_to_peak_load)) * 0.5;
    }

    if (vl_params.withinSetPointToLateStage(infection_params.time_since_infection)) {
        return vl_params.set_point_viral_load;
    }

    // assume greater than time to late stage given that we pass the other "ifs".
    //if (vl_params.greaterTimeToLateStage(infection_params.time_since_infection)) {
        if (infection_params.viral_load < vl_params.late_stage_viral_load) {
            return vl_params.set_point_viral_load + ((vl_params.late_stage_viral_load - vl_params.set_point_viral_load) *
                    (infection_params.time_since_infection - vl_params.time_infection_to_late_stage) /
                    ((infection_params.dur_inf - 1) - vl_params.time_infection_to_late_stage)) * 0.5;
        } else {
            return vl_params.late_stage_viral_load;
        }
    //}
}

} /* namespace TransModel */
