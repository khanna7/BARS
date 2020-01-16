/*
 * OffArtFlagEndEvent.cpp
 *
 *  Created on: Oct 11 , 2019
 *      Author: Babak
 */

#include "OffArtFlagEndEvent.h"
#include "Stats.h"
#include "Jail.h"

namespace TransModel {

OffArtFlagEndEvent::OffArtFlagEndEvent(std::shared_ptr<Person> person, Jail* jail) :
    person_(person), jail_{jail} {
}

void OffArtFlagEndEvent::operator()() {

    // might be dead prior to this event occuring
    if (!person_->isDead() && jail_->artOverrideEnded(person_)) {
        person_->setArtForcedOff(false);
    }
}

OffArtFlagEndEvent::~OffArtFlagEndEvent() {
}

} /* namespace TransModel */

