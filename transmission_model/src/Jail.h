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
    std::vector<PersonPtr> jailed_pop;  //keep jailed person
    std::map<unsigned int, std::vector<EdgePtr<Person>>> jailed_pop_net; //map person ID to its network
    
    int total_jailed_=0;
    int total_jailed_with_hist_=0;        //total jailed who had previous jail history 
    int total_released_ =0;    
    int total_dead_in_jail_=0;      
    int total_ser_time_1day_week_=0;      //total jailed with serving time of 1 day till less than a week
    int total_ser_time_week_month_=0;     //total jailed with serving time of a week till less than a month   
    int total_ser_time_1month_6months_=0;  //total jailed with serving time of a month till less than 6 months   
    int total_ser_time_6months_1year_=0;   //total jailed with serving time of 6 months till less than 1 year   
    int total_ser_time_more_than_year_=0; //total jailed with serving time of more than a year

    int total_infected_inside_jail_=0;    //accumulative total of agents infected inside jail  


    void updateJailServingTimeStats(double serving_time);
    double retentionNetworkProbability(double serving_time);
    double vulnerabilityMean(double serving_time);

  public:
    Jail(Network<Person>* net);
    virtual ~Jail();

    std::map<unsigned int, std::vector<EdgePtr<Person>>>::iterator begin() { return jailed_pop_net.begin();}
    std::map<unsigned int, std::vector<EdgePtr<Person>>>::iterator end() { return jailed_pop_net.end();}

    void releasePerson(double tick, PersonPtr);
    void addPerson(double tick, PersonPtr person);
    void removeDeadPerson(double time, PersonPtr person);
    void runInternalInfectionTransmission(double time);

    bool isServingTimeCompleted(PersonPtr person, double time);
    void checkAndReleaseTimeServedPopulation(Network<Person>& net,double time);

    int populationSize();
    int infectedPopulationSize();
    int uninfectedPopulationSize();
    float infectionIncidence();
    
    int numberOfPersonsJailedAtTime(double);
    int numberOfPersonsToBeReleasedAtTime(double);
    float avgPopulationAge();
    float avgPopulationServingTime();
    int onArtPopulationSize();
    int onPrepPopulationSize();

    void printPopulationInfo(double time);
    void printPopulationID();
    void printPopulationServingTime();
    void printPopulationAge();
    void printPopulationIDsAndTheirNework();
    void printPopulationDiseaseInfo();
    void printPopulationInfoOnART();

    int totalJailed() {
      return total_jailed_;
    }

    int totalJailedWithPreviousHistory() {
      return total_jailed_with_hist_;
    }

    int totalReleased() {
      return total_released_;
    }

    int totalDeadInJail() {
      return total_dead_in_jail_;
    }

    int totalServingTime_1d_1w() {
      return total_ser_time_1day_week_;
    }

    int totalServingTime_1w_1m() {
      return total_ser_time_week_month_;
    }

    int totalServingTime_1m_6m() {
      return total_ser_time_1month_6months_;
    }

    int totalServingTime_6m_1y() {
      return total_ser_time_6months_1year_;
    }

    int totalServingTime_1y_plus() {
      return total_ser_time_more_than_year_;
    }

    int totalInfectedInsideJail() {
      return total_infected_inside_jail_;
    }

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
