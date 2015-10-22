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

class NetworkTests: public ::testing::Test {

protected:

	RNetwork* r_net;
	SEXP edge1, edge2, edge3, edge4;
	SEXP v1, v2, v3;

	NetworkTests() : r_net(0),
			edge1(0), edge2(0), edge3(0), edge4(0), v1(0), v2(0), v3(0) {
		std::string cmd = "source(file=\"../test_data/is_active_test.R\")";
		r->parseEvalQ(cmd);

		r_net = new RNetwork(r, "triangle");
		edge1 = r_net->edgeList()[0];
		edge2 = r_net->edgeList()[1];
		edge3 = r_net->edgeList()[2];
		edge4 = r_net->edgeList()[3];

		v1 = r_net->vertexList()[0];
		v2 = r_net->vertexList()[1];
		v3 = r_net->vertexList()[2];
	}

	virtual ~NetworkTests() {
		delete r_net;
	}

};

TEST_F(NetworkTests, TestNetworkAttributes) {
	ASSERT_EQ(3, r_net->getVertexCount());
	ASSERT_EQ(2, r_net->getVertexAttribute<int>(0, "age"));
	ASSERT_EQ(12, r_net->getVertexAttribute<int>(1, "age"));
	ASSERT_EQ(18, r_net->getVertexAttribute<int>(2, "age"));
}

TEST_F(NetworkTests, VertexActivity) {
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

TEST_F(NetworkTests, TestEdgeIsActive) {
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

	for (int i = 6; i < 1000; ++i) {
		ASSERT_TRUE(is_edge_active(edge2, i, false));
		ASSERT_FALSE(is_edge_active(edge1, i, false));
		ASSERT_FALSE(is_edge_active(edge3, i, false));
	}
}

// we don't need too many tests here because
// the actual insert spell is done by stat-net
// produced code, so assume that's correct.
TEST_F(NetworkTests, TestEdgeActivate) {
	// no activity spells
	ASSERT_FALSE(is_edge_active(edge4, 11, false));
	activate_edge(edge4, 10, 20);
	ASSERT_TRUE(is_edge_active(edge4, 11, false));

	// existing activity spell
	ASSERT_FALSE(is_edge_active(edge3, 11, false));
	activate_edge(edge3, 10, 20);
	ASSERT_TRUE(is_edge_active(edge3, 11, false));
}

// TODO Add unit tests for vertex activation and deactivation


TEST_F(NetworkTests, TestEdgeDeactivate) {
	Rcpp::Function print_debug((*r)["print.debug"]);
	// edge 4 has no activity list, this should add
	// an activity list
	deactivate_edge(edge4, 1, R_PosInf);
	// true so will return true if activity list doesn't exist
	ASSERT_FALSE(is_edge_active(edge4, 2, true));

	 // TODO more of these
}

}

