
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "BasePrepIntervention.h"

namespace TransModel {

BasePrepIntervention::BasePrepIntervention(PrepUptakeData& prep_data, AgeFilterPtr filter, double age_threshold) : PrepIntervention(prep_data),
    candidates(), filter_(filter), age_threshold_(age_threshold), prep_data_(prep_data), total_negatives(0), orig_k(prep_data.use) {
        onYearEnded();
}
    
BasePrepIntervention::~BasePrepIntervention() {

}

void BasePrepIntervention::reset() {
    candidates.clear();
    total_negatives = 0;
}

void BasePrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (filter_(person, age_threshold_)) {
        ++total_negatives;
        if (!person->isOnPrep()) {
            candidates.push_back(person);
        }
    }
}

void BasePrepIntervention::run(double tick,  std::vector<PersonPtr>& put_on_prep, Network<Person>& network) {
    // base_prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (lt_not_on_preps / lt_total_negs);
    std::shared_ptr<Log> log = Logger::instance()->getLog(BASE_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidates.size();
    double prep_p = 0;
    unsigned int count = 0;
    if (candidates.size() > 0) {
        prep_p = (prep_data_.stop * prep_data_.use) / (candidates.size() / (double)total_negatives);
        for (auto& person : candidates) {
            if (repast::Random::instance()->nextDouble() <= prep_p) {
                putOnPrep(tick, person, PrepStatus::ON);
                put_on_prep.push_back(person);
                ++count;
            }
        }
    }
     (*log) << "," << count << "," << prep_p << "\n";

}

void BasePrepIntervention::onYearEnded() {
    if (year <= prep_data_.years_to_increment && prep_data_.increment > 0) {
        double increment = ((prep_data_.increment * prep_data_.years_to_increment) / prep_data_.years_to_increment) * year;
        prep_data_.use += increment;
        ++year;
    }
}

}