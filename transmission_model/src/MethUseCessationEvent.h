#ifndef METHUSECESSATIONEVENT_H
#define METHUSECESSATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"
#include "MethUse.h"

namespace TransModel {

class MethUseCessationEvent : public repast::Functor {
private:
    PersonPtr person_;
    double timestamp_;
public:
    MethUseCessationEvent(std::shared_ptr<Person> person, double timestamp);
    virtual ~MethUseCessationEvent();
    void operator()();
};

}
#endif // METHUSECESSATIONEVENT_H
