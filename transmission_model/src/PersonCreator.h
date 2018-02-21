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
#include "TestingConfigurator.h"
#include "PREPAdherenceConfigurator.h"
#include "TransmissionRunner.h"
#include "common.h"
#include "GeometricDistribution.h"
#include "ARTLagCalculator.h"

namespace TransModel {


class PersonCreator {

private:
	int id;
	std::shared_ptr<TransmissionRunner> trans_runner_;
	TestingConfigurator testing_configurator;
	PREPAdherenceConfigurator prep_adherence_configurator;
	double detection_window_;
	ARTLagCalculator art_lag_calculator;

public:
	PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner, double detection_window,  ARTLagCalculator);
	virtual ~PersonCreator();

	PersonPtr operator()(Rcpp::List& val, double model_tick, double burnin_last_tick);
	PersonPtr operator()(double tick, float age);

	void updateTesting(std::shared_ptr<Person> p, double size_of_timestep);
	void updatePREPAdherence(std::shared_ptr<Person> p);
};

} /* namespace TransModel */

#endif /* SRC_PERSONCREATOR_H_ */
