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
#include "Network.h"

namespace TransModel {

struct PrepUseData {

	double base_use_lt, base_use_gte, base_use_yor;
	double daily_p_prob_lt, daily_p_prob_gte;
	double daily_stop_prob_lt, daily_stop_prob_gte, daily_stop_prob_sd;
	double increment_lt, increment_gte, increment_sd, increment_yor;
	int years_to_increase;

	double alpha;
	double yor_old_extra, yor_young_extra;

    PrepUseData();
};

std::ostream& operator <<(std::ostream& stream, const PrepUseData& prep_data);


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

	virtual void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) = 0;
	virtual void run(double tick) = 0;
	virtual void onYearEnded() = 0;
};

} /* namespace TransModel */

#endif /* SRC_PREPUPTAKEMANAGER_H_ */
