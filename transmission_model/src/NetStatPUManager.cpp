#include "repast_hpc/Random.h"
#include "repast_hpc/Schedule.h"
#include "repast_hpc/RepastProcess.h"

#include "Stats.h"
#include "NetStatPUManager.h"
#include "PrepCessationEvent.h"

namespace TransModel {


NetStatPUManager::NetStatPUManager(PrepUseData data, double age_threshold, float topn) : PrepUptakeManager(data, age_threshold),
        prob_lt(0), prob_gte(0), prob_selected(0), neg_count(0), top_n(topn),
        cessation_generator(data.daily_stop_prob_netstat, 1.1) {

    // (p * k) / (1 - k) where k is use and is p_prob
    prob_lt = (prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (1 - prep_data.base_use_lt);
    prob_gte =  (prep_data.daily_p_prob_gte * prep_data.base_use_gte) / (1 - prep_data.base_use_gte);
    onYearEnded();
}

NetStatPUManager::~NetStatPUManager() {
}

void NetStatPUManager::onYearEnded() {
    if (year <= prep_data.years_to_increase) {
        double k = prep_data.increment_netstat * year;
        double p = prep_data.daily_stop_prob_netstat;
        prob_selected = (p * k) / (1 - k);
        ++year;
    }
}

void NetStatPUManager::processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) {
    if (!person->isOnPrep()) {
        ++neg_count;
        double prob = person->age() < age_threshold_ ? prob_lt : prob_gte;
        if (repast::Random::instance()->nextDouble() <= prob) {
            updateUse(tick, person);
        } 
    }
}

void NetStatPUManager::run(double tick, Network<Person>& net) {
    float selected_count = neg_count * top_n;
    if (selected_count > 0) {
        double p = prob_selected * (neg_count / selected_count);
        //std::cout << "p: " << p <<  ", nc: " << neg_count << ", topn: " << top_n << ", selected_count: " << selected_count << std::endl;
        NetworkStats<Person> stats(net);
        int threshold = (int)selected_count;
        selectForPrep(tick, threshold, p, stats);
    }
    neg_count = 0;
}

void NetStatPUManager::updatePrepUse(double tick, std::shared_ptr<Person>& person) {
    //std::cout << "on prep" << std::endl;
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    person->goOnPrep(tick, stop_time);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON_INTERVENTION));
    stats->personDataRecorder()->recordPREPStart(person, tick);
    runner.scheduleEvent(stop_time, repast::Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}

EigenPUManager::EigenPUManager(PrepUseData data, double age_threshold, float topn) : NetStatPUManager(data, age_threshold, topn) {}

EigenPUManager::~EigenPUManager() {}

void EigenPUManager::selectForPrep(double tick, unsigned int threshold, double p, NetworkStats<Person>& stats) {
    std::vector<std::pair<std::shared_ptr<Person>, double>> results;
    stats.eigenCentrality(results);
    unsigned int count = 0;
    for (auto result : results) {
        if (!result.first->isOnPrep()) {
            if (repast::Random::instance()->nextDouble() <= p) {
                updatePrepUse(tick, result.first);
            }

            ++count;
            if (count == threshold) {
                break;
            }
        }
    }
   // std::cout << "count: " << count << std::endl;
}

DegreePUManager::DegreePUManager(PrepUseData data, double age_threshold, float topn) : NetStatPUManager(data, age_threshold, topn) {}

DegreePUManager::~DegreePUManager() {}

void DegreePUManager::selectForPrep(double tick, unsigned int threshold, double p, NetworkStats<Person>& stats) {
    std::vector<std::pair<std::shared_ptr<Person>, long>> results;
    stats.degree(results);
    int count = 0;
    for (auto result : results) {
        if (!result.first->isOnPrep()) {
            if (repast::Random::instance()->nextDouble() <= p) {
                updatePrepUse(tick, result.first);
            }

            ++count;
            if (count == threshold) {
                break;
            }
        }
    }
    //std::cout << "count: " << count << std::endl;
}

}