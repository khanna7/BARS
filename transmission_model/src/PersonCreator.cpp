/*
 * PersonCreator.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: nick
 */

#include "repast_hpc/Schedule.h"
#include "repast_hpc/RepastProcess.h"

#include "Parameters.h"

#include "PersonCreator.h"
#include "adherence_functions.h"
#include "Diagnoser.h"
#include "Stats.h"
#include "ARTScheduler.h"
#include "file_utils.h"

using namespace Rcpp;

namespace TransModel {

PersonCreator::PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner, double detection_window, ARTLagCalculator art_lag_calc) :
        id(0), trans_runner_(trans_runner), testing_configurator(create_testing_configurator()),
        prep_adherence_configurator(create_prep_adherence_configurator()), detection_window_(detection_window),
        art_lag_calculator(art_lag_calc){
}

PersonCreator::~PersonCreator() {
}

int calculate_role(int network_type) {
    double insertive =
            network_type == STEADY_NETWORK_TYPE ?
                    Parameters::instance()->getDoubleParameter(PR_INSERTIVE_MAIN) :
                    Parameters::instance()->getDoubleParameter(PR_INSERTIVE_CASUAL);
    double receptive = (
            network_type == STEADY_NETWORK_TYPE ?
                    Parameters::instance()->getDoubleParameter(PR_RECEPTIVE_MAIN) :
                    Parameters::instance()->getDoubleParameter(PR_RECEPTIVE_CASUAL)) + insertive;

    double draw = repast::Random::instance()->nextDouble();
    if (draw <= insertive) {
        return INSERTIVE;
    } else if (draw <= receptive) {
        return RECEPTIVE;
    } else {
        return VERSATILE;
    }
}

PersonPtr PersonCreator::operator()(double tick, float age) {
    int status = (int) repast::Random::instance()->getGenerator(CIRCUM_STATUS_BINOMIAL)->next();
    double size_of_timestep = Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
    Diagnoser diagnoser(detection_window_, 0);
    PersonPtr person = std::make_shared<Person>(id++, age, status == 1, calculate_role(STEADY_NETWORK_TYPE),
            calculate_role(CASUAL_NETWORK_TYPE), diagnoser);
    testing_configurator.configurePerson(person, size_of_timestep);

    PrepParameters prep(PrepStatus::OFF, 0, 0);
    person->prep_ = prep;
    prep_adherence_configurator.configurePerson(person);

    return person;
}

PersonPtr PersonCreator::operator()(Rcpp::List& val, double model_tick, double burnin_last_tick) {
    //std::cout << "------------" << std::endl;
    //Rf_PrintValue(val);
    float age = as<float>(val["age"]);
    bool circum_status = as<bool>(val["circum.status"]);
    int role_main = as<int>(val["role_main"]);
    int role_casual = role_main;
    if (val.containsElementNamed("role_casual")) {
        role_casual = as<int>(val["role_casual"]);
    }

    //float next_test_at = tick + as<double>(val["time.until.next.test"]);
    // float detection_window,  unsigned int test_count, test_prob
    Diagnoser diagnoser(detection_window_, as<unsigned int>(val["number.of.tests"]), 0);
    PersonPtr person = std::make_shared<Person>(id++, age, circum_status, role_main, role_casual, diagnoser);
    double size_of_timestep = Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
    bool testable = !(as<bool>(val["non.testers"]));
    if (val.containsElementNamed("testing.probability")) {
        double test_prob = as<double>(val["testing.probability"]);
        person->updateDiagnoser(test_prob, testable);
    } else {
        testing_configurator.configurePerson(person, size_of_timestep);
        // update only the testable property to what's passed from R
        person->updateDiagnoser(person->diagnoser().testingProbability(), testable);
    }

    person->diagnosed_ = as<bool>(val["diagnosed"]);
    if (person->diagnosed_ && val.containsElementNamed("time_since_diagnosed")) {
        person->infection_parameters_.time_of_diagnosis = as<float>(val["time_of_diagnosis"]);
        person->infection_parameters_.time_since_diagnosed = as<float>(val["time_since_diagnosed"]);
        person->setARTLag(as<float>(val["art_lag"]));
    }
    person->infection_parameters_.cd4_count = as<float>(val["cd4.count.today"]);

    bool infected = as<bool>(val["inf.status"]);
    if (infected) {
        person->infection_parameters_.infection_status = true;
        person->infection_parameters_.time_since_infection = as<float>(val["time.since.infection"]);
        person->infection_parameters_.time_of_infection = as<float>(val["time.of.infection"]);
        person->infection_parameters_.age_at_infection = as<float>(val["age.at.infection"]);
        person->infection_parameters_.dur_inf = trans_runner_->durationOfInfection();
        person->infection_parameters_.art_status = as<bool>(val["art.status"]);
        person->infection_parameters_.viral_load = as<float>(val["viral.load.today"]);

        if (person->infection_parameters_.art_status) {
            // onART at end of burnin -- need to finish up and schedule check
            person->infection_parameters_.time_since_art_init = as<float>(val["time.since.art.initiation"]);
            person->infection_parameters_.time_of_art_init = as<float>(val["time.of.art.initiation"]);
            person->infection_parameters_.vl_art_traj_slope = as<float>(val["vl.art.traj.slope"]);
            person->infection_parameters_.cd4_at_art_init = as<float>(val["cd4.at.art.initiation"]);
            person->infection_parameters_.vl_at_art_init = as<float>(val["vl.at.art.initiation"]);
            Stats::instance()->recordARTEvent(model_tick, person->id(), true);

            if (val.containsElementNamed("adherence.category")) {
                initialize_art_adherence(person, model_tick, static_cast<AdherenceCategory>(as<int>(val["adherence.category"])));
            } else {
                initialize_art_adherence(person, model_tick);
            }

        } else if (person->diagnosed_ && burnin_last_tick > 0) {
            // diagnosed and in lag period at end of burnin
            float time_of_art_init = as<float>(val["time.of.art.initiation"]);
            if (isnan(time_of_art_init)) {
                double lag = person->infection_parameters_.art_lag - (burnin_last_tick - person->infection_parameters_.time_of_diagnosis);
                Stats::instance()->personDataRecorder()->recordInitialARTLag(person, lag);

                double art_at_tick = lag + model_tick; // - correction;
                if (art_at_tick < 0) {
                    // do it immediately
                    art_at_tick = 1 + model_tick;
                }

                ARTPostBurninScheduler* scheduler = new ARTPostBurninScheduler(art_at_tick, person);
                repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(art_at_tick - 0.1,
                    repast::Schedule::FunctorPtr(scheduler));
            
            } else {
                // diagnosed and has been on ART at some point, but not now
                person->infection_parameters_.time_of_art_init = as<float>(val["time.of.art.initiation"]);
                float time_since_art_init = as<float>(val["time.since.art.initiation"]);
                // time_since_art_init only makes sense if person is on ART
                // but initializing art_adherence uses it to schedule so we
                // temporarily set it here to the duration so we can schedule properly.
                person->infection_parameters_.time_since_art_init = burnin_last_tick - person->infection_parameters_.time_of_art_init;
                person->infection_parameters_.vl_art_traj_slope = as<float>(val["vl.art.traj.slope"]);
                person->infection_parameters_.cd4_at_art_init = as<float>(val["cd4.at.art.initiation"]);
                person->infection_parameters_.vl_at_art_init = as<float>(val["vl.at.art.initiation"]);
                Stats::instance()->recordARTEvent(model_tick, person->id(), true);

                if (val.containsElementNamed("adherence.category")) {
                    initialize_art_adherence(person, model_tick, static_cast<AdherenceCategory>(as<int>(val["adherence.category"])));
                } else {
                    initialize_art_adherence(person, model_tick);
                }
                // set it back to avoid breaking the onART assumption 
                // of time since art init
                person->infection_parameters_.time_since_art_init = time_since_art_init;

            }
        }
    } else {
        //  the prep.status attribute only exists in uninfected persons in the R model
        PrepStatus status = as<bool>(val["prep.status"]) ? PrepStatus::ON : PrepStatus::OFF;
        if (status == PrepStatus::ON) {
            Stats::instance()->recordPREPEvent(model_tick, person->id(), static_cast<int>(PrepStatus::ON));
        }

        // same values as when a person is created from scratch
        double time_of_init = 0;
        double time_of_cess = 0;
        if (val.containsElementNamed("time.of.prep.initiation")) {
             time_of_init = as<double>(val["time.of.prep.initiation"]);
            if (burnin_last_tick > 0) {
                // how much time left to be on prep when carried over
                // from burnin
                time_of_cess = as<double>(val["time.of.prep.cessation"]);
                double remaining = time_of_cess - burnin_last_tick;
                //double tofi = time_of_init, tofc = time_of_cess;
                time_of_init = -(burnin_last_tick - time_of_init);
                time_of_cess = remaining;
                //std::cout << "orig tofi: " << tofi << ", orig tofc: " << tofc <<
                //		", burnin_last_tick: " << burnin_last_tick << ", new tofc: " << time_of_cess << std::endl;
            } else {
                // add 1 so they spend at least a day on prep and .1 so occurs after main loop
                time_of_cess = as<double>(val["time.of.prep.cessation"]) + 1.1;
            }
        }

        if (val.containsElementNamed("prep.adherence.category")) {
            AdherenceCategory cat = static_cast<AdherenceCategory>(as<int>(val["prep.adherence.category"]));
            PrepParameters prep(status, time_of_init, time_of_cess);
            person->prep_ = prep;
            prep_adherence_configurator.configurePerson(person, cat);
        } else {
            PrepParameters prep(status, time_of_init, time_of_cess);
            person->prep_ = prep;
            prep_adherence_configurator.configurePerson(person);
        }
    }

    return person;
}

void PersonCreator::updateTesting(std::shared_ptr<Person> p, double size_of_timestep) {
    testing_configurator.configurePerson(p, size_of_timestep);
}

void PersonCreator::updatePREPAdherence(std::shared_ptr<Person> p) {
    prep_adherence_configurator.configurePerson(p);
}

} /* namespace TransModel */
