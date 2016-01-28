/*
 * Person.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include "RCpp.h"
#include "DiseaseParameters.h"

namespace TransModel {

class Person {

private:
	int id_, age_;
	InfectionParameters infection_parameters_;
	float infectivity_;
	bool prep_;

public:
	Person(int id, int age);

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

	bool onPrep() const {
		return prep_;
	}

	const InfectionParameters& infectionParameters() const {
		return infection_parameters_;
	}

	bool isYoung() const;

	bool isInfected() const {
		return infection_parameters_.infection_status;
	}

	float infectivity() const {
		return infectivity_;
	}

	float timeSinceInfection() const {
		return infection_parameters_.time_since_infection;
	}

	void setViralLoad(float viral_load);

	void setCD4Count(float cd4_count);

	void setInfectivity(float infectivity);

	void setAge(int age);

	/**
	 * Infects this Person and sets whether or not they are on ART
	 * and the duration of the infection.
	 */
	void infect(bool onART, float duration_of_infection);

	/**
	 * Updates age, etc. of person.
	 */
	void updateVitals(float size_of_timestep);

	/**
	 * Checks if person is dead of old age. This doesn't kill
	 * the person, it just checks.
	 */
	bool deadOfAge(int max_age);

	/**
	 * Checks if person is dead of AIDS. This doesn't kill
	 * the person, it just checks.
	 */
	bool deadOfAIDS();

};

} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
