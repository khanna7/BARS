/*
 * PrepUpdateManagerAlpha.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Stats.h"
#include "PrepCessationEvent.h"
#include "ProportionalPrepUptakeManager.h"

namespace TransModel {

ProportionalPrepUptakeManager::ProportionalPrepUptakeManager(PrepUseData& data, double age_threshold) : PrepUptakeManager(data, age_threshold),
		uninfected_count(0), young(), old(), base_use((data.base_use_lt + data.base_use_gte) / 2),
		stop_prob((data.daily_p_prob_gte + data.daily_p_prob_lt) / 2), k(0) {
	onYearEnded();
}

ProportionalPrepUptakeManager::~ProportionalPrepUptakeManager() {}

void ProportionalPrepUptakeManager::onYearEnded()  {
	if (prep_data.years_to_increase == 0) {
		k = base_use;
		//std::cout << "Proportional PUM " << year << ", k: " << k << std::endl;
	} else if (year <= prep_data.years_to_increase) {
		double increment = (prep_data.increment_lt + prep_data.increment_gte) / 2;
		k = base_use + (increment * year);
		//std::cout << "Proportional PUM " << year << ", k: " << k << std::endl;
		year++;
	}
}

void ProportionalPrepUptakeManager::processPerson(double tick, PersonPtr& person) {
	if (!person->isInfected()) {
		++uninfected_count;
		if (!person->isOnPrep()) {
			std::vector<PersonPtr>& vec = person->age() < age_threshold_ ? young : old;
			vec.push_back(person);
		}
	}
}

void ProportionalPrepUptakeManager::run(double tick) {
	int n = (int)(uninfected_count * k * stop_prob);
	double ny = young.size() * prep_data.alpha;
	double no = old.size();
	double total = ny + no;
	double threshold = ny / total;

	//std::cout << "n: " << n << ", alpha: " << prep_data.alpha << ", young size: " <<
	//		young.size() << ", ny: " << ny << ", no: " << no << ", threshold: " <<
	//		threshold << std::endl;


	repast::Random* rnd = repast::Random::instance();

	for (int i = 0; i < n; ++i) {
		double draw = rnd->nextDouble();
		std::vector<PersonPtr>& vec = draw <= threshold ? young : old;

		repast::IntUniformGenerator gen = rnd->createUniIntGenerator(0, vec.size() - 1);
		int idx = (int)gen.next();
		PersonPtr& p = vec[idx];
		while (p->isOnPrep()) {
			int idx = (int)gen.next();
			p = vec[idx];
		}
		updateUse(tick, p);
	}
	uninfected_count = 0;
	young.clear();
	old.clear();

}

} /* namespace TransModel */
