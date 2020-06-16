#include "PartnerWasJailedExpirationEvent.h"

namespace TransModel {

PartnerWasJailedExpirationEvent::PartnerWasJailedExpirationEvent(Person *person, int tick) :
    person_(person), tick_(tick) {
}

void PartnerWasJailedExpirationEvent::operator()() {
    // might be dead or migh have gotten another jailed parter
    if (!person_->isDead() && person_->parterWasJailedExpirationTick() == tick_) {
        person_->setPartnerWasJailedToFalse();
    }
}


PartnerWasJailedExpirationEvent::~PartnerWasJailedExpirationEvent() {
}
} /* namespace TransModel */
