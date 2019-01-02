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
    float circumcision_multiplier_, condom_multiplier_,
    infective_insertive_multiplier_;
    float duration_;



public:
    TransmissionRunner(float circumcision_multiplier, float condom_multiplier,
            float infective_insertive_multiplier,  float duration);
    virtual ~TransmissionRunner();

    /**
     * Returns true if the infector has infected the infectee. Note that
     * this will not actually set the infected flag on the infectee, it
     * only runs the algorithm. It also assume that infector is infected
     * and infectee is uninfected.
     *
     * @param infector the infected person
     * @param infectee the uninfected partner
     * @param edge_type the type of edge (steady or casual)
     */
    bool determineInfection(PersonPtr& infector, PersonPtr& infectee, bool condom_used, int edge_type);

    /**
     * Sets the infection flag, time of infection etc on the specified person and
     * starts him or her on ART with some probability.
     */
    void infect(PersonPtr infectee, float time);

    /**
     * Gets the expected duration of the infection.
     */
    float durationOfInfection();


};

} /* namespace TransModel */

#endif /* SRC_TRANSMISSIONRUNNER_H_ */
