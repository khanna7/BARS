#include <iostream>

#include "DrugUseCessationEvent.h"
#include "adherence_functions.h"

using namespace std;

namespace TransModel {

DrugUseCessationEvent::DrugUseCessationEvent(std::shared_ptr<Person> person, SubstanceUseType drug, double timestamp) :
    person_(person), drug_(drug), timestamp_(timestamp) {
}

void DrugUseCessationEvent::operator()() {
    if (!person_->isDead()) {
        person_->goOffDrug(drug_);
        DrugUse::goOnOffDrug(person_);
    }
}

DrugUseCessationEvent::~DrugUseCessationEvent() {
}

}
