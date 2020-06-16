#ifndef SRC_RELEASEDPARTNEREXPIRATIONEVENT_H
#define SRC_RELEASEDPARTNEREXPIRATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class ReleasedPartnerExpirationEvent : public repast::Functor {
private:
    Person *person_;
    int id_;

public:
    ReleasedPartnerExpirationEvent(Person *person, int id);
    virtual ~ReleasedPartnerExpirationEvent();

    void operator()();
};

} /* namespace TransModel */

#endif // SRC_RELEASEDPARTNEREXPIRATIONEVENT_H
