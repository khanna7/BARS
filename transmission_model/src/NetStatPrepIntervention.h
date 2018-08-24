#ifndef SRC_NETSTATPREPINTERVENTION_H_
#define SRC_NETSTATPREPINTERVENTION_H_

#include <vector>

#include "NetworkStats.h"
#include "Network.h"
#include "Person.h"
#include "NetworkStats.h"
#include "PrepInterventionManager.h"


namespace TransModel {

class NetStatPrepIntervention : public PrepIntervention {

private:
    AgeFilterPtr filter_;
    double age_threshold_, k;
    PrepUptakeData prep_data_;
    unsigned int total_negatives;
    float top_n_;
    unsigned int candidate_count;

public:
    NetStatPrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold,
        float top_n);
    virtual ~NetStatPrepIntervention();

    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, Network<Person>& network) override {}
    void run(double tick, std::vector<std::shared_ptr<Person>> ranked_persons);
    void onYearEnded() override;
};


using Ranker = void(*)(NetworkStats<Person>&, std::vector<std::shared_ptr<Person>>&);

void degree_ranker(NetworkStats<Person>&, std::vector<std::shared_ptr<Person>>&);
void eigen_ranker(NetworkStats<Person>&, std::vector<std::shared_ptr<Person>>&);

class CompositeNetStatPrepIntervention : public IPrepIntervention {

private:
    std::vector<std::shared_ptr<NetStatPrepIntervention>> interventions;
    Ranker ranker_;
    
public:
    CompositeNetStatPrepIntervention(Ranker ranker);
    ~CompositeNetStatPrepIntervention();
    void addNetIntervention(std::shared_ptr<NetStatPrepIntervention> intervention);
    void reset() override;
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, Network<Person>& network) override;
    void onYearEnded() override;
};

}

#endif