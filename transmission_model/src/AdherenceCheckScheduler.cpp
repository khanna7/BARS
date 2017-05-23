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
		if (person_->isOnART() && !go_on_art) {
			// go off art when already on
			person_->goOffART();
			Stats::instance()->personDataRecorder().recordARTStop(person_, timestamp_);
			Stats::instance()->personDataRecorder().incrementNonAdheredIntervals(person_);
			Stats::instance()->recordARTEvent(timestamp_, person_->id(), false);
		} else if (!person_->isOnART() && go_on_art) {
			person_->goOnART(timestamp_);
			Stats::instance()->personDataRecorder().recordARTStart(person_, timestamp_);
			Stats::instance()->recordARTEvent(timestamp_, person_->id(), true);
			Stats::instance()->personDataRecorder().incrementAdheredIntervals(person_);
		}

		double adherence_check_at = timestamp_
				+ Parameters::instance()->getDoubleParameter(PARTIAL_ART_ADHER_WINDOW_LENGTH);
		repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at - 0.1,
				repast::Schedule::FunctorPtr(new ARTAdherenceCheckScheduler(person_, adherence_check_at)));
	}
}


PrepAdherenceCheckScheduler::PrepAdherenceCheckScheduler(std::shared_ptr<Person> person, double timestamp) :
		person_(person), timestamp_(timestamp) {
}

PrepAdherenceCheckScheduler::~PrepAdherenceCheckScheduler() {
}

void PrepAdherenceCheckScheduler::operator()() {
	if (!person_->isDead() && person_->prepStatus() != PrepStatus::OFF_INFECTED) {
		bool go_on_prep = (repast::Random::instance()->nextDouble() <= person_->prepAdherence().probability);
		if (person_->isOnPrep() && !go_on_prep) {
			// go off prep when already on
			person_->goOffPrep(PrepStatus::OFF);
			Stats::instance()->personDataRecorder().recordPREPStop(person_.get(), timestamp_, PrepStatus::OFF);
			Stats::instance()->recordPREPEvent(timestamp_, person_->id(), static_cast<int>(PrepStatus::OFF));
		} else if (!person_->isOnPrep() && go_on_prep) {
			person_->goOnPrep(timestamp_);
			Stats::instance()->personDataRecorder().recordPREPStart(person_, timestamp_);
			Stats::instance()->recordPREPEvent(timestamp_, person_->id(), static_cast<int>(PrepStatus::ON));
		}

		double adherence_check_at = timestamp_
				+ Parameters::instance()->getDoubleParameter(PREP_DECISION_FREQUENCY);
		repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at - 0.1,
				repast::Schedule::FunctorPtr(new PrepAdherenceCheckScheduler(person_, adherence_check_at)));
	}
}

} /* namespace TransModel */
