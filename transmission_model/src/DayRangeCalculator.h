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

namespace TransModel {

class DayRangeBin {
public:
	DayRangeBin(double prob, double min, double max);
	~DayRangeBin();

	double calculateLag(float size_of_timestep);

	double probability() const {
		return prob_;
	}

	void setProbability(double val) {
		prob_ = val;
	}

private:

	double prob_;
	repast::IntUniformGenerator gen;
};

/**
 * Calculates the lag between diagnosis and ART initiation.
 */
class DayRangeCalculator {
private:
	friend class DayRangeCalculatorCreator;

	std::vector<DayRangeBin> bins_;

	DayRangeCalculator(std::vector<DayRangeBin>& bins);

public:
	virtual ~DayRangeCalculator();

	double calculateLag(float size_of_timestep);
};

class DayRangeCalculatorCreator {

private:
	std::vector<DayRangeBin> bins;

public:
	DayRangeCalculatorCreator();
	virtual ~DayRangeCalculatorCreator();

	void addBin(const std::string& bin_definition);

	std::shared_ptr<DayRangeCalculator> createCalculator();

	void clear() {
		bins.clear();
	}
};

} /* namespace TransModel */

#endif /* SRC_DAYRANGECALCULATOR_H_ */
