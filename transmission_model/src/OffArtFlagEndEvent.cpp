/*
 * OffArtFlagEndEvent.cpp
 *
 *  Created on: Oct 11 , 2019
 *      Author: Babak
 */

#include "OffArtFlagEndEvent.h"
#include "Stats.h"

namespace TransModel {

OffArtFlagEndEvent::OffArtFlagEndEvent(std::shared_ptr<Person> person) :
    person_(person){
}

void OffArtFlagEndEvent::operator()() {

    // might be dead prior to this event occuring
    if (!person_->isDead()) {
        person_->setArtForcedOff(false);
    }
}

OffArtFlagEndEvent::~OffArtFlagEndEvent() {
}

} /* namespace TransModel */

