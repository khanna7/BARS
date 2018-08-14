#include "repast_hpc/Random.h"
#include "repast_hpc/Schedule.h"
#include "repast_hpc/RepastProcess.h"

#include "Stats.h"
#include "NetStatPUManager.h"
#include "PrepCessationEvent.h"

namespace TransModel {

bool gte(PersonPtr person, double threshold) {
    return person->age() >= threshold;
}

bool lt(PersonPtr person, double threshold) {
    return person->age() < threshold;
}


NetStatPUManager::NetStatPUManager(PrepUseData data, double age_threshold, float topn) : PrepUptakeManager(data, age_threshold),
        pu_base(prep_data, age_threshold), extra_lt(prep_data.increment_net_lt, prep_data.daily_stop_prob_net_lt), 
        extra_gte(prep_data.increment_net_gte, prep_data.daily_stop_prob_net_gte), top_n(topn) {

    onYearEnded();
}

NetStatPUManager::~NetStatPUManager() {
}

void NetStatPUManager::onYearEnded() {
    if (year <= prep_data.years_to_increase) {
        extra_lt.updateBaseProbability(year);
        extra_gte.updateBaseProbability(year);
        ++year;
    }
}

PUExtra& NetStatPUManager::selectPUExtra(double age) {
    return age < age_threshold_ ? extra_lt : extra_gte;
}

void NetStatPUManager::processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) {
    if (!person->isOnPrep()) {
        double age = person->age();
        if (pu_base.evaluate(age)) {
            updateUse(tick, person);
        } else {
            PUExtra& pu = selectPUExtra(age);
            pu.incrementNegativeCount();
        }
    }
}

void NetStatPUManager::run(double tick, PUExtra& extra, std::vector<std::shared_ptr<Person>>& results,
    AgeFilterPtr filter) {
    
    float selected_count = extra.negativeCount() * top_n;
    if (selected_count > 0) {
        unsigned int threshold = (unsigned int)selected_count;
        extra.preRun(selected_count);
        unsigned int count = 0;
        for (auto& person : results) {
            if (!person->isOnPrep() && filter(person, age_threshold_)) {
                if(extra.evaluate()) {
                    updatePrepUse(tick, person, extra.prepDelay());
                }
                ++count;
                if (count == threshold) {
                    break;
                }
            }
        }
    }
    extra.postRun();
}

void NetStatPUManager::run(double tick, Network<Person>& net) {
    NetworkStats<Person> stats(net);
    std::vector<std::shared_ptr<Person>> results;
    selectForPrep(stats, results);
    run(tick, extra_lt, results, &lt);
    run(tick, extra_gte, results, &gte); 

    // float selected_count = neg_count * top_n;
    // if (selected_count > 0) {
    //     double p = prob_selected * (neg_count / selected_count);
    //     //std::cout << "p: " << p <<  ", nc: " << neg_count << ", topn: " << top_n << ", selected_count: " << selected_count << std::endl;
    //     NetworkStats<Person> stats(net);
    //     int threshold = (int)selected_count;
    //     selectForPrep(tick, threshold, p, stats);
    // }
    // neg_count = 0;
}

void NetStatPUManager::updatePrepUse(double tick, std::shared_ptr<Person>& person, double delay) {
    //std::cout << "on prep" << std::endl;
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    double stop_time = tick + delay;
    person->goOnPrep(tick, stop_time);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON_INTERVENTION));
    stats->personDataRecorder()->recordPREPStart(person, tick);
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}

EigenPUManager::EigenPUManager(PrepUseData data, double age_threshold, float topn) : NetStatPUManager(data, age_threshold, topn) {}

EigenPUManager::~EigenPUManager() {}

void EigenPUManager::selectForPrep(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& results) {
    stats.eigenCentrality(results);
}

DegreePUManager::DegreePUManager(PrepUseData data, double age_threshold, float topn) : NetStatPUManager(data, age_threshold, topn) {}

DegreePUManager::~DegreePUManager() {}

void DegreePUManager::selectForPrep(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& results) {
    stats.degree(results);
}

}