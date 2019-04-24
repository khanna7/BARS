/*
 * Jail.cpp
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#include "Jail.h"
//#include "Edge.h"
//#include "Network.h"
//#include "common.h"

namespace TransModel {

Jail::Jail() {
}

Jail::Jail(int capacity) {
    jailed_pop.reserve(capacity);
}

Jail::~Jail() {
}

/**
* Add a person to the jail (list) 
*/ 
//template<typename V>
void Jail::addPerson(PersonPtr person, VertexIter<Person>& iter, Network<Person>& net, double time, double serving_time) {
    std::cout << "+Jail:JailPerson, time: " << time << ", time: " << serving_time << ", person.id: " << person->id() << std::endl;
    if(std::find(jailed_pop.begin(), jailed_pop.end(), person) != jailed_pop.end()) {
        std::cout << "*****ERROR in Jail::addPerson:***** Person is already in jail" << std::endl;
    } else {
        person->getInJail(time, serving_time);
        jailed_pop.push_back(person);

        std::vector<EdgePtr<Person>> edges;
        net.getEdges(person, edges);

        jailed_pop_net.emplace(person, edges);

        //bool res = net.removeVertex(person->id());
        //bool res = net.removeVertex(person);
        iter = net.removeVertex(iter);

        //for (auto edge : edges) 
            //cout << "id: " << edge->id() << ", type: " << edge->type() << ", v1.id:" << edge->v1()->id()<< ", v2.id:" << edge->v2()->id() << endl;
    }
}

/**
* Release (remove) a person from the jail (list) 
*/ 
void Jail::releasePerson(PersonPtr person, Network<Person>& net, double time) {
    std::cout << "-Jail:ReleasePerson(), time: " << time << ", person.id: " << person->id() << std::endl;
    person->getOutJail(time);
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());

    std::map<PersonPtr, std::vector<EdgePtr<Person>>>::iterator it;
    it=jailed_pop_net.find(person);
    
    //std::cout << "show the id for found person: " << it->first->id()<< std::endl;
    std::vector<EdgePtr<Person>> edges;  
    edges = it->second; 

    net.addVertex(person);

    for (auto edge : edges) {
        //cout << "id: " << edge->id() << ", type: " << edge->type() << ", v1.id:" << edge->v1()->id()<< ", v2.id:" << edge->v2()->id() << endl;
        if (edge->v1()->id() == person->id()) {
            if ((!edge->v2()->isJailed()) && (!edge->v2()->isDead())) {
               net.addEdge(edge->v1()->id(), edge->v2()->id(), edge->type());
            }
        }
        else { //edge->v2()->id() == person->id()
            if ((!edge->v1()->isJailed()) && (!edge->v1()->isDead())) {
               net.addEdge(edge->v2()->id(), edge->v1()->id(), edge->type());
            }
        }
    }
    
    jailed_pop_net.erase (it);  
}

/**
* Remove a person from the jail (list) 
*/ 
void Jail::removeDeadPerson(PersonPtr person, double time) {
    std::cout << "-Jail:removeDeadPerson(), time: " <<time << ", person.id: " << person->id() << std::endl;
    person->getOutJail(time);  //should a dead person be set out of jail when dies? 
    person->setDead(true);
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());

    std::map<PersonPtr, std::vector<EdgePtr<Person>>>::iterator it;
    it=jailed_pop_net.find(person);
    jailed_pop_net.erase (it);  
}

/**
* Check (iterate over) the jail (list) and release (remove) a person from the jail 
*/ 
void Jail::checkAndReleaseTimeServedPopulation(Network<Person>& net, double time) {
    //std::cout << "-Jail:checkAndReleaseIfTimeServed(), time: " << time << std::endl;
    for (auto& p : jailed_pop) {
        if (isServingTimeCompleted(p, time)) {
            releasePerson(p, net, time);
        }
    }
}

/**
* Boolean function to verify whether the jail serving time of a person is completed 
*/ 
bool Jail::isServingTimeCompleted(PersonPtr person, double current_time) {
    //std::cout << "-Jail:isServingTimeCompleted, time: " << current_time << std::endl;
    if ((current_time - person->timeOfJail()) < person->jailServingTime()) {
        return false;
    }
    else return true;
}

/**
* Gets the jail population size  
*/ 
int  Jail::populationSize() {
    return jailed_pop.size();
}

/**
* Gets the average of ages of the jailed population  
*/ 
float Jail::avgPopulationAge() {
    //std::cout << "Jail:avgPopulationAge()" << std::endl;
    float sumOfAges=0;
    for (auto& p : jailed_pop) {
        sumOfAges+= p->age();
    }
    float avgAges = sumOfAges/jailed_pop.size();
    return avgAges;
}

/**
* Gets the average of serving times of the jailed population  
*/ 
float Jail::avgPopulationServingTime() {
    //std::cout << "Jail:avgPopulationServingTime()" << std::endl;
    float sumOfSevingTimes=0;
    for (auto& p : jailed_pop) {
        sumOfSevingTimes+= p->jailServingTime();
    }
    return sumOfSevingTimes/jailed_pop.size();
}

/**
* Debugging helper function to print the key informaton of the jail population  
*/ 
void Jail::printPopulationInfo(double current_time) {
    std::cout << "======Jail::printPopulationInfo: ========" << std::endl;
    std::cout << "---IDs:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    
    std::cout << std::endl << "---Time of jail:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->timeOfJail() << ", ";
    
    std::cout << std::endl << "---Age at first jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtFirstJail() << ", ";

    std::cout << std::endl << "---Age at time of jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtJail() << ", ";

    std::cout << std::endl << "---Age:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->age() << ", ";
    
    std::cout << std::endl << "---Serving time:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->jailServingTime() << ", ";

    std::cout << std::endl << "---Time since jailed:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << (current_time - p->timeOfJail()) << ", ";

    std::cout << std::endl << "---isServingTimeCompleted:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << isServingTimeCompleted(p,current_time) << ", ";

    std::cout << std::endl << "===========================================" << std::endl;
}

/**
* Debugging helper function to print the IDs of the jail population  
*/ 
void Jail::printPopulationID() {
    std::cout << "======Jail::printPopulationID: ========" << std::endl;
    //for(std::vector<T>::iterator it = jailed_pop.begin(); it != jailed_pop.end(); ++it) {
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    std::cout << std::endl << "===========================================" << std::endl;
}

/**
* Debugging helper function to print the jail serving times  of the jail population  
*/ 
void Jail::printPopulationServingTime() {
    std::cout << "======Jail::printPopulationServingTimes: =======" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->jailServingTime() << ", ";
    std::cout << std::endl << "==========================================" << std::endl;
}


/**
* Debugging helper function to print the age related informaton of the jail population  
*/ 
void Jail::printPopulationAge() {
    std::cout << "======Jail::printPopulationAge =========" << std::endl;
    std::cout << "---IDs:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    
    std::cout << std::endl << "---Time of jail:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->timeOfJail() << ", ";
    
    //std::cout << std::endl << "---Age at first jail:" << std::endl;
    //for (auto& p : jailed_pop)
        //std::cout << p->ageAtFirstJail() << ", ";

    std::cout << std::endl << "---Age at time of jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtJail() << ", ";

    std::cout << std::endl << "---Age:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->age() << ", ";

    std::cout << std::endl << "---Age change since jailed:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << (p->age() - p->ageAtJail()) << ", ";


    std::cout << std::endl << "===========================================" << std::endl;
}

void Jail::printPopulationIDsAndTheirNework() {
    std::cout << "======Jail::printPopulationIDsAndTheirNework: ========" << std::endl;
    //for(std::vector<T>::iterator it = jailed_pop.begin(); it != jailed_pop.end(); ++it) {
    for (auto& pn : jailed_pop_net) {
        std::cout << pn.first->id() << ", ";
        for (auto edge : pn.second) {
            std::cout << "[i" << edge->id() << ",t" << edge->type() << "(" << edge->v1()->id()<< "," << edge->v2()->id() << ")]" << std::endl;
        }
    }
    std::cout << std::endl << "===========================================" << std::endl;
}


} /* namespace TransModel */

