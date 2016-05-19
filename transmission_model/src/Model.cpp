/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

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
#include "Stats.h"
#include "StatsBuilder.h"
#include "file_utils.h"
//#include "EventWriter.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace fs = boost::filesystem;

namespace TransModel {

struct PersonToVALForSimulate {

	void operator()(const PersonPtr& p, List& vertex, double tick) const {
		vertex["role"] = p->role();
		vertex["inf.status"] = p->isInfected();
	}
};

struct PersonToVAL {

	void operator()(const PersonPtr& p, List& vertex, double tick) const {
		vertex[C_ID] = p->id();
		vertex["age"] = p->age();
		vertex["cd4.count.today"] = p->infectionParameters().cd4_count;
		vertex["circum.status"] = p->isCircumcised();
		vertex["role"] = p->role();

		vertex["diagnosed"] = p->isDiagnosed();
		const Diagnoser<GeometricDistribution>& diagnoser = p->diagnoser();
		vertex["number.of.tests"] = diagnoser.testCount();
		vertex["time.until.next.test"] = diagnoser.timeUntilNextTest(tick);
		vertex["lag.bet.diagnosis.and.art.init"] = p->diagnosisARTLag();


		if (p->isInfected()) {
			vertex["infectivity"] = p->infectivity();
			vertex["art.covered"] = p->isARTCovered();
			vertex["art.status"] = p->isOnART();
			vertex["inf.status"] = p->isInfected();
			vertex["time.since.infection"] = p->infectionParameters().time_since_infection;
			vertex["time.of.infection"] = p->infectionParameters().time_of_infection;
			vertex["age.at.infection"] = p->infectionParameters().age_at_infection;
			vertex["viral.load.today"] = p->infectionParameters().viral_load;
		} else {
			vertex["infectivity"] = 0;
			vertex["art.covered"] = NA_LOGICAL;
			vertex["art.status"] = NA_LOGICAL;
			vertex["inf.status"] = false;
			vertex["time.since.infection"] = NA_REAL;
			vertex["time.of.infection"] = NA_REAL;
			vertex["age.at.infection"] = NA_REAL;
			vertex["viral.load.today"] = 0;
		}

		if (p->isOnART()) {
			vertex["time.since.art.initiation"] = p->infectionParameters().time_since_art_init;
			vertex["time.of.art.initiation"] = p->infectionParameters().time_of_art_init;
			vertex["vl.art.traj.slope"] = p->infectionParameters().vl_art_traj_slope;
			vertex["cd4.at.art.initiation"] = p->infectionParameters().cd4_at_art_init;
			vertex["vl.at.art.initiation"] = p->infectionParameters().vl_at_art_init;
		} else {
			vertex["time.since.art.initiation"] = NA_REAL;
			vertex["time.of.art.initiation"] = NA_REAL;
			vertex["vl.art.traj.slope"] = NA_REAL;
			vertex["cd4.at.art.initiation"] = NA_REAL;
			vertex["vl.at.art.initiation"] = NA_REAL;
		}
	}
};

ARTScheduler* create_art_scheduler(double tick, float size_of_timestep) {
	double art_delay = Parameters::instance()->getDoubleParameter(ART_INIT_TIME);
	double art_at_tick = art_delay / size_of_timestep + tick;
	return new ARTScheduler((float) art_at_tick);
}

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

void init_stats() {
	StatsBuilder builder;
	builder.countsWriter(Parameters::instance()->getStringParameter(COUNTS_PER_TIMESTEP_OUTPUT_FILE));
	builder.partnershipEventWriter(Parameters::instance()->getStringParameter(PARTNERSHIP_EVENTS_FILE));
	builder.infectionEventWriter(Parameters::instance()->getStringParameter(INFECTION_EVENTS_FILE));
	builder.biomarkerWriter(Parameters::instance()->getStringParameter(BIOMARKER_FILE));
	builder.deathEventWriter(Parameters::instance()->getStringParameter(DEATH_EVENT_FILE));
	builder.personDataRecorder(Parameters::instance()->getStringParameter(PERSON_DATA_FILE));

	builder.createStatsSingleton();
}

void init_network_save(Model* model) {
	string save_prop = Parameters::instance()->getStringParameter(NET_SAVE_AT);
	vector<string> ats;
	boost::split(ats, save_prop, boost::is_any_of(","));
	if (ats.size() > 0) {
		ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
		for (auto& at : ats) {
			boost::trim(at);
			if (at == "end") {
				//std::cout << "scheduling at end" << std::endl;
				runner.scheduleEndEvent(Schedule::FunctorPtr(new MethodFunctor<Model>(model, &Model::saveRNetwork)));
			} else {
				double tick = stod(at);
				//std::cout << "scheduling at " << tick << std::endl;
				runner.scheduleEvent(tick + 0.1,
						Schedule::FunctorPtr(new MethodFunctor<Model>(model, &Model::saveRNetwork)));
			}
		}
	}
}

void init_biomarker_logging(Network<Person>& net, std::set<int>& ids_to_log) {
	int number_to_log = Parameters::instance()->getIntParameter(BIOMARKER_LOG_COUNT);
	std::vector<PersonPtr> persons;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		persons.push_back(*iter);
	}

	IntUniformGenerator gen = Random::instance()->createUniIntGenerator(0, persons.size() - 1);
	for (int i = 0; i < number_to_log; ++i) {
		int id = (int) gen.next();
		while (ids_to_log.find(id) != ids_to_log.end()) {
			id = (int) gen.next();
		}
		ids_to_log.emplace(id);
	}
}

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var, const std::string& cas_net_var) :
		R(ri), net(false), trans_runner(create_transmission_runner()), cd4_calculator(create_CD4Calculator()), viral_load_calculator(
				create_ViralLoadCalculator()), viral_load_slope_calculator(create_ViralLoadSlopeCalculator()), current_pop_size {
				0 }, previous_pop_size { 0 }, stage_map { }, persons_to_log { },
				person_creator{trans_runner, Parameters::instance()->getDoubleParameter(DAILY_TESTING_PROB),
					Parameters::instance()->getDoubleParameter(DETECTION_WINDOW)} {

	// get initial stats
	init_stats();

	List rnet = as<List>((*R)[net_var]);
	initialize_network(rnet, net, person_creator, MAIN_NETWORK_TYPE);
	rnet = as<List>((*R)[cas_net_var]);
	initialize_edges(rnet, net, CASUAL_NETWORK_TYPE);

	init_stage_map(stage_map);
	init_network_save(this);

	current_pop_size = net.vertexCount();

	init_biomarker_logging(net, persons_to_log);
	Stats* stats = TransModel::Stats::instance();
	stats->currentCounts().main_edge_count = net.edgeCount(MAIN_NETWORK_TYPE);
	stats->currentCounts().casual_edge_count = net.edgeCount(CASUAL_NETWORK_TYPE);
	stats->currentCounts().size = net.vertexCount();
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		PersonPtr p = *iter;
		if (p->isInfected()) {
			++stats->currentCounts().infected;
		}
		stats->personDataRecorder().initRecord(p, 0);
	}
	stats->resetForNextTimeStep();

	init_generators();

	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleStop(Parameters::instance()->getDoubleParameter("stop.at"));
	runner.scheduleEvent(1, 1, Schedule::FunctorPtr(new MethodFunctor<Model>(this, &Model::step)));

	runner.scheduleEndEvent(Schedule::FunctorPtr(new MethodFunctor<Model>(this, &Model::atEnd)));
}

void Model::atEnd() {
	// forces stat writing via destructors
	delete Stats::instance();
}

Model::~Model() {
}

void Model::updateThetaForm(const std::string& var_name) {
	NumericVector theta_form = as<NumericVector>((*R)[var_name]);
	theta_form[0] = theta_form[0] + std::log(previous_pop_size) - std::log(current_pop_size);
	((*R)[var_name]) = theta_form;
}

void Model::countOverlap() {
	int target_type = 0;
	int other_type = 0;
	if (net.edgeCount(MAIN_NETWORK_TYPE) < net.edgeCount(CASUAL_NETWORK_TYPE)) {
		target_type = MAIN_NETWORK_TYPE;
		other_type = CASUAL_NETWORK_TYPE;
	} else {
		target_type = CASUAL_NETWORK_TYPE;
		other_type = MAIN_NETWORK_TYPE;
	}

	Stats* stats = Stats::instance();
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		if ((*iter)->type() == target_type) {
			EdgePtr<Person> edge = (*iter);
			if (net.hasEdge(edge->v1(), edge->v2(), other_type) || net.hasEdge(edge->v2(), edge->v1(), other_type)) {
				++(stats->currentCounts().overlaps);
			}
		}
	}
}

void Model::step() {
	double t = RepastProcess::instance()->getScheduleRunner().currentTick();
	Stats* stats = Stats::instance();
	stats->currentCounts().tick = t;

	PersonToVALForSimulate p2val;
	float max_survival = Parameters::instance()->getFloatParameter(MAX_AGE);
	float size_of_timestep = Parameters::instance()->getIntParameter(SIZE_OF_TIMESTEP);

	std::cout << " ---- " << t << " ---- " << std::endl;
	simulate(R, net, p2val, t);
	if (Parameters::instance()->getBooleanParameter(COUNT_OVERLAPS)) {
		countOverlap();
	} else {
		stats->currentCounts().overlaps = -1;
	}
	entries(t, size_of_timestep);
	runTransmission(t, size_of_timestep);
	updateVitals(t, size_of_timestep, max_survival);
	previous_pop_size = current_pop_size;
	current_pop_size = net.vertexCount();

	std::cout << "pop sizes: " << previous_pop_size << ", " << current_pop_size << std::endl;
	updateThetaForm("theta.form");
	updateThetaForm("theta.form_cas");

	stats->currentCounts().main_edge_count = net.edgeCount(MAIN_NETWORK_TYPE);
	stats->currentCounts().casual_edge_count = net.edgeCount(CASUAL_NETWORK_TYPE);
	stats->currentCounts().size = net.vertexCount();
	stats->resetForNextTimeStep();
}

void Model::updateVitals(double t, float size_of_timestep, int max_age) {
	float sex_acts_per_timestep = Parameters::instance()->getFloatParameter(NUM_SEX_ACTS_PER_TIMESTEP);
	unsigned int dead_count = 0;
	Stats* stats = Stats::instance();
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
		} else {
			++stats->currentCounts().uninfected;
		}

		if (persons_to_log.find(person->id()) != persons_to_log.end()) {
			stats->recordBiomarker(t, person);
		}

		person->step(size_of_timestep);
		if (dead(t, person, max_age)) {
			iter = net.removeVertex(iter);
			++dead_count;
		} else {
			++iter;
		}
	}
}

void Model::entries(double tick, float size_of_timestep) {
	float min_age = Parameters::instance()->getFloatParameter(MIN_AGE);
	size_t pop_size = net.vertexCount();
	if (pop_size > 0) {
		double births_prob = Parameters::instance()->getDoubleParameter(DAILY_ENTRY_RATE);
		PoissonGen birth_gen(Random::instance()->engine(),
				boost::random::poisson_distribution<>(births_prob * pop_size));
		DefaultNumberGenerator<PoissonGen> gen(birth_gen);
		int entries = (int) gen.next();
		Stats* stats = Stats::instance();
		stats->currentCounts().entries = entries;
		std::cout << "entries: " << entries << std::endl;

		ARTScheduler* art_scheduler = create_art_scheduler(tick, size_of_timestep);
		double infected_prob = Parameters::instance()->getDoubleParameter(INIT_HIV_PREV_ENTRIES);

		for (int i = 0; i < entries; ++i) {
			VertexPtr<Person> p = person_creator(tick, min_age);
			if (Random::instance()->nextDouble() <= infected_prob) {
				// as if infected at previous timestep
				float infected_at = tick - (size_of_timestep * 1);
				trans_runner->infect(p, infected_at);
				if (p->isARTCovered()) {
					art_scheduler->addPerson(p);
				}
				float viral_load = viral_load_calculator.calculateViralLoad(p->infectionParameters());
				p->setViralLoad(viral_load);
				// update cd4
				float cd4 = cd4_calculator.calculateCD4(p->age(), p->infectionParameters());
				p->setCD4Count(cd4);
				++stats->currentCounts().infected_at_entry;
				stats->recordInfectionEvent(infected_at, p);
			}
			net.addVertex(p);
			Stats::instance()->personDataRecorder().initRecord(p, tick);
		}

		RepastProcess::instance()->getScheduleRunner().scheduleEvent(art_scheduler->artAtTick() + 0.1,
				repast::Schedule::FunctorPtr(art_scheduler));
	}
}

std::string get_net_out_filename(const std::string& file_name) {
	long tick = floor(RepastProcess::instance()->getScheduleRunner().currentTick());
	fs::path filepath(file_name);
	std::string stem = filepath.stem().string();

	std::stringstream ss;
	ss << stem << "_" << tick << filepath.extension().string();
	fs::path newName(filepath.parent_path() / ss.str());

	return newName.string();

}

void Model::saveRNetwork() {
	List rnet;
	std::map<unsigned int, unsigned int> idx_map;
	PersonToVAL p2val;

	long tick = floor(RepastProcess::instance()->getScheduleRunner().currentTick());
	create_r_network(tick, rnet, net, idx_map, p2val, MAIN_NETWORK_TYPE);
	std::string file_name = Parameters::instance()->getStringParameter(NET_SAVE_FILE);
	as<Function>((*R)["nw_save"])(rnet, unique_file_name(get_net_out_filename(file_name)), tick);

	if (Parameters::instance()->contains(CASUAL_NET_SAVE_FILE)) {
		idx_map.clear();
		List cas_net;
		create_r_network(tick, cas_net, net, idx_map, p2val, CASUAL_NETWORK_TYPE);
		file_name = Parameters::instance()->getStringParameter(CASUAL_NET_SAVE_FILE);
		as<Function>((*R)["nw_save"])(cas_net, unique_file_name(get_net_out_filename(file_name)), tick);
	}
}

bool Model::dead(double tick, PersonPtr person, int max_age) {
	int death_count = 0;
	bool died = false;
	// dead of old age
	if (person->deadOfAge(max_age)) {
		++death_count;
		++Stats::instance()->currentCounts().age_deaths;
		Stats::instance()->recordDeathEvent(tick, person, DeathEvent::AGE);
		Stats::instance()->personDataRecorder().recordDeath(person, tick);
		died = true;
	}

	if (!died && person->deadOfInfection()) {
		// infection deaths
		++death_count;
		++Stats::instance()->currentCounts().infection_deaths;
		Stats::instance()->recordDeathEvent(tick, person, DeathEvent::INFECTION);
		Stats::instance()->personDataRecorder().recordDeath(person, tick);
		died = true;
	}

	person->setDead(died);
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
				Stats::instance()->recordInfectionEvent(time_stamp, out_p, in_p, false, (*iter)->type());
			}
		} else if (!out_p->isInfected() && in_p->isInfected()) {
			if (trans_runner->determineInfection(in_p, out_p)) {
				infecteds.push_back(out_p);
				Stats::instance()->recordInfectionEvent(time_stamp, in_p, out_p, false, (*iter)->type());
			}
		}
	}

	ARTScheduler* art_scheduler = create_art_scheduler(time_stamp, size_of_timestep);

	Stats* stats = Stats::instance();
	for (auto& person : infecteds) {
		// if person has multiple partners who are infected,
		// person gets multiple chances to become infected from them
		// and so may appear more than once in the infecteds list
		if (!person->isInfected()) {
			trans_runner->infect(person, time_stamp);
			if (person->isARTCovered()) {
				art_scheduler->addPerson(person);
			}
			++stats->currentCounts().infected;
			stats->personDataRecorder().recordInfection(person, time_stamp);
		}
	}

	RepastProcess::instance()->getScheduleRunner().scheduleEvent(art_scheduler->artAtTick() + 0.1,
			repast::Schedule::FunctorPtr(art_scheduler));
}

} /* namespace TransModel */
