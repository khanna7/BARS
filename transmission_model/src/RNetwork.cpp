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

List activate_vertex(SEXP val, int vertex_idx, SEXP vertex, double onset, double terminus) {
	if (onset == R_PosInf || terminus == R_NegInf)
		return vertex;

	if (onset > terminus) {
		throw invalid_argument("onset must be less than or equal to terminus when deactivating a spell.");
	}

	Rcpp::List atl = Rcpp::as<Rcpp::List>(vertex);
	// don't activate if "na"
	if (Rcpp::as<Rcpp::LogicalVector>(atl["na"])[0]) {
		return vertex;
	}

	SEXP old_active(R_NilValue);
	if (atl.containsElementNamed("active")) {
		old_active = (atl["active"]);
		// if vertex is already infinitely active then return
		if (as<NumericMatrix>(old_active)(0, ONSET) == R_NegInf
				&& as<NumericMatrix>(old_active)(0, TERMINUS) == R_PosInf) {
			return vertex;
		}
	}

	SEXP new_active = InsertSpell(old_active, onset, terminus, Rboolean(false));
	atl["active"] = new_active;
	as<List>(val)[vertex_idx] = atl;
	return atl;
}

void deactivate(List& atl, double onset, double terminus) {
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
	} else {
		NumericMatrix spell_matrix(as<NumericMatrix>(atl["active"]));
		atl["active"] = delete_spell(spell_matrix, onset, terminus);
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

List deactivate_vertex(SEXP vertex_attribute_list, int vertex_idx, SEXP vertex, double onset, double terminus) {
	List atl = as<List>(vertex);
	List parent = as<List>(vertex_attribute_list);
	deactivate(atl, onset, terminus);
	parent[vertex_idx] = atl;
	return atl;
}

void deactivate_edge(SEXP edge, double onset, double terminus) {
	Rcpp::List edge_as_list = Rcpp::as<Rcpp::List>(edge);
	Rcpp::List atl = Rcpp::as<Rcpp::List>(edge_as_list["atl"]);
	deactivate(atl, onset, terminus);
	// adding the "active" element creates a new atl for some reason
	// so the new atl has to be attached back to the parent
	edge_as_list["atl"] = atl;
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

List RNetwork::activateVertex(int vertex_idx, double onset, double terminus) {
	return activate_vertex(net["val"], vertex_idx, as<List>(net["val"])[vertex_idx], onset, terminus);
}

List RNetwork::activateVertex(int vertex_idx, SEXP vertex, double onset, double terminus) {
	return activate_vertex(net["val"], vertex_idx, as<List>(vertex), onset, terminus);
}

Rcpp::List RNetwork::deactivateVertex(int vertex_idx, double onset, double terminus) {
	return deactivate_vertex(net["val"], vertex_idx, as<List>(net["val"])[vertex_idx], onset, terminus);
}

Rcpp::List RNetwork::deactivateVertex(int vertex_idx, SEXP vertex, double onset, double terminus) {
	return deactivate_vertex(net["val"], vertex_idx, vertex, onset, terminus);
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

void collect_edges(int vertex_id, const List& list, List& mel, double at, std::vector<SEXP>& edges) {
	// integer vector contains R style indices
	for (int idx : as<IntegerVector>(list[vertex_id])) {
		SEXP edge = mel[idx - 1];
		if (is_edge_active(edge, at, false)) {
			edges.push_back(edge);
		}
	}
}

void RNetwork::edges(int vertex_id, double at, Neighborhood ngh, std::vector<SEXP>& edges) {
	// if network is undirected then ngh must be combined.
	if (!getNetworkAttribute<bool>("directed")) {
		ngh = COMBINED;
	}

	List mel = as<List>(net["mel"]);
	if (ngh == IN)
		collect_edges(vertex_id, as<List>(net["iel"]), mel, at, edges);
	else if (ngh == OUT)
		collect_edges(vertex_id, as<List>(net["oel"]), mel, at, edges);
	else {
		collect_edges(vertex_id, as<List>(net["iel"]), mel, at, edges);
		collect_edges(vertex_id, as<List>(net["oel"]), mel, at, edges);
	}
}

void RNetwork::simulate() {
	simulate_();
}

} /* namespace TransModel */
