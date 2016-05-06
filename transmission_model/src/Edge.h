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
	unsigned int id_;
	std::shared_ptr<V> v1_, v2_;
	double weight_;
	int type_;

public:
	/**
	 * Creates an edge from v1 to v2.
	 */
	Edge(unsigned int id, std::shared_ptr<V> v1, std::shared_ptr<V>, int type = 0);
	virtual ~Edge();

	std::shared_ptr<V> v1() const {
		return v1_;
	}

	std::shared_ptr<V> v2() const {
		return v2_;
	}

	double weight() const {
		return weight_;
	}

	void setWeight(double val) {
		weight_ = val;
	}

	unsigned int id() const {
		return id_;
	}

	int type() const {
		return type_;
	}



};

template<typename V>
Edge<V>::Edge(unsigned int id, std::shared_ptr<V> v1, std::shared_ptr<V> v2, int type) :
	id_(id), v1_(v1), v2_(v2), weight_(1.0), type_(type) {
}

template<typename V>
Edge<V>::~Edge() {
}

} /* namespace TransModel */

#endif /* SRC_EDGE_H_ */
