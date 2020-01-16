/*
 *OffArtFlagEndEvent.h
 *
 *  Created on: Oct 12 , 2019
 *      Author: Babak
 */

#ifndef SRC_OFFARTFLAGENDEVENT_H_
#define SRC_OFFARTFLAGENDEVENT_H_

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class Jail;

class OffArtFlagEndEvent : public repast::Functor {

private:
    PersonPtr person_;
    Jail* jail_;
    bool canceled;
    //double timestamp_;

public:
    OffArtFlagEndEvent(std::shared_ptr<Person> person, Jail* jail);
    virtual ~OffArtFlagEndEvent();

    void operator()();
    void cancel() {
        canceled = true;
    }
};

} /* namespace TransModel */

#endif /* SRC_OFFARTFLAGENDEVENT_H_ */
