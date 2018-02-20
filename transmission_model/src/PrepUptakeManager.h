/*
 * PrepUptakeManager.h
 *
 *  Created on: Feb 19, 2018
 *      Author: nick
 */

#ifndef SRC_PREPUPTAKEMANAGER_H_
#define SRC_PREPUPTAKEMANAGER_H_

#include <memory>

#include "Person.h"

namespace TransModel {

struct PrepUseData {

	double base_use_lt, base_use_gte;
	double daily_stop_prob_lt, daily_stop_prob_gte;
	double increment_lt, increment_gte;
	int years_to_increase;
	double alpha;
};


class PrepUptakeManager {

protected:
	GeometricDistribution cessation_generator_lt, cessation_generator_gte;
	double age_threshold_;
	int year;
	PrepUseData prep_data;

	void updateUse(double tick, std::shared_ptr<Person>& person);

public:
	PrepUptakeManager(PrepUseData data, double age_threshold);
	virtual ~PrepUptakeManager();

	virtual void processPerson(double tick, std::shared_ptr<Person>& person) = 0;
	virtual void run(double tick) = 0;
	virtual void onYearEnded() = 0;
};

} /* namespace TransModel */

#endif /* SRC_PREPUPTAKEMANAGER_H_ */
