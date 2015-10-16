/*
 * RNetwork.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include "RNetwork.h"

using namespace Rcpp;
using namespace std;

namespace TransModel {

RNetwork::RNetwork(shared_ptr<RInside>& r_ptr, const string& net_name) : r_ptr_(r_ptr), net_name_(net_name),
		net(as<List>((*r_ptr)[net_name])), simulate_((*r_ptr)["nw_simulate"]) {
}

RNetwork::~RNetwork() {
}

int RNetwork::getVertexCount() const {
	return getNetworkAttribute<int>("n");
}

List RNetwork::edgeList() {
	return as<List>(net["mel"]);
}

void RNetwork::updateRNetwork() {
	net = as<List>((*r_ptr_)[net_name_]);
}

void RNetwork::simulate() {
	simulate_();
}

} /* namespace TransModel */
