/*
 * AdherenceCategorySelector.h
 *
 *  Created on: Sep 23, 2016
 *      Author: nick
 */

#ifndef SRC_ADHERENCECATEGORYSELECTOR_H_
#define SRC_ADHERENCECATEGORYSELECTOR_H_

#include "AdherenceCategory.h"

namespace TransModel {

class AdherenceCategorySelector {
private:
	double never, always;

public:
	AdherenceCategorySelector(double never_adhere_prob, double always_adhere_prob, double partial_adhere_prob);
	virtual ~AdherenceCategorySelector();

	AdherenceCategory selectCategory();
};

} /* namespace TransModel */

#endif /* SRC_ADHERENCECATEGORYSELECTOR_H_ */
