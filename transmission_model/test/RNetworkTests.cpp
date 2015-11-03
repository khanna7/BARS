/*
 * RRNetworkTests.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: nick
 */

#include "RInside.h"

#include "RNetwork.h"
#include "REdge.h"
#include "gtest/gtest.h"

using namespace std;

namespace TransModel {

// R has to be global within this unit as it
// doesn't cleanup propertly when initialized within
// a method / function.
shared_ptr<RInside> r = make_shared<RInside>();

class RNetworkTests: public ::testing::Test {

protected:

	RNetwork* r_net;
	SEXP edge1, edge2, edge3, edge4;
	SEXP v1, v2, v3, v4;
	SEXP val;

	RNetworkTests() :
			r_net(0), edge1(0), edge2(0), edge3(0), edge4(0), v1(0), v2(0), v3(0), v4(0), val(0) {
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
		v4 = r_net->vertexList()[3];
	}

	virtual ~RNetworkTests() {
		delete r_net;
	}
};

TEST_F(RNetworkTests, TestNetworkAttributes) {
	ASSERT_EQ(4, r_net->getVertexCount());
	ASSERT_EQ(2, r_net->getVertexAttribute<int>(0, "age"));
	ASSERT_EQ(12, r_net->getVertexAttribute<int>(1, "age"));
	ASSERT_EQ(18, r_net->getVertexAttribute<int>(2, "age"));

	ASSERT_EQ(4, r_net->getEdgeCount());
}

TEST_F(RNetworkTests, VertexActivity) {
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

TEST_F(RNetworkTests, TestEdgeIsActive) {
	ASSERT_EQ(1, edge_out_idx(edge1));
	ASSERT_EQ(2, edge_in_idx(edge1));
	ASSERT_EQ(2, edge_out_idx(edge2));
	ASSERT_EQ(3, edge_in_idx(edge2));
	ASSERT_EQ(3, edge_out_idx(edge3));
	ASSERT_EQ(1, edge_in_idx(edge3));

	REdge redge1(edge1);
	ASSERT_TRUE(redge1.isActive(1, false));
	ASSERT_TRUE(is_edge_active(edge2, 1, false));
	ASSERT_TRUE(is_edge_active(edge3, 1, false));

	ASSERT_FALSE(is_edge_active(edge2, 2, false));
	ASSERT_FALSE(is_edge_active(edge3, 2, false));
	ASSERT_TRUE(redge1.isActive(2, false));

	for (int i = 6; i < 1000; ++i) {
		ASSERT_TRUE(is_edge_active(edge2, i, false));
		ASSERT_FALSE(redge1.isActive(i, false));
		ASSERT_FALSE(is_edge_active(edge3, i, false));
	}
}

TEST_F(RNetworkTests, TestVertexActivate) {
	// test with raw val
	SEXP val = Rcpp::as<Rcpp::List>((*r)["triangle"])["val"];
	// v2 has not spell list, activate it
	ASSERT_FALSE(is_vertex_active(v2, 2, false));

	v2 = activate_vertex(val, 1, v2, 1, 10);
	for (int i = 1; i < 10; ++i) {
		ASSERT_TRUE(is_vertex_active(v2, i, false));
	}
	ASSERT_FALSE(is_vertex_active(v2, 10, false));

	// activate via the network
	v2 = r_net->activateVertex(1, 15, 20);
	ASSERT_FALSE(is_vertex_active(v2, 12, false));
	ASSERT_TRUE(is_vertex_active(v2, 16, false));

	v2 = r_net->activateVertex(1, v2, 13, 21);
	ASSERT_TRUE(is_vertex_active(v2, 13, false));
	ASSERT_TRUE(is_vertex_active(v2, 20.9, false));

	// make sure network is updated as well.
	SEXP v2c = r_net->vertexList()[1];
	ASSERT_FALSE(is_vertex_active(v2c, 12, false));
	ASSERT_TRUE(is_vertex_active(v2c, 13, false));
	ASSERT_TRUE(is_vertex_active(v2c, 20.9, false));
}

TEST_F(RNetworkTests, TestVertexDeactivate) {
	v2 = r_net->activateVertex(1, 15, 20);
	v2 = r_net->activateVertex(1, 22, 25);

	v2 = r_net->deactivateVertex(1, 7, 16);
	ASSERT_FALSE(is_vertex_active(v2, 15, false));
	ASSERT_TRUE(is_vertex_active(v2, 17, false));

	v2 = r_net->deactivateVertex(1, v2, 18, 23);
	ASSERT_FALSE(is_vertex_active(v2, 22, false));

	v2 = r_net->deactivateVertex(1, v2, 24, R_PosInf);
	ASSERT_TRUE(is_vertex_active(v2, 23, false));
	ASSERT_FALSE(is_vertex_active(v2, 25, false));

	// make sure network is updated as well.
	v2 = r_net->vertexList()[1];
	ASSERT_TRUE(is_vertex_active(v2, 23, false));
	ASSERT_FALSE(is_vertex_active(v2, 25, false));
	ASSERT_FALSE(is_vertex_active(v2, 15, false));
	ASSERT_TRUE(is_vertex_active(v2, 17, false));
}

// we don't need too many tests here because
// the actual insert spell is done by stat-net
// produced code, so assume that's correct.
TEST_F(RNetworkTests, TestEdgeActivate) {
	// no activity spells
	ASSERT_FALSE(is_edge_active(edge4, 11, false));
	activate_edge(edge4, 10, 20);
	ASSERT_TRUE(is_edge_active(edge4, 11, false));

	// existing activity spell
	ASSERT_FALSE(is_edge_active(edge3, 11, false));
	activate_edge(edge3, 10, 20);
	ASSERT_TRUE(is_edge_active(edge3, 11, false));
}

TEST_F(RNetworkTests, TestEdgeDeactivate) {
	// edge 4 has no activity list, this should add
	// an activity list
	deactivate_edge(edge4, 1, R_PosInf);
	// true so will return true if activity list doesn't exist
	ASSERT_FALSE(is_edge_active(edge4, 2, true));

	ASSERT_TRUE(is_edge_active(edge1, 2, false));
	deactivate_edge(edge1, 1, 3);
	ASSERT_FALSE(is_edge_active(edge1, 2, false));

	activate_edge(edge1, 10, 15);
	ASSERT_TRUE(is_edge_active(edge1, 10, false));
	deactivate_edge(edge1, 5, 12.5);

	ASSERT_TRUE(is_edge_active(edge1, 12.5, false));
	ASSERT_TRUE(is_edge_active(edge1, 14.9, false));
	ASSERT_FALSE(is_edge_active(edge1, 11, false));
}

TEST_F(RNetworkTests, TestAddVertices) {
	std::vector<int> vert_ids;
	r_net->addVertices(10, vert_ids);
	ASSERT_EQ(10, vert_ids.size());

	int exp = 4;
	for (int i : vert_ids) {
		ASSERT_EQ(exp, i);
		SEXP v = r_net->vertexList()[i];
		ASSERT_NE(v, R_NilValue);
		++exp;
	}

	r_net->setVertexAttribute(4, "age", 10);
	SEXP v = r_net->vertexList()[4];
	ASSERT_EQ(10, r_net->getVertexAttribute<int>(4, "age"));
}

TEST_F(RNetworkTests, TestGetEdges) {
	// vertex one:
	// 1 -> 2 at 1, 3 -> 1 at 1
	// 2 -> 3 at 1 and 6+
	std::vector<SEXP> edges;
	r_net->edges(0, 1, IN, edges);
	ASSERT_EQ(1, edges.size());
	SEXP edge = edges[0];
	REdge redge(edge);
	ASSERT_EQ(0, redge.targetVertex());
	ASSERT_EQ(2, redge.sourceVertex());

	edges.clear();
	r_net->edges(0, 1, OUT, edges);
	ASSERT_EQ(1, edges.size());
	edge = edges[0];
	redge.bind(edge);
	ASSERT_EQ(1, redge.targetVertex());
	ASSERT_EQ(0, redge.sourceVertex());

	edges.clear();
	r_net->edges(0, 1, COMBINED, edges);
	ASSERT_EQ(2, edges.size());
	bool found_in, found_out;
	found_in = found_out = false;
	for (SEXP e : edges) {
		redge.bind(e);
		if (edge_in_idx(e) == 2) {
			ASSERT_EQ(1, redge.targetVertex());
			ASSERT_EQ(0, redge.sourceVertex());
			found_out = true;
		} else {
			ASSERT_EQ(0, redge.targetVertex());
			ASSERT_EQ(2, redge.sourceVertex());
			found_in = true;
		}
	}
	ASSERT_TRUE(found_in && found_out);

	edges.clear();
	r_net->edges(0, 1.5, COMBINED, edges);
	ASSERT_EQ(0, edges.size());

	edges.clear();
	r_net->edges(3, 1, COMBINED, edges);
	ASSERT_EQ(0, edges.size());
}

}

