/*
 * Person.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Rcpp.h"

#include "Person.h"
#include "adherence_functions.h"
#include "Parameters.h"
#include "CounselingAndBehavioralTreatmentCessationEvent.h"
#include "PrepCessationEvent.h"


using namespace Rcpp;
using namespace repast;

namespace TransModel {

Person::Person(int id, float age, bool circum_status,
               std::set<SubstanceUseType> substance_use, int steady_role,
               int casual_role, Diagnoser diagnoser) :
    id_(id), steady_role_(steady_role), casual_role_(casual_role), age_(age),
    circum_status_(circum_status), substance_use_(substance_use),
    infection_parameters_(), infectivity_(0), on_cb_treatment_(false),
    on_mirtazapine_treatment_(false), adhering_to_mirtazapine_(false),
    prep_(PrepStatus::OFF, -1, -1),
    prep_before_treatment_(prep_), dead_(false), diagnosed_(false),
    testable_(false), diagnoser_(diagnoser),
    art_adherence_{0, AdherenceCategory::NA},
    art_adherence_before_treatment_(art_adherence_), score_(0),
    jail_parameters_{} {
        //diagnoser_(diagnoser), art_adherence_{0, AdherenceCategory::NA}, score_(0), vulnerability_expiration_(0) {
}

//Person::Person(int id, std::shared_ptr<RNetwork> network, double timeOfBirth) : net(network), id_(id) {
//	setAge(0);
//	setInfected(false);
//	setTimeOfBirth(timeOfBirth);
//}

Person::~Person() {
}

void Person::infect(float duration_of_infection, float time) {
    infection_parameters_.dur_inf = duration_of_infection;
    infection_parameters_.infection_status = true;
    infection_parameters_.time_since_infection = 0;
    infection_parameters_.age_at_infection = age_;
    infection_parameters_.time_of_infection = time;
}


void Person::setAge(float age) {
    age_ = age;
}

void Person::setScore(double score) {
    score_ = score;
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

void Person::goOffART() {
    infection_parameters_.art_status = false;
}

void Person::goOnART(float time_stamp) {
    infection_parameters_.art_status = true;
    infection_parameters_.time_since_art_init = 0;
    infection_parameters_.time_of_art_init = time_stamp;
    infection_parameters_.cd4_at_art_init = infection_parameters_.cd4_count;
    infection_parameters_.vl_at_art_init = infection_parameters_.viral_load;
}

void Person::goOffPrep(PrepStatus off_status) {
    if (off_status == PrepStatus::ON)
        throw std::invalid_argument("goOffPrep -- status must be an off status");
    prep_.off(off_status);
}
void Person::goOnPrep(double start_time, double stop_time) {
    prep_.on(start_time, stop_time);
}

bool Person::step(float size_of_timestep, float threshold) {
    float prev_age = age_;
    age_ += size_of_timestep / 365;
    if (infection_parameters_.infection_status) {
        ++infection_parameters_.time_since_infection;
    }

    if (diagnosed_) {
        ++infection_parameters_.time_since_diagnosed;
    }

    if (infection_parameters_.art_status) {
        ++infection_parameters_.time_since_art_init;
    }

    return prev_age < threshold && age_ >= threshold;
}

bool Person::deadOfAge(int max_age) {
    return age_ > max_age;
}

bool Person::deadOfInfection() {
    return infection_parameters_.infection_status && !infection_parameters_.art_status &&
            infection_parameters_.time_since_infection >= infection_parameters_.dur_inf;
}

bool Person::diagnose(double tick) {
    Result result = diagnoser_.test(tick, infection_parameters_);
    if (result == Result::POSITIVE) {
           setDiagnosed(tick);
    }
    if (result != Result::NO_TEST) {
        Stats::instance()->recordTestingEvent(tick, id_, diagnosed_);
    }
    return diagnosed_;
}

void Person::updateDiagnoser(double test_prob, bool testable) {
    testable_ = testable;
    diagnoser_.setTestingProbability(test_prob);
}

void Person::updatePrepAdherence(AdherenceData& data) {
    prep_.setAdherenceData(data);
}

/**
 * Used by the jail release function to set Vulnerability Expiration Time (vulnerability time + current time) 
 * This is time period where a person newly released from jail remains vulnearble to change of behaviour 
 */
/*void Person::setVulnerabilityExpirationTime(double expiration_time) {
    vulnerability_expiration_ = expiration_time;
} */

/**
* to check whether the person (newly relased from jail) is vulnearble  
*/
/*bool Person::isVulnerable(double current_time) {
    if (current_time <= vulnerability_expiration_)
      return true;
    else return false; 
}*/

/**
* Function to be called when a person gets in the jail 
* Sets jail parameters, including an accumulative count of times the same person is jailed. 
*/ 
void Person::jailed(double time, double serving_time) {
    //std::cout << "Person: getInJail():, time " << time << ", serving time:"<< serving_time <<std::endl;
    jail_parameters_.accumulative_injail_count++;
    jail_parameters_.is_in_jail = true;
    jail_parameters_.time_of_jail = time;
    jail_parameters_.serving_time = serving_time;
    jail_parameters_.age_at_jail = age_;
    jail_parameters_.time_since_jailed = 0;
    /* if (jail_parameters_.is_first_time_jailed){
        jail_parameters_.age_at_first_jail = age_;
    } */
    //jail_parameters_.accumulative_time_in_jail=0;
}  

/**
* Function to be called when a person gets out of the jail 
* Sets is-in-jail flag to false as well as calulating and setting accumlative time a person has spent in jail. 
*/ 
void Person::releasedFromJail(double time_of_release) {
    //std::cout << "Person: getOutJail():, time " << current_time << std::endl;
    jail_parameters_.accumulative_time_in_jail += (time_of_release - jail_parameters_.time_of_jail);
    jail_parameters_.is_in_jail = false;
    if (jail_parameters_.is_first_time_jailed) { 
        jail_parameters_.is_first_time_jailed = false;
    }
}

void Person::setDiagnosed(double tick) {
    diagnosed_ = true;
    infection_parameters_.time_of_diagnosis = tick;
    infection_parameters_.time_since_diagnosed = 0;
    Stats::instance()->personDataRecorder()->recordDiagnosis(this, tick);
    if (prep_.status() == PrepStatus::ON) {
        prep_.off(PrepStatus::OFF_INFECTED);
        Stats::instance()->recordPREPEvent(tick, id(),
            static_cast<int>(PrepStatus::OFF_INFECTED),
            isSubstanceUser(SubstanceUseType::METH),
            isSubstanceUser(SubstanceUseType::CRACK),
            isSubstanceUser(SubstanceUseType::ECSTASY));
        Stats::instance()->personDataRecorder()->recordPREPStop(this,
            tick, PrepStatus::OFF_INFECTED);
    }
}


} /* namespace TransModel */
