
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "NetStatPrepIntervention.h"

namespace TransModel {

NetStatPrepIntervention::NetStatPrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold,
    float top_n) : PrepIntervention(prep_data),
    filter_(filter), age_threshold_(age_threshold), k(0), prep_data_(prep_data), total_negatives(0),
    top_n_(top_n), candidate_count(0) {
        onYearEnded();
}
    
NetStatPrepIntervention::~NetStatPrepIntervention() {}

void NetStatPrepIntervention::reset() {
    total_negatives = 0;
    candidate_count = 0;
}

void NetStatPrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (filter_(person, age_threshold_)) {
        ++total_negatives;
        if (!person->isOnPrep()) {
            ++candidate_count;
        }
    }
}

void NetStatPrepIntervention::run(double tick, std::vector<PersonPtr>& put_on_prep, std::vector<std::shared_ptr<Person>> ranked_persons) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    candidate_count -= put_on_prep.size();
    std::shared_ptr<Log> log =  Logger::instance()->getLog(NET_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidate_count;

    double prep_p = 0;
    unsigned int prep_count = 0;
    float selected_count = candidate_count * top_n_;

    if (selected_count > 0 && k > 0) {
        unsigned int count = 0;
        // k = (intervention only coverage), for that year
        // prop = k * p / (d / total_negatives)
        // d = top_n number of persons from candidates
        prep_p = (k * prep_data_.stop) / (selected_count / (double)total_negatives);
        unsigned int threshold = (unsigned int)selected_count;
        for (auto& person : ranked_persons) {
            if (!person->isOnPrep() && filter_(person, age_threshold_)) {
                ++count;
                if (repast::Random::instance()->nextDouble() <= prep_p) {
                    putOnPrep(tick, person, PrepStatus::ON_INTERVENTION);
                    put_on_prep.push_back(person);
                    ++prep_count;
                }
            
                if (count == threshold) {
                    break;
                }
            }
        }
    }
    
    (*log) << "," << ((unsigned int)selected_count) << "," << prep_count << "," << prep_p << "\n";
}

void NetStatPrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment && prep_data_.increment > 0) {
        k = ((prep_data_.increment * prep_data_.years_to_increment) / prep_data_.years_to_increment) * year;
        ++year;
    }
}

void degree_ranker(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& persons) {
    stats.degree(persons);
}

void eigen_ranker(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& persons) {
    stats.eigenCentrality(persons);
}

CompositeNetStatPrepIntervention::CompositeNetStatPrepIntervention(Ranker ranker) : interventions(),
    ranker_(ranker) {}

CompositeNetStatPrepIntervention::~CompositeNetStatPrepIntervention() {}

void CompositeNetStatPrepIntervention::addNetIntervention(std::shared_ptr<NetStatPrepIntervention> intervention) {
    interventions.push_back(intervention);
}
    
void CompositeNetStatPrepIntervention::reset() {
    for (auto& intervention : interventions) {
        intervention->reset();
    }
}

void CompositeNetStatPrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
     for (auto& intervention : interventions) {
        intervention->processPerson(person, network);
    }
}

void CompositeNetStatPrepIntervention::run(double tick, std::vector<PersonPtr>& put_on_prep, Network<Person>& network) {
    std::vector<std::shared_ptr<Person>> ranked_persons;
    NetworkStats<Person> stats(network);
    ranker_(stats, ranked_persons);

    for (auto& intervention : interventions) {
        intervention->run(tick, put_on_prep, ranked_persons);
    }
}


void CompositeNetStatPrepIntervention::onYearEnded() {
     for (auto& intervention : interventions) {
        intervention->onYearEnded();
    }
}

}