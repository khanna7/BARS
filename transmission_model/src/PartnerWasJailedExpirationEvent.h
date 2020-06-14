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
    PersonPtr person_;
    int to_false_tick_;

public:
    PartnerWasJailedExpirationEvent(std::shared_ptr<Person> person, int to_false_tick);
    virtual ~PartnerWasJailedExpirationEvent();

    void operator()();
};

} /* namespace TransModel */

#endif /* SRC_PARTNERWASJAILEDEXPIRATIONEVENT_H_ */
