#include "repast_hpc/Random.h"

#include "PUExtra.h"

namespace TransModel {

PUExtra::PUExtra(double increment, double p, int year) : increment_{increment}, p_{p}, base_prob{0},
    cessation_generator(p, 1.1) 
{
    updateBaseProbability(year);
}

PUExtra::~PUExtra() {}

void PUExtra::updateBaseProbability(int year) {
    double k = increment_ * year;
    base_prob = (p_ * k) / (1 - k);
}

void PUExtra::postRun() {
    neg_count = 0;
}

void PUExtra::incrementNegativeCount() {
    ++neg_count;
}

void PUExtra::preRun(double denom) {
    boosted_prob = base_prob * (neg_count / denom);
}

bool PUExtra::evaluate() {
    return repast::Random::instance()->nextDouble() <= boosted_prob;
}

double PUExtra::prepDelay() {
    return cessation_generator.next();
}


}