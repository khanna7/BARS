#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "ReleasedPartnerExpirationEvent.h"
#include "Parameters.h"

using namespace repast;

namespace TransModel {

ReleasedPartnerExpirationEvent::ReleasedPartnerExpirationEvent(PersonPtr person, int id) :
    person_(person), id_(id){
}

void ReleasedPartnerExpirationEvent::operator()() {
    if (!person_->isDead()) {
        person_->removeReleasedPartner(id_);
    }
}

ReleasedPartnerExpirationEvent::~ReleasedPartnerExpirationEvent() {
}

void scheduleReleasedPartnerExpiration(PersonPtr person, int id, double tick) {
    ScheduleRunner &runner = RepastProcess::instance()->getScheduleRunner();
    double released_partner_expiration_time = Parameters::instance()->getDoubleParameter(RELEASED_PARTNER_EXPIRATION_TIME);
    double schedule_at_tick = tick + released_partner_expiration_time + 0.1;
    runner.scheduleEvent(schedule_at_tick, Schedule::FunctorPtr(new ReleasedPartnerExpirationEvent(person, id)));
}

} /* namespace TrnasModel */
