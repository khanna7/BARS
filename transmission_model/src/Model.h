/*
 * Model.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <memory>
#include <vector>
#include <map>

#include "RInside.h"

#include "Person.h"
#include "Network.h"
#include "Stage.h"
#include "TransmissionRunner.h"
#include "CD4Calculator.h"
#include "ViralLoadCalculator.h"
#include "ViralLoadSlopeCalculator.h"
#include "PersonCreator.h"
#include "ARTInitLagCalculator.h"
#include "ARTScheduler.h"

namespace TransModel {

struct TransmissionParameters {
	double prop_steady_sex_acts, prop_casual_sex_acts;
	double prop_steady_condom, prop_casual_condom;
};

class Model {

private:
	std::shared_ptr<RInside> R;
	Network<Person> net;
	std::shared_ptr<TransmissionRunner> trans_runner;
	CD4Calculator cd4_calculator;
	ViralLoadCalculator viral_load_calculator;
	ViralLoadSlopeCalculator viral_load_slope_calculator;
	unsigned int current_pop_size, previous_pop_size;
	std::map<float, std::shared_ptr<Stage>> stage_map;
	std::set<int> persons_to_log;
	PersonCreator person_creator;
	TransmissionParameters trans_params;
	std::shared_ptr<ARTInitLagCalculator> art_lag_calculator;

	void runTransmission(double timestamp);
	bool dead(double tick, PersonPtr person, int max_survival);
	void entries(double tick, float size_of_time_step);
	void deactivateEdges(int id, double time);
	void updateVitals(double time, float size_of_time_step, int max_survival);
	void updateThetaForm(const std::string& var_name);
	void countOverlap();

	void initParamsForTransmission(int type, double& prob, bool& condom_used);
	void scheduleART(PersonPtr person, std::map<double, ARTScheduler*>& art_map, double tick, float size_of_timestep);

public:
	Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var, const std::string& cas_net_var);
	virtual ~Model();

	void step();
	void atEnd();
	void saveRNetwork();
};


} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
