/*
 * ARTLagCalculator.cpp
 *
 *  Created on: Oct 10, 2017
 *      Author: nick
 */

#include "ARTLagCalculator.h"

namespace TransModel {

ARTLagCalculator::ARTLagCalculator(std::shared_ptr<DayRangeCalculator> lower,
		std::shared_ptr<DayRangeCalculator> upper, float threshold) : lower_{lower}, upper_{upper}, threshold_{threshold} {
}

ARTLagCalculator::~ARTLagCalculator() {
}

double ARTLagCalculator::calculateLag(const std::shared_ptr<Person> p, float size_of_timestep) {
	if (p->age() < threshold_) {
		return lower_->calculateLag(size_of_timestep);
	} else {
		return upper_->calculateLag(size_of_timestep);
	}
}

} /* namespace TransModel */
