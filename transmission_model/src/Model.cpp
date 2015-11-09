/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "boost/random.hpp"
#include "repast_hpc/Random.h"
#include "Parameters.h"

#include "Model.h"
#include "network_utils.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace TransModel {

const std::string LIKE_AGE_BINOMIAL = "like.age.binomial";
const std::string UNLIKE_AGE_BINOMIAL = "unlike.age.binomial";
const std::string DAILY_DEATH_BINOMIAL = "daily.death.binomial";

typedef boost::variate_generator<boost::mt19937&, boost::binomial_distribution<>> BinomialGen;
typedef boost::variate_generator<boost::mt19937&, boost::poisson_distribution<>> PoissonGen;

typedef VertexPtr<Person> PersonPtr;

struct PersonCreator {

	int id;
	PersonCreator() :
			id(0) {
	}

	VertexPtr<Person> operator()(List& val) {
		int age = as<int>(val["age"]);
		bool infected = as<bool>(val["inf.status"]);
		return std::make_shared<Person>(id++, age, infected);
	}

};

struct YoungSetter {

	void operator()(const VertexPtr<Person>& p, List& vertex) const {
		vertex["young"] = p->isYoung() ? 1 : 0;
	}

};

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var) :
		R(ri), net(false), popsize(), max_id(0), stats() {

	List rnet = as<List>((*R)[net_var]);
	PersonCreator person_creator;
	initialize_network(rnet, net, person_creator);

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

	double like_age_prob = Parameters::instance()->getDoubleParameter(LIKE_AGE_PROB);
	BinomialGen like_age_gen(Random::instance()->engine(), boost::random::binomial_distribution<>(1, like_age_prob));
	Random::instance()->putGenerator(LIKE_AGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(like_age_gen));

	double unlike_age_prob = Parameters::instance()->getDoubleParameter(UNLIKE_AGE_PROB);
	BinomialGen unlike_age_gen(Random::instance()->engine(),
			boost::random::binomial_distribution<>(1, unlike_age_prob));
	Random::instance()->putGenerator(UNLIKE_AGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(unlike_age_gen));

	double daily_death_prob = Parameters::instance()->getDoubleParameter(DAILY_DEATH_PROB);
	BinomialGen dail_death_gen(Random::instance()->engine(),
			boost::random::binomial_distribution<>(1, daily_death_prob));
	Random::instance()->putGenerator(DAILY_DEATH_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(dail_death_gen));
}

Model::~Model() {
}

void infection_draw(PersonPtr infectee, PersonPtr infector, vector<PersonPtr>& infecteds) {
	NumberGenerator* gen =
			infector->isYoung() && infectee->isYoung() ?
					Random::instance()->getGenerator(LIKE_AGE_BINOMIAL) :
					Random::instance()->getGenerator(UNLIKE_AGE_BINOMIAL);
	int draw = (int) gen->next();
	if (draw) {
		infecteds.push_back(infectee);
	}
}

void Model::run(const std::string& output_file) {
	YoungSetter young_setter;
	int max_survival = Parameters::instance()->getIntParameter(MAX_SURVIVAL);
	for (int t = 2; t < 26; ++t) {
		std::cout << " ---- " << t << " ---- " << std::endl;
		simulate(R, net, young_setter, t);
		runTransmission(t);
		deaths(t, max_survival);
		births(t);

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
			VertexPtr<Person> p = make_shared<Person>(max_id, 0, false);
			p->setTimeOfBirth(time);
			net.addVertex(p);
			++max_id;
		}
	}
	popsize.push_back(net.vertexCount());
}

void Model::deaths(double time, int max_survival) {
	int death_count = 0;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd();) {
		PersonPtr person = (*iter);
		person->incrementAge();
		// dead via mortality
		if (person->age() == max_survival) {
			iter = net.removeVertex(iter);
			++death_count;
			stats.incrementCurrentOADeathCount(1);
		} else {
			++iter;
		}
	}

	// grim repear deaths
	NumberGenerator* gen = Random::instance()->getGenerator(DAILY_DEATH_BINOMIAL);
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd();) {
		if ((int) gen->next() == 1) {
			PersonPtr person = (*iter);
			iter = net.removeVertex(iter);
			++death_count;
			stats.incrementCurrentGRDeathCount(1);
		} else {
			++iter;
		}
	}
	std::cout << "deaths: " << death_count << std::endl;
}

void Model::runTransmission(double time) {
	vector<PersonPtr> infecteds;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		PersonPtr out_p = (*iter)->v1();
		PersonPtr in_p = (*iter)->v2();
		if (out_p->isInfected() && !in_p->isInfected()) {
			infection_draw(in_p, out_p, infecteds);
		} else if (!out_p->isInfected() && in_p->isInfected()) {
			infection_draw(out_p, in_p, infecteds);
		}
	}

	for (auto& person : infecteds) {
		person->setInfected(true, time);
	}
	stats.incrementCurrentTransmissionInfectedCount(infecteds.size());
	stats.incrementCurrentTotalInfectedCount(infecteds.size());
}

} /* namespace TransModel */
