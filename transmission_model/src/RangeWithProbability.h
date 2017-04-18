/*
 * RangeWithProbability.h
 *
 *  Created on: Feb 6, 2017
 *      Author: nick
 */

#ifndef SRC_RANGEWITHPROBABILITY_H_
#define SRC_RANGEWITHPROBABILITY_H_

#include <vector>

namespace TransModel {

struct RangeBin {
	float min, max;
	double prob;
};



class RangeWithProbability {

private:
	friend class RangeWithProbabilityCreator;
	std::vector<RangeBin> bins;

	RangeWithProbability(std::vector<RangeBin> range_bins);

public:
	virtual ~RangeWithProbability();
	bool run(float rangeValue, double draw);
};

class RangeWithProbabilityCreator {

private:
	std::vector<RangeBin> bins;

public:
	RangeWithProbabilityCreator();
	virtual ~RangeWithProbabilityCreator();

	void addBin(float min, float max, double prob);
	/**
	 * @bin_defintion format is x.min_max where x is ignored
	 */
	void addBin(const std::string& bin_definition, double bin_probability);
	RangeWithProbability createRangeWithProbability();

	void clear() {
		bins.clear();
	}

};


} /* namespace TransModel */

#endif /* SRC_RANGEWITHPROBABILITY_H_ */
