#ifndef DRUGUSE_H
#define DRUGUSE_H

#include <memory>
#include <vector>

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "GeometricDistribution.h"
#include "common.h"
#include "Person.h"

namespace TransModel {

class DrugUse {
private:
    GeometricDistribution cessation_generator;
    double proportion_on_;
    unsigned int length_;
    SubstanceUseType drug_;
    std::vector<std::shared_ptr<Person>> candidates;
    unsigned int total_pop;
public:
    DrugUse(SubstanceUseType drug, double proortion_on, int length);
    virtual ~DrugUse();

    void reset();
    void processPerson(std::shared_ptr<Person>& person);
    void run(double tick);

    void putOnDrug(double tick, std::shared_ptr<Person>& person);

    static void goOnOffDrug(PersonPtr& person);

};

}
#endif // DRUGUSE_H
