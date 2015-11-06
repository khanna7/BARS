/*
 * SpellTests.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "spell_functions.h"

using namespace Rcpp;

namespace TransModel {

void test_matrix(const NumericMatrix& mat, const std::vector<double>& expected) {
	for (int i = 0; i < mat.nrow(); ++i) {
		for (int j = 0; j < mat.ncol(); ++j) {
			ASSERT_EQ(expected[mat.nrow() * j + i], mat(i, j));
		}
	}
}

TEST(SpellTests, CreateMatrix) {
	// list is by column
	std::initializer_list<double> list { 1, 2, 7, 8, 12, 3 };
	std::vector<double> exp(list);

	NumericMatrix mat = create_matrix(3, 2, list);
	ASSERT_EQ(3, mat.nrow());
	ASSERT_EQ(2, mat.ncol());
	test_matrix(mat, exp);
}

TEST(SpellTests, FillExclude) {
	// list is by column
	std::initializer_list<double> list { 1, 2, 3, 4, 5, 6, 7, 8 };
	NumericMatrix mat = create_matrix(4, 2, list);
	NumericMatrix target(2, 2);
	fill_matrix_exclude_rows(mat, target, std::set<int> { 1, 3 });
	std::vector<double> exp { 1, 3, 5, 7 };
	test_matrix(target, exp);
}

TEST(SpellTests, MaxRowWithOnset) {
	NumericMatrix mat = create_matrix(3, 2, { 10, 22, 31, 20, 30, 40 });
	ASSERT_EQ(1, max_row_where_onset(mat, 22, std::equal_to<double>()));
	ASSERT_EQ(-1, max_row_where_onset(mat, 12, std::equal_to<double>()));

	ASSERT_EQ(0, max_row_where_onset(mat, 22, std::greater<double>()));
}

TEST(SpellTests, MatrixConcat) {
	std::initializer_list<double> list { 1, 2, 3, 4,   5, 6, 7, 8 };
	NumericMatrix one = create_matrix(4, 2, list);
	NumericMatrix two = create_matrix(2, 2, {-10, -12,   34, 108});
	NumericMatrix result(6, 2);
	concatenate(one, two, result);
	test_matrix(result, std::vector<double> { 1, 2, 3, 4, -10, -12,  5, 6, 7, 8, 34, 108});

	NumericMatrix three = create_matrix(3, 2, {-10, -12, 1,  34, 108, 42});
	NumericMatrix result2(9, 2);
	concatenate(one, two, three, result2);
	test_matrix(result2, std::vector<double> { 1, 2, 3, 4, -10, -12, -10, -12, 1,  5, 6, 7, 8, 34, 108, 34, 108, 42});
}

TEST(SpellTests, DeleteSpells) {
	// deleting interval starting after final terminus
	// so mat doesn't change
	NumericMatrix mat = create_matrix(2, 2, { 1, 3, 2, 4 });
	mat = delete_spell(mat, 5, 10);
	test_matrix(mat, std::vector<double> { 1, 3, 2, 4 });

	// onset the same as the final terminus
	// no change
	mat = delete_spell(mat, 4, 10);
	test_matrix(mat, std::vector<double> { 1, 3, 2, 4 });

	// terminus less than first onset
	// so no change
	mat = create_matrix(2, 2, { 10, 22, 20, 30 });
	mat = delete_spell(mat, 4, 9);
	test_matrix(mat, std::vector<double> { 10, 22, 20, 30 });

	// terminus equals first onset and onset != terminus
	mat = delete_spell(mat, 4, 10);
	test_matrix(mat, std::vector<double> { 10, 22, 20, 30 });

	// point deactivation but spell with point interval
	mat = create_matrix(3, 2, { 10, 12, 15, 11.5, 14, 15 });
	mat = delete_spell(mat, 12, 12);
	test_matrix(mat, std::vector<double> { 10, 12, 15, 11.5, 14, 15 });

	// point deativated and that row removed
	mat = create_matrix(3, 2, { 10, 12, 15, 11.5, 14, 15 });
	mat = delete_spell(mat, 15, 15);
	ASSERT_EQ(2, mat.rows());
	ASSERT_EQ(2, mat.cols());
	test_matrix(mat, std::vector<double> { 10, 12, 11.5, 14 });

	// point deativated and only row replaced with Inf, Inf
	mat = create_matrix(1, 2, { 10, 10 });
	mat = delete_spell(mat, 10, 10);
	ASSERT_EQ(1, mat.rows());
	ASSERT_EQ(2, mat.cols());
	test_matrix(mat, std::vector<double> { R_PosInf, R_PosInf });

	mat = create_matrix(3, 2, { 10, 12, 15, 11.5, 14, 17 });
	mat = delete_spell(mat, 1, 40);
	ASSERT_EQ(1, mat.rows());
	test_matrix(mat, std::vector<double> {R_PosInf, R_PosInf});
	ASSERT_FALSE(any_spell_active(mat, 10));

	mat = create_matrix(3, 2, { 10, 12, 15, 11.5, 14, 17 });
	mat = delete_spell(mat, 11, 15.5);
	ASSERT_EQ(2, mat.rows());
	test_matrix(mat, std::vector<double> {10.0, 15.5, 11, 17});

	mat = delete_spell(mat, 9, 10.5);
	ASSERT_EQ(2, mat.rows());
	test_matrix(mat, std::vector<double> {10.5, 15.5, 11, 17});

	mat = delete_spell(mat, 13, 16.5);
	ASSERT_EQ(2, mat.rows());
	test_matrix(mat, std::vector<double> {10.5, 16.5, 11, 17});

	mat = delete_spell(mat, 15, 20);
	ASSERT_EQ(1, mat.rows());
	test_matrix(mat, std::vector<double> {10.5, 11});
}

}

