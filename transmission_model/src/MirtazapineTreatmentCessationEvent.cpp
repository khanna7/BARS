#include "MirtazapineTreatmentCessationEvent.h"

namespace TransModel {

MirtazapineTreatmentCessationEvent::MirtazapineTreatmentCessationEvent(
        std::shared_ptr<Person> person, double timestamp):
    person_(person), timestamp_(timestamp) {
}

void MirtazapineTreatmentCessationEvent::operator()() {
    if (!person_->isDead()) {
        person_->goOffMirtazapineTreatment(timestamp_);
    }
}

MirtazapineTreatmentCessationEvent::~MirtazapineTreatmentCessationEvent() {}

}
