
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "NetStatPrepIntervention.h"

namespace TransModel {

NetStatPrepIntervention::NetStatPrepIntervention(PrepUptakeData& prep_data, std::shared_ptr<PrepAgeFilter> filter, 
    float top_n) : PrepIntervention(prep_data),
    filter_(filter), k(0), prep_data_(prep_data), total_negatives(0),
    top_n_(top_n), candidate_count(0) {
        onYearEnded();
}
    
NetStatPrepIntervention::~NetStatPrepIntervention() {}

bool NetStatPrepIntervention::isOn() {
    return top_n_ > 0 && prep_data_.increment > 0 && prep_data_.years_to_increment > 0;
}

void NetStatPrepIntervention::reset() {
    total_negatives = 0;
    candidate_count = 0;
}


void NetStatPrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (filter_->apply(person)) {
        ++total_negatives;
        if (!person->isOnPrep()) {
            ++candidate_count;
        }
    }
}

 void NetStatPrepIntervention::adjustCandidateCount(std::vector<PersonPtr>& put_on_prep) {
    for (auto& person : put_on_prep) {
        if (filter_->apply(person)) {
            --candidate_count;
        }
    }
 }

void NetStatPrepIntervention::run(double tick, std::vector<PersonPtr>& put_on_prep, std::vector<std::shared_ptr<Person>> ranked_persons) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    adjustCandidateCount(put_on_prep);
    std::shared_ptr<Log> log =  Logger::instance()->getLog(NET_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidate_count;

    double prep_p = 0;
    unsigned int prep_count = 0;
    float selected_count = candidate_count * top_n_;
    double adjustment = filter_->calcPrepStopAdjustment();
    if (selected_count > 0 && k > 0) {
        unsigned int count = 0;
        // k = (intervention only coverage), for that year
        // prop = k * p / (d / total_negatives)
        // d = top_n number of persons from candidates
        // lt:  prep_p = (k * (prep_data_.stop  + 0)) / (selected_count / (double)total_negatives);
        // gte: prep_p = (k * (prep_data_.stop  + 1/((max_age - threshold_age)*365))) / (selected_count / (double)total_negatives);
        prep_p = (k * (prep_data_.stop  + adjustment)) / (selected_count / (double)total_negatives);
        unsigned int threshold = (unsigned int)selected_count;
        for (auto& person : ranked_persons) {
            if (!person->isOnPrep() && filter_->apply(person)) {
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
    
    (*log) << "," << ((unsigned int)selected_count) << "," << prep_count << "," << adjustment << "," << prep_p << "\n";
}

void NetStatPrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment && prep_data_.increment > 0) {
        k = prep_data_.increment * year;
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

    for (auto& intervention : interventions) {
        if (intervention->isOn()) {
            if (ranked_persons.size() == 0) {
                NetworkStats<Person> stats(network);
                ranker_(stats, ranked_persons);
            }
            intervention->run(tick, put_on_prep, ranked_persons);
        }
    }
}


void CompositeNetStatPrepIntervention::onYearEnded() {
     for (auto& intervention : interventions) {
        intervention->onYearEnded();
    }
}

}