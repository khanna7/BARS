/*
 * Person.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "Person.h"

using namespace Rcpp;

namespace TransModel {

const double YOUNG_THRESHOLD = 16;

Person::Person(int id, int age, bool infected) : id_(id), age_(age), infected_(infected), time_of_infection(0),
		time_of_birth(-1) {
}

//Person::Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth) : net(network), id_(id) {
//	setAge(0);
//	setInfected(false);
//	setTimeOfBirth(timeOfBirth);
//}

Person::~Person() {
}


void Person::setTimeOfBirth(double time) {
	time_of_birth = time;
}

bool Person::isYoung() const {
	return age_ < YOUNG_THRESHOLD;
}

void Person::setAge(int age) {
	age_ = age;
}

void Person::setInfected(bool infected, double time) {
	infected_ = infected;
	time_of_infection = time;
}

void Person::incrementAge() {
	++age_;
}

} /* namespace TransModel */
