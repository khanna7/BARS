/*
 * NetworkTests.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "Network.h"
#include "network_utils.h"
#include "StatsBuilder.h"
#include "NetworkStats.h"
#include "RInstance.h"

using namespace TransModel;
using namespace Rcpp;
using namespace std;

class Agent {

private:
	unsigned int id_, age_;
	double score_;

public:

	Agent(unsigned int id, unsigned int age) :
			id_(id), age_(age), score_(0) {
	}

	unsigned int id() const {
		return id_;
	}

	int age() const {
		return age_;
	}

	void setAge(int age) {
		age_ = age;
	}

	double score() const {
		return score_;
	}

	void setScore(double score) {
		score_ = score;
	}

	bool hasPreviousJailHistory() {
		return false;
	}

	int timeOfRelease() {
		return -1;
	}

	bool hasReleasedPartner(int id) {
		return false;
	}

	void addReleasedPartner(int id, double tick) {}

	void removeReleasedPartner(int id) {}

};

class Assigner  {

public:
	Assigner() {}
	virtual ~Assigner() {}

	void initEdge(std::shared_ptr<Edge<Agent>> edge) {}
};


typedef std::shared_ptr<Agent> AgentPtr;

class NetworkTests: public ::testing::Test {

protected:

	NetworkTests() {
		std::string cmd = "source(file=\"../test_data/network_tests.R\")";
		RInstance::rptr->parseEvalQ(cmd);

		StatsBuilder builder("/dev");
		builder.countsWriter("null");
		builder.partnershipEventWriter("null");
		builder.infectionEventWriter("null");
		builder.biomarkerWriter("null");
		builder.deathEventWriter("null");
		builder.personDataRecorder("null");
		builder.testingEventWriter("null");
		builder.prepEventWriter("null");
		builder.artEventWriter("null");
		builder.createStatsSingleton(0, 0);
	}

	virtual ~NetworkTests() {
	}
};


TEST_F(NetworkTests, TestAdds) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);

	net.addVertex(one);
	net.addVertex(two);
	ASSERT_EQ(2, net.vertexCount());
	ASSERT_EQ(0, net.edgeCount());

	EdgePtr<Agent> e = net.addEdge(three, four);
	e->setWeight(14);
	ASSERT_EQ(4, net.vertexCount());
	int id = 1;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		AgentPtr agent = (*iter);
		ASSERT_EQ(id, agent->id());
		++id;
	}

	ASSERT_EQ(1, net.edgeCount());
	int count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		++count;
		EdgePtr<Agent> edge = (*iter);
		ASSERT_EQ(3, edge->v1()->id());
		ASSERT_EQ(4, edge->v2()->id());
		ASSERT_EQ(14, edge->weight());
		ASSERT_EQ(0, edge->type());
	}
	ASSERT_EQ(1, count);

	net.addEdge(two, one);
	count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		if (count == 0) {
			EdgePtr<Agent> edge = (*iter);
			ASSERT_EQ(3, edge->v1()->id());
			ASSERT_EQ(4, edge->v2()->id());
			ASSERT_EQ(14, edge->weight());
		} else {
			EdgePtr<Agent> edge = (*iter);
			ASSERT_EQ(2, edge->v1()->id());
			ASSERT_EQ(1, edge->v2()->id());
			ASSERT_EQ(1, edge->weight());
		}
		++count;
	}
	ASSERT_EQ(2, count);

	ASSERT_EQ(net.edgeCount(), net.edgeCount(0));
	net.addEdge(two, one, 10);
	ASSERT_TRUE(net.hasEdge(two, one, 10));
	ASSERT_EQ(1, net.edgeCount(10));
	ASSERT_EQ(3, net.edgeCount());
	ASSERT_EQ(1, net.outEdgeCount(two, 0));
	ASSERT_EQ(1, net.outEdgeCount(two, 10));
	auto iter = net.edgesBegin();
	advance(iter, 2);
	EdgePtr<Agent> ep = (*iter);
	ASSERT_EQ(2, ep->id());
	ASSERT_EQ(10, ep->type());
	ASSERT_EQ(2, ep->v1()->id());
	ASSERT_EQ(1, ep->v2()->id());
	ASSERT_EQ(1, ep->weight());

	net.addEdge(two, three, 10);
	ASSERT_FALSE(net.hasEdge(two, three));
	ASSERT_FALSE(net.hasEdge(two, three, 0));
	ASSERT_TRUE(net.hasEdge(two, three, 10));
	ASSERT_FALSE(net.hasEdge(three, two, 10));
}

TEST_F(NetworkTests, TestEdgeGet) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);

	net.addVertex(one);
	net.addEdge(three, one);
	net.addEdge(one, two, 13);
	net.addEdge(one, four);
	net.addEdge(three, four, 12);

	std::vector<EdgePtr<Agent>> vec;
	net.getEdges(one, vec);

	ASSERT_EQ(3, vec.size());
	EdgePtr<Agent> edge = vec[0];
	ASSERT_EQ(3, edge->v1()->id());
	ASSERT_EQ(1, edge->v2()->id());

	edge = vec[1];
	ASSERT_EQ(1, edge->v1()->id());
	ASSERT_EQ(2, edge->v2()->id());

	edge = vec[2];
	ASSERT_EQ(1, edge->v1()->id());
	ASSERT_EQ(4, edge->v2()->id());
}

TEST_F(NetworkTests, TestRemoves) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);

	net.addVertex(one);
	net.addEdge(three, one);
	net.addEdge(one, two);
	net.addEdge(one, four);
	net.addEdge(three, four);

	ASSERT_EQ(4, net.vertexCount());
	ASSERT_EQ(4, net.edgeCount());

	ASSERT_EQ(2, net.outEdgeCount(one));
	ASSERT_EQ(0, net.outEdgeCount(two));
	ASSERT_EQ(2, net.outEdgeCount(three));
	ASSERT_EQ(0, net.outEdgeCount(four));

	ASSERT_EQ(1, net.inEdgeCount(one));
	ASSERT_EQ(1, net.inEdgeCount(two));
	ASSERT_EQ(0, net.inEdgeCount(three));
	ASSERT_EQ(2, net.inEdgeCount(four));

	net.removeVertex(one);
	ASSERT_EQ(3, net.vertexCount());
	ASSERT_EQ(1, net.edgeCount());

	ASSERT_EQ(0, net.outEdgeCount(one));
	ASSERT_EQ(0, net.outEdgeCount(two));
	ASSERT_EQ(1, net.outEdgeCount(three));
	ASSERT_EQ(0, net.outEdgeCount(four));

	ASSERT_EQ(0, net.inEdgeCount(one));
	ASSERT_EQ(0, net.inEdgeCount(two));
	ASSERT_EQ(0, net.inEdgeCount(three));
	ASSERT_EQ(1, net.inEdgeCount(four));

	int count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		EdgePtr<Agent> edge = (*iter);
		ASSERT_EQ(3, edge->v1()->id());
		ASSERT_EQ(4, edge->v2()->id());
		ASSERT_EQ(1, edge->weight());
		++count;
	}

	ASSERT_EQ(1, count);
	int id = 2;
	count = 0;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		AgentPtr ap = (*iter);
		ASSERT_EQ(id, ap->id());
		++id;
		++count;
	}
	ASSERT_EQ(3, count);

	unsigned int edge_count = net.edgeCount();
	net.addEdge(two, one, 10);
	ASSERT_EQ(edge_count + 1, net.edgeCount());
	ASSERT_FALSE(net.removeEdge(two->id(), one->id(), 0));
	ASSERT_EQ(1, net.outEdgeCount(two, 10));
	ASSERT_EQ(1, net.inEdgeCount(one, 10));
	ASSERT_TRUE(net.removeEdge(two->id(), one->id(), 10));
	ASSERT_EQ(0, net.outEdgeCount(two, 10));
	ASSERT_EQ(0, net.inEdgeCount(one, 10));
	ASSERT_EQ(edge_count, net.edgeCount());
}

TEST_F(NetworkTests, TestRemovesByIter) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);

	net.addVertex(one);
	net.addEdge(three, one);
	net.addEdge(one, two, 13);
	net.addEdge(one, four);
	net.addEdge(three, four, 12);

	ASSERT_EQ(2, net.outEdgeCount(one));
	ASSERT_EQ(0, net.outEdgeCount(two));
	ASSERT_EQ(2, net.outEdgeCount(three));
	ASSERT_EQ(0, net.outEdgeCount(four));

	ASSERT_EQ(1, net.inEdgeCount(one));
	ASSERT_EQ(1, net.inEdgeCount(two));
	ASSERT_EQ(0, net.inEdgeCount(three));
	ASSERT_EQ(2, net.inEdgeCount(four));

	ASSERT_EQ(4, net.vertexCount());
	ASSERT_EQ(4, net.edgeCount());

	int i = 1;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd();) {
		AgentPtr p = (*iter);
		ASSERT_EQ(i, p->id());
		if (i == 1) {
			iter = net.removeVertex(iter);
		} else {
			++iter;
		}
		++i;
	}

	ASSERT_EQ(0, net.edgeCount(13));

	ASSERT_EQ(3, net.vertexCount());
	ASSERT_EQ(1, net.edgeCount());

	ASSERT_EQ(0, net.outEdgeCount(one));
	ASSERT_EQ(0, net.outEdgeCount(one, 13));
	ASSERT_EQ(0, net.outEdgeCount(two));
	ASSERT_EQ(1, net.outEdgeCount(three));
	ASSERT_EQ(0, net.outEdgeCount(four));

	ASSERT_EQ(0, net.inEdgeCount(one));
	ASSERT_EQ(0, net.inEdgeCount(two));
	ASSERT_EQ(0, net.inEdgeCount(three));
	ASSERT_EQ(1, net.inEdgeCount(four));

	int count = 0;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		EdgePtr<Agent> edge = (*iter);
		ASSERT_EQ(3, edge->v1()->id());
		ASSERT_EQ(4, edge->v2()->id());
		ASSERT_EQ(1, edge->weight());
		++count;
	}

	ASSERT_EQ(1, count);
	int id = 2;
	count = 0;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		AgentPtr ap = (*iter);
		ASSERT_EQ(id, ap->id());
		++id;
		++count;
	}
	ASSERT_EQ(3, count);
}

TEST_F(NetworkTests, TestRemovesByVertexIds) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);

	net.addVertex(one);
	net.addEdge(three, one);
	net.addEdge(one, two);
	net.addEdge(one, four);
	net.addEdge(three, four, 15);
	net.addEdge(three, four, 0);

	ASSERT_EQ(5, net.edgeCount());

	net.removeEdge(1, 2);

	ASSERT_EQ(4, net.edgeCount());
	ASSERT_EQ(3, net.edgeCount(0));
	ASSERT_EQ(1, net.edgeCount(15));

	auto eiter = net.edgesBegin();
	EdgePtr<Agent> edge = (*eiter);
	ASSERT_EQ(3, edge->v1()->id());
	ASSERT_EQ(1, edge->v2()->id());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(1, edge->v1()->id());
	ASSERT_EQ(4, edge->v2()->id());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(3, edge->v1()->id());
	ASSERT_EQ(4, edge->v2()->id());
	ASSERT_EQ(15, edge->type());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(3, edge->v1()->id());
	ASSERT_EQ(4, edge->v2()->id());
	ASSERT_EQ(0, edge->type());

	ASSERT_EQ(1, net.outEdgeCount(one));
	ASSERT_EQ(0, net.outEdgeCount(two));
	ASSERT_EQ(3, net.outEdgeCount(three));
	ASSERT_EQ(0, net.outEdgeCount(four));

	ASSERT_EQ(1, net.inEdgeCount(one));
	ASSERT_EQ(0, net.inEdgeCount(two));
	ASSERT_EQ(0, net.inEdgeCount(three));
	ASSERT_EQ(3, net.inEdgeCount(four));

}

TEST_F(NetworkTests, TestStats) {
	Network<Agent> net(false);

	AgentPtr one = std::make_shared<Agent>(1, 1);
	AgentPtr two = std::make_shared<Agent>(2, 1);
	AgentPtr three = std::make_shared<Agent>(3, 1);
	AgentPtr four = std::make_shared<Agent>(4, 1);
	AgentPtr five = std::make_shared<Agent>(5, 1);
	AgentPtr six = std::make_shared<Agent>(6, 1);

	net.addVertex(one);
	net.addVertex(two);
	net.addVertex(three);
	net.addVertex(four);
	net.addVertex(five);
	net.addVertex(six);

	net.addEdge(three, four);
	net.addEdge(three, five);
	net.addEdge(two, one);
	net.addEdge(six, three);
	net.addEdge(one, three);

	NetworkStats<Agent> stats(net);
	std::vector<std::pair<AgentPtr, long>> results;
	stats.degree(results);

	std::vector<AgentPtr> agents;
	stats.degree(agents);

	std::map<int, long> expected{{1, 2}, {2, 1}, {3, 4}, {4, 1}, {5, 1}, {6, 1}};
	ASSERT_EQ(results.size(), expected.size());

	for (auto& p : results) {
		int id = p.first->id(); 
		long degree = p.second;
		ASSERT_EQ(degree, expected.at(id));
	}

	ASSERT_EQ(results.size(), agents.size());
	// 3 has highest followed by 1
	ASSERT_EQ(3, agents[0]->id());
	ASSERT_EQ(1, agents[1]->id());
	agents.clear();
	
	results.clear();
	stats.degree(results, 3);
	ASSERT_EQ(results.size(), 3);

	int i = 0;
	for (auto& p : results) {
		int id = p.first->id(); 
		long degree = p.second;
		if (i == 0) {
			ASSERT_EQ(id, 3);
			ASSERT_EQ(degree, 4);
		} else if (i == 1) {
			ASSERT_EQ(id, 1);
			ASSERT_EQ(degree, 2);
		} else {
			ASSERT_EQ(degree, 1);
		};
		++i;
	}

	std::vector<std::pair<AgentPtr, double>> eigens;
	stats.eigenCentrality(eigens);
	ASSERT_EQ(net.vertexCount(), eigens.size());

	// for (auto& p : eigens) {
	// 	int id = p.first->id(); 
	// 	double e = p.second;
	// 	std::cout << id << " " << e << std::endl;
	// }

	stats.eigenCentrality(agents);

	// for (auto& p : agents) {
	// 	std::cout << p->id() << " " << p->score() << std::endl;
	// }

	ASSERT_EQ(3, agents[0]->id());
	ASSERT_EQ(1, agents[1]->id());
	ASSERT_EQ(2, agents[agents.size() - 1]->id());
	eigens.clear();
	stats.eigenCentrality(eigens, 3);

	i = 0;
	double e = 10;
	for (auto& p : eigens) {
		int id = p.first->id(); 
		double eigen = p.second;
		if (i == 0) {
			ASSERT_EQ(id, 3);
			ASSERT_LE(eigen, e);
		} else if (i == 1) {
			ASSERT_EQ(id, 1);
			ASSERT_LE(eigen, e);
		} else {
			ASSERT_LE(eigen, e);
		};
		++i;
		e = eigen;
	}
}

struct AgentCreator {

	int id;
	AgentCreator() :
			id(0) {
	}

	VertexPtr<Agent> operator()(List& val, int model_tick, double tick) {
		int age = as<int>(val["age"]);
		return std::make_shared<Agent>(id++, age);
	}
};

struct AgeSetter {

	List operator()(const VertexPtr<Agent>& agent, int idx, double time) const {
		List vertex = List::create();
		vertex["na"] = false;
		vertex["vertex_names"] = idx;
		vertex["age"] = agent->age();

		return vertex;
	}
};

TEST_F(NetworkTests, CreateRNetTests) {
	Network<Agent> net(false);
	List init_net = as<List>((*RInstance::rptr)["sn"]);
	AgentCreator creator;
	Assigner assigner;
	initialize_network(init_net, net, creator, assigner, 1);

	auto iter = net.verticesBegin();
	++iter;

	// remove a vertex
	// all edges with vertex id of 1, rn of 2 are gone
	bool ret = net.removeVertex(*iter);
	ASSERT_TRUE(ret);

	net.addVertex(std::make_shared<Agent>(5, 37));
	net.addEdge(0, 5);

	List rnet;
	map<unsigned int, unsigned int> idx_map;
	AgeSetter setter;
	create_r_network(1, rnet, net, idx_map, setter, 0);
	ASSERT_EQ(4, idx_map.size());
	// exp is vertex id
	ASSERT_EQ(0, idx_map.at(1));
	// deleted vertex with id 1 so empty here
	ASSERT_EQ(2, idx_map.at(2));
	ASSERT_EQ(3, idx_map.at(3));
	ASSERT_EQ(5, idx_map.at(4));

	// f should add an edge between 2 and 3 (c style)
	Function f = as<Function>((*RInstance::rptr)["try.net.func"]);
	SEXP changes = f();

	ASSERT_EQ(2, net.edgeCount());

	reset_network_edges(changes, net, idx_map, 1, assigner, 0);
	// still 2 because we deleted one and added one
	// in the try.net.func call
	ASSERT_EQ(2, net.edgeCount());

	auto eiter = net.edgesBegin();
	EdgePtr<Agent> edge = (*eiter);
	ASSERT_EQ(2, edge->v1()->id());
	ASSERT_EQ(0, edge->v2()->id());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(2, edge->v1()->id());
	ASSERT_EQ(5, edge->v2()->id());
}

TEST_F(NetworkTests, InitNetTest) {
	Network<Agent> net(false);
	List rnet = as<List>((*RInstance::rptr)["sn"]);
	AgentCreator creator;
	Assigner assigner;
	initialize_network(rnet, net, creator,assigner, 1);

	ASSERT_EQ(4, net.vertexCount());
	auto iter = net.verticesBegin();
	AgentPtr one = (*iter);
	ASSERT_EQ(0, one->id());
	ASSERT_EQ(2, one->age());

	++iter;
	AgentPtr two = (*iter);
	ASSERT_EQ(1, two->id());
	ASSERT_EQ(12, two->age());

	++iter;
	AgentPtr three = (*iter);
	ASSERT_EQ(2, three->id());
	ASSERT_EQ(18, three->age());

	++iter;
	AgentPtr four = (*iter);
	ASSERT_EQ(3, four->id());
	ASSERT_EQ(100, four->age());

	ASSERT_EQ(3, net.edgeCount());
	auto eiter = net.edgesBegin();
	EdgePtr<Agent> edge = (*eiter);
	ASSERT_EQ(0, edge->v1()->id());
	ASSERT_EQ(1, edge->v2()->id());
	ASSERT_EQ(1, edge->type());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(1, edge->v1()->id());
	ASSERT_EQ(2, edge->v2()->id());
	ASSERT_EQ(1, edge->type());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(2, edge->v1()->id());
	ASSERT_EQ(0, edge->v2()->id());
	ASSERT_EQ(1, edge->type());

	rnet = as<List>((*RInstance::rptr)["sn.1"]);
	initialize_edges(rnet, net, assigner, 2);

	ASSERT_EQ(5, net.edgeCount());
	eiter = net.edgesBegin();
	advance(eiter, 3);
	edge = (*eiter);
	ASSERT_EQ(0, edge->v1()->id());
	ASSERT_EQ(3, edge->v2()->id());
	ASSERT_EQ(2, edge->type());

	++eiter;
	edge = (*eiter);
	ASSERT_EQ(3, edge->v1()->id());
	ASSERT_EQ(1, edge->v2()->id());
	ASSERT_EQ(2, edge->type());
}

