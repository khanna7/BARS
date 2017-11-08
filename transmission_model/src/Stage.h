/*
 * Stage.h
 *
 *  Created on: Jan 18, 2016
 *      Author: nick
 */

#ifndef SRC_STAGE_H_
#define SRC_STAGE_H_

#include "DiseaseParameters.h"
#include "Range.h"

namespace TransModel {

class Stage {

protected:
	float baseline_infectivity_, multiplier_;
	float vl_inc;

private:
	Range<float> range_;

public:
	Stage(float baseline_infectivity, float multiplier, const Range<float>& range, float viral_load_increment);
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
	AcuteStage(float baseline_infectivity, float multiplier, const Range<float>& range, float viral_load_increment);
	virtual ~AcuteStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

class ChronicStage: public Stage {

public:
	ChronicStage(float baseline_infectivity, const Range<float>& range, float viral_load_increment);
	virtual ~ChronicStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

class LateStage: public Stage {

public:
	LateStage(float baseline_infectivity, float multiplier, const Range<float>& range, float viral_load_increment);
	virtual ~LateStage();

	virtual float calculateInfectivity(const InfectionParameters& params) override;
};

} /* namespace TransModel */

#endif /* SRC_STAGE_H_ */
