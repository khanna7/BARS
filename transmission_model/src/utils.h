/*
 * utils.h
 *
 *  Created on: Feb 16, 2016
 *      Author: nick
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>

#include "Parameters.h"

// this has to come after Parameters.h so that the
// RInside Free macro doesn't conflict with openmpi's
// Free function.
#include "RInside.h"

namespace TransModel {

/**
 * Parses a string into individual parameter key value pairs and then creates
 * the corresponding variables in R.
 *
 * @param param_string a string with the format X=Y,...
 */
void param_string_to_R_vars(const std::string& param_string, std::shared_ptr<RInside> R);


/**
 *
 */
void init_parameters(const std::string& non_derived, const std::string& derived, const std::string& param_string, Parameters* params, std::shared_ptr<RInside> R);

}



#endif /* SRC_UTILS_H_ */
