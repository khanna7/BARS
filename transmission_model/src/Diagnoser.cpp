/*
 * Diagnoser.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: nick
 */

#include "repast_hpc/Random.h"

#include "Diagnoser.h"

namespace TransModel {

Diagnoser::Diagnoser(float detection_window, unsigned int test_count, double test_probability) :
        detection_window_ { detection_window }, last_test_at_{-1}, test_count_ { test_count }, test_prob {
            test_probability } {
}


Diagnoser::Diagnoser(float detection_window, double test_probability) : detection_window_{detection_window}, last_test_at_{-1},
        test_count_{0}, test_prob {test_probability} {
}

unsigned int Diagnoser::testCount() const {
    return test_count_;
}

double Diagnoser::lastTestAt() const {
    return last_test_at_;
}

Result Diagnoser::test(double tick, const InfectionParameters& inf_params) {
    if (repast::Random::instance()->nextDouble() <= test_prob) {
        ++test_count_;
        last_test_at_ = tick;
        if (inf_params.infection_status && inf_params.time_since_infection >= detection_window_) {
            return Result::POSITIVE;
        } else {
            return Result::NEGATIVE;
        }
    }
    return Result::NO_TEST;
}

}


