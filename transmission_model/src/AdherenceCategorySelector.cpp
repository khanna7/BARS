/*
 * AdherenceCategorySelector.cpp
 *
 *  Created on: Sep 23, 2016
 *      Author: nick
 */

#include "repast_hpc/Random.h"

#include "AdherenceCategorySelector.h"

namespace TransModel {

const double EPSILON = .00001;

AdherenceCategorySelector::AdherenceCategorySelector(double never_adhere_prob, double always_adhere_prob, double partial_adhere_prob) : never(never_adhere_prob),
		always(never_adhere_prob + always_adhere_prob) {
	if (always + partial_adhere_prob < 1 - EPSILON || always + partial_adhere_prob > EPSILON + 1) {
		throw std::domain_error("Invalid adherence probabilities: they must sum to 1");
	}
}

AdherenceCategorySelector::~AdherenceCategorySelector() {}

AdherenceCategory AdherenceCategorySelector::selectCategory() {
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= never) {
		return AdherenceCategory::NEVER;
	} else if (draw <= always) {
		return AdherenceCategory::ALWAYS;
	} else {
		return AdherenceCategory::PARTIAL;
	}
}

} /* namespace TransModel */
