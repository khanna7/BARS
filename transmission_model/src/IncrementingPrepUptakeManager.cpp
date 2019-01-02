/*
 * PrepUptakeManager.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Stats.h"
#include "PrepCessationEvent.h"
#include "IncrementingPrepUptakeManager.h"

using namespace repast;

namespace TransModel {

IncrementingPrepUptakeManager::IncrementingPrepUptakeManager(PrepUseData data, double age_threshold) : PrepUptakeManager(data, age_threshold), prob_lt(0),
        prob_gte(0) {
    onYearEnded();
    //std::cout << prob_lt << ", " << prob_gte << std::endl;
}

IncrementingPrepUptakeManager::~IncrementingPrepUptakeManager() {
}

double calc_prob(double use, double increment, double p, int year) {
    // double p = Parameters::instance()->getDoubleParameter(stop_prob_key);
    // double k = Parameters::instance()->getDoubleParameter(prep_use_key
    // (p * k) / (1 - k);
    double k = use + (increment * year);
    double val = (p * k) / (1 - k);
    //std::cout <<  RepastProcess::instance()->getScheduleRunner().currentTick() << ": year: " <<  year <<
    //				", k: " << k << ", prob: " << val << std::endl;
    return val;
}

void IncrementingPrepUptakeManager::onYearEnded() {
    if (prep_data.years_to_increase == 0) {
        prob_lt = calc_prob(prep_data.base_use_lt, prep_data.increment_lt, prep_data.daily_p_prob_lt, 0);
        prob_gte = calc_prob(prep_data.base_use_gte, prep_data.increment_gte, prep_data.daily_p_prob_gte, 0);
    } else if (year <= prep_data.years_to_increase) {
        //std::cout << "Incrementing PUM " << year << std::endl;
        prob_lt = calc_prob(prep_data.base_use_lt, prep_data.increment_lt, prep_data.daily_p_prob_lt, year);
        prob_gte = calc_prob(prep_data.base_use_gte, prep_data.increment_gte, prep_data.daily_p_prob_gte, year);
        ++year;

    }
}

void IncrementingPrepUptakeManager::processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) {
    double prob = person->age() < age_threshold_ ? prob_lt : prob_gte;
    if (!person->isOnPrep() && Random::instance()->nextDouble() <= prob) {
        updateUse(tick, person);
    }
}

} /* namespace TransModel */
