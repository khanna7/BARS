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

PrepUptakeManager::PrepUptakeManager(PrepUseData data, double age_threshold) : cessation_generator_lt(data.daily_p_prob_lt, 1.1),
		cessation_generator_gte(data.daily_p_prob_gte, 1.1), age_threshold_(age_threshold), year(1), prep_data(data) {
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
