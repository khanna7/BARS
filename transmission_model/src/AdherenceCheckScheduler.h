/*
 * AdherenceCheckScheduler.h
 *
 *  Created on: Sep 23, 2016
 *      Author: nick
 */

#ifndef SRC_ADHERENCECHECKSCHEDULER_H_
#define SRC_ADHERENCECHECKSCHEDULER_H_

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class ARTAdherenceCheckScheduler : public repast::Functor {
private:
	PersonPtr person_;
	double timestamp_;
public:
	ARTAdherenceCheckScheduler(std::shared_ptr<Person> person, double timestamp);
	virtual ~ARTAdherenceCheckScheduler();

	void operator()();
};

} /* namespace TransModel */

#endif /* SRC_ADHERENCECHECKSCHEDULER_H_ */
