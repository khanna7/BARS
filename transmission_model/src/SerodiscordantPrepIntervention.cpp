
#include "repast_hpc/Random.h"

#include "SerodiscordantPrepIntervention.h"
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

SerodiscordantPrepIntervention::SerodiscordantPrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold, NetworkType type) :
 PrepIntervention(prep_data),
    candidates(), filter_(filter), age_threshold_(age_threshold), k(0), prep_data_(prep_data), total_negatives(0),
    edge_filter(choose_edge_filter(type)) {
        onYearEnded();
}
    
SerodiscordantPrepIntervention::~SerodiscordantPrepIntervention() {

}

void SerodiscordantPrepIntervention::reset() {
    candidates.clear();
    total_negatives = 0;
}

void SerodiscordantPrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (filter_(person, age_threshold_)) {
        ++total_negatives;
        if (!person->isOnPrep()) {
            std::vector<EdgePtr<Person>> edges;
            network.getEdges(person, edges);
            for (auto edge : edges) {
                if (edge_filter(edge)) {
                    candidates.push_back(person);
                    break;
                }
            }
        }
    }
}

void SerodiscordantPrepIntervention::run(double tick, Network<Person>& network) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    std::shared_ptr<Log> log =  Logger::instance()->getLog(SERO_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidates.size();
    unsigned int count = 0;
    double prep_p = 0;
    if (candidates.size() > 0) {
        // unboosted prob
        prep_p = (prep_data_.stop * k) / (candidates.size() / (double)total_negatives);
        // boosted prob
        prep_p = prep_p * ((double)total_negatives / candidates.size());
        for (auto& person : candidates) {
            if (!person->isOnPrep() && repast::Random::instance()->nextDouble() <= prep_p) {
                putOnPrep(tick, person, PrepStatus::ON_INTERVENTION);
                ++count;
            }
        }
    }

    (*log) << "," << count << "," << prep_p << "\n";
}

void SerodiscordantPrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment) {
        k = prep_data_.increment * year;
        ++year;
    }
}

}