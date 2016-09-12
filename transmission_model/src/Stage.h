/*
 * Stage.h
 *
 *  Created on: Jan 18, 2016
 *      Author: nick
 */

#ifndef SRC_STAGE_H_
#define SRC_STAGE_H_

#include "DiseaseParameters.h"

namespace TransModel {

/**
 * Range from [x, y)
 */
template<typename T>
class Range {

private:
	T start_, end_;

public:
	Range(T start, T end);
	virtual ~Range();
	bool within(T val);
};

template<typename T>
Range<T>::Range(T start, T end) :
		start_(start), end_(end) {
}

template<typename T>
Range<T>::~Range() {
}

template<typename T>
bool Range<T>::within(T val) {
	return val >= start_ && val < end_;
}

class Stage {

protected:
	float baseline_infectivity_, multiplier_;

private:
	Range<float> range_;

public:
	Stage(float baseline_infectivity, float multiplier, const Range<float>& range);
	virtual ~Stage();

	/**
	 * Gets whether or not in this Stage given the time since
	 * infection.
	 */
	bool in(float time_since_infection);
	virtual float calculateInfectivity(const InfectionParameters& params) = 0;
};

class AcuteStage: public Stage {

public:
	AcuteStage(float baseline_infectivity, float multiplier, const Range<float>& range);
	virtual ~AcuteStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

class ChronicStage: public Stage {

public:
	ChronicStage(float baseline_infectivity, const Range<float>& range);
	virtual ~ChronicStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

class LateStage: public Stage {

public:
	LateStage(float baseline_infectivity, float multiplier, const Range<float>& range);
	virtual ~LateStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

} /* namespace TransModel */

#endif /* SRC_STAGE_H_ */
