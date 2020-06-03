
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "RandomSelectionPrepIntervention.h"

namespace TransModel {

RandomSelectionPrepIntervention::RandomSelectionPrepIntervention(PrepUptakeData& prep_data, std::vector<std::shared_ptr<PrepFilter> > filters) : PrepIntervention(prep_data, filters),
    candidates(), k(0), prep_data_(prep_data), total_negatives(0) {
    onYearEnded();
}

RandomSelectionPrepIntervention::~RandomSelectionPrepIntervention() {}

void RandomSelectionPrepIntervention::reset() {
    candidates.clear();
    total_negatives = 0;
}

void RandomSelectionPrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (runFilters(person)) {
        ++total_negatives;
        if (!person->isOnPrep(false)) {
            candidates.emplace(person->id(), person);
        }
    }
}

void RandomSelectionPrepIntervention::trimCandidates(std::vector<PersonPtr>& put_on_prep) {
    for (auto& person : put_on_prep) {
        candidates.erase(person->id());
    }
}

void RandomSelectionPrepIntervention::run(double tick,  std::vector<PersonPtr>& put_on_prep, Network<Person>& network) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    trimCandidates(put_on_prep);
    std::shared_ptr<Log> log = Logger::instance()->getLog(RANDOM_SELECTION_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidates.size();
    double prep_p = 0;
    unsigned int count = 0;
    double adjustment = calcPrepStopAdjustment();
    if (candidates.size() > 0 && k > 0) {
        prep_p = ((double)total_negatives * k * (prep_data_.stop + adjustment)) / candidates.size();
        for (auto& kv : candidates) {
            if (repast::Random::instance()->nextDouble() <= prep_p) {
                putOnPrep(tick, kv.second, PrepStatus::ON_INTERVENTION);
                put_on_prep.push_back(kv.second);
                ++count;
            }
        }
    }
    
    (*log) << "," << count << "," << adjustment << "," << prep_p << "\n";
}

void RandomSelectionPrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment && prep_data_.increment > 0) {
        k = prep_data_.increment *  year;
        ++year;
    }
}

}
