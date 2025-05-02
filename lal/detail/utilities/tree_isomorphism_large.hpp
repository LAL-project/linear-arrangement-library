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

namespace lal {
namespace detail {
namespace isomorphism {

/// A tuple of the algorithm in \cite Aho1974a.
typedef std::vector<std::size_t> AHUTuple;
/// Useful typedef.
typedef std::vector<std::vector<node>> LevelList;

/// Useful data structure for the isomorphism test.
struct tuple_node {
	AHUTuple tuple;
	node v;
	[[nodiscard]] inline bool operator< (const tuple_node& t) const noexcept
	{
		return tuple < t.tuple;
	}
	[[nodiscard]] inline bool operator== (const tuple_node& t) const noexcept
	{
		return tuple == t.tuple;
	}
};

/**
 * @brief Distributes the vertices of the tree @e t over height levels.
 * @param t Rooted tree.
 * @param[out] levels A series of lists of vertices per height level.
 */
inline void gather_vertices_by_level(
	const graphs::rooted_tree& t, LevelList& levels
) noexcept
{

	const uint64_t n = t.get_num_nodes();
	array<uint64_t> height(n);
	height[t.get_root()] = 0;
	levels.push_back({t.get_root()});

	BFS bfs(t);
	bfs.set_use_rev_edges(false);
	bfs.set_process_neighbour(
		[&](const auto&, const node v, const node w, const bool)
		{
			height[w] = height[v] + 1;
			if (levels.size() <= height[w]) {
				levels.emplace_back(0);
			}
			levels[height[w]].push_back(w);
		}
	);
	bfs.start_at(t.get_root());
}

/**
 * @brief Assign ids to the vertices in the level previous to the current one.
 * @param t Rooted tree.
 * @param S Sequence of tuple identifiers that determines the current level.
 * @param[out] tuple_ids The tuple and id for every vertex of the tree.
 */
inline void calculate_ids(
	const graphs::rooted_tree& t,
	const array<tuple_node>& S,
	array<AHUTuple>& tuple_ids
) noexcept
{

	const auto assign_id = [&](const std::size_t j, const std::size_t id)
	{
		const node v = S[j].v;
		const node parent = t.get_in_neighbors(v)[0];
		tuple_ids[parent].push_back(id);
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
 * @returns True or false.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic_large(
	const graphs::rooted_tree& t_1, const graphs::rooted_tree& t_2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	const uint64_t n = t_1.get_num_nodes();

	isomorphism::LevelList levels_1, levels_2;

	isomorphism::gather_vertices_by_level(t_1, levels_1);
	isomorphism::gather_vertices_by_level(t_2, levels_2);

	// different depth levels, the trees are not isomorphic
	if (levels_1.size() != levels_2.size()) {
		return false;
	}

	array<isomorphism::AHUTuple> tuple_ids_1(n);
	array<isomorphism::AHUTuple> tuple_ids_2(n);

	for (node u = 0; u < n; ++u) {
		if (t_1.get_out_degree(u) == 0) {
			tuple_ids_1[u] = {0};
		}
		if (t_2.get_out_degree(u) == 0) {
			tuple_ids_2[u] = {0};
		}
	}

	array<isomorphism::tuple_node> S_1;
	array<isomorphism::tuple_node> S_2;

	bool stop = false;
	std::size_t h = levels_1.size() - 1;
	while (not stop) {
		if (h == 0) {
			stop = true;
		}

		// Step 1: Construct the sequences of tuples of the current level.
		S_1.resize<false>(levels_1[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_1[h]) {
				S_1[idx].tuple = std::move(tuple_ids_1[v]);
				S_1[idx].v = v;
				++idx;
			}
		}
		S_2.resize<false>(levels_2[h].size());
		{
			std::size_t idx = 0;
			for (const node v : levels_2[h]) {
				S_2[idx].tuple = std::move(tuple_ids_2[v]);
				S_2[idx].v = v;
				++idx;
			}
		}

		// Step 2: Sort the sequences of the current level. If they differ,
		// the trees are *not* isomorphic.
		std::sort(S_1.begin(), S_1.end());
		std::sort(S_2.begin(), S_2.end());

		if (S_1 != S_2) {
			return false;
		}

		// Step 3: Construct the ids of the current tuples.
		if (h > 0) {
			isomorphism::calculate_ids(t_1, S_1, tuple_ids_1);
			isomorphism::calculate_ids(t_2, S_2, tuple_ids_2);

			--h;
		}
	}

	return true;
}

} // namespace detail
} // namespace lal
