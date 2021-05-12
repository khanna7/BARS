#include "MirtazapineTreatmentCessationEvent.h"

namespace TransModel {

void goOffMirtazapineTreatment(PersonPtr person, double tick) {
    person->setOnMirtazapineTreatment(false);
    if (person->adheringToMirtazapineTreatment()) {
        person->setAdheringToMirtazapineTreatment(false);
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
}

MirtazapineTreatmentCessationEvent::MirtazapineTreatmentCessationEvent(
        std::shared_ptr<Person> person, double timestamp):
    person_(person), timestamp_(timestamp) {
}

void MirtazapineTreatmentCessationEvent::operator()() {
    if (!person_->isDead()) {
        goOffMirtazapineTreatment(person_, timestamp_);
    }
}

MirtazapineTreatmentCessationEvent::~MirtazapineTreatmentCessationEvent() {}

}
