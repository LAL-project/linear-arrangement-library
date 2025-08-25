/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <algorithm>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/output.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/sorting/radix_sort.hpp>
#include <lal/detail/utilities/tree_isomorphism_tuple.hpp>

namespace lal {
namespace detail {
namespace isomorphism {

/**
 * @brief Assign ids to the vertices in the level previous to the current one.
 * @param t Input rooted tree.
 * @param S Sequence of tuple identifiers that determines the current level.
 * @param[out] tuple_ids The tuple and id for every vertex of the tree.
 */
inline void calculate_ids_large(
	const graphs::rooted_tree& t,
	const array<tuple_node>& S,
	array<AHUTuple>& tuple_ids,
	std::size_t& max_value
) noexcept
{

	const auto assign_id = [&](const std::size_t j, const std::size_t id)
	{
		const node v = S[j].v;
		const node parent = t.get_in_neighbors(v)[0];
		tuple_ids[parent].push_back(id);
		max_value = std::max(max_value, id);
	};

	std::size_t id = 1;
	std::size_t j = 0;
	assign_id(j, id);

	++j;
	while (j < S.size()) {
		if (S[j].tuple != S[j - 1].tuple) {
			++id;
		}
		assign_id(j, id);
		++j;
	}
}

/**
 * @brief Assign ids to the vertices in the level previous to the current one.
 * @param parents Head vector of the tree.
 * @param S Sequence of tuple identifiers that determines the current level.
 * @param[out] tuple_ids The tuple and id for every vertex of the tree.
 */
inline void calculate_ids_large(
	const array<node>& parents,
	const array<tuple_node>& S,
	array<AHUTuple>& tuple_ids,
	std::size_t& max_value
) noexcept
{

	const auto assign_id = [&](const std::size_t j, const std::size_t id)
	{
		const node v = S[j].v;
		const node parent = parents[v];
		tuple_ids[parent].push_back(id);
		max_value = std::max(max_value, id);
	};

	std::size_t id = 1;
	std::size_t j = 0;
	assign_id(j, id);

	++j;
	while (j < S.size()) {
		if (S[j].tuple != S[j - 1].tuple) {
			++id;
		}
		assign_id(j, id);
		++j;
	}
}

} // namespace isomorphism

/**
 * @brief Test whether two rooted trees are isomorphic or not.
 *
 * See \cite Aho1974a for further details on the algorithm.
 * @param t1 First rooted tree.
 * @param t2 Second rooted tree.
 * @returns Whether the two rooted trees are isomorphic or not.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic_tuple_large(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	const uint64_t n = t1.get_num_nodes();

	isomorphism::LevelList levels_1, levels_2;

	isomorphism::gather_vertices_by_level(t1, levels_1);
	isomorphism::gather_vertices_by_level(t2, levels_2);

	// different depth levels, the trees are not isomorphic
	if (levels_1.size() != levels_2.size()) {
		return false;
	}

	array<isomorphism::AHUTuple> tuple_ids_1(n);
	array<isomorphism::AHUTuple> tuple_ids_2(n);

	for (node u = 0; u < n; ++u) {
		if (t1.get_out_degree(u) == 0) {
			tuple_ids_1[u] = {0};
		}
		if (t2.get_out_degree(u) == 0) {
			tuple_ids_2[u] = {0};
		}
	}

	array<isomorphism::tuple_node> S1;
	array<isomorphism::tuple_node> S2;

	std::size_t max_value_S1 = 0;
	std::size_t max_value_S2 = 0;

	bool stop = false;
	std::size_t h = levels_1.size() - 1;
	while (not stop) {
		if (h == 0) {
			stop = true;
		}

		// Step 1: Construct the sequences of tuples of the current level.
		std::size_t max_length_S1 = 0;
		S1.resize<false>(levels_1[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_1[h]) {
				S1[idx].tuple = std::move(tuple_ids_1[v]);
				max_length_S1 = std::max(S1[idx].tuple.size(), max_length_S1);

				S1[idx].v = v;
				++idx;
			}
		}
#if defined DEBUG
		assert(max_length_S1 > 0);
#endif

		std::size_t max_length_S2 = 0;
		S2.resize<false>(levels_2[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_2[h]) {
				S2[idx].tuple = std::move(tuple_ids_2[v]);
				max_length_S2 = std::max(S2[idx].tuple.size(), max_length_S2);

				S2[idx].v = v;
				++idx;
			}
		}
#if defined DEBUG
		assert(max_length_S2 > 0);
#endif

		// Step 2: Sort the sequences of the current level. If they differ,
		// the trees are *not* isomorphic.
		sorting::radix_sort<sorting::sort_type::non_decreasing>(
			S1, max_value_S1, max_length_S1
		);
		sorting::radix_sort<sorting::sort_type::non_decreasing>(
			S2, max_value_S2, max_length_S2
		);

		if (S1 != S2) {
			return false;
		}

		// Step 3: Construct the ids of the current tuples.
		if (h > 0) {
			isomorphism::calculate_ids_large(
				t1, S1, tuple_ids_1, max_value_S1
			);
			isomorphism::calculate_ids_large(
				t2, S2, tuple_ids_2, max_value_S2
			);

			--h;
		}
	}

	return true;
}

/**
 * @brief Test whether two rooted trees are isomorphic or not.
 *
 * See \cite Aho1974a for further details on the algorithm.
 * @param t1 First rooted tree.
 * @param r1 Root of the first tree.
 * @param t2 Second rooted tree.
 * @param r2 Root of the second tree.
 * @returns Whether the two rooted trees are isomorphic or not.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic_tuple_large(
	const graphs::free_tree& t1,
	const node r1,
	const graphs::free_tree& t2,
	const node r2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	const uint64_t n = t1.get_num_nodes();

	isomorphism::LevelList levels_1, levels_2;

	array<node> parents_1(n);
	array<node> parents_2(n);

	isomorphism::gather_vertices_by_level(t1, r1, levels_1, parents_1);
	isomorphism::gather_vertices_by_level(t2, r2, levels_2, parents_2);

	// different depth levels, the trees are not isomorphic
	if (levels_1.size() != levels_2.size()) {
		return false;
	}

	array<isomorphism::AHUTuple> tuple_ids_1(n);
	array<isomorphism::AHUTuple> tuple_ids_2(n);

	for (node u = 0; u < n; ++u) {
		if (t1.get_degree(u) == 1) {
			tuple_ids_1[u] = {0};
		}
		if (t2.get_degree(u) == 1) {
			tuple_ids_2[u] = {0};
		}
	}

	array<isomorphism::tuple_node> S1;
	array<isomorphism::tuple_node> S2;

	std::size_t max_value_S1 = 0;
	std::size_t max_value_S2 = 0;

	bool stop = false;
	std::size_t h = levels_1.size() - 1;
	while (not stop) {
		if (h == 0) {
			stop = true;
		}

		// Step 1: Construct the sequences of tuples of the current level.
		std::size_t max_length_S1 = 0;
		S1.resize<false>(levels_1[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_1[h]) {
				S1[idx].tuple = std::move(tuple_ids_1[v]);
				max_length_S1 = std::max(S1[idx].tuple.size(), max_length_S1);

				S1[idx].v = v;
				++idx;
			}
		}
#if defined DEBUG
		assert(max_length_S1 > 0);
#endif

		std::size_t max_length_S2 = 0;
		S2.resize<false>(levels_2[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_2[h]) {
				S2[idx].tuple = std::move(tuple_ids_2[v]);
				max_length_S2 = std::max(S2[idx].tuple.size(), max_length_S2);

				S2[idx].v = v;
				++idx;
			}
		}
#if defined DEBUG
		assert(max_length_S2 > 0);
#endif

		// Step 2: Sort the sequences of the current level. If they differ,
		// the trees are *not* isomorphic.
		sorting::radix_sort<sorting::sort_type::non_decreasing>(
			S1, max_value_S1, max_length_S1
		);
		sorting::radix_sort<sorting::sort_type::non_decreasing>(
			S2, max_value_S2, max_length_S2
		);

		if (S1 != S2) {
			return false;
		}

		// Step 3: Construct the ids of the current tuples.
		if (h > 0) {
			max_value_S1 = 0;
			isomorphism::calculate_ids_large(
				parents_1, S1, tuple_ids_1, max_value_S1
			);
			max_value_S2 = 0;
			isomorphism::calculate_ids_large(
				parents_2, S2, tuple_ids_2, max_value_S2
			);

			--h;
		}
	}

	return true;
}

} // namespace detail
} // namespace lal
