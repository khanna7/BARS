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
#include "RNetwork.h"

namespace TransModel {

typedef std::shared_ptr<Person> PersonPtr;

class Model {

private:
	std::shared_ptr<RInside> R;
	std::map<int, PersonPtr> persons;
	std::shared_ptr<RNetwork> net;
	std::vector<int> popsize;

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
