/*
 * PersonCreator.h
 *
 *  Created on: Feb 15, 2016
 *      Author: nick
 */

#ifndef SRC_PERSONCREATOR_H_
#define SRC_PERSONCREATOR_H_

#include <memory>

#include "RInside.h"

#include "Person.h"
#include "TransmissionRunner.h"
#include "common.h"
#include "GeometricDistribution.h"

namespace TransModel {


class PersonCreator {

private:
	int id;
	std::shared_ptr<TransmissionRunner> trans_runner_;
	std::shared_ptr<GeometricDistribution> dist;
	double detection_window_;

public:
	PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner, double daily_testing_prob, double detection_window);
	virtual ~PersonCreator();

	PersonPtr operator()(Rcpp::List& val, double tick);
	PersonPtr operator()(double tick, float age);
};

} /* namespace TransModel */

#endif /* SRC_PERSONCREATOR_H_ */
