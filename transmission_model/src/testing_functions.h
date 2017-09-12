/*
 * testing_functions.h
 *
 *  Created on: Sep 11, 2017
 *      Author: nick
 */

#ifndef SRC_TESTING_FUNCTIONS_H_
#define SRC_TESTING_FUNCTIONS_H_

#include "ProbDist.h"

namespace TransModel {

class TestingDist {

private:
	int min_, max_;

public:
	TestingDist(int min, int max);
	double next(double size_of_timestep);
};

ProbDist<TestingDist> create_testing_dist();

}


#endif /* SRC_TESTING_FUNCTIONS_H_ */
