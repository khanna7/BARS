#include "ReleasedPartnerExpirationEvent.h"

namespace TransModel {

ReleasedPartnerExpirationEvent::ReleasedPartnerExpirationEvent(Person *person, int id) :
    person_(person), id_(id){
}

void ReleasedPartnerExpirationEvent::operator()() {
    if (!person_->isDead()) {
        person_->printReleaedPartners();
        person_->removeReleasedPartner(id_);
    }
}

ReleasedPartnerExpirationEvent::~ReleasedPartnerExpirationEvent() {
}

} /* namespace TrnasModel */
