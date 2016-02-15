/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/Utilities.h"

#include "Parameters.h"
#include "Model.h"
#include "network_utils.h"
#include "common.h"
#include "TransmissionRunner.h"
#include "DiseaseParameters.h"
#include "PersonCreator.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace TransModel {



struct PersonToVAL {

	void operator()(const PersonPtr& p, List& vertex) const {

	}

};

shared_ptr<TransmissionRunner> create_transmission_runner() {
	float circ_mult = (float) Parameters::instance()->getDoubleParameter(CIRC_MULT);
	float prep_mult = (float) Parameters::instance()->getDoubleParameter(PREP_MULT);
	string str_dur_inf = Parameters::instance()->getStringParameter(DUR_INF_BY_AGE);
	vector<string> tokens;
	// TODO error checking for correct number of values
	repast::tokenize(str_dur_inf, tokens, ",");
	vector<float> dur_inf_by_age;
	for (auto s : tokens) {
		dur_inf_by_age.push_back((float) std::atof(s.c_str()));
	}
	return make_shared<TransmissionRunner>(circ_mult, prep_mult, dur_inf_by_age);

}

CD4Calculator create_CD4Calculator() {
	// float size_of_timestep, float cd4_recovery_time,
	// float cd4_at_infection_male, float per_day_cd4_recovery
	float size_of_timestep = (float) Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
	float cd4_recovery_time = (float) Parameters::instance()->getDoubleParameter(CD4_RECOVERY_TIME);
	float cd4_at_infection = (float) Parameters::instance()->getDoubleParameter(CD4_AT_INFECTION);
	float per_day_cd4_recovery = (float) Parameters::instance()->getDoubleParameter(PER_DAY_CD4_RECOVERY);

	return CD4Calculator(size_of_timestep, cd4_recovery_time, cd4_at_infection, per_day_cd4_recovery);
}

ViralLoadCalculator create_ViralLoadCalculator() {
	SharedViralLoadParameters params;
	params.time_infection_to_peak_load = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_PEAK_LOAD);
	params.time_infection_to_set_point = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_SET_POINT);
	params.time_infection_to_late_stage = (float) Parameters::instance()->getDoubleParameter(
			TIME_INFECTION_TO_LATE_STAGE);
	params.time_to_full_supp = (float) Parameters::instance()->getDoubleParameter(TIME_TO_FULL_SUPP);
	params.peak_viral_load = (float) Parameters::instance()->getDoubleParameter(PEAK_VIRAL_LOAD);
	params.set_point_viral_load = (float) Parameters::instance()->getDoubleParameter(SET_POINT_VIRAL_LOAD);
	params.late_stage_viral_load = (float) Parameters::instance()->getDoubleParameter(LATE_STAGE_VIRAL_LOAD);
	params.undetectable_viral_load = (float) Parameters::instance()->getDoubleParameter(UNDETECTABLE_VIRAL_LOAD);

	return ViralLoadCalculator(params);
}

void init_stage_map(map<float, shared_ptr<Stage>> &stage_map) {
	float acute_max = (float) Parameters::instance()->getDoubleParameter(ACUTE_RANGE_MAX_NUMERATOR);
	float chronic_max = (float) Parameters::instance()->getDoubleParameter(CHRONIC_RANGE_MAX_NUMERATOR);
	float late_max = (float) Parameters::instance()->getDoubleParameter(LATE_RANGE_MAX_NUMERATOR);
	float acute_mult = (float) Parameters::instance()->getDoubleParameter(ACUTE_MULTIPLIER);
	float late_mult = (float) Parameters::instance()->getDoubleParameter(LATE_MULTIPLIER);
	float baseline_infectivity = (float) Parameters::instance()->getDoubleParameter(MIN_CHRONIC_INFECTIVITY_UNADJ);

	stage_map.emplace(acute_max, make_shared<AcuteStage>(baseline_infectivity, acute_mult, Range<float>(1, acute_max)));
	stage_map.emplace(chronic_max,
			make_shared<ChronicStage>(baseline_infectivity, Range<float>(acute_max, chronic_max)));
	stage_map.emplace(late_max,
			make_shared<LateStage>(baseline_infectivity, late_mult, Range<float>(chronic_max, late_max)));
}

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var) :
		R(ri), net(false), trans_runner(create_transmission_runner()), cd4_calculator(create_CD4Calculator()), viral_load_calculator(
				create_ViralLoadCalculator()), popsize(), max_id(0), stage_map(), stats() {

	List rnet = as<List>((*R)[net_var]);
	PersonCreator person_creator(trans_runner);
	initialize_network(rnet, net, person_creator);
	init_stage_map(stage_map);

	popsize.push_back(net.vertexCount());
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

	float art_coverage_rate = Parameters::instance()->getDoubleParameter(ART_COVERAGE_RATE);
	BinomialGen coverage(repast::Random::instance()->engine(),
			boost::random::binomial_distribution<>(1, art_coverage_rate));
	Random::instance()->putGenerator(ART_COVERAGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(coverage));

}

Model::~Model() {
}

void Model::run(const std::string& output_file) {
	PersonToVAL p2val;
	int max_survival = Parameters::instance()->getIntParameter(MAX_SURVIVAL);
	float size_of_timestep = Parameters::instance()->getIntParameter(SIZE_OF_TIMESTEP);
	for (int t = 2; t < 26; ++t) {
		std::cout << " ---- " << t << " ---- " << std::endl;
		simulate(R, net, p2val, t);
		births(t);
		runTransmission(t);
		updateVitals(size_of_timestep, max_survival);

		NumericVector theta_form = as<NumericVector>((*R)["theta.form"]);
		std::cout << "pop sizes: " << popsize[t - 2] << ", " << popsize[t - 1] << std::endl;
		theta_form[0] = theta_form[0] + std::log(popsize[t - 2]) - std::log(popsize[t - 1]);
		((*R)["theta_form"]) = theta_form;

		std::cout << "edge count: " << net.edgeCount() << std::endl;
		stats.incrementCurrentEdgeCount(net.edgeCount());
		stats.incrementCurrentSize(net.vertexCount());
		stats.resetForNextTimeStep();
	}
	stats.writeToCSV(output_file);
}

void Model::updateVitals(float size_of_timestep, int max_survival) {
	float sex_acts_per_timestep = (float) Parameters::instance()->getDoubleParameter(SEX_ACTS_PER_TIMESTEP);
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd();) {
		PersonPtr person = (*iter);
		// update viral load
		if (person->isInfected()) {
			float viral_load = viral_load_calculator.calculateViralLoad(person->infectionParameters());
			person->setViralLoad(viral_load);
			// update cd4
			float cd4 = cd4_calculator.calculateCD4(person->age(), person->infectionParameters());
			person->setCD4Count(cd4);

			// TODO: Q. update vl.slope???

			// select stage, and use it
			float infectivity = stage_map.upper_bound(person->timeSinceInfection())->second->calculateInfectivity(
					person->infectionParameters(), sex_acts_per_timestep);
			person->setInfectivity(infectivity);
		}

		person->updateVitals(size_of_timestep);
		if (dead(person, max_survival)) {
			iter = net.removeVertex(iter);
		} else {
			++iter;
		}
	}
}

// TODO rename to entries
void Model::births(double time) {
	size_t pop_size = net.vertexCount();
	if (pop_size > 0) {
		double births_prob = Parameters::instance()->getDoubleParameter(DAILY_BIRTH_RATE);
		PoissonGen birth_gen(Random::instance()->engine(),
				boost::random::poisson_distribution<>(births_prob * pop_size));
		DefaultNumberGenerator<PoissonGen> gen(birth_gen);
		int births = (int) gen.next();
		stats.incrementCurrentBirthCount(births);
		std::cout << "births: " << births << std::endl;
		for (int i = 0; i < births; ++i) {
			// TODO should set to min age and do circumsize draw
			VertexPtr<Person> p = make_shared<Person>(max_id, 0, false);
			//p->setTimeOfBirth(time);
			net.addVertex(p);
			++max_id;
		}
	}
	popsize.push_back(net.vertexCount());
}

bool Model::dead(PersonPtr person, int max_survival) {
	int death_count = 0;
	bool died = false;
	// dead via mortality
	if (person->deadOfAge(max_survival)) {
		++death_count;
		stats.incrementCurrentOADeathCount(1);
		died = true;
	}

	if (!died && person->deadOfAIDS()) {
		// grim repear deaths
		++death_count;
		stats.incrementCurrentGRDeathCount(1);
		died = true;
	}

	std::cout << "deaths: " << death_count << std::endl;
	return died;
}

void Model::runTransmission(double time) {
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

	for (auto& person : infecteds) {
		// if person has multiple partners who are infected,
		// person gets multiple chances to become infected from them
		// and so may appear more than once in the infecteds list
		if (!person->isInfected()) {
			trans_runner->infect(person);
			stats.incrementCurrentTransmissionInfectedCount(1);
			stats.incrementCurrentTotalInfectedCount(1);
		}
	}
}

} /* namespace TransModel */
