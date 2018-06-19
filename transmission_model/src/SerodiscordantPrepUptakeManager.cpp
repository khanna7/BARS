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
        prob_lt(0), prob_gte(0), prob_sd(0), neg_count(0), serodiscordants(),
        cessation_generator(data.daily_stop_prob_sd, 1.1), edge_filter(choose_edge_filter(type)) {

    // (p * k) / (1 - k) where k is use and is p_prob
    prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (1 - prep_data.base_use_lt);
    prob_gte =  (prep_data.daily_p_prob_gte * prep_data.base_use_gte) / (1 - prep_data.base_use_gte);
    onYearEnded();
}

SerodiscordantPrepUptakeManager::~SerodiscordantPrepUptakeManager() {
}

void SerodiscordantPrepUptakeManager::onYearEnded() {
    if (year <= prep_data.years_to_increase) {
        double k = prep_data.increment_sd * year;
        double p = prep_data.daily_stop_prob_sd;
        prob_sd = (p * k) / (1 - k);
        ++year;
    }
}

void SerodiscordantPrepUptakeManager::processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) {
    if (!person->isOnPrep()) {
        ++neg_count;
        double prob = person->age() < age_threshold_ ? prob_lt : prob_gte;
        if (repast::Random::instance()->nextDouble() <= prob) {
            updateUse(tick, person);
        } else {
            std::vector<EdgePtr<Person>> edges;
            network.getEdges(person, edges);
            for (auto edge : edges) {
                if (edge_filter(edge)) {
                    serodiscordants.push_back(person);
                    break;
                }
            }
        }
    }
}

void SerodiscordantPrepUptakeManager::run(double tick, Network<Person>& net) {
    double sd_count = serodiscordants.size();
    if (sd_count > 0) {
        double p = prob_sd * (neg_count / sd_count);
        for (auto person : serodiscordants) {
            if (repast::Random::instance()->nextDouble() <= p) {
                updateSDUse(tick, person);
            }
        }
    }
    neg_count = 0;
    serodiscordants.clear();
}

void SerodiscordantPrepUptakeManager::updateSDUse(double tick, std::shared_ptr<Person>& person) {
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    person->goOnPrep(tick, stop_time);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON_INTERVENTION));
    stats->personDataRecorder()->recordPREPStart(person, tick);
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}


}
