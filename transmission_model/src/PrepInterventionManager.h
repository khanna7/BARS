#ifndef SRC_PREPINTERVENTIONMANAGER_H_
#define SRC_PREPINTERVENTIONMANAGER_H_

#include <memory>
#include <vector>

#include "Network.h"
#include "GeometricDistribution.h"
#include "PrepStatus.h"
#include "common.h"


namespace TransModel {

using AgeFilterPtr = bool(*)(PersonPtr, double);

bool gte(PersonPtr person, double threshold); 
bool lt(PersonPtr person, double threshold); 

struct PrepUptakeData {
    // k, p
    double use, stop;
    // used to initialize the cessation generator
    double cessation_stop;
    double increment;
    int years_to_increment;
};

std::ostream& operator <<(std::ostream& stream, const PrepUptakeData& prep_data);

class IPrepIntervention {
public:
    IPrepIntervention() {}
    virtual ~IPrepIntervention() {}

    virtual void reset() = 0;
    virtual void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) = 0;
    virtual void run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) = 0;
    virtual void onYearEnded() = 0;
};


class PrepIntervention : public IPrepIntervention {

private:
    GeometricDistribution cessation_generator;

protected:
    int year;

public:
    PrepIntervention(PrepUptakeData& data);
    virtual ~PrepIntervention();

    virtual void reset() = 0;
    virtual void processPerson(std::shared_ptr<Person>& person, Network<Person>& network) = 0;
    virtual void run(double tick,  std::vector<PersonPtr>& put_on_prep, Network<Person>& network) = 0;
    virtual void onYearEnded() = 0;

    void putOnPrep(double tick, std::shared_ptr<Person>& person, PrepStatus cause);

};

class PrepInterventionManager {

private:
    std::vector<std::shared_ptr<IPrepIntervention>> interventions;

public:
    PrepInterventionManager();
    ~PrepInterventionManager();

    void addIntervention(std::shared_ptr<IPrepIntervention> intervention);
    void processPerson(std::shared_ptr<Person>& person, Network<Person>& network);
    void run(double tick, Network<Person>& network);
    void onYearEnded();
    void reset();
};

}

#endif