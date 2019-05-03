/*
 * Jail.h
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#ifndef SRC_JAIL_H_
#define SRC_JAIL_H_

#include <map>

#include "repast_hpc/Schedule.h"

#include "Person.h"
#include "Network.h"
#include "Edge.h"


namespace TransModel {

class Jail {

  private:
    Network<Person>* net_;
    std::vector<PersonPtr> jailed_pop;
    std::map<unsigned int, std::vector<EdgePtr<Person>>> jailed_pop_net;

  public:
    Jail(Network<Person>* net);
    virtual ~Jail();

    std::map<unsigned int, std::vector<EdgePtr<Person>>>::iterator begin() { return jailed_pop_net.begin();}
    std::map<unsigned int, std::vector<EdgePtr<Person>>>::iterator end() { return jailed_pop_net.end();}

    void releasePerson(double tick, PersonPtr);
    void addPerson(double tick, PersonPtr person);
    void removeDeadPerson(double time, PersonPtr person);

    bool isServingTimeCompleted(PersonPtr person, double time);
    void checkAndReleaseTimeServedPopulation(Network<Person>& net,double time);

    int populationSize();
    float avgPopulationAge();
    float avgPopulationServingTime();

    void printPopulationInfo(double time);
    void printPopulationID();
    void printPopulationServingTime();
    void printPopulationAge();
    void printPopulationIDsAndTheirNework();
};

class ReleaseEvent : public repast::Functor {

private:
    PersonPtr person_;
    Jail* jail_;

public:
    ReleaseEvent(std::shared_ptr<Person> person, Jail* jail);
    virtual ~ReleaseEvent();

    void operator()();
};


} /* namespace TransModel */

#endif /* SRC_JAIL_H_ */
