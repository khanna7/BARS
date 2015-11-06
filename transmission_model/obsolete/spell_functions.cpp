/*
 * spell_functions.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: nick
 */

#include "spell_functions.h"

using namespace Rcpp;

namespace TransModel {

NumericMatrix create_matrix(const int nrows, const int ncols, std::initializer_list<double> list) {
	return NumericMatrix(nrows, ncols, list.begin());
}

void fill_matrix_exclude_rows(const NumericMatrix& source, NumericMatrix& target, std::set<int> excluded_rows) {
	if (target.nrow() != source.nrow() - excluded_rows.size()) {
		throw std::invalid_argument("num rows in target != num rows in source - excluded rows.");
	}

	int j = 0;
	for (int i = 0, n = source.nrow(); i < n; ++i) {
		if (excluded_rows.find(i) == excluded_rows.end()) {
			target(j, ONSET) = source(i, ONSET);
			target(j, TERMINUS) = source(i, TERMINUS);
			++j;
		}
	}
}

void concatenate(const NumericMatrix& one, const NumericMatrix& two, NumericMatrix& newMatrix) {
	if (newMatrix.rows() != one.rows() + two.rows())
		throw std::invalid_argument("newMatrix row count != one + two row count");

	for (int r = 0, rows = one.rows(); r < rows; r++) {
		newMatrix(r, ONSET) = one(r, ONSET);
		newMatrix(r, TERMINUS) = one(r, TERMINUS);
	}

	int offset = one.rows();
	for (int r = 0, rows = two.rows(); r < rows; r++) {
		newMatrix(r + one.rows(), ONSET) = two(r, ONSET);
		newMatrix(r + one.rows(), TERMINUS) = two(r, TERMINUS);
	}
}

void concatenate(const NumericMatrix& one, const NumericMatrix& two, const NumericMatrix& three, NumericMatrix& newMatrix) {
	if (newMatrix.rows() != one.rows() + two.rows() + three.rows())
		throw std::invalid_argument("newMatrix row count != one + two + three row count");

	for (int r = 0, rows = one.rows(); r < rows; r++) {
		newMatrix(r, ONSET) = one(r, ONSET);
		newMatrix(r, TERMINUS) = one(r, TERMINUS);
	}

	int offset = one.rows();
	for (int r = 0, rows = two.rows(); r < rows; r++) {
		newMatrix(r + offset, ONSET) = two(r, ONSET);
		newMatrix(r + offset, TERMINUS) = two(r, TERMINUS);
	}

	offset += two.rows();
	for (int r = 0, rows = three.rows(); r < rows; r++) {
			newMatrix(r + offset, ONSET) = three(r, ONSET);
			newMatrix(r + offset, TERMINUS) = three(r, TERMINUS);
		}
}

bool any_spell_active(const NumericMatrix& spell_matrix, double at) {
	if (spell_matrix(0, ONSET) == R_NegInf && spell_matrix(0, TERMINUS) == R_PosInf)
			return true;

		for (int i = spell_matrix.rows() - 1; i >= 0; --i) {
			if (spell_matrix(i, ONSET) == R_PosInf)
				continue;
			if (spell_matrix(i, ONSET) == spell_matrix(i, TERMINUS)) {
				if (at == spell_matrix(i, ONSET))
					return true;
			} else if (spell_matrix(i, TERMINUS) == R_PosInf && at == R_PosInf) {
				return true;
			} else if (spell_matrix(i, ONSET) <= at && at < spell_matrix(i, TERMINUS)) {
				return true;
			}
		}
		return false;
}

NumericMatrix delete_spell(NumericMatrix& spell_matrix, double onset, double terminus) {
	int lr_idx = spell_matrix.nrow() - 1;
	if (onset > spell_matrix(lr_idx, TERMINUS)
			|| (onset == spell_matrix(lr_idx, TERMINUS) && onset != spell_matrix(lr_idx, ONSET))) {
		return spell_matrix;
	}

	if (terminus < spell_matrix(0, ONSET) || (terminus == spell_matrix(0, ONSET) && onset != terminus))
		return spell_matrix;

	if (onset == terminus) {
		int max_row = max_row_where_onset(spell_matrix, onset, std::equal_to<double>());
		if (max_row != -1 && spell_matrix(max_row, ONSET) == spell_matrix(max_row, TERMINUS)) {
			if (lr_idx == 0) {
				return create_matrix(1, 2, { R_PosInf, R_PosInf });
			} else {
				NumericMatrix ret(lr_idx, 2);
				fill_matrix_exclude_rows(spell_matrix, ret, std::set<int> { max_row });
				return ret;
			}

		} else {
			return spell_matrix;
		}
	}

	int erow, lrow;
	erow = lrow = -1;

	if (onset <= spell_matrix(0, ONSET)) {
		erow = -1;
	} else {
		erow = max_row_where_onset(spell_matrix, onset, std::greater<double>());
	}

	if (terminus > spell_matrix(lr_idx, TERMINUS)
			|| (terminus == spell_matrix(lr_idx, TERMINUS)
					&& spell_matrix(lr_idx, ONSET) != spell_matrix(lr_idx, TERMINUS))) {
		lrow = lr_idx + 1;

	} else if (terminus == spell_matrix(lr_idx, TERMINUS)
			&& spell_matrix(lr_idx, ONSET) == spell_matrix(lr_idx, TERMINUS)) {
		lrow = lr_idx;

	} else {
		lrow = min_row_where_terminus(spell_matrix, terminus, std::less<double>());
		if (lrow > 0 && spell_matrix(lrow - 1, TERMINUS) == terminus && spell_matrix(lrow - 1, ONSET) == terminus) {
			lrow -= 1;
		}
	}

	NumericMatrix* splitspell(nullptr);
	if (lrow == erow) {
		splitspell = new NumericMatrix(1, 2, spell_matrix.column(lrow).begin());
	}

	if (erow != -1 && onset < spell_matrix(erow, TERMINUS)) {
		spell_matrix(erow, TERMINUS) = onset;
	}

	if (lrow != lr_idx + 1 && terminus > spell_matrix(lrow, ONSET)) {
		if (lrow == erow) {
			(*splitspell)(0, ONSET) = terminus;
			lrow += 1;
		} else {
			spell_matrix(lrow, ONSET) = terminus;
		}
	}

	if (erow == -1 && lrow == lr_idx + 1) {
		delete splitspell;
		return create_matrix(1, 2, { R_PosInf, R_PosInf });
	}

	if (erow == -1) {
		delete splitspell;
		return spell_matrix(Range(lrow, lr_idx), _);
	}

	if (lrow == lr_idx + 1 && splitspell != nullptr) {
		NumericMatrix newMatrix(erow + 1 + splitspell->rows(), 2);
		concatenate(spell_matrix(Range(0, erow), _), *splitspell, newMatrix);
		delete splitspell;
		return newMatrix;
	}

	if (lrow == lr_idx + 1) {
		return spell_matrix(Range(0, erow), _);
	}

	if (erow + 1 == lrow && splitspell == nullptr) {
		return spell_matrix;
	}

	if (erow + 1 == lrow) {
		NumericMatrix newMatrix(erow + 1 + splitspell->rows() + (lr_idx - lrow) + 1, 2);
		concatenate(spell_matrix(Range(0, erow), _), *splitspell,
				spell_matrix(Range(lrow, lr_idx), _), newMatrix);
		delete splitspell;
		return newMatrix;
	}

	delete splitspell;
	// range to exclude is erow + 1 : lrow - 1 inclusive;
	int span = (lrow - 1) - (erow + 1) + 1;
	NumericMatrix newMatrix(spell_matrix.rows() - span, 2);
	std::set<int> exclude;
	for (int i = erow + 1; i <= lrow - 1; ++i) {
		exclude.insert(i);
	}
	fill_matrix_exclude_rows(spell_matrix, newMatrix, exclude);
	return newMatrix;
}

}

