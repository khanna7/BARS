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

class OffArtFlagEndEvent : public repast::Functor {

private:
    PersonPtr person_;
    //double timestamp_;

public:
    OffArtFlagEndEvent(std::shared_ptr<Person> person);
    virtual ~OffArtFlagEndEvent();

    void operator()();
};

} /* namespace TransModel */

#endif /* SRC_OFFARTFLAGENDEVENT_H_ */
