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
#include <map>

#include "RInside.h"

#include "Person.h"
#include "Network.h"

namespace TransModel {

class Model {

private:
	std::shared_ptr<RInside> R;
	Network<Person> net;
	std::vector<unsigned int> popsize;
	unsigned int max_id;

	void runTransmission(double timestamp);
	void deaths(double time, int max_survival);
	void births(double time);
	void deactivateEdges(int id, double time);

public:
	Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var);
	virtual ~Model();

	void run();
};


} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
