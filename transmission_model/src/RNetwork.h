/*
 * RNetwork.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_RNETWORK_H_
#define SRC_RNETWORK_H_

#include <memory>

#include "boost/iterator/filter_iterator.hpp"
#include "boost/iterator/transform_iterator.hpp"

#include "RInside.h"

namespace TransModel {


struct MELToEdgePair : public std::unary_function<std::pair<int, int>, SEXP> {
	std::pair<int, int> operator()(SEXP exp) {
		Rcpp::List mel_item = Rcpp::as<Rcpp::List>(exp);
		return std::make_pair((Rcpp::as<Rcpp::IntegerVector>(mel_item["outl"]))[0],
				(Rcpp::as<Rcpp::IntegerVector>(mel_item["inl"]))[0]);
	}
};

struct IsActiveEdge {
	bool operator()(SEXP exp) {
		Rcpp::List mel_item = Rcpp::as<Rcpp::List>(exp);
		Rcpp::List atl = Rcpp::as<Rcpp::List>(mel_item["atl"]);
		return !(Rcpp::as<Rcpp::LogicalVector>(atl["na"]))[0];
	}
};


typedef boost::filter_iterator<IsActiveEdge, Rcpp::List::iterator> EdgeFilterIter;
typedef boost::transform_iterator<MELToEdgePair, std::pair<int, int> > EdgeIterator;

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
