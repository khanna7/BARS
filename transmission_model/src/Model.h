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
#include "Stats.h"
#include "Stage.h"
#include "TransmissionRunner.h"
#include "CD4Calculator.h"
#include "ViralLoadCalculator.h"

namespace TransModel {

class Model {

private:
	std::shared_ptr<RInside> R;
	Network<Person> net;
	std::shared_ptr<TransmissionRunner> trans_runner;
	CD4Calculator cd4_calculator;
	ViralLoadCalculator viral_load_calculator;
	std::vector<unsigned int> popsize;
	unsigned int max_id;
	std::map<float, std::shared_ptr<Stage>> stage_map;
	Stats stats;

	void runTransmission(double timestamp);
	bool dead(PersonPtr person, int max_survival);
	void births(double time);
	void deactivateEdges(int id, double time);
	void updateVitals(float size_of_time_step, int max_survival);

public:
	Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var);
	virtual ~Model();

	void run(const std::string& output_file);
};


} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
