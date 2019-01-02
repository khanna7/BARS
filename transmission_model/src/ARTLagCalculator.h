/*
 * ARTLagCalculator.h
 *
 *  Created on: Oct 10, 2017
 *      Author: nick
 */

#ifndef SRC_ARTLAGCALCULATOR_H_
#define SRC_ARTLAGCALCULATOR_H_

#include<memory>

#include "DayRangeCalculator.h"
#include "Person.h"

namespace TransModel {

class ARTLagCalculator {

private:
    std::shared_ptr<DayRangeCalculator> lower_, upper_;
    float threshold_;

public:
    /**
     * @param lower the calculator to use for persons < the threshold
     * @param upper the calculator to use for persons >=  the threshold
     */
    ARTLagCalculator(std::shared_ptr<DayRangeCalculator> lower, std::shared_ptr<DayRangeCalculator> upper, float threshold);
    virtual ~ARTLagCalculator();

    double calculateLag(const std::shared_ptr<Person> p, float size_of_timestep);
};

} /* namespace TransModel */

#endif /* SRC_ARTLAGCALCULATOR_H_ */
