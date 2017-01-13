/*
 * main.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nick
 */

#include "boost/mpi.hpp"
#include "gtest/gtest.h"
#include "RInside.h"

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	boost::mpi::environment env(argc, argv);
	return RUN_ALL_TESTS();
}

