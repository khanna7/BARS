/*
 * SeriodiscordantPrepUptakeManager.h
 *
 *  Created on: May 30, 2018
 *      Author: nick
 */

#ifndef SRC_SERODISCORDANTPREPUPTAKEMANAGER_H_
#define SRC_SERODISCORDANTPREPUPTAKEMANAGER_H_

#include <memory>

#include "PrepUptakeManager.h"

namespace TransModel {

using EdgeFilterPtr = bool(*)(EdgePtr<Person>);

class SerodiscordantPrepUptakeManager : public PrepUptakeManager {

private:
    double prob_lt, prob_gte, prob_sd;
    unsigned int neg_count;
    std::vector<std::shared_ptr<Person>> serodiscordants;
    GeometricDistribution cessation_generator;
    EdgeFilterPtr edge_filter;

public:
    SerodiscordantPrepUptakeManager(PrepUseData data, double age_threshold, NetworkType type);
    virtual ~SerodiscordantPrepUptakeManager();

    void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, Network<Person>& net) override;

    void onYearEnded() override;
    void updateSDUse(double tick, std::shared_ptr<Person>& person);
};

}

#endif /* SRC_SERODISCORDANTPREPUPTAKEMANAGER_H_ */
