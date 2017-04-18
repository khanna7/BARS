/*
 * PrepCessationEvent.h
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#ifndef SRC_PREPCESSATIONEVENT_H_
#define SRC_PREPCESSATIONEVENT_H_

#include <memory>

#include "repast_hpc/Schedule.h"
#include "Person.h"

namespace TransModel {

class PrepCessationEvent : public repast::Functor {

private:
	PersonPtr person_;
	double timestamp_;

public:
	PrepCessationEvent(std::shared_ptr<Person> person, double timestamp);
	virtual ~PrepCessationEvent();

	void operator()();
};

} /* namespace TransModel */

#endif /* SRC_PREPCESSATIONEVENT_H_ */
