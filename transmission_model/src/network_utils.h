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
#include "Stats.h"

using namespace Rcpp;

namespace TransModel {

template<typename V, typename F>
void create_r_network(List& rnet, Network<V>& net, std::map<unsigned int, unsigned int>& idx_to_v_map,
		const F& attributes_setter, int edge_type) {
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

		unsigned int in_count = net.inEdgeCount(v, edge_type);
		iel[c_index] = IntegerVector(in_count);

		unsigned int out_count = net.outEdgeCount(v, edge_type);
		oel[c_index] = IntegerVector(out_count);

		++idx;
	}
	rnet["val"] = val;

	List mel(net.edgeCount(edge_type));

	int eidx = 1;
	for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
		EdgePtr<V> edge = (*iter);
		if (edge->type() == edge_type) {
			List eal;
			List atl;
			atl["na"] = false;
			eal["atl"] = atl;
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
	create_r_network(rnet, net, idx_map, attributes_setter, MAIN_NETWORK_TYPE);
	//Rf_PrintValue(rnet);
	SEXP changes = as<Function>((*R)["nw_simulate"])(rnet, time);
	reset_network_edges(changes, net, idx_map, time, MAIN_NETWORK_TYPE);

	List cas_net;
	idx_map.clear();
	create_r_network(cas_net, net, idx_map, attributes_setter, CASUAL_NETWORK_TYPE);
	changes = as<Function>((*R)["n_cas_simulate"])(cas_net, time);
	reset_network_edges(changes, net, idx_map, time, CASUAL_NETWORK_TYPE);
}

template<typename V>
void reset_network_edges(SEXP& changes, Network<V>& net, const std::map<unsigned int, unsigned int>& idx_map,
		double time, int edge_type) {
	NumericMatrix matrix = as<NumericMatrix>(changes);

	int added = 0;
	int removed = 0;
	for (int r = 0, n = matrix.rows(); r < n; ++r) {
		int in = idx_map.at(matrix(r, 1));
		int out = idx_map.at(matrix(r, 2));
		int to = matrix(r, 3);
		if (to) {
			net.addEdge(out, in, edge_type);
			++added;
			Stats::instance()->recordPartnershipEvent(time, out, in, PartnershipEvent::STARTED, edge_type);
		} else {
			bool res = net.removeEdge(out, in, edge_type);
			if (!res) {
				throw std::domain_error("Updating from tergm changes: trying to remove an edge that doesn't exist");
			}
			Stats::instance()->recordPartnershipEvent(time, out, in, PartnershipEvent::ENDED, edge_type);
			++removed;
		}
	}

	//int expected = count + added - removed;
	//std::cout << "expected: " << expected << ", edge count: " << net.edgeCount() << std::endl;
}

/**
 * Initializes specified network from the rnet. Each entry in the
 * rnet's val becomes a Vertex, and the appropriate edges are created.
 */
template<typename V, typename F>
void initialize_network(List& rnet, Network<V>& net, F& vertex_creator, int edge_type = 0) {
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
		net.addEdge(as<int>(edge["outl"]) - 1, as<int>(edge["inl"]) - 1, edge_type);
	}
}

/**
 * Adds the edges in the r network to the Network net.
 */
template<typename V>
void initialize_edges(List& rnet, Network<V>& net, int edge_type) {
	List mel = as<List>(rnet["mel"]);
	for (auto& sexp : mel) {
		List edge = as<List>(sexp);
		net.addEdge(as<int>(edge["outl"]) - 1, as<int>(edge["inl"]) - 1, edge_type);
	}
}

}

#endif /* SRC_NETWORK_UTILS_H_ */
