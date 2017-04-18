/*
 * RTests.cpp
 *
 *  Created on: May 16, 2016
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "RInstance.h"

using namespace Rcpp;

void test_tick_exists(const std::string& net_name) {
	List net = as<List>((*RInstance::rptr)[net_name]);
	List gal = as<List>(net["gal"]);
	ASSERT_EQ(10, as<int>(gal["tick"]));
}

void test_tick_not_exists(const std::string& net_name) {
	List net = as<List>((*RInstance::rptr)[net_name]);
	List gal = as<List>(net["gal"]);
	ASSERT_FALSE(gal.containsElementNamed("tick"));
}

/*
 * Test that existing main and casual
 * network are properly replaced by the
 * serialized ones.
 */
TEST(LoadTests, TestNetworkLoading) {
	std::string cmd = "source(file=\"../r/network_model/transmission_model.R\")";
	RInstance::rptr->parseEvalQ(cmd);
	test_tick_not_exists("nw");
	test_tick_not_exists("n_cas");

	cmd = "source(file=\"../r/network_model/load_serialized_networks.R\")";
	RInstance::rptr->parseEvalQ(cmd);
	as<Function>((*RInstance::rptr)["load.networks"])("../test_data/main_network_10.RDS", "../test_data/casual_network_10.RDS");
	test_tick_exists("nw");
	test_tick_exists("n_cas");
}



