/*
 * PrepCessationEvent.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Parameters.h"
#include "PrepCessationEvent.h"

#include "Stats.h"

using namespace repast;

namespace TransModel {

PrepCessationEvent::PrepCessationEvent(std::shared_ptr<Person> person, double timestamp) :
        person_(person), timestamp_(timestamp) {
}

void PrepCessationEvent::operator()() {
    // might be dead or may have gone off prep by becoming infected
    // prior to this event occuring
    bool reinject = false;

    if (!person_->isDead() && person_->isOnPrep()) {
        if (person_->prepStatus() == PrepStatus::ON_LAI) {
            if (repast::Random::instance()->nextDouble() < Parameters::instance()->getDoubleParameter(LAI_PREP_LAI_REINJECT_PROB)) {
                reinject = true;
            }
        }
        person_->goOffPrep(PrepStatus::OFF);
        Stats::instance()->personDataRecorder()->recordPREPStop(person_.get(), timestamp_, PrepStatus::OFF);
        Stats::instance()->recordPREPEvent(timestamp_, person_->id(), static_cast<int>(PrepStatus::OFF));
        if (reinject) {
            ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
            double stop_time = timestamp_ + Parameters::instance()->getDoubleParameter(LAI_PREP_LAI_LENGTH_DAYS);
            PrepStatus status = PrepStatus::ON_LAI;
            person_->goOnPrep(timestamp_, stop_time, status);
            Stats* stats = Stats::instance();
            stats->recordPREPEvent(timestamp_, person_->id(), static_cast<int>(status));
            stats->personDataRecorder()->recordPREPStart(person_, timestamp_);
            runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new PrepCessationEvent(person_, stop_time)));
        }
    }
}

PrepCessationEvent::~PrepCessationEvent() {
}

} /* namespace TransModel */
