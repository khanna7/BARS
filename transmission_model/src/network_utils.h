/*
 * network_utils.h
 *
 *  Created on: Nov 3, 2015
 *      Author: nick
 */

#ifndef SRC_NETWORK_UTILS_H_
#define SRC_NETWORK_UTILS_H_

#include <map>
#include <exception>

#include "RInside.h"

#include "Network.h"

using namespace Rcpp;

namespace TransModel {

template<typename V, typename F>
void create_r_network(List& rnet, Network<V>& net, std::map<unsigned int, unsigned int>& idx_to_v_map,
		const F& attributes_setter) {
	List gal;
	gal["n"] = net.vertexCount();
	gal["directed"] = false;
	gal["hyper"] = false;
	gal["loops"] = false;
	gal["multiple"] = false;
	gal["bipartite"] = false;
	gal["mnext"] = net.edgeCount() + 1;
	rnet["gal"] = gal;

	unsigned int vCount = net.vertexCount();
	List val(vCount);
	List iel(vCount);
	List oel(vCount);

	std::map<unsigned int, unsigned int> v_to_idx_map;
	std::map<unsigned int, unsigned int> iel_idx_map;
	std::map<unsigned int, unsigned int> oel_idx_map;

	int idx = 1;
	for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
		List vlist;
		VertexPtr<V> v = (*iter);
		idx_to_v_map.emplace(idx, v->id());
		v_to_idx_map.emplace(v->id(), idx);
		vlist["na"] = false;
		vlist["vertex.names"] = idx;
		attributes_setter(v, vlist);

		int c_index = idx - 1;
		val[c_index] = vlist;
		unsigned int in_count = net.inEdgeCount(v);
		if (in_count > 0) {
			iel_idx_map.emplace(v->id(), 0);
		}
		iel[c_index] = IntegerVector(in_count);
		unsigned int out_count = net.outEdgeCount(v);
		if (out_count > 0) {
			oel_idx_map.emplace(v->id(), 0);
		}
		oel[c_index] = IntegerVector(out_count);

		++idx;
	}
	rnet["val"] = val;

	List mel(net.edgeCount());

	int eidx = 1;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		List eal;
		List atl;
		atl["na"] = false;
		eal["atl"] = atl;

		EdgePtr<V> edge = (*iter);
		unsigned int in_idx = v_to_idx_map.at(edge->v2()->id());
		unsigned int out_idx = v_to_idx_map.at(edge->v1()->id());
		eal["inl"] = in_idx;
		eal["outl"] = out_idx;
		mel(eidx - 1) = eal;

		int idx = iel_idx_map[edge->v2()->id()];
		as<IntegerVector>(iel[in_idx - 1])(idx) = eidx;
		iel_idx_map[edge->v2()->id()] = ++idx;

		idx = oel_idx_map[edge->v1()->id()];
		as<IntegerVector>(oel[out_idx - 1])(idx) = eidx;
		++idx;
		oel_idx_map[edge->v1()->id()] = idx;

		++eidx;
	}

	rnet["oel"] = oel;
	rnet["iel"] = iel;
	rnet["mel"] = mel;
}

template<typename V, typename F>
void simulate(std::shared_ptr<RInside> R, Network<V>& net, const F& attributes_setter, double time) {
	// rn vertex to n vertex
	std::map<unsigned int, unsigned int> idx_map;
	List rnet;
	create_r_network(rnet, net, idx_map, attributes_setter);
	//Rf_PrintValue(rnet);
	SEXP changes = as<Function>((*R)["nw_simulate"])(rnet, time);
	reset_network_edges(changes, net, idx_map);
}

template<typename V>
void reset_network_edges(SEXP& changes, Network<V>& net, const std::map<unsigned int, unsigned int>& idx_map) {
	NumericMatrix matrix = as<NumericMatrix>(changes);
	for (int r = 0, n = matrix.rows(); r < n; ++r) {
		int out = idx_map.at(matrix(r, 1));
		int in = idx_map.at(matrix(r, 2));
		int to = matrix(r, 3);
		if (to) {
			net.addEdge(out, in);
		} else {
			net.removeEdge(out, in);
		}
	}
}

/**
 * Initializes specified network from the rnet. Each entry in the
 * rnet's val becomes a Vertex, and the appropriate edges are created.
 */
template<typename V, typename F>
void initialize_network(List& rnet, Network<V>& net, F vertex_creator) {
	if (net.vertexCount() != 0)
		throw std::invalid_argument("Cannot initialize network: network is not empty");
	List val = as<List>(rnet["val"]);
	for (auto& sexp : val) {
		List v = as<List>(sexp);
		VertexPtr<V> vp = vertex_creator(v);
		net.addVertex(vp);
	}

	List mel = as<List>(rnet["mel"]);
	for (auto& sexp : mel) {
		List edge = as<List>(sexp);
		net.addEdge(as<int>(edge["outl"]) - 1, as<int>(edge["inl"]) - 1);
	}
}

}

#endif /* SRC_NETWORK_UTILS_H_ */
