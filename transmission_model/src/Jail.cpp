/*
 * Jail.cpp
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Jail.h"
#include "Parameters.h"

namespace TransModel {

ReleaseEvent::ReleaseEvent(std::shared_ptr<Person> person, Jail* jail) : person_(person), jail_(jail)
{}

ReleaseEvent::~ReleaseEvent() {}

void ReleaseEvent::operator()() {
    if (!person_->isDead()) {
        double tick = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
        jail_->releasePerson(tick, person_);
    }
}

Jail::Jail(Network<Person>* net) : net_(net) {}

Jail::~Jail() {}

int get_jail_time() {
    int jail_term_from =  Parameters::instance()->getIntParameter(INPUT_LOWER_JAIL_TERM_PROB);
    int jail_term_to   =  Parameters::instance()->getIntParameter(INPUT_UPPER_JAIL_TERM_PROB);
    repast::IntUniformGenerator jail_term_gen = repast::Random::instance()->createUniIntGenerator(jail_term_from, jail_term_to);
    return jail_term_gen.next();
}

/**
* Add a person to the jail (list) 
*/ 
//template<typename V>
void Jail::addPerson(double tick, PersonPtr person) {
    int serving_time = get_jail_time();
    // std::cout << "+Jail:JailPerson, time: " << tick << ", duration: " << serving_time << ", person.id: " << person->id() << std::endl;
    if(std::find(jailed_pop.begin(), jailed_pop.end(), person) != jailed_pop.end()) {
        std::cout << "*****ERROR in Jail::addPerson:***** Person is already in jail" << std::endl;
    } else {
        person->jailed(tick, serving_time);
        jailed_pop.push_back(person);

        std::vector<EdgePtr<Person>> edges;
        net_->getEdges(person, edges);

        jailed_pop_net.emplace(person->id(), edges);
        // std::cout << person->id() << std::endl;
        // for (auto edge : edges) {
        //     std::cout << "\tRemoved Edge: (" << edge->v1()->id() << ", " <<  edge->v2()->id() << ", " << edge->type() << ")" << std::endl;
        // }
        net_->removeVertex(person);
    }

    // schedule a release event for the person
    // + .1 to 
    double stop_time = tick + serving_time + .1;
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new ReleaseEvent(person, this)));
}

/**
* Release (remove) a person from the jail (list) 
*/ 
void Jail::releasePerson(double tick, PersonPtr person) {
    //std::cout << "-Jail::releasePerson(), time: " << tick << ", person.id: " << person->id() << std::endl;
    person->releasedFromJail(tick);
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());
    //std::cout << "Net contains p: " << net_->contains(person) << std::endl;
    net_->addVertex(person);
    std::vector<EdgePtr<Person>> edges = jailed_pop_net.at(person->id());
   
    // TODO: set condom use from the old edge onto the new edge
    for (auto edge : edges) {
        PersonPtr source = edge->v1();
        PersonPtr target = edge->v2();
        
        // if person is source, then make sure target is still valid
        if ((source->id() == person->id() && !target->isJailed() && !target->isDead()) ||
        // if person is target, then make sure source is still valid
            (target->id() == person->id() && !source->isJailed() && !source->isDead())) {
                //std::cout << "Adding Edge: (" << source->id() << ", " << target->id() << ", " << edge->type() << ")" << std::endl;
                net_->addEdge(source, target, edge->type());
        }
    }
    jailed_pop_net.erase(person->id());
}

/**
* Remove a person from the jail (list) 
*/ 
void Jail::removeDeadPerson(double time, PersonPtr person) {
    // std::cout << "-Jail:removeDeadPerson(), time: " <<time << ", person.id: " << person->id() << std::endl;
    // person->releasedFromJail(time);  //should a dead person be set out of jail when dies? 
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());
    jailed_pop_net.erase(person->id());  
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
    for (auto& item : jailed_pop_net) {
        std::cout << item.first << ", ";
        for (auto edge : item.second) {
            std::cout << "[i" << edge->id() << ",t" << edge->type() << "(" << edge->v1()->id()<< "," << edge->v2()->id() << ")]" << std::endl;
        }
    }
    std::cout << std::endl << "===========================================" << std::endl;
}


} /* namespace TransModel */

