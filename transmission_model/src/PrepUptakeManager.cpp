/*
 * PrepUptakeManager.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Stats.h"
#include "PrepCessationEvent.h"
#include "PrepUptakeManager.h"

using namespace repast;

namespace TransModel {

PrepUseData::PrepUseData() : base_use_lt(-9999), base_use_gte(-9999), base_use_yor(-9999),
 daily_p_prob_lt(-9999), daily_p_prob_gte(-9999),
 daily_stop_prob_lt(-9999), daily_stop_prob_gte(-9999), daily_stop_prob_sd(-9999),
 increment_lt(-9999), increment_gte(-9999), increment_sd(-9999), increment_yor(-9999),
 years_to_increase(-9999), alpha(-9999), yor_old_extra(-9999), yor_young_extra(-9999) {}

std::ostream& operator <<(std::ostream& out, const PrepUseData& data) {
    out << "base_use_lt: " << data.base_use_lt << "\n";
    out << "base_use_gte: " << data.base_use_gte << "\n";
    out << "base_use_yor: " << data.base_use_yor << "\n";
    out << "daily_p_prob_lt: " << data.daily_p_prob_lt << "\n";
    out << "daily_p_prob_gte: " << data.daily_p_prob_gte << "\n";
    out << "daily_stop_prob_lt: " << data.daily_stop_prob_lt << "\n";
    out << "daily_stop_prob_gte: " << data.daily_stop_prob_gte << "\n";
    out << "daily_stop_prob_sd: " << data.daily_stop_prob_sd << "\n";
    out << "increment_lt: " << data.increment_lt << "\n";
    out << "increment_gte: " << data.increment_gte << "\n";
    out << "increment_sd: " << data.increment_sd << "\n";
    out << "increment_yor: " << data.increment_yor << "\n";
    out << "years_to_increase: " << data.years_to_increase << "\n";
    out << "alpha: " << data.alpha << "\n";
    out << "yor_old_extra: " << data.yor_old_extra << "\n";
    out << "yor_young_extra: " << data.yor_young_extra << "\n";

    return out;
}


PrepUptakeManager::PrepUptakeManager(PrepUseData data, double age_threshold) : cessation_generator_lt(data.daily_stop_prob_lt, 1.1),
		cessation_generator_gte(data.daily_stop_prob_gte, 1.1), age_threshold_(age_threshold), year(1), prep_data(data) {
}

PrepUptakeManager::~PrepUptakeManager() {}

void PrepUptakeManager::updateUse(double tick, std::shared_ptr<Person>& person) {
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	double delay = person->age() < age_threshold_ ? cessation_generator_lt.next() : cessation_generator_gte.next();
	double stop_time = tick + delay;
	person->goOnPrep(tick, stop_time);
	Stats* stats = Stats::instance();
	stats->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON));
	stats->personDataRecorder()->recordPREPStart(person, tick);
	runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}

} /* namespace TransModel */
