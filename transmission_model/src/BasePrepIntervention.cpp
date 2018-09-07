
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "BasePrepIntervention.h"

namespace TransModel {

BasePrepIntervention::BasePrepIntervention(PrepUptakeData& prep_data, std::shared_ptr<PrepAgeFilter> filter) : PrepIntervention(prep_data),
    candidates(), filter_(filter), prep_data_(prep_data), total_negatives(0)  {
        onYearEnded();
}
    
BasePrepIntervention::~BasePrepIntervention() {

}

void BasePrepIntervention::reset() {
    candidates.clear();
    total_negatives = 0;
}

void BasePrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (filter_->apply(person)) {
        ++total_negatives;
        if (!person->isOnPrep()) {
            candidates.push_back(person);
        }
    }
}

void BasePrepIntervention::run(double tick,  std::vector<PersonPtr>& put_on_prep, Network<Person>& network) {
    std::shared_ptr<Log> log = Logger::instance()->getLog(BASE_LOG);
    (*log) << tick << ",";
    (*log) << (unsigned int)candidates.size();
    double prep_p = 0;
    unsigned int count = 0;
    double adjustment = filter_->calcPrepStopAdjustment();
    if (candidates.size() > 0) {
        // lt:  prep_p = (prep_data_.use * (prep_data_.stop  + 0)) / (candidates.size() / (double)total_negatives);
        // gte: prep_p = (prep_data_.use * (prep_data_.stop  + 1/((max_age - threshold_age)*365))) / (candidates.size() / (double)total_negatives);
        prep_p = (prep_data_.use * (prep_data_.stop  + adjustment)) / (candidates.size() / (double)total_negatives);
        for (auto& person : candidates) {
            if (repast::Random::instance()->nextDouble() <= prep_p) {
                putOnPrep(tick, person, PrepStatus::ON);
                put_on_prep.push_back(person);
                ++count;
            }
        }
    }
    (*log) << "," << count << "," << adjustment << "," << prep_p << "\n";
}

}