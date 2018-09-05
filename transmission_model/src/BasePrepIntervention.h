#ifndef SRC_BASEPREPINTERVENTION_H_
#define SRC_BASEPREPINTERVENTION_H_

#include <vector>

#include "Network.h"
#include "Person.h"
#include "PrepInterventionManager.h"


namespace TransModel {

class BasePrepIntervention : public PrepIntervention {

private:
    std::vector<std::shared_ptr<Person>> candidates;
    AgeFilterPtr filter_;
    double age_threshold_;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;
    double orig_k;

public:
    BasePrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold);
    virtual ~BasePrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) override;
    void onYearEnded() override {}

};

}

#endif