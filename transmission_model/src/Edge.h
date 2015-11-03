/*
 * Edge.h
 *
 *  Created on: Nov 2, 2015
 *      Author: nick
 */

#ifndef SRC_EDGE_H_
#define SRC_EDGE_H_

#include <memory>

namespace TransModel {

template<typename V>
class Edge {

private:
	std::shared_ptr<V> v1_, v2_;
	double weight_;

public:
	/**
	 * Creates an edge from v1 to v2.
	 */
	Edge(std::shared_ptr<V> v1, std::shared_ptr<V>, double weight);
	virtual ~Edge();

	std::shared_ptr<V> v1() {
		return v1_;
	}

	std::shared_ptr<V> v2() {
		return v2_;
	}

	double weight() {
		return weight_;
	}

};

template<typename V>
Edge<V>::Edge(std::shared_ptr<V> v1, std::shared_ptr<V> v2, double weight) : v1_(v1), v2_(v2), weight_(weight) {
}

template<typename V>
Edge<V>::~Edge() {
}

} /* namespace TransModel */

#endif /* SRC_EDGE_H_ */
