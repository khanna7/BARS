#ifndef SRC_PUBASE_H_
#define SRC_PUBASE_H_

#include "PrepUptakeManager.h"

namespace TransModel {

class PUBase {

private:
    friend std::ostream& operator <<(std::ostream& out, const PUBase& pu);
    double prob_lt, prob_gte, threshold;

public:
    PUBase(PrepUseData& data, double age_threshold);
    ~PUBase();

    bool evaluate(double age);
};

std::ostream& operator <<(std::ostream& out, const PUBase& pu);

}

#endif