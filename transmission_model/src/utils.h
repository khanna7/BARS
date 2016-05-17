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
 * Adds the variables in the specified r_file to the specified parameters as named
 * parameters. This should be run before any other R code as it sources the file
 * and calls ls() to get the variables.
 */
void add_from_R(const std::string& r_file, Parameters* params, std::shared_ptr<RInside> R);

}



#endif /* SRC_UTILS_H_ */
