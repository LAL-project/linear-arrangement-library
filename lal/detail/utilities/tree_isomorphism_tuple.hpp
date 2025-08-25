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
	[[nodiscard]] std::size_t operator[] (const std::size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < tuple.size());
#endif
		return tuple[i];
	}
	[[nodiscard]] std::size_t size() const noexcept
	{
		return tuple.size();
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
 * @brief Distributes the vertices of the tree @e t over height levels.
 * @param t Input rooted tree tree, as a @ref lal::graphs::free_tree object.
 * @param r Root of the tree.
 * @param[out] levels A series of lists of vertices per height level.
 * @param[out] parents A vector of the parent of each vertex.
 */
inline void gather_vertices_by_level(
	const graphs::free_tree& t,
	const node r,
	LevelList& levels,
	array<node>& parents
) noexcept
{

	const uint64_t n = t.get_num_nodes();
	array<uint64_t> height(n);
	height[r] = 0;
	parents[r] = n + 1;
	levels.push_back({r});

	BFS bfs(t);
	bfs.set_use_rev_edges(false);
	bfs.set_process_neighbour(
		[&](const auto&, const node v, const node w, const bool)
		{
			height[w] = height[v] + 1;
			parents[w] = v;
			if (levels.size() <= height[w]) {
				levels.emplace_back(0);
			}
			levels[height[w]].push_back(w);
		}
	);
	bfs.start_at(r);
}

} // namespace isomorphism
} // namespace detail
} // namespace lal
