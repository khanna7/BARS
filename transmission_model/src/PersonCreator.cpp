/*
 * PersonCreator.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: nick
 */

#include "Parameters.h"

#include "PersonCreator.h"
#include "Diagnoser.h"
#include "art_functions.h"


using namespace Rcpp;

namespace TransModel {

PersonCreator::PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner, double daily_testing_prob, double detection_window) :
		id(0), trans_runner_(trans_runner), dist{std::make_shared<GeometricDistribution>(daily_testing_prob, 1)}, detection_window_(detection_window) {
}

PersonCreator::~PersonCreator() {
}

int calculate_role() {
	double insertive = Parameters::instance()->getDoubleParameter(PR_INSERTIVE);
	double receptive = Parameters::instance()->getDoubleParameter(PR_RECEPTIVE) + insertive;

	double draw = repast::Random::instance()->nextDouble();
	if (draw <= insertive) {
		return 0;
	} else if (draw <= receptive) {
		return 1;
	} else {
		return 2;
	}
}

PersonPtr PersonCreator::operator()(double tick, float age) {
	int status = (int) repast::Random::instance()->getGenerator(CIRCUM_STATUS_BINOMIAL)->next();
	Diagnoser<GeometricDistribution> diagnoser(tick, detection_window_, dist);
	PersonPtr person = std::make_shared<Person>(id++, age, status == 1, calculate_role(), diagnoser);
	person->testable_= ((int) repast::Random::instance()->getGenerator(NON_TESTERS_BINOMIAL)->next()) == 0;
	person->prep_ = ((int) repast::Random::instance()->getGenerator(PREP_BINOMIAL)->next()) == 1 ? PrepStatus::ON : PrepStatus::OFF;

	return person;
}

PersonPtr PersonCreator::operator()(Rcpp::List& val, double tick) {
	//std::cout << "------------" << std::endl;
	//Rf_PrintValue(val);
	float age = as<float>(val["age"]);
	bool circum_status = as<bool>(val["circum.status"]);
	int role = as<int>(val["role"]);

	float next_test_at = tick + as<double>(val["time.until.next.test"]);
	// float detection_window, float next_test_at, unsigned int test_count, std::shared_ptr<G> generator
	Diagnoser<GeometricDistribution> diagnoser(detection_window_, next_test_at, as<unsigned int>(val["number.of.tests"]), dist);
	PersonPtr person = std::make_shared<Person>(id++, age, circum_status, role, diagnoser);
	person->diagnosed_ = as<bool>(val["diagnosed"]);
	person->testable_ = !(as<bool>(val["non.testers"]));
	person->infection_parameters_.cd4_count = as<float>(val["cd4.count.today"]);

	bool infected = as<bool>(val["inf.status"]);
	if (infected) {
		person->infection_parameters_.infection_status = true;
		person->infection_parameters_.time_since_infection = as<float>(val["time.since.infection"]);
		person->infection_parameters_.time_of_infection = as<float>(val["time.of.infection"]);
		person->infection_parameters_.age_at_infection = as<float>(val["age.at.infection"]);
		person->infection_parameters_.dur_inf_by_age =
				trans_runner_->durInfByAge(person->infection_parameters_.age_at_infection);
		person->infection_parameters_.art_status = as<bool>(val["art.status"]);
		if (person->infection_parameters_.art_status) {
			person->infection_parameters_.time_since_art_init = as<float>(val["time.since.art.initiation"]);
			person->infection_parameters_.time_of_art_init = as<float>(val["time.of.art.initiation"]);
			person->infection_parameters_.vl_art_traj_slope = as<float>(val["vl.art.traj.slope"]);
			person->infection_parameters_.cd4_at_art_init = as<float>(val["cd4.at.art.initiation"]);
			person->infection_parameters_.vl_at_art_init = as<float>(val["vl.at.art.initiation"]);

			if (val.containsElementNamed("adherence.category")) {
				person->setAdherence(static_cast<AdherenceCategory>(as<int>(val["adherence.category"])));
				if (person->adherence() == AdherenceCategory::PARTIAL) {
					schedule_adherence(person, tick);
				}
			} else {
				initialize_adherence(person, tick);
				if (person->adherence() == AdherenceCategory::NEVER) {
					person->infection_parameters_.art_status = false;
				}
			}
		}

		person->infection_parameters_.viral_load = as<float>(val["viral.load.today"]);
	} else {
		//  the prep.status attribute only exists in uninfected persons in the R model
		person->prep_ = as<bool>(val["prep.status"]) ? PrepStatus::ON : PrepStatus::OFF;
	}

	return person;
}

} /* namespace TransModel */
