#include <iostream>

#include "CounselingAndBehavioralTreatmentCessationEvent.h"

using namespace std;

namespace TransModel {

void goOffCounselingAndBehavioralTreatment(PersonPtr person, double tick) {
    person->setOnCounselingAndBehavioralTreatment(false);
    person->setArtAdherence(person->artAdherenceBeforeTreatment());
    if ((person->prepParametersBeforeTreatment().status() == PrepStatus::OFF &&
            person->prepParameters().status() == PrepStatus::ON) ||
            (person->prepParametersBeforeTreatment().status() == PrepStatus::ON &&
             person->prepParametersBeforeTreatment().stopTime() < tick)) {
        person->goOffPrep(PrepStatus::OFF);
        Stats::instance()->personDataRecorder()->recordPREPStop(person.get(),
                                                                tick,
                                                                PrepStatus::OFF);
        Stats::instance()->recordPREPEvent(tick, person->id(),
                                           static_cast<int>(PrepStatus::OFF),
                                           person->isSubstanceUser(SubstanceUseType::METH),
                                           person->isSubstanceUser(SubstanceUseType::CRACK),
                                           person->isSubstanceUser(SubstanceUseType::ECSTASY));
    }
    person->setPrepParametersAdherenceData({person->prepParametersBeforeTreatment().prepEffectiveness(),
                            person->prepParametersBeforeTreatment().adherenceCagegory()});
}

CounselingAndBehavioralTreatmentCessationEvent::CounselingAndBehavioralTreatmentCessationEvent(std::shared_ptr<Person> person, double timestamp) :
    person_(person), timestamp_(timestamp) {
}

void CounselingAndBehavioralTreatmentCessationEvent::operator()() {
    if (!person_->isDead()) {
        goOffCounselingAndBehavioralTreatment(person_, timestamp_);
    }
}

CounselingAndBehavioralTreatmentCessationEvent::~CounselingAndBehavioralTreatmentCessationEvent() {
}

}
