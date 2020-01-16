/*
 * AdherenceCheckScheduler.cpp
 *
 *  Created on: Sep 23, 2016
 *      Author: nick
 */


#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"

#include "AdherenceCheckScheduler.h"

#include "Parameters.h"
#include "Stats.h"

namespace TransModel {

ARTAdherenceCheckScheduler::ARTAdherenceCheckScheduler(std::shared_ptr<Person> person, double timestamp) :
        person_(person), timestamp_(timestamp) {
}

ARTAdherenceCheckScheduler::~ARTAdherenceCheckScheduler() {
}

void ARTAdherenceCheckScheduler::operator()() {
    if (!person_->isDead()) {
        bool go_on_art = repast::Random::instance()->nextDouble() <= (person_->artAdherence().probability);
        if (person_->isOnART(false) && !go_on_art) {
            // go off art when already on
            person_->goOffART();
            Stats::instance()->personDataRecorder()->recordARTStop(person_, timestamp_);
            Stats::instance()->personDataRecorder()->incrementNonAdheredIntervals(person_);
            Stats::instance()->recordARTEvent(timestamp_, person_->id(), false);
        } else if (!person_->isOnART(false) && go_on_art) {
            person_->goOnART(timestamp_);
            Stats::instance()->personDataRecorder()->recordARTStart(person_, timestamp_);
            Stats::instance()->recordARTEvent(timestamp_, person_->id(), true);
            Stats::instance()->personDataRecorder()->incrementAdheredIntervals(person_);
        }

        double adherence_check_at = timestamp_
                + Parameters::instance()->getDoubleParameter(PARTIAL_ART_ADHER_WINDOW_LENGTH);
        repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at - 0.1,
                repast::Schedule::FunctorPtr(new ARTAdherenceCheckScheduler(person_, adherence_check_at)));
    }
}

} /* namespace TransModel */
