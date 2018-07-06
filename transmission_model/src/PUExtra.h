#ifndef SRC_PUEXTRA_H_
#define SRC_PUEXTRA_H_

#include "PrepUptakeManager.h"

namespace TransModel {

class PUExtra {

private:
    double increment_, p_;
    double base_prob, boosted_prob;
    unsigned int neg_count;
    GeometricDistribution cessation_generator;

public:
    PUExtra(double increment, double p, int year = 1);
    ~PUExtra();

    void updateBaseProbability(int year);
    void incrementNegativeCount();
    void preRun(double denom);
    void postRun();
    bool evaluate();
    double prepDelay();
};

}

#endif