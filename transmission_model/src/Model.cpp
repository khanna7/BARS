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
#include "REdge.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace TransModel {

const std::string LIKE_AGE_BINOMIAL = "like.age.binomial";
const std::string UNLIKE_AGE_BINOMIAL = "unlike.age.binomial";
const std::string DAILY_DEATH_BINOMIAL = "daily.death.binomial";

typedef boost::variate_generator<boost::mt19937&, boost::binomial_distribution<>> BinomialGen;
typedef boost::variate_generator<boost::mt19937&, boost::poisson_distribution<>> PoissonGen;

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var) :
		R(ri), persons(), net(), popsize() {
	net = make_shared<RNetwork>(ri, net_var);

	for (int i = 0; i < net->getVertexCount(); ++i) {
		persons.emplace(std::make_pair(i, make_shared<Person>(i, net)));
	}

	popsize.push_back(persons.size());

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

void infection_draw(PersonPtr infectee, PersonPtr infector, vector<int>& infected_idxs) {
	NumberGenerator* gen =
			infector->isYoung() && infectee->isYoung() ?
					Random::instance()->getGenerator(LIKE_AGE_BINOMIAL) :
					Random::instance()->getGenerator(UNLIKE_AGE_BINOMIAL);
	int draw = (int) gen->next();
	if (draw) {
		infected_idxs.push_back(infectee->id());
	}
}

void Model::run() {
	int max_survival = Parameters::instance()->getIntParameter(MAX_SURVIVAL);
	for (int t = 2; t < 26; ++t) {
		std::cout << " ---- " << t << " ---- " << std::endl;
		(*R)["time"] = t;
		net->simulate();
		net->updateNetworkFromR();
		runTransmission(t);
		deaths(t, max_survival);
		births(t);

		NumericVector theta_form = as<NumericVector>((*R)["theta.form"]);
		std::cout << "pop sizes: " << popsize[t - 2] << ", " << popsize[t - 1] << std::endl;
		theta_form[0] = theta_form[0] + std::log(popsize[t - 2]) - std::log(popsize[t - 1]);
		net->updateNetworkToR();

		std::cout << "edge count: " << net->getEdgeCount() << std::endl;
		std::cout << "active edge count: " << net->getActiveEdgeCount(t) << std::endl;
	}
}

void Model::deactivateEdges(int id, double time) {
	vector<SEXP> edges;
	net->edges(id, time, COMBINED, edges);
	for (auto& edge : edges) {
		deactivate_edge(edge, time, R_PosInf);
	}
}

void print(std::vector<int>& ids) {
	int count = 0;
	for (int i : ids) {
		std::cout << ", " << i;
		if (++count == 20) {
			std::cout << "\n";
		}
	}
	std::cout << std::endl;
}

void Model::births(double time) {
	size_t pop_size = persons.size();
	if (pop_size > 0) {
		double births_prob = Parameters::instance()->getDoubleParameter(DAILY_BIRTH_RATE);
		PoissonGen birth_gen(Random::instance()->engine(),
				boost::random::poisson_distribution<>(births_prob * pop_size));
		DefaultNumberGenerator<PoissonGen> gen(birth_gen);
		int births = (int) gen.next();
		std::cout << "births: " << births << std::endl;
		if (births > 0) {
			vector<int> ids;
			net->addVertices(births, ids);
			for (int id : ids) {
				persons.emplace(make_pair(id, make_shared<Person>(id, net, time)));
				net->activateVertex(id, time, R_PosInf);
			}
		}
	}
	popsize.push_back(persons.size());
}

void Model::deaths(double time, int max_survival) {

	// persons list should contain only active vertices
	// increment age and mortality
	int death_count = 0;
	for (auto iter = persons.begin(); iter != persons.end();) {
		PersonPtr person = iter->second;
		person->incrementAge();

		// dead via mortality: deactivated and removed from persons
		// vector
		if (person->age() == max_survival) {
			if (person->id() == 457) {
				std::cout << "457 dead" << std::endl;
			}
			deactivateEdges(person->id(), time);
			++death_count;
			// persons should deactivate in destructor.
			// removing from list should make shared_ptr ref count = 0
			// so destroyed
			iter = persons.erase(iter);
		} else {
			++iter;
		}
	}

	// persons list should contain only active vertices
	// grim repear deaths
	NumberGenerator* gen = Random::instance()->getGenerator(DAILY_DEATH_BINOMIAL);
	for (auto iter = persons.begin(); iter != persons.end();) {
		if ((int) gen->next() == 1) {
			PersonPtr person = iter->second;
			person->setTimeOfDeath(time);
			deactivateEdges(person->id(), time);
			iter = persons.erase(iter);
			++death_count;
		} else {
			++iter;
		}
	}
	std::cout << "deaths: " << death_count << std::endl;
}

void Model::runTransmission(double time) {
	vector<int> infected_idxs;

	List edgeList = net->edgeList();
	REdge edge;
	for (auto& item : edgeList) {
		edge.bind(item);
		if (edge.isActive(time, false)) {
			int out_idx = edge.targetVertex();
			int in_idx = edge.sourceVertex();
			auto oi = persons.find(out_idx);
			auto ii = persons.find(in_idx);
			if (oi != persons.end() && ii != persons.end()) {
				PersonPtr out_p = oi->second;
				PersonPtr in_p = ii->second;
				if (out_p->isInfected() && !in_p->isInfected()) {
					infection_draw(in_p, out_p, infected_idxs);
				} else if (!out_p->isInfected() && in_p->isInfected()) {
					infection_draw(out_p, in_p, infected_idxs);
				}
			}
		}
	}

	for (auto idx : infected_idxs) {
		shared_ptr<Person> p = persons[idx];
		p->setInfected(true);
		p->setInfectionTime(time);
	}
}

} /* namespace TransModel */
