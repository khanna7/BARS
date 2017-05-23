/*
 * ARTScheduler.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: nick
 */

#include "Parameters.h"

#include "ARTScheduler.h"

#include "adherence_functions.h"
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
	for (auto& p : persons) {
		// person might be die in between ART is scheduled
		// and actually going on ART.
		if (!p->isDead()) {
			initialize_art_adherence(p, time_stamp_);
			p->goOnART(time_stamp_);
			Stats::instance()->personDataRecorder().recordARTStart(p, time_stamp_);
			Stats::instance()->recordARTEvent(time_stamp_, p->id(), true);
		}
	}
}

void ARTScheduler::addPerson(PersonPtr person) {
	persons.push_back(person);
}

} /* namespace TransModel */
