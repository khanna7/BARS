#ifndef METHUSE_H
#define METHUSE_H

#include <memory>
#include <vector>

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "GeometricDistribution.h"
#include "common.h"
#include "Person.h"

namespace TransModel {

class MethUse {
private:
    GeometricDistribution cessation_generator;
    double proportion_on_;
    unsigned int length_;
    std::vector<std::shared_ptr<Person>> candidates;
    unsigned int total_pop;
public:
    MethUse(double proortion_on, int length);
    virtual ~MethUse();

    void reset();
    void processPerson(std::shared_ptr<Person>& person);
    void run(double tick);

    void putOnMeth(double tick, std::shared_ptr<Person>& person);


};

}
#endif // METHUSE_H
