/*
 * ARTInitLagCalculator.h
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#ifndef SRC_ARTINITLAGCALCULATOR_H_
#define SRC_ARTINITLAGCALCULATOR_H_

#include <memory>

#include "repast_hpc/Random.h"

namespace TransModel {

extern const double NEVER_INIT_ART;

class ARTInitLagBin {
public:
	ARTInitLagBin(double prob, double min, double max);
	~ARTInitLagBin();

	double calculateLag(double size_of_timestep);

	double probability() const {
		return prob_;
	}

private:
	double prob_;
	repast::IntUniformGenerator gen;
};

/**
 * Calculates the lag between diagnosis and ART initiation.
 */
class ARTInitLagCalculator {
private:
	friend class ARTInitCalculatorCreator;

	ARTInitLagBin bin1, bin2, bin3, bin4, bin5, bin6;

	ARTInitLagCalculator(double di2m, double di24m, double di46m, double di68m, double di810m, double di1012m);

public:
	virtual ~ARTInitLagCalculator();

	double calculateLag(float size_of_timestep);
};

class ARTInitCalculatorCreator {
private:
	double di2m, di2_4m, di4_6m, di6_8m, di8_10m, di10_12m;
	double no_init;

public:
	ARTInitCalculatorCreator();
	virtual ~ARTInitCalculatorCreator();

	void diagInit2m(double val);
	void diagInit2to4m(double val);
	void diagInit4to6m(double val);
	void diagInit6to8m(double val);
	void diagInit8to10m(double val);
	void diagInit10to12m(double val);
	void diagNeverInit(double val);

	std::shared_ptr<ARTInitLagCalculator> createCalculator();
};

} /* namespace TransModel */

#endif /* SRC_ARTINITLAGCALCULATOR_H_ */
