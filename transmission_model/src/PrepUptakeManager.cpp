/*
 * PrepUptakeManager.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: nick
 */
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "PrepUptakeManager.h"
#include "Stats.h"
#include "PrepCessationEvent.h"

using namespace repast;

namespace TransModel {

PrepUptakeManager::PrepUptakeManager(PrepUseData data) : prep_data(data), prob_lt(0), prob_gte(0), cessation_generator_lt(data.daily_stop_prob_lt, 1.1),
		cessation_generator_gte(data.daily_stop_prob_gte), year(1) {
	updateOnPrepProbability();
}

PrepUptakeManager::~PrepUptakeManager() {
}

double calc_prob(double use, double increment, double stop, int year) {
	// double p = Parameters::instance()->getDoubleParameter(stop_prob_key);
	// double k = Parameters::instance()->getDoubleParameter(prep_use_key
	// (p * k) / (1 - k);
	double k = use + (increment * year);
	double val = (stop * k) / (1 - k);
	//std::cout <<  RepastProcess::instance()->getScheduleRunner().currentTick() << ": year: " <<  year <<
	//				", k: " << k << ", prob: " << val << std::endl;
	return val;
}

void PrepUptakeManager::updateOnPrepProbability() {
	if (year <= prep_data.years_to_increase) {
		prob_lt = calc_prob(prep_data.base_use_lt, prep_data.increment_lt, prep_data.daily_stop_prob_lt, year);
		prob_gte = calc_prob(prep_data.base_use_gte, prep_data.increment_gte, prep_data.daily_stop_prob_gte, year);
		++year;
	}
}

void PrepUptakeManager::updateUse(double tick, double age_threshold, std::shared_ptr<Person>& person) {
	double prob = person->age() < age_threshold ? prob_lt : prob_gte;
	if (!person->isOnPrep() && Random::instance()->nextDouble() <= prob) {
		ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
		double delay = person->age() < age_threshold ? cessation_generator_lt.next() : cessation_generator_gte.next();
		double stop_time = tick + delay;
		person->goOnPrep(tick, stop_time);
		Stats::instance()->recordPREPEvent(tick, person->id(), static_cast<int>(PrepStatus::ON));
		Stats::instance()->personDataRecorder()->recordPREPStart(person, tick);
		runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
	}
}

} /* namespace TransModel */
