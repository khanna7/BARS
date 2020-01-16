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

class Jail;

class OffPrepFlagEndEvent : public repast::Functor {

private:
    PersonPtr person_;
    Jail* jail_;
    bool canceled;
    //double timestamp_;

public:
    OffPrepFlagEndEvent(std::shared_ptr<Person> person, Jail* jail);
    virtual ~OffPrepFlagEndEvent();

    void operator()();

    void cancel() {
        canceled = true;
    }
};

} /* namespace TransModel */

#endif /* SRC_OFFPREPFLAGENDEVENT_H_ */
