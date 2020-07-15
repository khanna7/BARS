#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "ReleasedPartnerExpirationEvent.h"
#include "Parameters.h"

using namespace repast;

namespace TransModel {

ReleasedPartnerExpirationEvent::ReleasedPartnerExpirationEvent(PersonPtr person, int id, double expiration_tick) :
    person_(person), id_(id), expiration_tick_(expiration_tick){
}

void ReleasedPartnerExpirationEvent::operator()() {
    if (!person_->isDead()) {
        if (person_->hasReleasedPartner(id_) && person_->releasedPartnerExpirationTick(id_) == expiration_tick_) {
            person_->removeReleasedPartner(id_);
        }
    }
}

ReleasedPartnerExpirationEvent::~ReleasedPartnerExpirationEvent() {
}

void scheduleReleasedPartnerExpiration(PersonPtr person, int id, double scheduled_at_tick) {
    ScheduleRunner &runner = RepastProcess::instance()->getScheduleRunner();
    double released_partner_expiration_time = Parameters::instance()->getDoubleParameter(RELEASED_PARTNER_EXPIRATION_TIME);
    double expire_at_tick = scheduled_at_tick + released_partner_expiration_time + 0.1;
    person->setReleasedPartnerExpirationTick(id, expire_at_tick);
    runner.scheduleEvent(expire_at_tick, Schedule::FunctorPtr(new ReleasedPartnerExpirationEvent(person, id, expire_at_tick)));
}

} /* namespace TrnasModel */
