/*
 * PersonCreator.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: nick
 */

#include "PersonCreator.h"

using namespace Rcpp;

namespace TransModel {

PersonCreator::PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner) :
		id(0), trans_runner_(trans_runner) {
}

PersonCreator::~PersonCreator() {
}

PersonPtr PersonCreator::operator()(Rcpp::List& val) {
	float age = as<float>(val["age"]);
	bool circum_status = as<bool>(val["circum.status"]);
	int role = as<int>(val["role"]);
	PersonPtr person = std::make_shared<Person>(id++, age, circum_status, role);
	person->infection_parameters_.cd4_count = as<float>(val["cd4.count.today"]);

	bool infected = as<bool>(val["inf.status"]);
	if (infected) {
		person->infection_parameters_.infection_status = true;
		person->infection_parameters_.time_since_infection = as<float>(val["time.since.infection"]);
		person->infection_parameters_.time_of_infection = as<float>(val["time.of.infection"]);
		person->infection_parameters_.age_at_infection = as<float>(val["age.at.infection"]);
		person->infection_parameters_.dur_inf_by_age =
				trans_runner_->durInfByAge(person->infection_parameters_.age_at_infection);
		person->infection_parameters_.art_covered = as<bool>(val["art.covered"]);
		person->infection_parameters_.art_status = as<bool>(val["art.status"]);
		if (person->infection_parameters_.art_status) {
			person->infection_parameters_.time_since_art_init = as<float>(val["time.since.art.initiation"]);
			person->infection_parameters_.time_of_art_init = as<float>(val["time.of.art.initiation"]);
			person->infection_parameters_.vl_art_traj_slope = as<float>(val["vl.art.traj.slope"]);
			person->infection_parameters_.cd4_at_art_init = as<float>(val["cd4.at.art.initiation"]);
			person->infection_parameters_.vl_at_art_init = as<float>(val["vl.at.art.initiation"]);
		}

		person->infection_parameters_.viral_load = as<float>(val["viral.load.today"]);
	}
	return person;
}

} /* namespace TransModel */
