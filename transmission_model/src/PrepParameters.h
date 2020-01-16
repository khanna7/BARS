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

namespace TransModel {

class PrepParameters {

private:
    PrepStatus status_;
    bool prep_forced_off;
    double start_time_, stop_time_;
    AdherenceData adherence;

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
        return adherence.category;
    }

    const double prepEffectiveness() const {
        // not actually the probability but prep effectiveness value
        return adherence.probability;
    }

    void on(double start_time, double stop_time);

    void off(PrepStatus status) {
        status_ = status;
        start_time_ = -1;
        stop_time_ = -1;
    }

    void offInfected() {
        status_ = PrepStatus::OFF_INFECTED;
    }

    void setAdherenceData(AdherenceData data) {
        adherence = data;
    }

    void setPrepForcedOff(bool val) {
        prep_forced_off = val;
    }

    bool prepForcedOff() const {
        return prep_forced_off;
    }

};

} /* namespace TransModel */

#endif /* SRC_PREPPARAMETERS_H_ */
