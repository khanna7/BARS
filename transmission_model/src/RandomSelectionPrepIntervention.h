#ifndef SRC_RANDOMSELECTIONPREPINTERVENTION_H_
#define SRC_RANDOMSELECTIONPREPINTERVENTION_H_

#include <vector>

#include "Network.h"
#include "Person.h"
#include "PrepInterventionManager.h"
#include "PrepFilter.h"


namespace TransModel {

class RandomSelectionPrepIntervention : public PrepIntervention {

private:
    std::map<unsigned int, std::shared_ptr<Person>> candidates;
    double k;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;

    void trimCandidates(std::vector<PersonPtr>& put_on_prep);

public:
    RandomSelectionPrepIntervention(PrepUptakeData& prep_data, std::vector<std::shared_ptr<PrepFilter>> filters);
    virtual ~RandomSelectionPrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) override;
    void onYearEnded() override;

};

}

#endif
