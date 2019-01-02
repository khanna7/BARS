/*
 * Diagnoser.h
 *
 *  Created on: May 18, 2016
 *      Author: nick
 */

#ifndef SRC_DIAGNOSER_H_
#define SRC_DIAGNOSER_H_

#include <memory>

#include "repast_hpc/Random.h"

#include "common.h"
#include "DiseaseParameters.h"

namespace TransModel {

enum class Result {POSITIVE, NEGATIVE, NO_TEST};

class Diagnoser {

private:
    float detection_window_;
    double last_test_at_;
    unsigned int test_count_;
    double test_prob;

public:
    Diagnoser(float detection_window, unsigned int test_count, double daily_proability);
    Diagnoser(float detection_window, double daily_proability);

    Result test(double tick, const InfectionParameters& inf_params);

    unsigned int testCount() const;

    double lastTestAt() const;

    double testingProbability() const {
        return test_prob;
    }

    void setTestingProbability(double prob) {
        test_prob = prob;
    }

};

} /* namespace TransModel */

#endif /* SRC_DIAGNOSER_H_ */
