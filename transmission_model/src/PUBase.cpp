
#include "PUBase.h"

namespace TransModel {

PUBase::PUBase(PrepUseData& prep_data, double age_threshold) : prob_lt((prep_data.daily_p_prob_lt * prep_data.base_use_lt) / (1 - prep_data.base_use_lt)),
    prob_gte((prep_data.daily_p_prob_gte * prep_data.base_use_gte) / (1 - prep_data.base_use_gte)), threshold(age_threshold) {
}

PUBase::~PUBase() {}

bool PUBase::evaluate(double age) {
    double prob = age < threshold ? prob_lt : prob_gte;
    return repast::Random::instance()->nextDouble() <= prob;
}

}