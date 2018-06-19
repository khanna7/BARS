/*
 * PrepUptakeManager.h
 *
 *  Created on: Feb 7, 2018
 *      Author: nick
 */

#ifndef SRC_INCREMENTINGPREPUPTAKEMANAGER_H_
#define SRC_INCREMENTINGPREPUPTAKEMANAGER_H_

#include "PrepUptakeManager.h"
#include "Person.h"

namespace TransModel {

class IncrementingPrepUptakeManager : public PrepUptakeManager {

private:
    double prob_lt, prob_gte;

public:
    IncrementingPrepUptakeManager(PrepUseData data, double age_threshold);
    virtual ~IncrementingPrepUptakeManager();

    virtual void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) override;
    virtual void run(double tick, Network<Person>& net) override {}

    virtual void onYearEnded() override;
};

} /* namespace TransModel */

#endif /* SRC_INCREMENTINGPREPUPTAKEMANAGER_H_ */
