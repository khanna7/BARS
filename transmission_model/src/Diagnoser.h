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

template<typename G>
class Diagnoser {

private:
	float detection_window_;
	double next_test_at_;
	unsigned int test_count_;
	std::shared_ptr<G> next_test_generator_;

public:
	Diagnoser(float detection_window, float next_test_at, unsigned int test_count, std::shared_ptr<G> generator);
	Diagnoser(double tick, float detection_window, std::shared_ptr<G> generator);
	virtual ~Diagnoser();

	bool test(double tick, const InfectionParameters& inf_params);

	double timeUntilNextTest(double current_tick);

	unsigned int testCount() const;

};

template<typename G>
Diagnoser<G>::Diagnoser(float detection_window, float next_test_at, unsigned int test_count, std::shared_ptr<G> generator) :
		detection_window_ { detection_window }, next_test_at_ { next_test_at }, test_count_ { test_count }, next_test_generator_ {
				generator } {
}

template<typename G>
Diagnoser<G>::Diagnoser(double tick, float detection_window, std::shared_ptr<G> generator) : detection_window_{detection_window},
	next_test_at_{tick + generator->next()}, test_count_{0}, next_test_generator_ {generator } {
}

template<typename G>
Diagnoser<G>::~Diagnoser() {
}

template<typename G>
unsigned int Diagnoser<G>::testCount() const {
	return test_count_;
}

template<typename G>
bool Diagnoser<G>::test(double tick, const InfectionParameters& inf_params) {
	if (next_test_at_ <= tick) {
		++test_count_;
		if (inf_params.infection_status && tick - inf_params.time_of_infection >= detection_window_) {
			return true;
		} else {
			double time_until_next_test = next_test_generator_->next();
			next_test_at_ = tick + time_until_next_test;
		}
	}
	return false;
}

template<typename G>
double Diagnoser<G>::timeUntilNextTest(double current_tick) {
	return next_test_at_ - current_tick;
}

} /* namespace TransModel */

#endif /* SRC_DIAGNOSER_H_ */
