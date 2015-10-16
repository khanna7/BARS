/*
 * Model.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <memory>
#include <vector>

#include "RInside.h"

#include "Person.h"
#include "RNetwork.h"

namespace TransModel {

typedef std::shared_ptr<Person> PersonPtr;

class Model {

private:
	std::shared_ptr<RInside> R;
	std::vector<PersonPtr> persons;
	std::shared_ptr<RNetwork> net;

	void runTransmission(double timestamp, double like_age_prob, double unlike_age_prob);

public:
	Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var);
	virtual ~Model();

	void run();


};

} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
