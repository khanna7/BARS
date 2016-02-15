/*
 * PersonCreator.h
 *
 *  Created on: Feb 15, 2016
 *      Author: nick
 */

#ifndef SRC_PERSONCREATOR_H_
#define SRC_PERSONCREATOR_H_

#include <memory>

#include "RInside.h"

#include "Person.h"
#include "TransmissionRunner.h"
#include "common.h"

namespace TransModel {


class PersonCreator {

private:
	int id;
	std::shared_ptr<TransmissionRunner> trans_runner_;

public:
	PersonCreator(std::shared_ptr<TransmissionRunner>& trans_runner);
	virtual ~PersonCreator();

	PersonPtr operator()(Rcpp::List& val);
};

} /* namespace TransModel */

#endif /* SRC_PERSONCREATOR_H_ */
