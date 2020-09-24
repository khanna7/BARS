#include "JailEvents.h"
#include "Jail.h"

namespace TransModel {

EndForcedOffEvent::EndForcedOffEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for) :
    scheduled_for_{scheduled_for}, person_{person}, jail_{jail}, canceled{false} {}


OffArtFlagEndEvent::OffArtFlagEndEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for) :
    EndForcedOffEvent(person, jail, scheduled_for) {}

void OffArtFlagEndEvent::operator()() {
    // might be dead prior to this event occuring
    if (!person_->isDead()) {
        if (!canceled) {
            std::cout << "offArtFlagEndEvent: " << scheduledFor() << " " << person_->id() << std::endl; 
            jail_->artOverrideEnded(person_);
            person_->setArtForcedOff(false);
        }
    }

}



OffPrepFlagEndEvent::OffPrepFlagEndEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for) :
    EndForcedOffEvent(person, jail, scheduled_for) {}


void OffPrepFlagEndEvent::operator()() {
    // might be dead prior to this event occuring
    if (!person_->isDead()) {
        jail_->prepOverrideEnded(person_);
        if (!canceled) {
            person_->setPrepForcedOff(false);
        }
    }
}



}
