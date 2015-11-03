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


template<typename V>
class Network {

private:
	bool directed_;

protected:

	unsigned int edge_idx;

	VertexMap<V> vertices;
	EdgeMap<V> edges;
	std::map<unsigned int, std::set<unsigned int>> oel, iel;

	virtual void doAddEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight) = 0;

public:

	Network(bool directed);
	virtual ~Network();

	void addVertex(std::shared_ptr<V>& vertex);
	void removeVertex(std::shared_ptr<V>& vertex);

	void addEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight = 1);

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
void Network<V>::addVertex(std::shared_ptr<V>& vertex) {
	vertices.emplace(std::make_pair(vertex->getId(), vertex));
}

template<typename V>
void Network<V>::addEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight) {
	if (vertices.find(source->getId()) == vertices.end()) {
		addVertex(source);
	}

	if (vertices.find(target->getId()) == vertices.end()) {
		addVertex(target);
	}

	edges.emplace(edge_idx, std::make_shared<Edge<V>>(source, target, weight));
	doAddEdge(source, target, weight);
	++edge_idx;
}

template<typename V>
class DirectedNetwork: public Network<V> {

protected:

	virtual void doAddEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight);

public:
	DirectedNetwork();
	virtual ~DirectedNetwork();
};

template<typename V>
DirectedNetwork<V>::DirectedNetwork() :
		Network<V>(true) {
}

template<typename V>
DirectedNetwork<V>::~DirectedNetwork() {
}

template<typename V>
void DirectedNetwork<V>::doAddEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight) {
	auto out_iter = Network<V>::oel.find(source->getId());
	if (out_iter == Network<V>::oel.end()) {
		Network<V>::oel.emplace(source->getId(), std::set<unsigned int> { Network<V>::edge_idx });
	} else {
		out_iter->second.push_back(Network<V>::edge_idx);
	}

	auto in_iter = Network<V>::iel.find(target->getId());
	if (in_iter == Network<V>::iel.end()) {
		Network<V>::iel.emplace(target->getId(), std::set<unsigned int> { Network<V>::edge_idx });
	} else {
		in_iter->second.emplace(Network<V>::edge_idx);
	}
}

template<typename V>
class UndirectedNetwork: public Network<V> {

protected:

	virtual void doAddEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight);

public:
	UndirectedNetwork();
	virtual ~UndirectedNetwork();
};

template<typename V>
UndirectedNetwork<V>::UndirectedNetwork() :
		Network<V>::Network(false) {
}

template<typename V>
UndirectedNetwork<V>::~UndirectedNetwork() {
}

template<typename V>
void UndirectedNetwork<V>::doAddEdge(std::shared_ptr<V>& source, std::shared_ptr<V>& target, double weight) {
	// use only the oel for non-directed
	auto out_iter = Network<V>::oel.find(source->getId());
	if (out_iter == Network<V>::oel.end()) {
		Network<V>::oel.emplace(source->getId(), std::set<unsigned int> { Network<V>::edge_idx });
	} else {
		out_iter->second.emplace(Network<V>::edge_idx);
	}

	out_iter = Network<V>::oel.find(target->getId());
	if (out_iter == Network<V>::oel.end()) {
		Network<V>::oel.emplace(target->getId(), std::set<unsigned int> { Network<V>::edge_idx });
	} else {
		out_iter->second.emplace(Network<V>::edge_idx);
	}
}

}
/* namespace TransModel */

#endif /* SRC_NETWORK_H_ */
