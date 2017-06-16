/*
 * art_functions.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nick
 */

#ifndef SRC_ADHERENCE_FUNCTIONS_H_
#define SRC_ADHERENCE_FUNCTIONS_H_

#include <memory>

#include "Person.h"
#include "AdherenceCategory.h"

namespace TransModel {

void initialize_art_adherence(std::shared_ptr<Person> person, double tick, AdherenceCategory category);
void initialize_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick);
void schedule_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick);

AdherenceData initialize_prep_adherence(AdherenceCategory category);
AdherenceData initialize_prep_adherence();

}





#endif /* SRC_ADHERENCE_FUNCTIONS_H_ */
