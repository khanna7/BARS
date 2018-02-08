/*
 * GeometricDistribution.h
 *
 *  Created on: May 18, 2016
 *      Author: nick
 */

#ifndef SRC_GEOMETRICDISTRIBUTION_H_
#define SRC_GEOMETRICDISTRIBUTION_H_

#include "boost/random/geometric_distribution.hpp"

namespace TransModel {

class GeometricDistribution {

private:
	boost::random::geometric_distribution<> dist;
	double increment_;

public:
	GeometricDistribution(double p, double increment = 0);
	virtual ~GeometricDistribution();

	double next();
};

} /* namespace TransModel */

#endif /* SRC_GEOMETRICDISTRIBUTION_H_ */
