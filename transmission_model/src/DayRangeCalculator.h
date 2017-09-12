/*
 * DayRangeLagCalculator.h
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#ifndef SRC_DAYRANGECALCULATOR_H_
#define SRC_DAYRANGECALCULATOR_H_

#include <memory>

#include "repast_hpc/Random.h"

#include "ProbDist.h"
#include "GeometricDistribution.h"

namespace TransModel {

class DayRangeBin {
public:
	DayRangeBin(double min, double max);
	~DayRangeBin();

	double calculateLag(float size_of_timestep);
private:


	GeometricDistribution gen;
};

/**
 * Calculates the lag between diagnosis and ART initiation.
 */
class DayRangeCalculator {
private:
	friend class DayRangeCalculatorCreator;

	ProbDist<std::shared_ptr<DayRangeBin>> dist;
	DayRangeCalculator(ProbDist<std::shared_ptr<DayRangeBin>> prob_dist);

public:
	virtual ~DayRangeCalculator();

	double calculateLag(float size_of_timestep);
};

class DayRangeCalculatorCreator {

private:
	ProbDistCreator<std::shared_ptr<DayRangeBin>> pd_creator;

public:
	DayRangeCalculatorCreator();
	virtual ~DayRangeCalculatorCreator();

	void addBin(const std::string& bin_definition);

	std::shared_ptr<DayRangeCalculator> createCalculator();

	void clear() {
		pd_creator.clear();
	}
};

} /* namespace TransModel */

#endif /* SRC_DAYRANGECALCULATOR_H_ */
