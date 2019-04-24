/*
 * Jail.h
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#ifndef SRC_JAIL_H_
#define SRC_JAIL_H_

#include "Rcpp.h"
#include "Person.h"
#include "Network.h"
#include "Edge.h"
//#include <map>

namespace TransModel
{

class Jail
{

  private:
    friend Person;
    //unsigned int capacity_;
    std::vector<PersonPtr> jailed_pop;
    std::map<PersonPtr, std::vector<EdgePtr<Person>>> jailed_pop_net;

  public:
    Jail();
    Jail(int capacity);
    virtual ~Jail();

    std::map<PersonPtr, std::vector<EdgePtr<Person>>>::iterator begin() { return jailed_pop_net.begin();}
    std::map<PersonPtr, std::vector<EdgePtr<Person>>>::iterator end() { return jailed_pop_net.end();}

    //template<typename V>
    void addPerson(PersonPtr person, VertexIter<Person>& iter, Network<Person>& net, double time, double serving_time);

    void releasePerson(PersonPtr person, Network<Person>& net, double time);
    void removeDeadPerson(PersonPtr person, double time);
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

} /* namespace TransModel */

#endif /* SRC_JAIL_H_ */
