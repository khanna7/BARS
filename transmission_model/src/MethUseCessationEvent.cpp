#include <iostream>

#include "MethUseCessationEvent.h"
#include "adherence_functions.h"

using namespace std;

namespace TransModel {

MethUseCessationEvent::MethUseCessationEvent(std::shared_ptr<Person> person, double timestamp) :
    person_(person), timestamp_(timestamp) {
}

void MethUseCessationEvent::operator()() {
    if (!person_->isDead()) {
        std::cout << person_->id() << " GOING OFF METH" << std::endl;
        person_->goOffMeth();
        cout << "OFF? " << person_->isSubstanceUser(SubstanceUseType::METH);
    }
}

MethUseCessationEvent::~MethUseCessationEvent() {
}

}
