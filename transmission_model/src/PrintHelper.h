/*
 * PrintHelper.h
 *
 *  Created on: Aug 1, 2019
 *      Author: Babak
 */

#include "DiseaseParameters.h"
#include "ViralLoadCalculator.h"
#include "PrepParameters.h"
#include "AdherenceCategory.h"
#include "PrepStatus.h"


#ifndef SRC_PRINTHELPER_H_
#define SRC_PRINTHELPER_H_

using namespace std;

namespace TransModel {

struct PrintHelper {

    static void printPersonNetwork(PersonPtr person, Network<Person>* net) {
        std::cout << "network for person id: " << person->id() << std::endl;
        std::vector<EdgePtr<Person>> edges;
        net->getEdges(person, edges);
        for (auto edge : edges) {
            std::cout << "[i" << edge->id() << ",t" << edge->type() << "(" << edge->v1()->id()<< "," << edge->v2()->id() << ")]" << std::endl;
        }
    }

    static void printPersonInfectionPar(PersonPtr person) {
        std::cout << "-----Infection parameters for person id: ---- " << person->id() << std::endl;
        InfectionParameters infection_par = person->infectionParameters();
        std::cout << "    infection.age_at_infection: " << infection_par.age_at_infection << std::endl;
        std::cout << "    infection.status: " << infection_par.infection_status << std::endl;
        std::cout << "    time_of_infection: " << infection_par.time_of_infection << std::endl;
        std::cout << "    time_of_diagnosis: " << infection_par.time_of_diagnosis << std::endl;
        std::cout << "    time_since_diagnosis: " << infection_par.time_since_diagnosed << std::endl;
        std::cout << "    dur_inf: " << infection_par.dur_inf << std::endl;

        std::cout << "    cd4.count: " << infection_par.cd4_count << std::endl;
        
        std::cout << "    art.status: " << infection_par.art_status << std::endl;
        std::cout << "    art_lag: " << infection_par.art_lag<< std::endl;
        std::cout << "    viral_load: " << infection_par.viral_load<< std::endl;
        std::cout << "    vl_at_art_init: " << infection_par.vl_at_art_init<< std::endl;
        std::cout << "    time_of_art_init: " << infection_par.time_of_art_init << std::endl;
        std::cout << "    time_since_art_init: " << infection_par.time_since_art_init << std::endl;
        std::cout << "    vl_art_traj_slope: " << infection_par.vl_art_traj_slope<< std::endl;
        std::cout << "    cd4_at_art_init: " << infection_par.cd4_at_art_init<< std::endl;
    }


    static void printPersonViralLoad(PersonPtr person) {
        //std::cout << "-----person id: ---- " << person->id() << std::endl;

        std::cout << "-----person id: ---- " << person->id() << ", isOnART: " << person->isOnART() << std::endl;
        InfectionParameters infection_par = person->infectionParameters();

        //std::cout << "  calculateViralLoadART  viral_load: " << viral_load << ", time_since_art_init "  << infection_params.time_since_art_init <<", time_to_full_supp: " << vl_params.time_to_full_supp <<std::endl;

        //std::cout << "    cd4.count: " << infection_par.cd4_count << std::endl;
        //std::cout << "    art.status: " << infection_par.art_status << std::endl;
        std::cout << "    infection.status: " << infection_par.infection_status << ", ";
        std::cout << "    art.status: " << infection_par.art_status <<", ";
        //std::cout << "    art_lag: " << infection_par.art_lag<< std::endl;
        std::cout << "    viral_load: " << infection_par.viral_load<< std::endl;
        //std::cout << "    vl_at_art_init: " << infection_par.vl_at_art_init<< std::endl;
        //std::cout << "    time_of_art_init: " << infection_par.time_of_art_init << std::endl;
        std::cout << "    time_since_art_init: " << infection_par.time_since_art_init << std::endl;
        //std::cout << "    vl_art_traj_slope: " << infection_par.vl_art_traj_slope<< std::endl;
        //std::cout << "    cd4_at_art_init: " << infection_par.cd4_at_art_init<< std::endl;
    }


    static void printPersonPrepPar(PersonPtr person) {
        std::cout << "-----PrEP parameters for person id: ---- " << person->id() << std::endl;
        PrepParameters prep_par = person->prepParameters();
        PrepStatus status = prep_par.status();

        switch(status){
           case PrepStatus::OFF: 
            std::cout << "    status is OFF" << std::endl;
            break; 

            case PrepStatus::ON: 
            std::cout << "    status is ON" << std::endl;
            break; 

            case PrepStatus::OFF_INFECTED: 
            std::cout << "    status is OFF_INFECTED" << std::endl;
            break; 

            case PrepStatus::ON_INTERVENTION: 
            std::cout << "    status is ON_INTERVENTION" << std::endl;
            break; 

            default: 
            std::cout << "    status is unknoen!!!" << std::endl;
            break; 
        }
       
        std::cout << "    start time: " << prep_par.startTime() << std::endl;
        std::cout << "    stop time: " << prep_par.stopTime() << std::endl;
        std::cout << "    prep effectiveness (adh.prob): " << prep_par.prepEffectiveness() << std::endl;

        AdherenceCategory adh_cat = prep_par.adherenceCagegory(); 

        switch(adh_cat){
           case AdherenceCategory::NEVER: 
            std::cout << "    adherence is NEVER" << std::endl;
            break; 

            case AdherenceCategory:: ALWAYS: 
            std::cout << "    adherence is ALWAYS" << std::endl;
            break; 

            case AdherenceCategory::PARTIAL_PLUS: 
            std::cout << "    adherence is PARTIAL_PLUS" << std::endl;
            break; 

            case AdherenceCategory::PARTIAL_MINUS: 
            std::cout << "    adherence is PARTIAL_MINUS" << std::endl;
            break; 

            default: 
            std::cout << "    adherence is NA !" << std::endl;
            break; 
        }

    }


    static void printPersonArtPar(PersonPtr person) {
        std::cout << "-----person id: ---- " << person->id() << std::endl;
        AdherenceData art_adh = person->artAdherence();
        std::cout << "    ART prob: " << art_adh.probability << std::endl;

        AdherenceCategory adh_cat = art_adh.category; 

        switch(adh_cat){
           case AdherenceCategory::NEVER: 
            std::cout << "    adherence is NEVER" << std::endl;
            break; 

            case AdherenceCategory:: ALWAYS: 
            std::cout << "    adherence is ALWAYS" << std::endl;
            break; 

            case AdherenceCategory::PARTIAL_PLUS: 
            std::cout << "    adherence is PARTIAL_PLUS" << std::endl;
            break; 

            case AdherenceCategory::PARTIAL_MINUS: 
            std::cout << "    adherence is PARTIAL_MINUS" << std::endl;
            break; 

            default: 
            std::cout << "    adherence is NA !" << std::endl;
            break; 
        }
    }

   static void printSharedViralLoadPar(ViralLoadCalculator viral_load_calculator) {

        std::cout << "-----Shared viral load parameters: ---- " << std::endl;
        SharedViralLoadParameters svlp = viral_load_calculator.sharedViralLoadParameters();
        std::cout << "    time_infection_to_peak_load: " << svlp.time_infection_to_peak_load << std::endl;
        std::cout << "    time_infection_to_set_point: " << svlp.time_infection_to_set_point << std::endl;
        std::cout << "    time_infection_to_late_stage: " << svlp.time_infection_to_late_stage << std::endl;
        std::cout << "    time_to_full_supp: " << svlp.time_to_full_supp << std::endl;
        std::cout << "    peak_viral_load: " << svlp.peak_viral_load << std::endl;
        std::cout << "    set_point_viral_load: " << svlp.set_point_viral_load << std::endl;
        std::cout << "    late_stage_viral_load: " << svlp.late_stage_viral_load << std::endl;
        std::cout << "    undetectable_viral_load: " << svlp.undetectable_viral_load << std::endl;
    }
    
};


} /* namespace TransModel */

#endif /* SRC_PRINTHELPER_H_ */