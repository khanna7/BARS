/*
 * PrepUpdateManagerAlpha.h
 *
 *  Created on: Feb 19, 2018
 *      Author: nick
 */

#ifndef SRC_PROPORTIONALPREPUPTAKEMANAGER_H_
#define SRC_PROPORTIONALPREPUPTAKEMANAGER_H_

#include <vector>

#include "common.h"
#include "Person.h"
#include "PrepUptakeManager.h"

namespace TransModel {

class ProportionalPrepUptakeManager : public PrepUptakeManager {

private:
	int uninfected_count;
	std::vector<PersonPtr> young, old;
	double base_use, stop_prob, k;
	double y_extra, o_extra;

public:
	ProportionalPrepUptakeManager(PrepUseData& data, double age_threshold);
	virtual ~ProportionalPrepUptakeManager();

	virtual void processPerson(double tick, std::shared_ptr<Person>& person) override;
	virtual void run(double tick) override;
	virtual void onYearEnded() override;

};

} /* namespace TransModel */

#endif /* SRC_PROPORTIONALPREPUPTAKEMANAGER_H_ */
