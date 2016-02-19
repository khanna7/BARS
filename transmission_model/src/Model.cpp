/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"

#include "Parameters.h"
#include "Model.h"
#include "network_utils.h"
#include "common.h"
#include "TransmissionRunner.h"
#include "DiseaseParameters.h"
#include "PersonCreator.h"
#include "ARTScheduler.h"
#include "EventWriter.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace TransModel {

struct PersonToVAL {

	void operator()(const PersonPtr& p, List& vertex) const {
	}
};

shared_ptr<TransmissionRunner> create_transmission_runner() {
	float circ_mult = (float) Parameters::instance()->getDoubleParameter(CIRCUM_MULT);
	float prep_mult = (float) Parameters::instance()->getDoubleParameter(PREP_MULT);

//	string str_dur_inf = Parameters::instance()->getStringParameter(DUR_INF_BY_AGE);
//	vector<string> tokens;
//	// TODO error checking for correct number of values
//	repast::tokenize(str_dur_inf, tokens, ",");
//	for (auto s : tokens) {
//		dur_inf_by_age.push_back((float) std::atof(s.c_str()));
//	}
	// not by age yet
	float duration = Parameters::instance()->getFloatParameter(DUR_INF_BY_AGE);
	vector<float> dur_inf_by_age(4, duration);
	return make_shared<TransmissionRunner>(circ_mult, prep_mult, dur_inf_by_age);
}

CD4Calculator create_CD4Calculator() {
	// float size_of_timestep, float cd4_recovery_time,
	// float cd4_at_infection_male, float per_day_cd4_recovery
	float size_of_timestep = (float) Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
	float cd4_recovery_time = (float) Parameters::instance()->getDoubleParameter(CD4_RECOVERY_TIME);
	float cd4_at_infection = (float) Parameters::instance()->getDoubleParameter(CD4_AT_INFECTION_MALE);
	float per_day_cd4_recovery = (float) Parameters::instance()->getDoubleParameter(PER_DAY_CD4_RECOVERY);

	BValues b_values;
	b_values.b1_ref = Parameters::instance()->getFloatParameter(B1_REF);
	b_values.b2_african = Parameters::instance()->getFloatParameter(B2_AFRICAN);
	b_values.b3_female = Parameters::instance()->getFloatParameter(B3_FEMALE);
	b_values.b4_cd4_ref = Parameters::instance()->getFloatParameter(B4_CD4_REF);
	b_values.b5_african = Parameters::instance()->getFloatParameter(B5_AFRICAN);
	b_values.b6_age_15to29 = Parameters::instance()->getFloatParameter(B6_AGE_15TO29);
	b_values.b6_age_30to39 = Parameters::instance()->getFloatParameter(B6_AGE_30TO39);
	b_values.b6_age_40to49 = Parameters::instance()->getFloatParameter(B6_AGE_40TO49);
	b_values.b6_age_50ormore = Parameters::instance()->getFloatParameter(B6_AGE_50ORMORE);
	return CD4Calculator(size_of_timestep, cd4_recovery_time, cd4_at_infection, per_day_cd4_recovery, b_values);
}

ViralLoadCalculator create_ViralLoadCalculator() {
	SharedViralLoadParameters params;
	params.time_infection_to_peak_load = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_PEAK_VIRAL_LOAD);
	params.time_infection_to_set_point = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_VIRAL_SET_POINT);
	params.time_infection_to_late_stage = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_LATE_STAGE);
	params.time_to_full_supp = (float) Parameters::instance()->getDoubleParameter(TIME_TO_FULL_SUPP);
	params.peak_viral_load = (float) Parameters::instance()->getDoubleParameter(PEAK_VIRAL_LOAD);
	params.set_point_viral_load = (float) Parameters::instance()->getDoubleParameter(SET_POINT_VIRAL_LOAD);
	params.late_stage_viral_load = (float) Parameters::instance()->getDoubleParameter(LATE_STAGE_VIRAL_LOAD);
	params.undetectable_viral_load = (float) Parameters::instance()->getDoubleParameter(UNDETECTABLE_VL);

	return ViralLoadCalculator(params);
}

ViralLoadSlopeCalculator create_ViralLoadSlopeCalculator() {
	float time_to_full_supp = Parameters::instance()->getFloatParameter(TIME_TO_FULL_SUPP);
	float undetectable_vl = Parameters::instance()->getFloatParameter(UNDETECTABLE_VL);
	return ViralLoadSlopeCalculator(undetectable_vl, time_to_full_supp);
}

void init_stage_map(map<float, shared_ptr<Stage>> &stage_map) {
	float acute_max = (float) Parameters::instance()->getDoubleParameter(ACUTE_LENGTH_MAX);
	float chronic_max = (float) Parameters::instance()->getDoubleParameter(CHRONIC_LENGTH_MAX);
	//float late_max = (float) Parameters::instance()->getDoubleParameter(LATE_LENGTH_MAX);
	float acute_mult = (float) Parameters::instance()->getDoubleParameter(ACUTE_MULT);
	float late_mult = (float) Parameters::instance()->getDoubleParameter(LATE_MULT);
	float baseline_infectivity = (float) Parameters::instance()->getDoubleParameter(MIN_CHRONIC_INFECTIVITY_UNADJ);

	stage_map.emplace(acute_max, make_shared<AcuteStage>(baseline_infectivity, acute_mult, Range<float>(1, acute_max)));
	stage_map.emplace(chronic_max,
			make_shared<ChronicStage>(baseline_infectivity, Range<float>(acute_max, chronic_max)));
	// make late_max essentially open ended as infected persons
	// on ART can be in late stage forever where stage is not necessarily
	// the medical stage, but the Stage class for our purposes.
	float late_max = std::numeric_limits<float>::max();
	stage_map.emplace(late_max,
			make_shared<LateStage>(baseline_infectivity, late_mult, Range<float>(chronic_max, late_max)));
}

void init_generators() {
	float art_coverage_rate = Parameters::instance()->getDoubleParameter(BASELINE_ART_COVERAGE_RATE);
	BinomialGen coverage(repast::Random::instance()->engine(),
			boost::random::binomial_distribution<>(1, art_coverage_rate));
	Random::instance()->putGenerator(ART_COVERAGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(coverage));

	float circum_rate = Parameters::instance()->getDoubleParameter(CIRCUM_RATE);
	BinomialGen rate(repast::Random::instance()->engine(), boost::random::binomial_distribution<>(1, circum_rate));
	Random::instance()->putGenerator(CIRCUM_STATUS_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(rate));
}



Model::Model(shared_ptr<RInside>& ri, const std::string& net_var) :
		R(ri), net(false), trans_runner(create_transmission_runner()), cd4_calculator(create_CD4Calculator()), viral_load_calculator(
				create_ViralLoadCalculator()), viral_load_slope_calculator(create_ViralLoadSlopeCalculator()), current_pop_size{0},
				previous_pop_size{0}, max_id{0}, stage_map(), stats() {

	List rnet = as<List>((*R)[net_var]);
	PersonCreator person_creator(trans_runner);
	initialize_network(rnet, net, person_creator);
	init_stage_map(stage_map);

	current_pop_size = net.vertexCount();
	max_id = net.vertexCount();

	// get initial stats
	stats.incrementCurrentEdgeCount(net.edgeCount());
	stats.incrementCurrentSize(net.vertexCount());
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		if ((*iter)->isInfected()) {
			stats.incrementCurrentTotalInfectedCount(1);
		}
	}
	stats.resetForNextTimeStep();

	init_generators();

	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleStop(Parameters::instance()->getDoubleParameter("stop.at"));
	runner.scheduleEvent(1, 1, Schedule::FunctorPtr(new MethodFunctor<Model>(this, &Model::step)));

	EventWriter::initialize(Parameters::instance()->getStringParameter(EVENT_FILE),
			Parameters::instance()->getIntParameter(EVENT_FILE_BUFFER_SIZE));

}

Model::~Model() {
}

void Model::step() {
	PersonToVAL p2val;
	float max_survival = Parameters::instance()->getFloatParameter(MAX_AGE);
	float size_of_timestep = Parameters::instance()->getIntParameter(SIZE_OF_TIMESTEP);

	double t = RepastProcess::instance()->getScheduleRunner().currentTick();
	std::cout << " ---- " << t << " ---- " << std::endl;
	simulate(R, net, p2val, t);
	entries(t);
	runTransmission(t, size_of_timestep);
	updateVitals(size_of_timestep, max_survival);
	previous_pop_size = current_pop_size;
	current_pop_size = net.vertexCount();

	NumericVector theta_form = as<NumericVector>((*R)["theta.form"]);
	std::cout << "pop sizes: " << previous_pop_size << ", " << current_pop_size << std::endl;
	theta_form[0] = theta_form[0] + std::log(previous_pop_size) - std::log(current_pop_size);
	((*R)["theta_form"]) = theta_form;

	std::cout << "edge count: " << net.edgeCount() << std::endl;
	stats.incrementCurrentEdgeCount(net.edgeCount());
	stats.incrementCurrentSize(net.vertexCount());
	stats.resetForNextTimeStep();

	//stats.writeToCSV(output_file);
}

void Model::updateVitals(float size_of_timestep, int max_age) {
	float sex_acts_per_timestep = Parameters::instance()->getFloatParameter(NUM_SEX_ACTS_PER_TIMESTEP);
	unsigned int dead_count = 0;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd();) {
		PersonPtr person = (*iter);
		// update viral load
		if (person->isInfected()) {
			if (person->isOnART()) {
				float slope = viral_load_slope_calculator.calculateSlope(person->infectionParameters());
				person->setViralLoadARTSlope(slope);
			}

			float viral_load = viral_load_calculator.calculateViralLoad(person->infectionParameters());
			person->setViralLoad(viral_load);
			// update cd4
			float cd4 = cd4_calculator.calculateCD4(person->age(), person->infectionParameters());
			person->setCD4Count(cd4);

			// select stage, and use it
			float infectivity = stage_map.upper_bound(person->timeSinceInfection())->second->calculateInfectivity(
					person->infectionParameters(), sex_acts_per_timestep);
			person->setInfectivity(infectivity);
		}

		person->step(size_of_timestep);
		if (dead(person, max_age)) {
			iter = net.removeVertex(iter);
			++dead_count;
		} else {
			++iter;
		}
	}
	std::cout << "dead: " << dead_count << std::endl;
}

void Model::entries(double time) {
	float min_age = Parameters::instance()->getFloatParameter(MIN_AGE);
	size_t pop_size = net.vertexCount();
	if (pop_size > 0) {
		double births_prob = Parameters::instance()->getDoubleParameter(DAILY_ENTRY_RATE);
		PoissonGen birth_gen(Random::instance()->engine(),
				boost::random::poisson_distribution<>(births_prob * pop_size));
		DefaultNumberGenerator<PoissonGen> gen(birth_gen);
		int entries = (int) gen.next();
		stats.incrementCurrentBirthCount(entries);
		std::cout << "entries: " << entries << std::endl;
		for (int i = 0; i < entries; ++i) {
			int status = (int) repast::Random::instance()->getGenerator(CIRCUM_STATUS_BINOMIAL)->next();
			VertexPtr<Person> p = make_shared<Person>(max_id, min_age, status == 1);
			net.addVertex(p);
			++max_id;
		}
	}
}

bool Model::dead(PersonPtr person, int max_age) {
	int death_count = 0;
	bool died = false;
	// dead via mortality
	if (person->deadOfAge(max_age)) {
		++death_count;
		stats.incrementCurrentOADeathCount(1);
		died = true;
	}

	if (!died && person->deadOfInfection()) {
		// grim repear deaths
		++death_count;
		stats.incrementCurrentGRDeathCount(1);
		died = true;
	}

	return died;
}

void Model::runTransmission(double time_stamp, float size_of_timestep) {
	vector<PersonPtr> infecteds;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		PersonPtr out_p = (*iter)->v1();
		PersonPtr in_p = (*iter)->v2();
		if (out_p->isInfected() && !in_p->isInfected()) {
			if (trans_runner->determineInfection(out_p, in_p)) {
				infecteds.push_back(in_p);
			}
		} else if (!out_p->isInfected() && in_p->isInfected()) {
			if (trans_runner->determineInfection(in_p, out_p)) {
				infecteds.push_back(out_p);
			}
		}
	}


	double art_delay = Parameters::instance()->getDoubleParameter(ART_INIT_TIME);
	double art_at_tick = art_delay / size_of_timestep + time_stamp;
	ARTScheduler* art_scheduler = new ARTScheduler((float)art_at_tick);

	for (auto& person : infecteds) {
		// if person has multiple partners who are infected,
		// person gets multiple chances to become infected from them
		// and so may appear more than once in the infecteds list
		if (!person->isInfected()) {
			trans_runner->infect(person, time_stamp);
			if (person->isARTCovered()) {
				art_scheduler->addPerson(person);
			}
			stats.incrementCurrentTransmissionInfectedCount(1);
			stats.incrementCurrentTotalInfectedCount(1);
		}
	}

	RepastProcess::instance()->getScheduleRunner().scheduleEvent(art_at_tick + 0.1, repast::Schedule::FunctorPtr(art_scheduler));
}

} /* namespace TransModel */
