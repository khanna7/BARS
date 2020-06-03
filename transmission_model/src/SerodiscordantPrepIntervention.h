#ifndef SRC_SERODISCORDANTPREPINTERVENTION_H_
#define SRC_SERODISCORDANTPREPINTERVENTION_H_

#include <vector>
#include <map>

#include "Network.h"
#include "Person.h"
#include "PrepInterventionManager.h"

namespace TransModel {

using EdgeFilterPtr = bool(*)(EdgePtr<Person>);

class SerodiscordantPrepIntervention : public PrepIntervention {

private:
    std::map<unsigned int, std::shared_ptr<Person>> candidates;
    double k;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;
    EdgeFilterPtr edge_filter;

    void trimCandidates(std::vector<PersonPtr>& put_on_prep);

public:
    SerodiscordantPrepIntervention(PrepUptakeData& prep_data, std::vector<std::shared_ptr<PrepFilter>> filters,
        NetworkType type);
    virtual ~SerodiscordantPrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) override;
    void onYearEnded() override;

};

}

#endif
