#include "CounselingAndBehavioralTreatmentCessationEvent.h"

namespace TransModel {

CounselingAndBehavioralTreatmentCessationEvent::CounselingAndBehavioralTreatmentCessationEvent(std::shared_ptr<Person> person, double timestamp) :
    person_(person), timestamp_(timestamp) {
}

void CounselingAndBehavioralTreatmentCessationEvent::operator()() {
    if (!person_->isDead()) {
        person_->goOffCounselingAndBehavioralTreatment(timestamp_);
    }
}

CounselingAndBehavioralTreatmentCessationEvent::~CounselingAndBehavioralTreatmentCessationEvent() {
}

}
