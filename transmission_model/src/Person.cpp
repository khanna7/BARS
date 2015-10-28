/*
 * Person.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "Person.h"

using namespace Rcpp;

namespace TransModel {

const std::string AGE = "age";
const std::string YOUNG = "young";
const std::string INFECTION_STATUS = "inf.status";
//const std::string TIME_SINCE_INFECTION = "time.since.infection";
const std::string INFECTION_TIME = "inf.time";
const std::string TIME_OF_DEATH = "time.of.death";
const std::string TIME_OF_BIRTH = "time.of.birth";


Person::Person(int id, std::shared_ptr<RNetwork> network) :
		net(network), id_(id) {
}

Person::Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth) : net(network), id_(id) {
	setAge(0);
	setInfected(false);
	setTimeOfBirth(timeOfBirth);
}

Person::~Person() {
	// deactivate when destroyed
	// std::cout << "deactivating " << id_ << std::endl;
	net->deactivateVertex(id_, R_NegInf, R_PosInf);
}

int Person::age() const {
	return net->getVertexAttribute<int>(id_, AGE);
}

int Person::id() const {
	return id_;
}

void Person::setTimeOfBirth(double time) {
	net->setVertexAttribute<double>(id_, TIME_OF_BIRTH, time);
}

bool Person::isYoung() const {
	return net->getVertexAttribute<double>(id_, YOUNG) == 1;
}

bool Person::isInfected() const {
	return net->getVertexAttribute<double>(id_, INFECTION_STATUS) == 1;
}

double Person::infectionTime() const {
	return net->getVertexAttribute<double>(id_, INFECTION_TIME);
}

void Person::setAge(int age) {
	net->setVertexAttribute<int>(id_, AGE, age);
	net->setVertexAttribute<double>(id_, YOUNG, age <= 15 ? 1 : 0);
}

void Person::setInfected(bool infected) {
	net->setVertexAttribute<double>(id_, INFECTION_STATUS, infected ? 1 : 0);
}

void Person::setInfectionTime(double ts) {
	net->setVertexAttribute<double>(id_, INFECTION_TIME, ts);
}

void Person::setTimeOfDeath(double time) {
	net->setVertexAttribute<double>(id_, TIME_OF_DEATH, time);
}

void Person::incrementAge() {
	int age = this->age();
	setAge(age + 1);
}

} /* namespace TransModel */
