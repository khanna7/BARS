/*
 * spell_functions.h
 *
 *  Created on: Oct 21, 2015
 *      Author: nick
 */

#ifndef SRC_SPELL_FUNCTIONS_H_
#define SRC_SPELL_FUNCTIONS_H_

#include "Rcpp.h"

namespace TransModel {

const int ONSET = 0;
const int TERMINUS = 1;

template<typename Predicate>
int max_row_where_onset(Rcpp::NumericMatrix& spell_matrix, double onset, Predicate p) {
	for (int i = spell_matrix.nrow() - 1; i >= 0; --i) {
		if (p(onset, spell_matrix(i, ONSET)))
			return i;
	}
	return -1;
}

template<typename Predicate>
int min_row_where_terminus(Rcpp::NumericMatrix& spell_matrix, double terminus, Predicate p) {
	for (int i = 0, n = spell_matrix.nrow(); i < n; ++i) {
		if (p(terminus, spell_matrix(i, TERMINUS)))
			return i;
	}
	return -1;
}

bool any_spell_active(const Rcpp::NumericMatrix& spell_matrix, double at);

void concatenate(const Rcpp::NumericMatrix& one, const Rcpp::NumericMatrix& two, Rcpp::NumericMatrix& newMatrix);

void concatenate(const Rcpp::NumericMatrix& one, const Rcpp::NumericMatrix& two, const Rcpp::NumericMatrix& three,
		Rcpp::NumericMatrix& newMatrix);

/**
 * Creates a matrix of the specified dimensions filled with the specified data.
 * The specified data should be in column order.
 *
 * @param list a list of size nrows * ncols in column order
 */
Rcpp::NumericMatrix create_matrix(const int nrows, const int ncols, std::initializer_list<double> list);

void fill_matrix_exclude_rows(const Rcpp::NumericMatrix& source, Rcpp::NumericMatrix& target,
		std::set<int> excluded_rows);

Rcpp::NumericMatrix delete_spell(Rcpp::NumericMatrix& spell_matrix, double onset, double terminus);

}

#endif /* SRC_SPELL_FUNCTIONS_H_ */
