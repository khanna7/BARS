/*
 *OffPrepFlagEndEvent.h
 *
 *  Created on: Oct 12 , 2019
 *      Author: Babak
 */

#ifndef SRC_OFFPREPFLAGENDEVENT_H_
#define SRC_OFFPREPFLAGENDEVENT_H_

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class OffPrepFlagEndEvent : public repast::Functor {

private:
    PersonPtr person_;
    //double timestamp_;

public:
    OffPrepFlagEndEvent(std::shared_ptr<Person> person);
    virtual ~OffPrepFlagEndEvent();

    void operator()();
};

} /* namespace TransModel */

#endif /* SRC_OFFPREPFLAGENDEVENT_H_ */
