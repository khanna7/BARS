/*
 * RNetwork.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "RNetwork.h"

using namespace Rcpp;

namespace TransModel {

RNetwork::RNetwork(List network) : net(network) {

}

RNetwork::~RNetwork() {
}

int RNetwork::getVertexCount() const {
	return getNetworkAttribute<int>("n");
}

List RNetwork::edgeList() {
	return as<List>(net["mel"]);
}

void RNetwork::updateRNetwork(SEXP network) {
		net = as<List>(network);
	}

} /* namespace TransModel */
