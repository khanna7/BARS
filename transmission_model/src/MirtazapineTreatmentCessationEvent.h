#ifndef MIRTAZAPINETREATMENTCESSATIONEVENT_H
#define MIRTAZAPINETREATMENTCESSATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class MirtazapineTreatmentCessationEvent : public repast::Functor {
private:
    PersonPtr person_;
    double timestamp_;

public:
    MirtazapineTreatmentCessationEvent(std::shared_ptr<Person> person, double timestamp);
    virtual ~MirtazapineTreatmentCessationEvent();

    void operator()();
};

}
#endif // MIRTAZAPINETREATMENTCESSATIONEVENT_H
