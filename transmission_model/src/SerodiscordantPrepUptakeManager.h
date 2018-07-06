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
#include "PUBase.h"
#include "PUExtra.h"

namespace TransModel {

using EdgeFilterPtr = bool(*)(EdgePtr<Person>);

class SerodiscordantPrepUptakeManager : public PrepUptakeManager {

private:
    PUBase pu_base;
    PUExtra extra_lt, extra_gte;
    std::vector<std::shared_ptr<Person>> serodiscordants_lt, serodiscordants_gte;
    EdgeFilterPtr edge_filter;

    PUExtra& selectPUExtra(double age);
    void run(double tick, PUExtra& extra, std::vector<std::shared_ptr<Person>>& serodiscordants);
    std::vector<std::shared_ptr<Person>>& selectVector(double age);
    void updateSDUse(double tick, std::shared_ptr<Person>& person, double delay);

public:
    SerodiscordantPrepUptakeManager(PrepUseData data, double age_threshold, NetworkType type);
    virtual ~SerodiscordantPrepUptakeManager();

    void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) override;
    void run(double tick, Network<Person>& net) override;
    void onYearEnded() override;
};

}

#endif /* SRC_SERODISCORDANTPREPUPTAKEMANAGER_H_ */
