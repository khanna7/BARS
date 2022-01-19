#ifndef DRUGUSECESSATIONEVENT_H
#define DRUGUSECESSATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"
#include "DrugUse.h"

namespace TransModel {

class DrugUseCessationEvent : public repast::Functor {
private:
    PersonPtr person_;
    double timestamp_;
    SubstanceUseType drug_;
public:
    DrugUseCessationEvent(std::shared_ptr<Person> person, SubstanceUseType drug, double timestamp);
    virtual ~DrugUseCessationEvent();
    void operator()();
};

}
#endif // DRUGUSECESSATIONEVENT_H
