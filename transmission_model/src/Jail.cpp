/*
 * Jail.cpp
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak Mahdavi Ardestani
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Jail.h"
#include "Parameters.h"

#include "GeometricDistribution.h"
#include "Stats.h"

#include "PrintHelper.h"

#include "CSVReader.h"

#include "OffArtFlagEndEvent.h"
#include "OffPrepFlagEndEvent.h"

#include "CSVWriter.h"

using namespace repast;

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

Jail::Jail(Network<Person>* net) : net_(net),  net_decay_prob_main(), net_decay_prob_casual() {
    std::set<std::string> header = { "prep", "art"};
    //csv_writer.addHeader(header);

    string net_decay_prob_file_main = Parameters::instance()->getStringParameter(NETWORK_DECAY_PROB_MAIN_FILE);
    CSVReader reader_main(net_decay_prob_file_main);
    std::vector<double> v;
    while (reader_main.next(v)) {
        net_decay_prob_main.push_back(v[0]);
    }

    string net_decay_prob_file_casual = Parameters::instance()->getStringParameter(NETWORK_DECAY_PROB_CASUAL_FILE);
    CSVReader reader_casual(net_decay_prob_file_casual);
    while (reader_casual.next(v)) {
        net_decay_prob_casual.push_back(v[0]);
    }
}

Jail::~Jail() {}

double get_jail_time() {
 
    //double serving_time_prob = Parameters::instance()->getDoubleParameter(JAIL_SERVING_TIME_MEAN_PROB);
    double serving_time_mean = Parameters::instance()->getDoubleParameter(JAIL_SERVING_TIME_MEAN);
    GeometricDistribution jail_term_gen = GeometricDistribution((1/serving_time_mean), 0);
    double jail_serving_time = jail_term_gen.next();

    return jail_serving_time+1; //+1 to avoid having 0 day
}

/**
* Add a person to the jail (list) 
*/ 
void Jail::addPerson(double tick, PersonPtr person) {
    double serving_time = get_jail_time();
    if (Parameters::instance()->getBooleanParameter(IS_CARE_DISRUPTION_ON)) {
        person->setPrepForcedOff(true); //care disruption mechanism, turn on offPrEP flag immediately
        // disable ART override while in jail
        person->setArtForcedOff(false);
    }

    if(std::find(jailed_pop.begin(), jailed_pop.end(), person) != jailed_pop.end()) {
        std::cout << "*****ERROR in Jail::addPerson:***** Person is already in jail" << std::endl;
    } else {
        person->jailed(tick, serving_time);
        jailed_pop.push_back(person);

        std::vector<EdgePtr<Person>> edges;
        net_->getEdges(person, edges);

        jailed_pop_net.emplace(person->id(), edges);

        net_->removeVertex(person);
    }

    // schedule a release event for the person
    // + .1 to 
    double stop_time = tick + serving_time + .1;
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new ReleaseEvent(person, this)));

    updateJailServingTimeStats(serving_time);
    total_jailed_++;
    if (person->hasPreviousJailHistory())
        total_jailed_with_hist_++;
}


/**
* Release (remove) a person from the jail (list) 
*/ 
void Jail::releasePerson(double tick, PersonPtr person) {
    person->releasedFromJail(tick);
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());
    net_->addVertex(person);
    std::vector<EdgePtr<Person>> edges = jailed_pop_net.at(person->id());


    float ret_multiplier = Parameters::instance()->getFloatParameter(NETWORK_RETENTION_MULTIPLIER);
   
    int time_spent_in_jail = tick - person->timeOfJail(); //time spent in jail, in case it would be different from person->jailServingTime()

    for (auto edge : edges) {
        PersonPtr source = edge->v1();
        PersonPtr target = edge->v2();
        
        // if person is source, then make sure target is still valid
        if ((source->id() == person->id() && !target->isJailed() && !target->isDead()) ||
        // if person is target, then make sure source is still valid
            (target->id() == person->id() && !source->isJailed() && !source->isDead())) {
                double retention_prob;
                if (edge->type() == STEADY_NETWORK_TYPE)  //0
                    retention_prob = net_decay_prob_main[time_spent_in_jail];
                else //CASUAL_NETWORK_TYPE, 1
                  retention_prob = net_decay_prob_casual[time_spent_in_jail]; 

                if (Parameters::instance()->getBooleanParameter(IS_NETWORK_DISRUPTION_ON)) {
                    if (Random::instance()->nextDouble() <= retention_prob * ret_multiplier) {
                            EdgePtr<Person> new_edge = net_->addEdge(source, target, edge->type());
                            new_edge->setCondomUseProbability(edge->condomUseProbability());
                    }
                }
                else {
                    EdgePtr<Person> new_edge = net_->addEdge(source, target, edge->type());
                    new_edge->setCondomUseProbability(edge->condomUseProbability());
                }
        }
    }

    if (Parameters::instance()->getBooleanParameter(IS_CARE_DISRUPTION_ON)) {
        //double vulnerability_mean = vulnerabilityMean(person->jailServingTime());
        float post_release_interference_period_mean = Parameters::instance()->getFloatParameter(POST_RELEASE_INTERFERENCE_PERIOD_MEAN);
        GeometricDistribution post_release_interference_dur_gen = GeometricDistribution((1/post_release_interference_period_mean), 0);

        unsigned int id = person->id();
        ScheduleRunner& schedule = RepastProcess::instance()->getScheduleRunner();

        int post_release_interf_duration_prep = (int) post_release_interference_dur_gen.next();
        // + 0.1 in case this tick
        double off_prep_flag_change_time = tick + post_release_interf_duration_prep + 0.1;
        
        OffPrepFlagEndEvent* prep_evt = new OffPrepFlagEndEvent(person, this);
        auto p_iter = prep_evts.find(id);
        if (p_iter != prep_evts.end()) {
            p_iter->second->cancel();
        }
        prep_evts[id] = prep_evt;
        schedule.scheduleEvent(off_prep_flag_change_time, Schedule::FunctorPtr(prep_evt));
            
        person->setArtForcedOff(true); //care disruption; PrEP has been already off when jailed
        int post_release_interf_duration_art = (int) post_release_interference_dur_gen.next();
        double off_art_flag_change_time = tick + post_release_interf_duration_art + 0.1;    
        OffArtFlagEndEvent* art_evt = new OffArtFlagEndEvent(person, this);
        auto a_iter = art_evts.find(id);
        if (a_iter != art_evts.end()) {
            a_iter->second->cancel();
        }
        art_evts[id] = art_evt;
        schedule.scheduleEvent(off_art_flag_change_time, Schedule::FunctorPtr(art_evt));

        // std::cout << off_prep_flag_change_time << ", " << off_art_flag_change_time << std::endl;

        //std::vector<std::string> vals;
        //vals.push_back(to_string(post_release_interf_duration_prep));
        //vals.push_back(to_string(post_release_interf_duration_art));
        //csv_writer.addRow(vals);
    }

    jailed_pop_net.erase(person->id());
    total_released_++;

    if (person->isInfected()) {
        Stats* stats = Stats::instance();
        ++stats->currentCounts().infected_before_release;
    }
}

/**
* Remove a person from the jail (list) 
*/ 
void Jail::removeDeadPerson(double time, PersonPtr person) {
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());
    unsigned int id = person->id();
    jailed_pop_net.erase(id);  
    total_dead_in_jail_++;
    
    auto p_iter = prep_evts.find(id);
    if (p_iter != prep_evts.end()) {
        p_iter->second->cancel();
        prep_evts.erase(p_iter);
    }

    auto a_iter = art_evts.find(id);
    if (a_iter != art_evts.end()) {
        a_iter->second->cancel();
        art_evts.erase(a_iter);
    }
}

/**
* Function for applying internal infection transmission among jailed populaiton  
*/
void Jail::runInternalInfectionTransmission(double time) {
    Stats* stats = Stats::instance();
    double incidence_rate = Parameters::instance()->getDoubleParameter(IN_JAIL_INFECTION_INCIDENCE_RATE); // 0.000182
   
    double expected_infection = incidence_rate * uninfectedPopulationSize();
   
    //one single person infection randomly if the probability occurs 
    if (repast::Random::instance()->nextDouble() <= expected_infection) {

        if (jailed_pop.size()>2) {
            unsigned int rand_person_index = Random::instance()->createUniIntGenerator(0, jailed_pop.size()-1).next();
            PersonPtr p  = jailed_pop[rand_person_index];
            //std::cout << "random person:" <<p -> id() <<std::endl; 
            float duration_of_infection = Parameters::instance()->getFloatParameter(DURATION_OF_INFECTION); 
            p-> infect(duration_of_infection, time);
           
            total_infected_inside_jail_++;
            stats->currentCounts().incrementInfected(p); // we add it to internal_infected list 
            stats->currentCounts().incrementInfectedInJail(); // although we keeep a separate counter stats on injail infections
            stats->personDataRecorder()->recordInfection(p, time, InfectionSource::INJAIL);
        }
    }

    //currently used as probablity to apply for each incarcerated individual
    //this can potentially be changed to one single person infection if the probability occurs 
    /*float duration_of_infection = Parameters::instance()->getFloatParameter(DURATION_OF_INFECTION); 
    for (auto& p : jailed_pop) {
        if (!p->isInfected()) {
            if (repast::Random::instance()->nextDouble() <= incidence_rate) {
                p-> infect(duration_of_infection, time);
                if (Parameters::instance()->getBooleanParameter(IS_CARE_DISRUPTION_ON)) {
                    p-> setOffPrepFlag(true); //care disruption mechanism, turn on offPrEP flag immediately
                }
                total_infected_inside_jail_++;
                stats->currentCounts().incrementInfected(p); // we add it to internal_infected list 
                stats->currentCounts().incrementInfectedInJail(); // although we keeep a separate counter stats on injail infections
                stats->personDataRecorder()->recordInfection(p, time, InfectionSource::INJAIL);
            }
        }
    }
    */
}

/**
* Private function to update jail serving time stats  
*/ 
void Jail::updateJailServingTimeStats(double serving_time) {
    //@TODO write these values in the appropriate parameter file

    switch ((int)serving_time) { 
        case 1 ... 6: 
            total_ser_time_1day_week_++;            
            break; 
        case 7 ... 30: 
            total_ser_time_week_month_++; 
            break; 
        case 31 ... 180: 
            total_ser_time_1month_6months_++; 
            break; 
        case 181 ... 365: 
            total_ser_time_6months_1year_++; 
            break; 
        default: 
            total_ser_time_more_than_year_++; 
            break; 
    } 
}


/**
* Private function to get retention network probablity at the time of release
*/ 
double Jail::retentionNetworkProbability(double serving_time) {

    //@TODO write these values in the appropriate parameter file

    switch ((int)serving_time) { 
        case 1 ... 6: 
            return 0.95;            
            break; 
        case 7 ... 30: 
            return 0.80; 
            break; 
        case 31 ... 180: 
            return 0.50; 
            break; 
        case 181 ... 365: 
            return 0.30; 
            break; 
        default: //more than 1 year
            return 0.25; 
            break; 
    } 
}

/**
* Private function to get vulnerability mean at the time of release based on serving time
*/ 
double Jail::vulnerabilityMean(double serving_time) {

    //@TODO write these values in the appropriate parameter file

    switch ((int)serving_time) { 
        case 1 ... 6: 
            return 3;            
            break; 
        case 7 ... 30: 
            return 10; 
            break; 
        case 31 ... 180: 
            return 15; 
            break; 
        case 181 ... 365: 
            return 42; 
            break; 
        default: 
            return 63; 
            break; 
    } 
}

/**
* Gets the jail population size  
*/ 
int  Jail::populationSize() {
    return jailed_pop.size();
}


/**
* Returns total number of infected persons in jail 
*/ 
int Jail::infectedPopulationSize() {
    int totalInfectedPop=0;
    for (auto& p : jailed_pop) {
        if (p->isInfected()) {
            totalInfectedPop++;
        }
    }
    return totalInfectedPop;
}

/**
* Returns total number of uninfected persons in jail 
*/ 
int Jail::uninfectedPopulationSize() {
    int totaluninfectedPop=0;
    for (auto& p : jailed_pop) {
        if (!p->isInfected()) {
            totaluninfectedPop++;
        }
    }
    return totaluninfectedPop;
}

/**
* Returns total number of onART persons in jail 
*/ 
int Jail::onArtPopulationSize() {
    int totalOnArtPop=0;
    for (auto& p : jailed_pop) {
        if (p->isOnART(false)) {
            totalOnArtPop++;
        }
    }
    return totalOnArtPop;
}

/**
* Returns total number of onPrEP persons in jail 
*/ 
int Jail::onPrepPopulationSize() {
    int totalOnPrepPop=0;
    for (auto& p : jailed_pop) {
        if (p->isOnPrep(false)) {
            totalOnPrepPop++;
        }
    }
    return totalOnPrepPop;
}


/**
* Calcuates and returns the infection incidence proportion 
*/ 
float Jail::infectionIncidence() {

    return (float) infectedPopulationSize()/(float)uninfectedPopulationSize();
}

/**
* Returns the number persons jailed at a given day (time) 
*/ 
int Jail::numberOfPersonsJailedAtTime(double time) {
    float totalJailed=0;
    for (auto& p : jailed_pop) {
        if (p->timeOfJail() == time) {
            totalJailed++;
        }
    }
    return totalJailed;
}

/**
* Returns the number persons to be released at given day (time) in the future
* (time must be bigger than current time)
*/ 
int Jail::numberOfPersonsToBeReleasedAtTime(double future_time) {
    double current_time = RepastProcess::instance()->getScheduleRunner().currentTick();
    float totalRelease=0;
    if (future_time> current_time) {
        double time_diff = future_time - current_time;
        for (auto& p : jailed_pop) {
            if ((p->timeOfJail()+ p->jailServingTime()+ time_diff)  == future_time) {
                totalRelease++;
            }
        }
    }
    return totalRelease;
}

/**
* Calulates the average of ages of the jailed population  
*/ 
float Jail::avgPopulationAge() {
    float sumOfAges=0;
    for (auto& p : jailed_pop) {
        sumOfAges+= p->age();
    }
    float avgAges = sumOfAges/jailed_pop.size();
    return avgAges;
}


/**
* Calculates the average of serving times of the jailed population  
*/ 
float Jail::avgPopulationServingTime() {
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
    
    /* std::cout << std::endl << "---Age at first jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtFirstJail() << ", "; */

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


/**
* Debugging helper function to print the disease related informaton of the jail population  
*/ 
void Jail::printPopulationDiseaseInfo() {
    std::cout << "======Jail::printPopulationDiseaseInfo =========" << std::endl;
    std::cout << "---IDs:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    
    std::cout << std::endl << "---Time of jail:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->timeOfJail() << ", ";

    std::cout << std::endl << "---Age at time of jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtJail() << ", ";

    std::cout << std::endl << "---Age:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->age() << ", ";

    std::cout << std::endl << "---Age change since jailed:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << (p->age() - p->ageAtJail()) << ", ";

    std::cout << std::endl << "---steady_role?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->steady_role() << ", ";

    std::cout << std::endl << "---casual_role?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->casual_role() << ", ";

    std::cout << std::endl << "---isInfected?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->isInfected() << ", ";   
    
    std::cout << std::endl << "---timeSinceInfection?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->timeSinceInfection() << ", ";   
    
    std::cout << std::endl << "---isCircumcised?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->isCircumcised() << ", ";   

    std::cout << std::endl << "---isOnPrep?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->isOnPrep(false) << ", ";

    std::cout << std::endl << "---isOnART?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->isOnART(false) << ", ";   

    std::cout << std::endl << "---infectivity?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->infectivity() << ", ";

    std::cout << std::endl << "---diagnoser.lastTestAt?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->diagnoser().lastTestAt() << ", ";

    std::cout << std::endl << "---score?:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->score() << ", ";

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


void Jail::printPopulationInfoOnART() {

    std::cout << "=====Jail::printPopulationInfoOnART: ======" << std::endl;
    //std::cout << std::endl << "---isOnART?:" << std::endl;
    for (auto& p : jailed_pop) {
        if (p->isOnART(false))
            std::cout << p->id() << ", "; 
        //PrintHelper::printPersonViralLoad(p);
    } 

    std::cout << std::endl << "===========================================" << std::endl;
}

void Jail::prepOverrideEnded(PersonPtr person) {
    prep_evts.erase(person->id());
}

void Jail::artOverrideEnded(PersonPtr person) {
    art_evts.erase(person->id());
}


} /* namespace TransModel */

