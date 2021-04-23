/*
 * PrepCessationEvent.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#include "PrepCessationEvent.h"

#include "Stats.h"

namespace TransModel {

PrepCessationEvent::PrepCessationEvent(std::shared_ptr<Person> person, double timestamp) :
        person_(person), timestamp_(timestamp) {
}

void PrepCessationEvent::operator()() {
    // might be dead or may have gone off prep by becoming infected
    // prior to this event occuring
    if (!person_->isDead() && person_->isOnPrep(false) &&
            !person_->onCounselingAndBehavioralTreatment() &&
            !person_->onMirtazapineTreatment()) {
        person_->goOffPrep(PrepStatus::OFF);
        Stats::instance()->personDataRecorder()->recordPREPStop(person_.get(),
                                                                timestamp_,
                                                                PrepStatus::OFF);
        Stats::instance()->recordPREPEvent(timestamp_, person_->id(),
                                           static_cast<int>(PrepStatus::OFF),
                                           person_->isSubstanceUser(SubstanceUseType::METH),
                                           person_->isSubstanceUser(SubstanceUseType::CRACK),
                                           person_->isSubstanceUser(SubstanceUseType::ECSTASY));
    }
}

PrepCessationEvent::~PrepCessationEvent() {
}

} /* namespace TransModel */
