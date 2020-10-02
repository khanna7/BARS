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

void scheduleReleasedPartnerExpiration(PersonPtr person, int id, double scheduled_for) {
    ScheduleRunner &runner = RepastProcess::instance()->getScheduleRunner();
    person->setReleasedPartnerExpirationTick(id, scheduled_for);
    runner.scheduleEvent(scheduled_for, Schedule::FunctorPtr(new ReleasedPartnerExpirationEvent(person, id, scheduled_for)));
}

} /* namespace TrnasModel */
