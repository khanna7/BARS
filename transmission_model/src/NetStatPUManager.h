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

namespace TransModel {

class NetStatPUManager : public PrepUptakeManager {
    
    private:

        double prob_lt, prob_gte, prob_selected;
        unsigned int neg_count;
        float top_n;
        GeometricDistribution cessation_generator;
        
    protected:
        virtual void selectForPrep(double tick, unsigned int threshold, double p, NetworkStats<Person>& stats) = 0;
        void updatePrepUse(double tick, std::shared_ptr<Person>& person);

    public:
        NetStatPUManager(PrepUseData data, double age_threshold, float topn);
        ~NetStatPUManager();

        void onYearEnded();
        void processPerson(double tick, std::shared_ptr<Person>& person, Network<Person>& network) override;
        void run(double tick, Network<Person>& net) override;
};

class EigenPUManager : public NetStatPUManager {


    protected:
        void selectForPrep(double tick, unsigned int threshold, double p, NetworkStats<Person>& stats);
    
    public:
        EigenPUManager(PrepUseData data, double age_threshold, float topn);
        ~EigenPUManager();
};


class DegreePUManager : public NetStatPUManager {


    protected:
        void selectForPrep(double tick, unsigned int threshold, double p, NetworkStats<Person>& stats);
    
    public:
        DegreePUManager(PrepUseData data, double age_threshold, float topn);
        ~DegreePUManager();
};

} // namespace TransModel

#endif