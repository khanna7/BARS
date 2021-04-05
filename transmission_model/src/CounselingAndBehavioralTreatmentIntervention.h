#ifndef COUNSELINGANDBEHAVIORALTREATMENTINTERVENTION_H
#define COUNSELINGANDBEHAVIORALTREATMENTINTERVENTION_H

#include <memory>
#include <vector>

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "GeometricDistribution.h"
#include "common.h"
#include "Person.h"

namespace TransModel {

class CounselingAndBehavioralTreatmentIntervention {
private:
    GeometricDistribution cessation_generator;
    double proportion_on_;
    unsigned int length_;
    std::vector<std::shared_ptr<Person>> candidates;
    unsigned int total_substance_users;

public:
    CounselingAndBehavioralTreatmentIntervention(double proortion_on, int length);
    virtual ~CounselingAndBehavioralTreatmentIntervention();

    void reset();
    void processPerson(std::shared_ptr<Person>& person);
    void run(double tick);

    void putOnTreatment(double tick, std::shared_ptr<Person>& person);


};

}
#endif // COUNSELINGANDBEHAVIORALTREATMENTINTERVENTION_H
