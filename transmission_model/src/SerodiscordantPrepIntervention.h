#ifndef SRC_SERODISCORDANTPREPINTERVENTION_H_
#define SRC_SERODISCORDANTPREPINTERVENTION_H_

#include <vector>

#include "Network.h"
#include "Person.h"
#include "PrepInterventionManager.h"


namespace TransModel {

using EdgeFilterPtr = bool(*)(EdgePtr<Person>);

class SerodiscordantPrepIntervention : public PrepIntervention {

private:
    std::vector<std::shared_ptr<Person>> candidates;
    AgeFilterPtr filter_;
    double age_threshold_, k;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;
    EdgeFilterPtr edge_filter;

public:
    SerodiscordantPrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold,
        NetworkType type);
    virtual ~SerodiscordantPrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, Network<Person>& network) override;
    void onYearEnded() override;

};

}

#endif