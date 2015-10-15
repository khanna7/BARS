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


Person::Person(int id, std::shared_ptr<RNetwork> network) :
		net(network), id_(id) {
}

Person::~Person() {
}

int Person::age() const {
	return net->getVertexAttribute<int>(id_, AGE);
}

int Person::id() const {
	return id_;
}

bool Person::isYoung() const {
	return net->getVertexAttribute<bool>(id_, YOUNG);
}

bool Person::isInfected() const {
	return net->getVertexAttribute<bool>(id_, INFECTION_STATUS);
}

int Person::infectionTime() const {
	return net->getVertexAttribute<int>(id_, INFECTION_TIME);
}

void Person::setAge(int age) {
	net->setVertexAttribute<int>(id_, AGE, age);
}

void Person::setInfected(bool infected) {
	net->setVertexAttribute<bool>(id_, INFECTION_STATUS, infected);
}

void Person::setInfectionTime(int ts) {
	net->setVertexAttribute<int>(id_, INFECTION_TIME, ts);
}

} /* namespace TransModel */
