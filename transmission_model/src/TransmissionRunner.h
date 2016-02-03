/*
 * TransmissionRunner.h
 *
 *  Created on: Jan 19, 2016
 *      Author: nick
 */

#ifndef SRC_TRANSMISSIONRUNNER_H_
#define SRC_TRANSMISSIONRUNNER_H_

#include<vector>

#include "repast_hpc/Random.h"

#include "common.h"

namespace TransModel {

class TransmissionRunner {

private:
	float circumcision_multiplier_, prep_multiplier_;
	std::vector<float> dur_inf_by_age;

public:
	TransmissionRunner(float circumcision_multiplier, float prep_multiplier,
			std::vector<float>& given_dur_inf_by_age);
	virtual ~TransmissionRunner();

	/**
	 * Returns true if the infector has infected the infectee. Note that
	 * this will not actually set the infected flag on the infectee, it
	 * only runs the algorithm. It also assume that infector is infected
	 * and infectee is uninfected.
	 *
	 * @param infector the infected person
	 * @param infectee the uninfected partner
	 */
	bool determineInfection(PersonPtr infector, PersonPtr infectee);

	/**
	 * Sets the infection flag, time of infection etc on the specified person and
	 * starts him or her on ART with some probability.
	 */
	void infect(PersonPtr infectee);


};

} /* namespace TransModel */

#endif /* SRC_TRANSMISSIONRUNNER_H_ */
