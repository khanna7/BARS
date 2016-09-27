/*
 * ARTScheduler.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: nick
 */

#include "Parameters.h"

#include "ARTScheduler.h"
#include "Stats.h"

//#include "EventWriter.h"
//#include "Events.h"

namespace TransModel {

ARTScheduler::ARTScheduler(float time_stamp) :
		time_stamp_(time_stamp), persons { } {
}

ARTScheduler::~ARTScheduler() {
}

void ARTScheduler::operator()() {
	double prob = Parameters::instance()->getDoubleParameter(PROB_ART_ADHER_FOR_PARTIAL);
	for (auto& p : persons) {
		// person might be die in between ART is scheduled
		// and actually going on ART.
		if (!p->isDead()) {
			if (p->adherence() == AdherenceCategory::ALWAYS) {
				p->goOnART(time_stamp_);
				Stats::instance()->personDataRecorder().recordARTStart(p, time_stamp_);
				Stats::instance()->recordARTEvent(time_stamp_, p->id(), true);
			} else if (p->adherence() == AdherenceCategory::PARTIAL && repast::Random::instance()->nextDouble() <= prob) {
				p->goOnART(time_stamp_);
				Stats::instance()->personDataRecorder().recordARTStart(p, time_stamp_);
				Stats::instance()->recordARTEvent(time_stamp_, p->id(), true);
				Stats::instance()->personDataRecorder().incrementAdheredIntervals(p);
			}
		}
	}
}

void ARTScheduler::addPerson(PersonPtr person) {
	persons.push_back(person);
}

} /* namespace TransModel */
