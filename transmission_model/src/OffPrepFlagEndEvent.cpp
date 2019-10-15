/*
 * OffPrepFlagEndEvent.cpp
 *
 *  Created on: Oct 11 , 2019
 *      Author: Babak
 */

#include "OffPrepFlagEndEvent.h"
#include "Stats.h"

namespace TransModel {

OffPrepFlagEndEvent::OffPrepFlagEndEvent(std::shared_ptr<Person> person) :
        person_(person){
}

void OffPrepFlagEndEvent::operator()() {

    // might be dead prior to this event occuring
    if (!person_->isDead()) {
        person_ -> setOffPrepFlag(false);
    }
}

OffPrepFlagEndEvent::~OffPrepFlagEndEvent() {
}

} /* namespace TransModel */

