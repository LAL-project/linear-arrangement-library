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
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <algorithm>
#include <iostream>

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

/**
 * @brief Fast tree non-isomorphism test.
 *
 * @tparam tree_t Tree type.
 * @param t1 One tree.
 * @param t2 Another tree.
 * @returns Whether the input trees are, might be, or are not isomorphic.
 * @returns 0 if the trees ARE isomorphic
 * @returns 1 if the trees ARE NOT isomorphic:
 * - number of vertices do not coincide,
 * - number of leaves do not coincide,
 * - second moment of degree do not coincide,
 * - maximum vertex degrees do not coincide,
 * @returns 2 if the trees MIGHT BE isomorphic
 */
template <graphs::Tree tree_t>
[[nodiscard]] char fast_non_iso(const tree_t& t1, const tree_t& t2) noexcept
{
	// check number of nodes
	if (t1.get_num_nodes() != t2.get_num_nodes()) {
		return 1;
	}

	const uint64_t n = t1.get_num_nodes();

	// trees ARE isomorphic
	if (n <= 2) {
		return 0;
	}

	uint64_t nL_t1 = 0;		// number of leaves of t1
	uint64_t nL_t2 = 0;		// number of leaves of t2
	uint64_t k2_t1 = 0;		// sum of squared degrees of t1
	uint64_t k2_t2 = 0;		// sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = to_uint64(t1.get_degree(u));
		const uint64_t ku2 = to_uint64(t2.get_degree(u));

		nL_t1 += t1.get_degree(u) == 1;
		nL_t2 += t2.get_degree(u) == 1;
		k2_t1 += ku1 * ku1;
		k2_t2 += ku2 * ku2;
		maxdeg_t1 = (maxdeg_t1 < ku1 ? ku1 : maxdeg_t1);
		maxdeg_t2 = (maxdeg_t2 < ku2 ? ku2 : maxdeg_t2);
	}

	// check number of leaves
	if (nL_t1 != nL_t2) {
		return 1;
	}
	// check maximum degree
	if (maxdeg_t1 != maxdeg_t2) {
		return 1;
	}
	// check sum of squared degrees
	if (k2_t1 != k2_t2) {
		return 1;
	}

	// trees MIGHT BE isomorphic
	return 2;
}

template <typename T>
inline std::ostream&
operator<< (std::ostream& os, const std::vector<T>& v) noexcept
{
	if (v.size() > 0) {
		os << v[0];
		for (std::size_t i = 1; i < v.size(); ++i) {
			os << ' ' << v[i];
		}
	}
	return os;
}

typedef std::vector<std::size_t> AHUTuple;

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

typedef std::vector<std::vector<node>> LevelList;

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

/**
 * @brief Test whether two rooted trees are isomorphic or not.
 *
 * See \cite Aho1974a for further details on the algorithm.
 * @param t1 First rooted tree.
 * @param t2 Second rooted tree.
 * @returns True or false.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic(
	const graphs::rooted_tree& t_1, const graphs::rooted_tree& t_2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	const uint64_t n = t_1.get_num_nodes();

	LevelList levels_1, levels_2;

	std::cout << "tree 1:\n";
	std::cout << "·    "_tab << t_1 << '\n';
	std::cout << "tree 2:\n";
	std::cout << "·    "_tab << t_2 << '\n';

	std::cout << "Levels for tree 1...\n";
	gather_vertices_by_level(t_1, levels_1);
	std::cout << "Levels for tree 2...\n";
	gather_vertices_by_level(t_2, levels_2);

	std::cout << "(1) Vertices by level:\n";
	for (std::size_t i = 0; i < levels_1.size(); ++i) {
		std::cout << "    " << i << ": " << levels_1[i] << '\n';
	}
	std::cout << '\n';
	std::cout << "(2) Vertices by level:\n";
	for (std::size_t i = 0; i < levels_2.size(); ++i) {
		std::cout << "    " << i << ": " << levels_2[i] << '\n';
	}
	std::cout << '\n';

	// different depth levels, the trees are not isomorphic
	if (levels_1.size() != levels_2.size()) {
		return false;
	}

	array<AHUTuple> tuple_ids_1(n);
	array<AHUTuple> tuple_ids_2(n);

	for (node u = 0; u < n; ++u) {
		if (t_1.get_out_degree(u) == 0) {
			tuple_ids_1[u] = {0};
		}
		if (t_2.get_out_degree(u) == 0) {
			tuple_ids_2[u] = {0};
		}
	}

	array<tuple_node> S_1;
	array<tuple_node> S_2;

	bool stop = false;
	std::size_t h = levels_1.size() - 1;
	while (not stop) {
		std::cout << "h= " << h << '\n';

		if (h == 0) {
			stop = true;
		}

		// Step 1: Construct the sequences of tuples of the current level.
		std::cout << "Step 1...\n";
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
		std::cout << "Step 2...\n";
		std::sort(S_1.begin(), S_1.end());
		std::sort(S_2.begin(), S_2.end());

		std::cout << "(1) Tuples of level " << h << '\n';
		for (std::size_t i = 0; i < S_1.size(); ++i) {
			std::cout << "    " << i << ": " << S_1[i].v << " <-> "
					  << S_1[i].tuple << '\n';
		}

		std::cout << "(2) Tuples of level " << h << '\n';
		for (std::size_t i = 0; i < S_2.size(); ++i) {
			std::cout << "    " << i << ": " << S_2[i].v << " <-> "
					  << S_2[i].tuple << '\n';
		}

		if (S_1 != S_2) {
			return false;
		}

		// Step 3: Construct the ids of the current tuples.
		if (h > 0) {
			std::cout << "Step 3...\n";
			calculate_ids(t_1, S_1, tuple_ids_1);
			calculate_ids(t_2, S_2, tuple_ids_2);

			--h;
		}

		std::cout << "    next h= " << h << '\n';

		// char step;
		// std::cin >> step;
	}

	return true;
}

} // namespace detail
} // namespace lal
