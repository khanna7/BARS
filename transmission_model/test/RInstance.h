/*
 * RTests.h
 *
 *  Created on: Nov 4, 2015
 *      Author: nick
 */

#ifndef TEST_RINSTANCE_H_
#define TEST_RINSTANCE_H_

#include <memory>

#include <RInside.h>

struct RInstance {

	// R has to be global within this unit as it
	// doesn't cleanup propertly when initialized within
	// a method / function.
	static std::shared_ptr<RInside> rptr;

};


#endif /* TEST_RINSTANCE_H_ */
