
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

SerodiscordantPrepIntervention::SerodiscordantPrepIntervention(PrepUptakeData& prep_data, std::shared_ptr<PrepAgeFilter> filter, NetworkType type) :
 PrepIntervention(prep_data),
    candidates(), filter_(filter), k(0), prep_data_(prep_data), total_negatives(0),
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
    if (filter_->apply(person)) {
        ++total_negatives;
        if (!person->isOnPrep(false)) {
            std::vector<EdgePtr<Person>> edges;
            network.getEdges(person, edges);
            for (auto edge : edges) {
                if (edge_filter(edge)) {
                    candidates.emplace(person->id(), person);
                    break;
                }
            }
        }
    }
}

void SerodiscordantPrepIntervention::trimCandidates(std::vector<PersonPtr>& put_on_prep) {
    for (auto& person : put_on_prep) {
        candidates.erase(person->id());
    }
}

void SerodiscordantPrepIntervention::run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    trimCandidates(put_on_prep);
    std::shared_ptr<Log> log =  Logger::instance()->getLog(SERO_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidates.size();
    unsigned int count = 0;
    double prep_p = 0;
    double adjustment = filter_->calcPrepStopAdjustment();
    if (candidates.size() > 0 && k > 0) {
        prep_p = ((double)total_negatives * k * (prep_data_.stop + adjustment)) / candidates.size();
        for (auto& kv : candidates) {
            // don't need to check if not on prep as the candiates should 
            // not longer contain those
            if (repast::Random::instance()->nextDouble() <= prep_p) {
                putOnPrep(tick, kv.second, PrepStatus::ON_INTERVENTION);
                put_on_prep.push_back(kv.second);
                ++count;
            }
        }
    }
    (*log) << "," << count << "," << adjustment << "," << prep_p << "\n";
}

void SerodiscordantPrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment && prep_data_.increment > 0) {
        k = prep_data_.increment *  year;
        ++year;
    }
}

}