/*
 * SeriodiscordantPrepUptakeManager.cpp
 *
 *  Created on: May 30, 2018
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/Schedule.h"
#include "repast_hpc/RepastProcess.h"

#include "Stats.h"
#include "SerodiscordantPrepUptakeManager.h"
#include "PrepCessationEvent.h"
#include "Logger.h"

namespace TransModel {

bool main_edge_filter(EdgePtr<Person> edge) {
    return edge->type() == STEADY_NETWORK_TYPE && edge->isSerodiscordant();
}

bool casual_edge_filter(EdgePtr<Person> edge) {
    return edge->type() == CASUAL_NETWORK_TYPE && edge->isSerodiscordant();
}

bool all_edge_filter(EdgePtr<Person> edge) {
    return edge->isSerodiscordant();
}

EdgeFilterPtr choose_edge_filter(NetworkType type) {
    if (type == NetworkType::MAIN) {
        return &main_edge_filter;
    } else if (type == NetworkType::CASUAL) {
        return &casual_edge_filter;
    } else {
        return &all_edge_filter;
    }
}

SerodiscordantPrepUptakeManager::SerodiscordantPrepUptakeManager(PrepUseData data, double age_threshold, NetworkType type) : PrepUptakeManager(data, age_threshold),
        pu_base(prep_data, age_threshold), extra_lt(prep_data.increment_sd_lt, prep_data.daily_stop_prob_sd_lt), 
        extra_gte(prep_data.increment_sd_gte, prep_data.daily_stop_prob_sd_gte), serodiscordants_lt(), serodiscordants_gte(),
        edge_filter(choose_edge_filter(type)) {

    onYearEnded();
}

SerodiscordantPrepUptakeManager::~SerodiscordantPrepUptakeManager() {
}

void SerodiscordantPrepUptakeManager::onYearEnded() {
    if (year <= prep_data.years_to_increase) {
        extra_lt.updateBaseProbability(year);
        extra_gte.updateBaseProbability(year);
        //double k = prep_data.increment_sd * year;
        //double p = prep_data.daily_stop_prob_sd;
        //prob_sd = (p * k) / (1 - k);
        //std::cout << "prob_sd: " <<  prob_sd << ", p: " << p << ", k: " << k << ", increment_sd: " << prep_data.increment_sd << ", year: " << year << std::endl;
        ++year;
    }
}

PUExtra& SerodiscordantPrepUptakeManager::selectPUExtra(double age) {
    return age < age_threshold_ ? extra_lt : extra_gte;
}

std::vector<std::shared_ptr<Person>>& SerodiscordantPrepUptakeManager::selectVector(double age) {
    return age < age_threshold_ ? serodiscordants_lt : serodiscordants_gte;
}

void SerodiscordantPrepUptakeManager::processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) {
    if (!person->isOnPrep()) {
        double age = person->age();
        PUExtra& pu = selectPUExtra(age);
        pu.incrementNegativeCount();
        if (pu_base.evaluate(age)) {
            updateUse(tick, person);
        } else {
            std::vector<EdgePtr<Person>> edges;
            network.getEdges(person, edges);
            std::vector<std::shared_ptr<Person>>& vec = selectVector(age);
            for (auto edge : edges) {
                if (edge_filter(edge)) {
                    vec.push_back(person);
                    break;
                }
            }
        }
    }
}

void SerodiscordantPrepUptakeManager::run(double tick, PUExtra& extra, std::vector<std::shared_ptr<Person>>& serodiscordants) {
    double sd_count = serodiscordants.size();
    unsigned int count = 0;
    shared_ptr<Log> log =  Logger::instance()->getLog(SERO_LOG);
    (*log) << sd_count;
    if (sd_count > 0) {
        extra.preRun(sd_count);
        for (auto person : serodiscordants) {
            // person could be in list multiple times if has multiple partners
            if (extra.evaluate() && !person->isOnPrep()) {
                    updateSDUse(tick, person, extra.prepDelay());
                    ++count;
            }
        }
    }
    

    (*log) << "," << count << "," << extra.boostedProb();
    
    extra.postRun();
    serodiscordants.clear();
}

void SerodiscordantPrepUptakeManager::run(double tick, Network<Person>& net) {
    shared_ptr<Log> log =  Logger::instance()->getLog(SERO_LOG);
    (*log) << tick << ",";
    run(tick, extra_lt, serodiscordants_lt);
    (*log) << ",";
    run(tick, extra_gte, serodiscordants_gte);
    (*log) << "\n";
}

void SerodiscordantPrepUptakeManager::updateSDUse(double tick, std::shared_ptr<Person>& person, double delay) {
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    double stop_time = tick + delay;
    person->goOnPrep(tick, stop_time);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON_INTERVENTION));
    stats->personDataRecorder()->recordPREPStart(person, tick);
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}


}
