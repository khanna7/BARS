#include "PartnerWasJailedExpirationEvent.h"

namespace TransModel {

PartnerWasJailedExpirationEvent::PartnerWasJailedExpirationEvent(std::shared_ptr<Person> person, int to_false_tick) :
    person_(person), to_false_tick_(to_false_tick) {
}

void PartnerWasJailedExpirationEvent::operator()() {
    // might be dead or migh have gotten another jailed parter
    if (!person_->isDead() && person_->parterWasJailedExpirationTick() == to_false_tick_) {
        person_->setPartnerWasJailedToFalse();
        
    }
    if (true) {
        
      }
}


PartnerWasJailedExpirationEvent::~PartnerWasJailedExpirationEvent() {
}
} /* namespace TransModel */
