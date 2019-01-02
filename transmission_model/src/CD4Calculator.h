/*
 * CD4Calculator.h
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#ifndef SRC_CD4CALCULATOR_H_
#define SRC_CD4CALCULATOR_H_

#include "DiseaseParameters.h"

namespace TransModel {

struct BValues {
    float b1_ref, b2_african, b3_female,
        b4_cd4_ref, b5_african, b6_age_15to29,
        b6_age_30to39, b6_age_40to49, b6_age_50ormore;
};

/**
 * Calculates the current CD4.
 */
class CD4Calculator {

private:
    float size_of_timestep_, cd4_recovery_time_,
    cd4_at_infection_male_, per_day_cd4_recovery_;
    BValues b_values_;

    float getB6AgeValue(float age);

public:
    CD4Calculator(float size_of_timestep, float cd4_recovery_time,
            float cd4_at_infection_male, float per_day_cd4_recovery, BValues& b_values);

    virtual ~CD4Calculator();

    /**
     * Assumes infected individual.
     */
    float calculateCD4(float age, const InfectionParameters& infection_params);
};

} /* namespace TransModel */

#endif /* SRC_CD4CALCULATOR_H_ */
