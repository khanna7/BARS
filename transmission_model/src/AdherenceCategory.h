/*
 * AdherenceCategory.h
 *
 *  Created on: Sep 23, 2016
 *      Author: nick
 */

#ifndef SRC_ADHERENCECATEGORY_H_
#define SRC_ADHERENCECATEGORY_H_

namespace TransModel {

enum class AdherenceCategory {NEVER, ALWAYS, PARTIAL_PLUS, PARTIAL_MINUS, NA};

struct AdherenceData {
	double probability;
	AdherenceCategory category;

    AdherenceData() {}
	AdherenceData(double prob, AdherenceCategory cat) : probability{prob}, category{cat} {}
};

} /* namespace TransModel */

#endif /* SRC_ADHERENCECATEGORY_H_ */
