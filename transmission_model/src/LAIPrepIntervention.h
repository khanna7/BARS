#ifndef LAIPREPINTERVENTION_H
#define LAIPREPINTERVENTION_H

#include <vector>

#include "PrepInterventionManager.h"
#include "Network.h"
#include "PrepAgeFilter.h"
#include "Person.h"

namespace TransModel {

class LAIPrepIntervention : public PrepIntervention {
private:
    std::vector<std::shared_ptr<Person>> candidates;

public:
    LAIPrepIntervention(PrepUptakeData& prep_data);
    virtual ~LAIPrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) override;
    void onYearEnded() override {}

};
}
#endif // LAIPREPINTERVENTION_H
