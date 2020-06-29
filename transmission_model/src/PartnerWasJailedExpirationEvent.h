/*
 * PartnerWasJailedExpirationEvent.h
 *
 *  Created on: June 11, 2020
 *      Author: Daniel Sheeler
 */

#ifndef SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_
#define SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_

#include "repast_hpc/Schedule.h"
#include "repast_hpc/RepastProcess.h"

#include "Parameters.h"
#include "Person.h"

namespace TransModel {

class PartnerWasJailedExpirationEvent : public repast::Functor {

private:
    PersonPtr person_;
    int tick_;

public:
    PartnerWasJailedExpirationEvent(PersonPtr person, double tick);
    virtual ~PartnerWasJailedExpirationEvent();

    void operator()();
};

void schedulePartnerWasJailedExpiration(PersonPtr Person, double tick);

} /* namespace TransModel */

#endif /* SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_ */
