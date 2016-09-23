/*
 * ARTInitLagCalculator.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#include <exception>

#include "ARTInitLagCalculator.h"

namespace TransModel {

const double NEVER_INIT_ART = -999999.0;

const double EPSILON = .00001;

ARTInitCalculatorCreator::ARTInitCalculatorCreator() : di2m{-1}, di2_4m{-1}, di4_6m{-1}, di6_8m{-1}, di8_10m{-1}, di10_12m{-1},
		no_init{-1} {}

ARTInitCalculatorCreator::~ARTInitCalculatorCreator() {
}

void ARTInitCalculatorCreator::diagInit2m(double val) {
	di2m = val;
}
void ARTInitCalculatorCreator::diagInit2to4m(double val) {
	di2_4m = val;
}
void ARTInitCalculatorCreator::diagInit4to6m(double val) {
	di4_6m = val;
}
void ARTInitCalculatorCreator::diagInit6to8m(double val) {
	di6_8m = val;
}
void ARTInitCalculatorCreator::diagInit8to10m(double val) {
	di8_10m = val;
}
void ARTInitCalculatorCreator::diagInit10to12m(double val) {
	di10_12m = val;
}
void ARTInitCalculatorCreator::diagNeverInit(double val) {
	no_init = val;
}

ARTInitLagBin::ARTInitLagBin(double prob, double min, double max) : prob_(prob), gen(repast::Random::instance()->createUniIntGenerator(min, max)) {}
ARTInitLagBin::~ARTInitLagBin() {}

double ARTInitLagBin::calculateLag(double size_of_timestep)  {
	return gen.next() / size_of_timestep;
}


std::shared_ptr<ARTInitLagCalculator> ARTInitCalculatorCreator::createCalculator() {
	if (di2m < 0 || di2_4m < 0 || di4_6m < 0 || di6_8m < 0 || di8_10m < 0 || di10_12m < 0 || no_init < 0) {
		throw std::domain_error("Invalid value used to initialize ARTInitLagCalculator. Values must be >= 0.");
	}

	double sum = di2m + di2_4m + di4_6m + di6_8m + di8_10m + di10_12m + no_init;
	if (sum < 1.0 - EPSILON || sum > 1.0 + EPSILON) {
		throw std::domain_error("Invalid value used to initialize ARTInitLagCalculator. Sum of values must equal 1.");
	}


	return std::shared_ptr<ARTInitLagCalculator>(new ARTInitLagCalculator(di2m, di2_4m, di4_6m, di6_8m, di8_10m, di10_12m));
}

ARTInitLagCalculator::ARTInitLagCalculator(double di2m, double di24m, double di46m, double di68m, double di810m, double di1012m) :
	bin1{di2m, 0, 2 * 30}, bin2{bin1.probability() + di24m, 2 * 30, 4 * 30}, bin3{di46m + bin2.probability(), 4 * 30, 6 * 30},
	bin4{bin3.probability() + di68m, 6 * 30, 8 * 30}, bin5{bin4.probability() + di810m, 8 * 30, 10 * 30}, bin6{bin5.probability() + di1012m,
		10 * 30, 12 * 30}
{}

ARTInitLagCalculator::~ARTInitLagCalculator() {}

double ARTInitLagCalculator::calculateLag(float size_of_timestep) {
	double prob = repast::Random::instance()->nextDouble();
	if (prob <= bin1.probability()) {
		return bin1.calculateLag(size_of_timestep);
	} else if (prob <= bin2.probability()) {
		return bin2.calculateLag(size_of_timestep);
	} else if (prob <= bin3.probability()) {
		return bin3.calculateLag(size_of_timestep);
	} else if (prob <= bin4.probability()) {
		return bin4.calculateLag(size_of_timestep);
	} else if (prob <= bin5.probability()) {
		return bin5.calculateLag(size_of_timestep);
	} else if (prob <= bin6.probability()) {
		return bin6.calculateLag(size_of_timestep);
	} else {
		return NEVER_INIT_ART;
	}
}

} /* namespace TransModel */
