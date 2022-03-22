/*
 * PrepParameters.h
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#ifndef SRC_PREPPARAMETERS_H_
#define SRC_PREPPARAMETERS_H_

#include "AdherenceCategory.h"
#include "PrepStatus.h"
#include "Parameters.h"

namespace TransModel {

class PrepParameters {

private:
    PrepStatus status_;
    double start_time_, stop_time_;
    AdherenceData oral_adherence;
public:
    PrepParameters(PrepStatus status, double start_time, double stop_time);
    PrepParameters(PrepStatus status, double start_time, double stop_time, AdherenceData& cat);
    virtual ~PrepParameters();

    const PrepStatus status() const {
        return status_;
    }

    double stopTime() const {
        return stop_time_;
    }

    double startTime() const {
        return start_time_;
    }

    const AdherenceCategory adherenceCagegory() const {
        return oral_adherence.category;
    }

    const double prepEffectiveness() const {
        // not actually the probability but prep effectiveness value
        if (status_ == PrepStatus::ON_LAI) return Parameters::instance()->getDoubleParameter(PREP_ALWAYS_ADHERENT_TR);
        else return oral_adherence.probability;
    }

    void on(double start_time, double stop_time, PrepStatus status);

    void off(PrepStatus status) {
        status_ = status;
        start_time_ = -1;
        stop_time_ = -1;
    }

    void offInfected() {
        status_ = PrepStatus::OFF_INFECTED;
    }

    void setAdherenceData(AdherenceData data) {
        oral_adherence = data;
    }
};

} /* namespace TransModel */

#endif /* SRC_PREPPARAMETERS_H_ */
