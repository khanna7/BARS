#ifndef SRC_RELEASEDPARTNEREXPIRATIONEVENT_H
#define SRC_RELEASEDPARTNEREXPIRATIONEVENT_H

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class ReleasedPartnerExpirationEvent : public repast::Functor {
private:
    PersonPtr person_;
    int id_;
    double expiration_tick_;

public:
    ReleasedPartnerExpirationEvent(PersonPtr person, int id, double expiration_tick);
    virtual ~ReleasedPartnerExpirationEvent();

    void operator()();
};

void scheduleReleasedPartnerExpiration(PersonPtr Person, int id, double scheduled_at_tick);

} /* namespace TransModel */

#endif // SRC_RELEASEDPARTNEREXPIRATIONEVENT_H
