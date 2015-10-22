/*
 * RNetwork.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include <exception>

#include "RNetwork.h"
#include "spell_functions.h"

//extern "C" {
#include "nd_spell_functions.h"
//}

using namespace Rcpp;
using namespace std;

namespace TransModel {

// based on networkDynamic is.active.c implementation
bool is_active(List atl, double at, bool default_active) {
	// return false if atl is "na"
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0])
		return false;

	if (!atl.containsElementNamed("active")) {
		return default_active;
	}

	SEXP spell_list = atl["active"];
	NumericMatrix spell_matrix = as<NumericMatrix>(spell_list);
	return any_spell_active(spell_matrix, at);
}

bool is_edge_active(SEXP edge, double at, bool default_active) {
	Rcpp::List edge_as_list = Rcpp::as<Rcpp::List>(edge);
	Rcpp::List atl = Rcpp::as<Rcpp::List>(edge_as_list["atl"]);
	return is_active(atl, at, default_active);
}

bool is_vertex_active(SEXP vertex, double at, bool default_active) {
	Rcpp::List atl = Rcpp::as<Rcpp::List>(vertex);
	return is_active(atl, at, default_active);
}

bool is_infinite(double val) {
	return val == R_PosInf || val == R_NegInf;
}

void activate_vertex(SEXP vertex, double onset, double terminus) {
	if (onset == R_PosInf || terminus == R_NegInf)
		return;

	if (onset > terminus) {
		throw invalid_argument("onset must be less than or equal to terminus when deactivating a spell.");
	}

	Rcpp::List atl = Rcpp::as<Rcpp::List>(vertex);
	// don't activate if "na"
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0]) {
		return;
	}

	SEXP old_active(R_NilValue);
	if (atl.containsElementNamed("active")) {
		old_active = atl["active"];
		// if vertex is already infinitely active then return
		if (old_active(0, ONSET) == R_NegInf && old_active(0, TERMINUS) == R_PosInf) {
			return;
		}
	}

	SEXP new_active = InsertSpell(old_active, onset, terminus, Rboolean(false));
	atl["active"] = new_active;
}

void deactivate(List atl, List parent, double onset, double terminus) {
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0]) {
		return;
	}

	if (onset > terminus) {
		throw invalid_argument("onset must be less than or equal to terminus when deactivating a spell.");
	}

	if (is_infinite(onset) && is_infinite(terminus)) {
		atl["active"] = create_matrix(1, 2, { R_PosInf, R_PosInf });
	} else if (!atl.containsElementNamed("active")) {
		if (is_infinite(onset)) {
			atl["active"] = create_matrix(1, 2, { terminus, R_PosInf });
		} else if (is_infinite(terminus)) {
			atl["active"] = create_matrix(1, 2, { R_NegInf, onset });
		} else {
			atl["active"] = create_matrix(2, 2, { R_NegInf, terminus, onset, R_PosInf });
		}
		// adding the "active" element creates a new atl for some reason
		// so the new atl has to be attached back to the parent
		parent["atl"] = atl;
	} else {
		NumericMatrix spell_matrix(as<NumericMatrix>(atl["active"]));
		atl["active"] = delete_spell(spell_matrix, onset, terminus);
		parent["atl"] = atl;
	}
}

void activate_edge(SEXP edge, double onset, double terminus) {
	if (onset == R_PosInf || terminus == R_NegInf)
		return;
	if (onset > terminus) {
		throw invalid_argument("onset must be less than or equal to terminus when deactivating a spell.");
	}

	Rcpp::List edge_as_list = Rcpp::as<Rcpp::List>(edge);
	Rcpp::List atl = Rcpp::as<Rcpp::List>(edge_as_list["atl"]);

	// don't activate if "na"
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0]) {
		return;
	}

	SEXP old_active(R_NilValue);
	if (atl.containsElementNamed("active")) {
		old_active = atl["active"];
	}
	SEXP new_active = InsertSpell(old_active, onset, terminus, Rboolean(false));
	atl["active"] = new_active;
	edge_as_list["atl"] = atl;
}

void deactivate_vertex(SEXP vertex, SEXP vertex_attribute_list, double onset, double terminus) {
	List atl = as<List>(vertex);
	List parent = as<List>(vertex_attribute_list);
	deactivate(atl, parent, onset, terminus);
}

void deactivate_edge(SEXP edge, double onset, double terminus) {
	Rcpp::List edge_as_list = Rcpp::as<Rcpp::List>(edge);
	Rcpp::List atl = Rcpp::as<Rcpp::List>(edge_as_list["atl"]);
	deactivate(atl, edge_as_list, onset, terminus);
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

List RNetwork::vertexList() {
	return as<List>(net["val"]);
}

void RNetwork::updateRNetwork() {
	net = as<List>((*r_ptr_)[net_name_]);
}

void RNetwork::simulate() {
	simulate_();
}

} /* namespace TransModel */
