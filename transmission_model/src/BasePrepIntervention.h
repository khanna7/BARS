#ifndef SRC_BASEPREPINTERVENTION_H_
#define SRC_BASEPREPINTERVENTION_H_

#include <vector>

#include "Network.h"
#include "Person.h"
#include "PrepInterventionManager.h"
#include "PrepAgeFilter.h"


namespace TransModel {

class BasePrepIntervention : public PrepIntervention {

private:
    std::vector<std::shared_ptr<Person>> candidates;
    std::shared_ptr<PrepAgeFilter> filter_;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;

public:
    BasePrepIntervention(PrepUptakeData& prep_data, std::shared_ptr<PrepAgeFilter> filter);
    virtual ~BasePrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) override;
    void onYearEnded() override {}

};

}

#endif