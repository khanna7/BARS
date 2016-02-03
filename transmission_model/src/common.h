/*
 * common.h
 *
 *  Created on: Jan 19, 2016
 *      Author: nick
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <memory>

#include "boost/random.hpp"

#include "Person.h"

namespace TransModel {

typedef std::shared_ptr<Person> PersonPtr;

typedef boost::variate_generator<boost::mt19937&, boost::binomial_distribution<>> BinomialGen;
typedef boost::variate_generator<boost::mt19937&, boost::poisson_distribution<>> PoissonGen;

const std::string ART_COVERAGE_BINOMIAL = "art.coverage.binomial";

}


#endif /* SRC_COMMON_H_ */
