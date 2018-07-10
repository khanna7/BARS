/*
 * Prep intervention that targets persons by eigen vector
 * centrality.
 */

#ifndef SRC_NETSTATPUMANAGER_H_
#define SRC_NETSTATPUMANAGER_H_

#include <vector>

#include "PrepUptakeManager.h"
#include "Person.h"
#include "NetworkStats.h"
#include "PUBase.h"
#include "PUExtra.h"

namespace TransModel {


using AgeFilterPtr = bool(*)(PersonPtr, double);

class NetStatPUManager : public PrepUptakeManager {
    
    private:

        PUBase pu_base;
        PUExtra extra_lt, extra_gte;
        float top_n;

        PUExtra& selectPUExtra(double age);
        void run(double tick, PUExtra& extra, std::vector<std::shared_ptr<Person>>& results,
            AgeFilterPtr filter);
        
    protected:
        virtual void selectForPrep(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& results) = 0;
        void updatePrepUse(double tick, std::shared_ptr<Person>& person, double delay);
        

    public:
        NetStatPUManager(PrepUseData data, double age_threshold, float topn);
        ~NetStatPUManager();

        void onYearEnded();
        void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) override;
        void run(double tick, Network<Person>& net) override;
};

class EigenPUManager : public NetStatPUManager {


    protected:
        void selectForPrep(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& results) override;
    
    public:
        EigenPUManager(PrepUseData data, double age_threshold, float topn);
        ~EigenPUManager();
};


class DegreePUManager : public NetStatPUManager {


    protected:
        void selectForPrep(NetworkStats<Person>& stats, std::vector<std::shared_ptr<Person>>& results) override;
    
    public:
        DegreePUManager(PrepUseData data, double age_threshold, float topn);
        ~DegreePUManager();
};

} // namespace TransModel

#endif