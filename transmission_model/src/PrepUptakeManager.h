/*
 * PrepUptakeManager.h
 *
 *  Created on: Feb 7, 2018
 *      Author: nick
 */

#ifndef SRC_PREPUPTAKEMANAGER_H_
#define SRC_PREPUPTAKEMANAGER_H_

#include "GeometricDistribution.h"
#include "Person.h"

namespace TransModel {

struct PrepUseData {

	double base_use_lt, base_use_gte;
	double daily_stop_prob_lt, daily_stop_prob_gte;
	double increment_lt, increment_gte;
	int years_to_increase;
};

class PrepUptakeManager {

private:
	PrepUseData prep_data;
	double prob_lt, prob_gte;
	GeometricDistribution cessation_generator_lt, cessation_generator_gte;
	int year;

public:
	PrepUptakeManager(PrepUseData data);
	virtual ~PrepUptakeManager();

	void updateOnPrepProbability();
	void updateUse(double tick, double age_threshold, std::shared_ptr<Person>& person);
};

} /* namespace TransModel */

#endif /* SRC_PREPUPTAKEMANAGER_H_ */
