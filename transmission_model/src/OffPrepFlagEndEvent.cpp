/*
 * OffPrepFlagEndEvent.cpp
 *
 *  Created on: Oct 11 , 2019
 *      Author: Babak
 */

#include "OffPrepFlagEndEvent.h"
#include "Stats.h"
#include "Jail.h"

namespace TransModel {

OffPrepFlagEndEvent::OffPrepFlagEndEvent(std::shared_ptr<Person> person, Jail* jail) :
        person_(person), jail_(jail) {
}

void OffPrepFlagEndEvent::operator()() {

    // might be dead prior to this event occuring
    if (!person_->isDead() && jail_->prepOverrideEnded(person_)) {
        person_->setPrepForcedOff(false);
    }
}

OffPrepFlagEndEvent::~OffPrepFlagEndEvent() {
}

} /* namespace TransModel */

