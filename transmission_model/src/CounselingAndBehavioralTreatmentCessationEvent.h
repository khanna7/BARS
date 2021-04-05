#ifndef COUNSELINGANDBEHAVIORALTREATMENTCESSATIONEVENT_H
#define COUNSELINGANDBEHAVIORALTREATMENTCESSATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class CounselingAndBehavioralTreatmentCessationEvent : public repast::Functor {
private:
    PersonPtr person_;
    double timestamp_;

public:
    CounselingAndBehavioralTreatmentCessationEvent(std::shared_ptr<Person> person, double timestamp);
     virtual ~CounselingAndBehavioralTreatmentCessationEvent();

     void operator()();
};

}
#endif // COUNSELINGANDBEHAVIORALTREATMENTCESSATIONEVENT_H
