/*
 * Network.h
 *
 *  Created on: Nov 2, 2015
 *      Author: nick
 */

#ifndef SRC_NETWORK_H_
#define SRC_NETWORK_H_

#include <map>
#include <set>

#include "boost/iterator/transform_iterator.hpp"

#include "Edge.h"

namespace TransModel {

template<typename V>
using EdgePtr = std::shared_ptr<Edge<V>>;

template<typename V>
using VertexPtr = std::shared_ptr<V>;

template<typename T>
struct GetVal {

	T operator()(const std::pair<unsigned int, T>& pair) const {
		return pair.second;
	}
};

template<typename V>
using EdgeMap = std::map<unsigned int, EdgePtr<V>>;

template<typename V>
using EdgeIter = boost::transform_iterator<GetVal<EdgePtr<V>>, typename EdgeMap<V>::iterator>;

template<typename V>
using VertexMap = std::map<unsigned int, VertexPtr<V>>;

template<typename V>
using VertexIter = boost::transform_iterator<GetVal<VertexPtr<V>>, typename VertexMap<V>::iterator>;


using EdgeList = std::map<unsigned int, std::set<unsigned int>>;

template<typename V>
class Network {

private:
	bool directed_;
	unsigned int edge_idx;

	VertexMap<V> vertices;
	EdgeMap<V> edges;
	EdgeList oel, iel;

	void doAddEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, double weight);
	void removeEdges(unsigned int idx, EdgeList& el, std::vector<EdgePtr<V>>& removed_edges);

public:

	Network(bool directed);
	virtual ~Network();

	void addVertex(const std::shared_ptr<V>& vertex);
	bool removeVertex(const std::shared_ptr<V>& vertex);
	bool removeVertex(unsigned int id);

	VertexIter<V> removeVertex(VertexIter<V> iter);

	void addEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, double weight = 1);
	void addEdge(unsigned int v1_idx, unsigned int v2_idx, double weight = 1);
	bool removeEdge(unsigned int v1_idx, unsigned int v2_idx);

	EdgeIter<V> edgesBegin();
	EdgeIter<V> edgesEnd();

	VertexIter<V> verticesBegin();
	VertexIter<V> verticesEnd();

	unsigned int vertexCount() const {
		return vertices.size();
	}

	unsigned int edgeCount() const {
		return edges.size();
	}

	/**
	 * Gets the number of edges into the specified vertex.
	 */
	unsigned int inEdgeCount(const VertexPtr<V>& vert);

	/**
	 * Gets the number of edges out from the specified vertex.
	 */
	unsigned int outEdgeCount(const VertexPtr<V>& vert);

	void clearEdges() {
		edges.clear();
		oel.clear();
		iel.clear();
	}
};

template<typename V>
Network<V>::Network(bool directed) :
		directed_(directed), edge_idx(0), vertices(), edges(), oel(), iel() {
}

template<typename V>
Network<V>::~Network() {
}

template<typename V>
EdgeIter<V> Network<V>::edgesBegin() {
	GetVal<EdgePtr<V>> gv;
	return EdgeIter<V>(edges.begin(), gv);
}

template<typename V>
EdgeIter<V> Network<V>::edgesEnd() {
	GetVal<EdgePtr<V>> gv;
	return EdgeIter<V>(edges.end(), gv);
}

template<typename V>
VertexIter<V> Network<V>::verticesBegin() {
	GetVal<VertexPtr<V>> gv;
	return VertexIter<V>(vertices.begin(), gv);
}

template<typename V>
VertexIter<V> Network<V>::verticesEnd() {
	GetVal<VertexPtr<V>> gv;
	return VertexIter<V>(vertices.end(), gv);
}

template<typename V>
unsigned int Network<V>::inEdgeCount(const VertexPtr<V>& vertex) {
	auto iter = iel.find(vertex->id());
	return iter == iel.end() ? 0 : iter->second.size();
}

template<typename V>
unsigned int Network<V>::outEdgeCount(const VertexPtr<V>& vertex) {
	auto iter = oel.find(vertex->id());
	return iter == oel.end() ? 0 : iter->second.size();
}

template<typename V>
void Network<V>::addVertex(const std::shared_ptr<V>& vertex) {
	vertices.emplace(std::make_pair(vertex->id(), vertex));
}

template<typename V>
bool Network<V>::removeEdge(unsigned int v1_idx, unsigned int v2_idx) {
	auto iter = oel.find(v1_idx);
	if (iter == oel.end()) return false;
	for (auto edge_idx : iter->second) {
		auto edge_iter = edges.find(edge_idx);
		if (edge_iter == edges.end())
			throw std::invalid_argument(
					"Unable to delete edge: edge " + std::to_string(edge_idx) + " does not exist.");
		EdgePtr<V> edge = edge_iter->second;
		if (edge->v2()->id() == v2_idx) {
			edges.erase(edge_iter);
			iter->second.erase(edge_idx);

			iel[v2_idx].erase(edge_idx);
			return true;
		}
	}
	return false;
}

template<typename V>
void Network<V>::removeEdges(unsigned int idx, EdgeList& el, std::vector<EdgePtr<V>>& removed_edges) {
	auto iter = el.find(idx);
	if (iter != el.end()) {
		// set of edges
		for (auto edge_idx_iter = iter->second.begin(); edge_idx_iter != iter->second.end();) {
			// remove the entry from mel
			auto mel_iter = edges.find(*edge_idx_iter);
			if (mel_iter == edges.end())
				throw std::invalid_argument(
						"Unable to delete edge: edge " + std::to_string(*edge_idx_iter) + " does not exist.");
			removed_edges.push_back(mel_iter->second);
			// erase that edge
			edges.erase(mel_iter);
			// remove the mel edge_id from the edge list set
			edge_idx_iter = iter->second.erase(edge_idx_iter);
		}
	}
}

template<typename V>
bool Network<V>::removeVertex(const std::shared_ptr<V>& vertex) {
	unsigned int id = vertex->id();
	return removeVertex(id);
}

template<typename V>
bool Network<V>::removeVertex(unsigned int id) {
	auto iter = vertices.find(id);
	if (iter != vertices.end()) {
		vertices.erase(iter);
		std::vector<EdgePtr<V>> removed_edges;
		removeEdges(id, oel, removed_edges);
		// for each edge get v2 id, remove edge id from oel for v2 id
		for (auto& edge : removed_edges) {
			iel[edge->v2()->id()].erase(edge->id());
		}

		removed_edges.clear();
		removeEdges(id, iel, removed_edges);
		// for each edge get v1 id, remove edge id from iel for v1 id
		for (auto& edge : removed_edges) {
			oel[edge->v1()->id()].erase(edge->id());
		}
		return true;
	}

	return false;
}

template<typename V>
VertexIter<V> Network<V>::removeVertex(VertexIter<V> iter) {
	VertexPtr<V> v = (*iter);
	unsigned int id = v->id();
	auto next_iter = ++iter;
	removeVertex(id);
	return next_iter;
}

template<typename V>
void Network<V>::doAddEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, double weight) {
	// assumes sanity checks have already occured
	edges.emplace(edge_idx, std::make_shared<Edge<V>>(edge_idx, source, target, weight));

	auto out_iter = Network<V>::oel.find(source->id());
	if (out_iter == Network<V>::oel.end()) {
		Network<V>::oel.emplace(source->id(), std::set<unsigned int> { edge_idx });
	} else {
		out_iter->second.emplace(edge_idx);
	}

	auto in_iter = Network<V>::iel.find(target->id());
	if (in_iter == Network<V>::iel.end()) {
		Network<V>::iel.emplace(target->id(), std::set<unsigned int> { edge_idx });
	} else {
		in_iter->second.emplace(edge_idx);
	}
	++edge_idx;
}

template<typename V>
void Network<V>::addEdge(unsigned int v1_idx, unsigned int v2_idx, double weight) {
	auto v1 = vertices.find(v1_idx);
	if (v1 == vertices.end())
		throw std::invalid_argument(
				"Unable to create edge: vertex " + std::to_string(v1_idx) + " not found in vertex map");
	auto v2 = vertices.find(v2_idx);
	if (v2 == vertices.end())
		throw std::invalid_argument(
				"Unable to create edge: vertex " + std::to_string(v2_idx) + " not found in vertex map");

	doAddEdge(v1->second, v2->second, weight);
}

template<typename V>
void Network<V>::addEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, double weight) {
	if (vertices.find(source->id()) == vertices.end()) {
		addVertex(source);
	}

	if (vertices.find(target->id()) == vertices.end()) {
		addVertex(target);
	}

	doAddEdge(source, target, weight);
}

}

/* namespace TransModel */

#endif /* SRC_NETWORK_H_ */
