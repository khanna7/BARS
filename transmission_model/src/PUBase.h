#ifndef SRC_PUBASE_H_
#define SRC_PUBASE_H_

#include "PrepUptakeManager.h"

namespace TransModel {

class PUBase {

private:
    double prob_lt, prob_gte, threshold;

public:
    PUBase(PrepUseData& data, double age_threshold);
    ~PUBase();

    bool evaluate(double age);
};

}

#endif