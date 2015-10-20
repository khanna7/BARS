/*
 * RNetworkTests.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nick
 */

#include "RInside.h"

#include "RNetwork.h"
#include "gtest/gtest.h"

using namespace std;

namespace TransModel {

shared_ptr<RInside> r = make_shared<RInside>();

TEST(NetworkTests, TestNetworkAttributes) {
	std::string cmd = "source(file=\"../test_data/is_active_test.R\")";
	r->parseEvalQ(cmd);

	RNetwork r_net(r, "triangle");
	ASSERT_EQ(3, r_net.getVertexCount());
	ASSERT_EQ(2, r_net.getVertexAttribute<int>(0, "age"));
	ASSERT_EQ(12, r_net.getVertexAttribute<int>(1, "age"));
	ASSERT_EQ(18, r_net.getVertexAttribute<int>(2, "age"));
}

TEST(NetworkTests, TestEdges) {
	std::string cmd = "source(file=\"../test_data/is_active_test.R\")";
	r->parseEvalQ(cmd);

	RNetwork r_net(r, "triangle");
	SEXP edge1(r_net.edgeList()[0]);
	SEXP edge2(r_net.edgeList()[1]);
	SEXP edge3(r_net.edgeList()[2]);

	ASSERT_EQ(1, edge_out_idx(edge1));
	ASSERT_EQ(2, edge_in_idx(edge1));
	ASSERT_EQ(2, edge_out_idx(edge2));
	ASSERT_EQ(3, edge_in_idx(edge2));
	ASSERT_EQ(3, edge_out_idx(edge3));
	ASSERT_EQ(1, edge_in_idx(edge3));

	ASSERT_TRUE(is_edge_active(edge1, 1, false));
	ASSERT_TRUE(is_edge_active(edge2, 1, false));
	ASSERT_TRUE(is_edge_active(edge3, 1, false));

	ASSERT_FALSE(is_edge_active(edge2, 2, false));
	ASSERT_FALSE(is_edge_active(edge3, 2, false));
	ASSERT_TRUE(is_edge_active(edge1, 2, false));
}

}

