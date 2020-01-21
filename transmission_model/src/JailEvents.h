#ifndef SRC_JAILEVENTS_H
#define SRC_JAILEVENTS_H

#include "repast_hpc/Schedule.h"

#include "Person.h"

namespace TransModel {

class Jail;

class EndForcedOffEvent : public repast::Functor {

private:
    double scheduled_for_;

protected:
    std::shared_ptr<Person> person_;
    Jail* jail_;
    bool canceled;

public:
    EndForcedOffEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for);
    virtual ~EndForcedOffEvent() {}

    virtual void operator()() = 0;

    void cancel() {
        canceled = true;
    }

    double scheduledFor() const {
        return scheduled_for_;
    }
};


class OffArtFlagEndEvent : public EndForcedOffEvent {

public:
    OffArtFlagEndEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for);
    virtual ~OffArtFlagEndEvent() {}
    virtual void operator()() override;
};

class OffPrepFlagEndEvent : public EndForcedOffEvent {

public:
    OffPrepFlagEndEvent(std::shared_ptr<Person> person, Jail* jail, double scheduled_for);
    virtual ~OffPrepFlagEndEvent() {}

    virtual void operator()() override;
};


}


#endif