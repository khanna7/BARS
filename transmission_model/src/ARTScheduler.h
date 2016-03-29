/*
 * ARTScheduler.h
 *
 *  Created on: Feb 17, 2016
 *      Author: nick
 */

#ifndef SRC_ARTSCHEDULER_H_
#define SRC_ARTSCHEDULER_H_

#include "repast_hpc/Schedule.h"

#include "common.h"

namespace TransModel {

/**
 * Schedulable functor that puts specified persons on ART.
 *
 */
class ARTScheduler : public repast::Functor {
private:
	float time_stamp_;
	std::vector<PersonPtr> persons;

public:
	ARTScheduler(float time_stamp);
	virtual ~ARTScheduler();
	void operator()();

	void addPerson(PersonPtr person);

	float artAtTick() const {
		return time_stamp_;
	}
};

} /* namespace TransModel */

#endif /* SRC_ARTSCHEDULER_H_ */
