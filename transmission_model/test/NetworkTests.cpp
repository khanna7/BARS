/*
 * NetworkTests.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "Network.h"

using namespace TransModel;

class Agent {

private:
	unsigned int id_;

public:

	Agent(unsigned int id) :
			id_(id) {
	}

	unsigned int getId() const {
		return id_;
	}
};

typedef std::shared_ptr<Agent> AgentPtr;

TEST(NetworkTests, TestUndirected) {
	UndirectedNetwork<Agent> net;

	AgentPtr one = std::make_shared<Agent>(1);
	AgentPtr two = std::make_shared<Agent>(2);
	AgentPtr three = std::make_shared<Agent>(3);
	AgentPtr four = std::make_shared<Agent>(4);

	net.addVertex(one);
	net.addVertex(two);
	ASSERT_EQ(2, net.vertexCount());
	ASSERT_EQ(0, net.edgeCount());

	net.addEdge(three, four, 14);
	ASSERT_EQ(4, net.vertexCount());
	int id = 1;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		AgentPtr agent = (*iter);
		ASSERT_EQ(id, agent->getId());
		++id;
	}

	ASSERT_EQ(1, net.edgeCount());
	int count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		++count;
		EdgePtr<Agent> edge = (*iter);
		ASSERT_EQ(3, edge->v1()->getId());
		ASSERT_EQ(4, edge->v2()->getId());
		ASSERT_EQ(14, edge->weight());
	}
	ASSERT_EQ(1, count);

	net.addEdge(two, one);
	count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		if (count == 0) {
			EdgePtr<Agent> edge = (*iter);
			ASSERT_EQ(3, edge->v1()->getId());
			ASSERT_EQ(4, edge->v2()->getId());
			ASSERT_EQ(14, edge->weight());
		} else {
			EdgePtr<Agent> edge = (*iter);
			ASSERT_EQ(2, edge->v1()->getId());
			ASSERT_EQ(1, edge->v2()->getId());
			ASSERT_EQ(1, edge->weight());
		}
		++count;
	}
	ASSERT_EQ(2, count);

}
