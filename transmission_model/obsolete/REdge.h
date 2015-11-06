/*
 * REdge.h
 *
 *  Created on: Oct 26, 2015
 *      Author: nick
 */

#ifndef SRC_REDGE_H_
#define SRC_REDGE_H_

#include "RInside.h"

namespace TransModel {

/**
 * A wrapper around an edge SEXP. This is meant to be used to easily access
 * edge data, rather than a kind of permanent object in the network.
 */
class REdge {
public:
	REdge();
	REdge(SEXP sexp);

	virtual ~REdge();

	void bind(SEXP sexp);

	/**
	 * Gets the index of the source vertex in C-style (0 based).
	 */
	int sourceVertex() const {
		return source;
	}

	/**
	 * Gets the index of the target vertex in C-style (0 based).
	 */
	int targetVertex() const {
		return target;
	}

	/**
	 * Gets whether or not the edge is active at the
	 * specified time. Returns true if 'at' is within _any_ of the edges activity
	 * spells. If the edges activity spell matrix is null, then 'default_active' is
	 * returned.
	 *
	 * @param edge the edge
	 * @param at the time to check if the edge is active at
	 * @param default_active the value to return if the edge's
	 * activity matrix is null.
	 */
	bool isActive(double at, bool defaultActive) const;

private:
	Rcpp::List edge;
	int source, target;
};

} /* namespace TransModel */

#endif /* SRC_REDGE_H_ */
