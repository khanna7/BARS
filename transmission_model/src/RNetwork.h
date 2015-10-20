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
	std::string net_name_;
	Rcpp::List net;
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

	int getVertexCount() const;

	void simulate();

	Rcpp::List edgeList();

	Rcpp::List vertexList();

	void updateRNetwork();
};

template <typename T>
T RNetwork::getVertexAttribute(int vertex_idx,  const std::string& attribute) {
    Rcpp::List v = Rcpp::as<Rcpp::List>(Rcpp::as<Rcpp::List>(net["val"])[vertex_idx]);
    return v[attribute];
}

template <typename T>
void RNetwork::setVertexAttribute(int vertex_idx, const std::string& attribute, T val) {
	Rcpp::List v = Rcpp::as<Rcpp::List>(Rcpp::as<Rcpp::List>(net["val"])[vertex_idx]);
	v[attribute] = val;
}

template <typename T>
T RNetwork::getNetworkAttribute(const std::string& attribute) const {
    Rcpp::List g = Rcpp::as<Rcpp::List>(net["gal"]);
    return g[attribute];
}


} /* namespace TransModel */

#endif /* SRC_RNETWORK_H_ */
