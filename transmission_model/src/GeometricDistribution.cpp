/*
 * GeometricDistribution.cpp
 *
 *  Created on: May 18, 2016
 *      Author: nick
 */

#include "repast_hpc/Random.h"

#include "GeometricDistribution.h"

namespace TransModel {

GeometricDistribution::GeometricDistribution(double p, double increment) : dist(p), increment_{increment} {}

GeometricDistribution::~GeometricDistribution() {
}

double GeometricDistribution::next() {
    return dist(repast::Random::instance()->engine()) + increment_;
}

} /* namespace TransModel */
