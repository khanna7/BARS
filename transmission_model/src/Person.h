/*
 * Person.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include "RCpp.h"

#include "RNetwork.h"

namespace TransModel {

class Person {

private:
	std::shared_ptr<RNetwork> net;
	int id_;

public:
	Person(int id, std::shared_ptr<RNetwork> network);
	Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth);

	virtual ~Person();

	/**
	 * Gets the id of this person. This is the C-style idx
	 * of the vertex. R idx is this + 1.
	 */

	int id() const;
	/**
	 * Gets the age of this Person.
	 */
	int age() const;

	bool isYoung() const;

	bool isInfected() const;

	//int timeSinceInfection() const;

	double infectionTime() const;

	void setAge(int age);

	void setInfected(bool infected);

	//void setTimeSinceInfection(int tsi);

	void setInfectionTime(double time);

	void setTimeOfDeath(double time);

	void setTimeOfBirth(double time);

	/**
	 * Increments the persons age by one. If age is > 15
	 * then young property is set to false.
	 */
	void incrementAge();
};


} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
