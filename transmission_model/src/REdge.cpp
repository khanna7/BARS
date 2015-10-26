/*
 * REdge.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: nick
 */

#include "REdge.h"
#include "RNetwork.h"

using namespace Rcpp;

namespace TransModel {

REdge::REdge() : edge(0), source(-1), target(-1) {

}

REdge::REdge(SEXP sexp) : edge(as<List>(sexp)) , source(edge_out_idx(edge) - 1),
		target(edge_in_idx(edge) - 1) {

}

REdge::~REdge() {
}

void REdge::bind(SEXP sexp) {
	edge = as<List>(sexp);
	source = edge_out_idx(edge) - 1;
	target = edge_in_idx(edge) - 1;
}

bool REdge::isActive(double at, bool defaultActive) const {
	return is_edge_active(edge, at, defaultActive);
}

} /* namespace TransModel */
