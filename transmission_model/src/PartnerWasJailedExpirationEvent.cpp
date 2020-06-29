#include "PartnerWasJailedExpirationEvent.h"

using namespace repast;

namespace TransModel {

PartnerWasJailedExpirationEvent::PartnerWasJailedExpirationEvent(PersonPtr person, double tick) :
    person_(person), tick_(tick) {
}

void PartnerWasJailedExpirationEvent::operator()() {
    // might be dead or migh have gotten another jailed parter
    if (!person_->isDead() && person_->parterWasJailedExpirationTick() == tick_) {
        person_->setPartnerWasJailed(false);
    }
}


PartnerWasJailedExpirationEvent::~PartnerWasJailedExpirationEvent() {
}

void schedulePartnerWasJailedExpiration(PersonPtr person, double tick) {
    ScheduleRunner &runner = RepastProcess::instance()->getScheduleRunner();
    int partner_was_jailed_expiration_time = Parameters::instance()->getIntParameter(PARTNER_WAS_JAILED_EXPIRATION_TIME);
    int schedule_at_tick = tick + partner_was_jailed_expiration_time + 0.1;
    person->setPartnerWasJailedExpirationTick(schedule_at_tick);
    runner.scheduleEvent(schedule_at_tick, Schedule::FunctorPtr(new PartnerWasJailedExpirationEvent(person, schedule_at_tick)));
}

} /* namespace TransModel */
