/*
 * Person.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "Person.h"

using namespace Rcpp;

namespace TransModel {

Person::Person(int id, float age, bool circum_status) :
		id_(id), age_(age), circum_status_(circum_status),
		infection_parameters_(), infectivity_(0), prep_(false) {
}

//Person::Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth) : net(network), id_(id) {
//	setAge(0);
//	setInfected(false);
//	setTimeOfBirth(timeOfBirth);
//}

Person::~Person() {
}

void Person::infect(bool onART, float duration_of_infection) {
	infection_parameters_.art_status = onART;
	infection_parameters_.dur_inf_by_age = duration_of_infection;
	infection_parameters_.infection_status = true;
	infection_parameters_.time_since_art_init = 0;
	infection_parameters_.time_since_infection = 0;
}

void Person::setAge(float age) {
	age_ = age;
}

void Person::setCD4Count(float cd4_count) {
	infection_parameters_.cd4_count = cd4_count;
}

void Person::setViralLoad(float viral_load) {
	infection_parameters_.viral_load = viral_load;
}

void Person::setInfectivity(float infectivity) {
	infectivity_ = infectivity;
}

void Person::updateVitals(float size_of_timestep) {
	age_ += size_of_timestep / 365;
	if (infection_parameters_.infection_status) {
		++infection_parameters_.time_since_infection;
	}

	if (infection_parameters_.art_status) {
		++infection_parameters_.time_since_art_init;
	}
}

bool Person::deadOfAge(int max_age) {
	return age_ >= max_age;
}

bool Person::deadOfAIDS() {
	return infection_parameters_.infection_status && !infection_parameters_.art_status &&
			infection_parameters_.time_since_infection >= infection_parameters_.dur_inf_by_age;
}

} /* namespace TransModel */
