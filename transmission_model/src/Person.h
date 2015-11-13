/*
 * Person.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include "RCpp.h"

namespace TransModel {

class Person {

private:

	int id_, age_;
	bool infected_;
	double time_of_infection, time_of_birth;

public:
	Person(int id, int age, bool infected);

	virtual ~Person();

	/**
	 * Gets the id of this person.
	 */

	int id() const {
		return id_;
	}

	/**
	 * Gets the age of this Person.
	 */
	int age() const {
		return age_;
	}

	bool isYoung() const;

	bool isInfected() const {
		return infected_;
	}

	double timeOfInfection() const {
		return time_of_infection;
	}

	double timeOfBirth() const {
		return time_of_birth;
	}

	void setAge(int age);

	void setInfected(bool infected, double time);

	void setTimeOfBirth(double time);

	/**
	 * Increments the persons age by one. If age is > 15
	 * then young property is set to false.
	 */
	void incrementAge();
};


} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
