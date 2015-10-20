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

const int ONSET = 0;
const int TERMINUS = 1;

// based on networkDynamic is.active.c implementation
bool is_edge_active(SEXP exp, double at, bool default_active) {
	Rcpp::List mel_item = Rcpp::as<Rcpp::List>(exp);
	Rcpp::List atl = Rcpp::as<Rcpp::List>(mel_item["atl"]);
	// return false if edge is "na"
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0])
		return false;

	SEXP spell_list = atl["active"];
	if (Rf_isNull(spell_list))
		return default_active;

	NumericMatrix spell_matrix = as<NumericMatrix>(spell_list);
	if (spell_matrix(0, ONSET) == R_NegInf && spell_matrix(0, TERMINUS) == R_PosInf)
		return true;

	for (int i = spell_matrix.rows() - 1; i >= 0; --i) {
		if (spell_matrix(i, ONSET) == R_PosInf)
			continue;
		if (spell_matrix(i, ONSET) == spell_matrix(i, TERMINUS)) {
			if (at == spell_matrix(i, ONSET))
				return true;
		} else if (spell_matrix(i, TERMINUS) == R_PosInf && spell_matrix(i, ONSET) == R_PosInf) {
			return true;
		} else if (spell_matrix(i, ONSET) <= at && at < spell_matrix(i, TERMINUS)) {
			return true;
		}
	}
	return false;
}

int edge_in_idx(SEXP edge) {
	return (as<IntegerVector>(as<List>(edge)["inl"]))[0];
}
int edge_out_idx(SEXP edge) {
	return (as<IntegerVector>(as<List>(edge)["outl"]))[0];
}

RNetwork::RNetwork(shared_ptr<RInside>& r_ptr, const string& net_name) :
		r_ptr_(r_ptr), net_name_(net_name), net(as<List>((*r_ptr)[net_name])), simulate_((*r_ptr)["nw_simulate"]) {
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
