/*
 * art_functions.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nick
 */

#ifndef SRC_ART_FUNCTIONS_H_
#define SRC_ART_FUNCTIONS_H_

#include <memory>

#include "Person.h"

namespace TransModel {

void initialize_adherence(std::shared_ptr<Person> person, double first_art_at_tick);

void schedule_adherence(std::shared_ptr<Person> person, double first_art_at_tick);

}





#endif /* SRC_ART_FUNCTIONS_H_ */
