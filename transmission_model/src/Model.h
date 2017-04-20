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
#include "DayRangeCalculator.h"
#include "ARTScheduler.h"
#include "CondomUseAssigner.h"
#include "RangeWithProbability.h"

namespace TransModel {

struct TransmissionParameters {
	double prop_steady_sex_acts, prop_casual_sex_acts;
};


enum class CauseOfDeath { NONE, AGE, INFECTION, ASM};

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
	std::shared_ptr<DayRangeCalculator> art_lag_calculator;
	std::shared_ptr<GeometricDistribution> cessation_generator;
	CondomUseAssigner condom_assigner;
	RangeWithProbability asm_runner;

	void runTransmission(double timestamp);
	CauseOfDeath dead(double tick, PersonPtr person, int max_survival);
	void entries(double tick, float size_of_time_step);
	void deactivateEdges(int id, double time);

	/**
	 * @param uninfected empty vector into which the uninfected are placed
	 */
	void updateVitals(double time, float size_of_time_step, int max_survival,std::vector<PersonPtr>& uninfected);
	void runExternalInfections(std::vector<PersonPtr>& uninfected, double time);

	void infectPerson(PersonPtr& person, double time_stamp);
	void updateThetaForm(const std::string& var_name);
	void countOverlap();

	bool hasSex(int type);
	void schedulePostDiagnosisART(PersonPtr person, std::map<double, ARTScheduler*>& art_map, double tick, float size_of_timestep);

	/**
	 * Initializes PrEP cessation events for the initial set of persons.
	 */
	void initPrepCessation();

	/**
	 * Put prep with the specified probability.
	 */
	void updatePREPUse(double tick, double prob, PersonPtr person);

public:
	Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var, const std::string& cas_net_var);
	virtual ~Model();

	void step();
	void atEnd();
	void saveRNetwork();
};


} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
