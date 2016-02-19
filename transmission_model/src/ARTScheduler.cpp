/*
 * ARTScheduler.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: nick
 */

#include "ARTScheduler.h"
#include "EventWriter.h"
#include "Events.h"

namespace TransModel {

ARTScheduler::ARTScheduler(float time_stamp) :
		time_stamp_(time_stamp), persons { } {
}

ARTScheduler::~ARTScheduler() {
}

void ARTScheduler::operator()() {
	for (auto& p : persons) {
		p->putOnART(time_stamp_);
		EventWriter::instance()->addEvent(time_stamp_, p, ON_ART);
	}
}

void ARTScheduler::addPerson(PersonPtr person) {
	persons.push_back(person);
}

} /* namespace TransModel */
