/*
 * PrepParameters.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#include "PrepParameters.h"

namespace TransModel {

PrepParameters::PrepParameters(PrepStatus status, double start_time, double stop_time) : status_(status), start_time_(start_time),
        stop_time_(stop_time), oral_adherence(0, AdherenceCategory::NA) {
}

PrepParameters::PrepParameters(PrepStatus status, double start_time, double stop_time, AdherenceData& data) : status_(status), start_time_(start_time),
        stop_time_(stop_time), oral_adherence(data) {
}

PrepParameters::~PrepParameters() {}

void PrepParameters::on(double start_time, double stop_time, PrepStatus status) {
        status_ = status;
        start_time_ = start_time;
        stop_time_ = stop_time;
    }

} /* namespace TransModel */
