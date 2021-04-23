
#include "repast_hpc/Random.h"

#include "Logger.h"
#include "BasePrepIntervention.h"

namespace TransModel {

BasePrepIntervention::BasePrepIntervention(PrepUptakeData& prep_data, std::vector<std::shared_ptr<PrepFilter>> filters) : PrepIntervention(prep_data, filters),
    candidates(), prep_data_(prep_data), total_negatives(0)  {
    onYearEnded();
}

BasePrepIntervention::~BasePrepIntervention() {

}

void BasePrepIntervention::reset() {
    candidates.clear();
    total_negatives = 0;
}

void BasePrepIntervention::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    if (runFilters(person)) {
        ++total_negatives;
        if (!person->isOnPrep(false)) {
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
    double adjustment = calcPrepStopAdjustment();
    if (candidates.size() > 0) {
        prep_p = ((double)total_negatives * prep_data_.use * (prep_data_.stop + adjustment)) / candidates.size();
        //std::cout << "treatment prob: " << prep_p << " target proportion on " << prep_data_.use << " current_proportion_on " << (total_negatives - candidates.size())/(double)total_negatives << " on / total " << (total_negatives - candidates.size()) << " / " << total_negatives << std::endl;
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
