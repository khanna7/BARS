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

	virtual ~Person();

	/**
	 * Gets the id of this person. This is also the vertex idx into
	 * the R network.
	 */

	int id() const;
	/**
	 * Gets the age of this Person.
	 */
	int age() const;

	bool isYoung() const;

	bool isInfected() const;

	//int timeSinceInfection() const;

	int infectionTime() const;

	void setAge(int age);

	void setInfected(bool infected);

	//void setTimeSinceInfection(int tsi);

	void setInfectionTime(int timestamp);

};


} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
