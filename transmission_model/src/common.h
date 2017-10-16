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

namespace TransModel {

class Person;

typedef std::shared_ptr<Person> PersonPtr;

typedef boost::variate_generator<boost::mt19937&, boost::binomial_distribution<>> BinomialGen;
typedef boost::variate_generator<boost::mt19937&, boost::poisson_distribution<>> PoissonGen;

const std::string NON_TESTERS_BINOMIAL_LT = "non.testers.binomial.lt";
const std::string NON_TESTERS_BINOMIAL_GTE = "non.testers.binomial.gte";

const std::string CIRCUM_STATUS_BINOMIAL = "circum.status.binomial";

const std::string C_ID = "c.id";

const int STEADY_NETWORK_TYPE = 0;
const int CASUAL_NETWORK_TYPE = 1;

const int VERSATILE = 0;
const int INSERTIVE = 1;
const int RECEPTIVE = 2;

}


#endif /* SRC_COMMON_H_ */
