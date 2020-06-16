/*
 * PartnerWasJailedExpirationEvent.h
 *
 *  Created on: June 11, 2020
 *      Author: Daniel Sheeler
 */

#ifndef SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_
#define SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class PartnerWasJailedExpirationEvent : public repast::Functor {

private:
    Person *person_;
    int tick_;

public:
    PartnerWasJailedExpirationEvent(Person *person, int tick);
    virtual ~PartnerWasJailedExpirationEvent();

    void operator()();
};

} /* namespace TransModel */

#endif /* SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_ */
