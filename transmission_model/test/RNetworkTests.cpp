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

// R has to be global within this unit as it
// doesn't cleanup propertly when initialized within
// a method / function.
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

TEST(NetworkTests, VertexActivity) {
	std::string cmd = "source(file=\"../test_data/is_active_test.R\")";
	r->parseEvalQ(cmd);

	RNetwork r_net(r, "triangle");

	SEXP v1(r_net.vertexList()[0]);
	SEXP v2(r_net.vertexList()[1]);
	SEXP v3(r_net.vertexList()[2]);

	for (int i = 1; i < 5; ++i) {
		ASSERT_TRUE(is_vertex_active(v1, i, false));
		// v2 and v3 have no spell list so should be inactive
		ASSERT_FALSE(is_vertex_active(v2, i, false));
		ASSERT_FALSE(is_vertex_active(v3, i, false));
	}

	// spell from (1 to 5)
	ASSERT_FALSE(is_vertex_active(v1, 5, false));
	// v2 and v3 have no spell list so should be inactive
	ASSERT_FALSE(is_vertex_active(v2, 5, false));
	ASSERT_FALSE(is_vertex_active(v3, 5, false));
	// should be true because no spell list but default_activity
	// is now true
	ASSERT_TRUE(is_vertex_active(v3, 5, true));
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

