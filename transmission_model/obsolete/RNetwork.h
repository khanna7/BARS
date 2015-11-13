/*
 * RNetwork.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_RNETWORK_H_
#define SRC_RNETWORK_H_

#include <memory>

#include "RInside.h"

namespace TransModel {

 enum Neighborhood {IN, OUT, COMBINED};


//struct MELToEdgePair : public std::unary_function<std::pair<int, int>, SEXP> {
//	std::pair<int, int> operator()(SEXP exp) {
//		Rcpp::List mel_item = Rcpp::as<Rcpp::List>(exp);
//		return std::make_pair((Rcpp::as<Rcpp::IntegerVector>(mel_item["outl"]))[0],
//				(Rcpp::as<Rcpp::IntegerVector>(mel_item["inl"]))[0]);
//	}
//};


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
bool is_edge_active(SEXP attribute_list, double at, bool default_active);

/**
 * Gets whether or not the vertex is active at the
 * specified time. Returns true if 'at' is within _any_ of the vertex's activity
 * spells. If the vertex's activity spell matrix is null, then 'default_active' is
 * returned.
 *
 * @param vertex the vertex
 * @param at the time to check if the edge is active at
 * @param default_active the value to return if the vertex
 * activity matrix is null.
 */
bool is_vertex_active(SEXP vertex, double at, bool default_active);

/**
 * Activates the specified vertex for the specified time period. The
 * SEXP representing the vertex may change as a result of setting the
 * spell, the vertex attribute list (val) is updated to reflect this, and
 * the new vertex List is returned.
 */
Rcpp::List activate_vertex(SEXP vertex_attribute_list, int vertex_idx, SEXP vertex, double onset, double terminus);

Rcpp::List deactivate_vertex(SEXP vertex_attribute_list, int vertex_idx, SEXP vertex, double onset, double terminus);

void activate_edge(SEXP edge, double onset, double terminus);

void deactivate_edge(SEXP edge, double onset, double terminus);

/**
 * Gets the R-style (starts with 1) index of incoming vertex of the edge.
 */
int  edge_in_idx(SEXP edge);

/**
 * Gets the R-style (starts with 1) index of outgoing vertex of the edge.
 */
int  edge_out_idx(SEXP edge);



//typedef boost::filter_iterator<IsActiveEdge, Rcpp::List::iterator> EdgeFilterIter;
//typedef boost::transform_iterator<MELToEdgePair, std::pair<int, int> > EdgeIterator;

class RNetwork {

private:
	std::shared_ptr<RInside> r_ptr_;
	Rcpp::List net;
	std::string net_name_, pid_name;
	Rcpp::Function simulate_;

public:
	RNetwork(std::shared_ptr<RInside>& r_ptr, const std::string& net_name);

	virtual ~RNetwork();

	template <typename T>
	T getVertexAttribute(int vertex_idx, const std::string& attribute);

	template <typename T>
	void setVertexAttribute(int vertex_idx, const std::string& attribute, T val);

	template <typename T>
	T getNetworkAttribute(const std::string& attribute) const;

	Rcpp::List activateVertex(int vertex_idx, double onset, double terminus);

	Rcpp::List activateVertex(int vertex_idx, SEXP vertex, double onset, double terminus);

	Rcpp::List deactivateVertex(int vertex_idx, double onset, double terminus);

	Rcpp::List deactivateVertex(int vertex_idx, SEXP vertex, double onset, double terminus);

	/**
	 * Adds the specified number of vertices to the network. The
	 * ids of these vertices will be returned in ids.
	 *
	 * @param num_to_add the number of vertices to add
	 * @param ids filled with the ids of the new vertices
	 */
	void addVertices(int num_to_add, std::vector<int>& ids);

	int getVertexCount() const;

	int getEdgeCount() const;

	int getActiveEdgeCount(double at) const;

	void simulate();

	Rcpp::List edgeList();

	Rcpp::List vertexList();

	/**
	 * Gets the edges for the specified vertex that are active at the specified time.
	 * Edges are returned in the specified edges vector.
	 */
	void edges(int vertex_id, double at, Neighborhood ngh, std::vector<SEXP>& edges);

	/**
	 * Updates the R environment with changes to the network
	 * made on the C++ model side.
	 */
	void updateNetworkToR();

	/**
	 * Updates the C++ model side with change made to the network
	 * in the R environment (e.g. from calling simulate).
	 */
	void updateNetworkFromR();
};

template <typename T>
T RNetwork::getVertexAttribute(int vertex_idx,  const std::string& attribute) {
	Rcpp::List val = Rcpp::as<Rcpp::List>(net["val"]);
	Rcpp::List v = Rcpp::as<Rcpp::List>(val[vertex_idx]);
	if (!v.containsElementNamed(attribute.c_str())) {
		std::cout << vertex_idx << " is missing attribute " << attribute << std::endl;
	}
    return v[attribute];
}

template <typename T>
void RNetwork::setVertexAttribute(int vertex_idx, const std::string& attribute, T v) {
	Rcpp::List val = Rcpp::as<Rcpp::List>(net["val"]);
	Rcpp::List vertex_atl = Rcpp::as<Rcpp::List>(val[vertex_idx]);
	vertex_atl[attribute] = Rcpp::wrap(v);
	val[vertex_idx] = vertex_atl;
	net["val"] = val;
}

template <typename T>
T RNetwork::getNetworkAttribute(const std::string& attribute) const {
    Rcpp::List g = Rcpp::as<Rcpp::List>(net["gal"]);
    return g[attribute];
}

} /* namespace TransModel */

#endif /* SRC_RNETWORK_H_ */
