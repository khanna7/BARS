/*
 * Person.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "Person.h"

using namespace Rcpp;

namespace TransModel {

Person::Person(int id, float age, bool circum_status, int role, Diagnoser<GeometricDistribution>& diagnoser) :
		id_(id), role_(role), age_(age), circum_status_(circum_status),
		infection_parameters_(), infectivity_(0), prep_(false), dead_(false), diagnoser_(diagnoser) {
}

//Person::Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth) : net(network), id_(id) {
//	setAge(0);
//	setInfected(false);
//	setTimeOfBirth(timeOfBirth);
//}

Person::~Person() {
}

void Person::infect(bool art_covered, float duration_of_infection, float time) {
	infection_parameters_.art_covered = art_covered;
	infection_parameters_.dur_inf_by_age = duration_of_infection;
	infection_parameters_.infection_status = true;
	infection_parameters_.time_since_infection = 0;
	infection_parameters_.age_at_infection = age_;
	infection_parameters_.time_of_infection = time;
}


void Person::setAge(float age) {
	age_ = age;
}

void Person::setCD4Count(float cd4_count) {
	infection_parameters_.cd4_count = cd4_count;
}

void Person::setViralLoadARTSlope(float slope) {
	infection_parameters_.vl_art_traj_slope = slope;
}

void Person::setViralLoad(float viral_load) {
	infection_parameters_.viral_load = viral_load;
}

void Person::setInfectivity(float infectivity) {
	infectivity_ = infectivity;
}

void Person::putOnART(float time_stamp) {
	infection_parameters_.art_status = true;
	infection_parameters_.time_since_art_init = 0;
	infection_parameters_.time_of_art_init = time_stamp;
	infection_parameters_.cd4_at_art_init = infection_parameters_.cd4_count;
	infection_parameters_.vl_at_art_init = infection_parameters_.viral_load;
}

void Person::step(float size_of_timestep) {
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

bool Person::deadOfInfection() {
	return infection_parameters_.infection_status && !infection_parameters_.art_status &&
			infection_parameters_.time_since_infection >= infection_parameters_.dur_inf_by_age;
}

bool Person::diagnose(double tick) {
	return diagnoser_.test(tick, infection_parameters_);
}

} /* namespace TransModel */
