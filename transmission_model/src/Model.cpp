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


using namespace Rcpp;
using namespace std;
using namespace repast;

namespace TransModel {

const std::string LIKE_AGE_BINOMIAL = "like.age.binomial";
const std::string UNLIKE_AGE_BINOMIAL = "unlike.age.binomial";


typedef boost::variate_generator<boost::mt19937&, boost::binomial_distribution<>> BinomialGen;

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var) : R(ri), persons(), net() {
	net = make_shared<RNetwork>(ri, net_var);

	for (int i = 0; i < net->getVertexCount(); ++i) {
		PersonPtr p = make_shared<Person>(i, net);
		persons.push_back(p);
	}

	double like_age_prob = Parameters::instance()->getDoubleParameter(LIKE_AGE_PROB);
	BinomialGen like_age_gen(Random::instance()->engine(), boost::random::binomial_distribution<>(1, like_age_prob));
	Random::instance()->putGenerator(LIKE_AGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(like_age_gen));

	double unlike_age_prob = Parameters::instance()->getDoubleParameter(UNLIKE_AGE_PROB);
	BinomialGen unlike_age_gen(Random::instance()->engine(), boost::random::binomial_distribution<>(1, unlike_age_prob));
	Random::instance()->putGenerator(UNLIKE_AGE_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(unlike_age_gen));
}

Model::~Model() {
}

void infection_draw(PersonPtr infectee, PersonPtr infector, double like_age_prob, double unlike_age_prob,
		vector<int>& infected_idxs) {
	NumberGenerator* gen = infector->isYoung() && infectee->isYoung() ? Random::instance()->getGenerator(LIKE_AGE_BINOMIAL) :
			Random::instance()->getGenerator(UNLIKE_AGE_BINOMIAL);
	int draw = (int)gen->next();
	if (draw) {
		infected_idxs.push_back(infectee->id());
	}
}

void Model::run() {
	double like_age_prob = Parameters::instance()->getDoubleParameter(LIKE_AGE_PROB);
	double unlike_age_prob = Parameters::instance()->getDoubleParameter(UNLIKE_AGE_PROB);
	for (int t = 2; t < 20; ++t) {
		std::cout << "t: " << t << " simulating" << std::endl;
		(*R)["time"] = t;
	    net->simulate();
	    net->updateRNetwork();
	    std::cout << "t: " << t << " transmitting" << std::endl;
	    runTransmission(t, like_age_prob, unlike_age_prob);
	}
}

void Model::runTransmission(double ts, double like_age_prob, double unlike_age_prob) {
	vector<int> infected_idxs;
	IsActiveEdge isActive;
	List edgeList = net->edgeList();
	for (auto& item : edgeList) {

		if (isActive(item)) {
			Rcpp::List mel_item = Rcpp::as<Rcpp::List>(item);
			// -1 because R list start at 1
			int out_idx = (Rcpp::as<Rcpp::IntegerVector>(mel_item["outl"]))[0] - 1;
			int in_idx = (Rcpp::as<Rcpp::IntegerVector>(mel_item["inl"]))[0] - 1;
			//std::cout << out_idx << ", " << in_idx << std::endl;
			PersonPtr out_p = persons[out_idx];
			PersonPtr in_p = persons[in_idx];
			if (out_p->isInfected() && !in_p->isInfected()) {
				infection_draw(in_p, out_p, like_age_prob, unlike_age_prob, infected_idxs);
			} else if (!out_p->isInfected() && in_p->isInfected()) {
				infection_draw(out_p, in_p, like_age_prob, unlike_age_prob, infected_idxs);
			}
		}
	}

	for (auto idx : infected_idxs) {
		shared_ptr<Person> p = persons[idx];
		p->setInfected(true);
		p->setInfectionTime(ts);
	}

}

} /* namespace TransModel */
