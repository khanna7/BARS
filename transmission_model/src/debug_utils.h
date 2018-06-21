/*
 * debug_utils.h
 *
 *  Created on: Jan 13, 2017
 *      Author: nick
 */

#ifndef SRC_DEBUG_UTILS_H_
#define SRC_DEBUG_UTILS_H_

#include "Network.h"
#include "Person.h"

namespace TransModel {

    void write_edges(Network<Person>& net, const std::string fname);

}



#endif /* SRC_DEBUG_UTILS_H_ */
