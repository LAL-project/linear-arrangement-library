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
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/graphs/traversal.hpp>

namespace lal {
namespace detail {

/**
 * @brief Completes the branchless path.
 *
 * The first vertex of degree different from 2 is vertex @e u. And the next
 * vertex in the sequence is vertex @e v.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param u First vertex of the path.
 * @param v Second vertex of the path.
 * @param bfs Breadth-First Search traversal object.
 * @param res Vector containing all branchless paths.
 * @param p Current branchless path.
 */
template <graphs::Tree tree_t>
void expand_branchless_path(
	const tree_t& t,
	const node u,
	const node v,
	BFS<tree_t>& bfs,
	std::vector<properties::branchless_path>& res,
	properties::branchless_path& p
) noexcept
{
	const uint64_t n = t.get_num_nodes();

	// the case of an edge...
	if (t.get_degree(u) != 2 and t.get_degree(v) != 2) {

		// avoid symmetric paths
		if (u > v) {
			return;
		}

#if defined DEBUG
		assert(t.has_edge(u, v) or t.has_edge(v, u));
#endif
		// initialize the path
		p.init(n);
		// set and add the first and second non-internal vertices
		p.add_node(u);
		p.set_h1(u);
		p.add_node(v);
		p.set_h2(v);
		// push the new path
		res.push_back(std::move(p));
		return;
	}

#if defined DEBUG
	assert(not bfs.node_was_visited(u));
#endif

	if (bfs.node_was_visited(v)) {
		return;
	}

	// initialize the path
	p.init(n);
	// set the first non-internal vertex and add it
	p.add_node(u);
	p.set_h1(u);

	// set 'u' as visited to avoid going 'back' in the tree
	bfs.set_visited(u, 1);

	// expand the new path
	bfs.start_at(v);

	// find the lowest *internal* vertex in lexicographic order
	const auto& seq = p.get_vertex_sequence();
	node lowest_lexicographic = n + 1;
	for (std::size_t i = 1; i < seq.size() - 1; ++i) {
		lowest_lexicographic = std::min(lowest_lexicographic, seq[i]);
	}
	p.set_lowest_lexicographic(lowest_lexicographic);

	// only let the internal vertices of the paths be visited
	bfs.set_visited(p.get_h1(), 0);
	bfs.set_visited(p.get_h2(), 0);

	// push the new path
	res.push_back(std::move(p));
}

/**
 * @brief Finds all branchless paths in a tree.
 *
 * The definition of branchless path used is the one in \cite Alemany2023a.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @returns The list of all branchless paths.
 */
template <graphs::Tree tree_t>
[[nodiscard]] std::vector<properties::branchless_path>
branchless_paths_compute(const tree_t& t) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Alemany2023a);
#endif

	const uint64_t n = t.get_num_nodes();

	// result of the function (to be returned)
	std::vector<properties::branchless_path> res;

	// path to be filled
	properties::branchless_path p;

	BFS bfs(t);

	// detect the last hub
	bfs.set_process_current(
		[&](const node u)
		{
			p.add_node(u);
			if (t.get_degree(u) != 2) {
				// The exploration will stop in the
				// next call to the 'terminate' function.
				p.set_h2(u);
			}
		}
	);
	// stop the traversal as soon as we find a vertex of degree != 2
	bfs.set_terminate(
		[&](const node u)
		{
			return t.get_degree(u) != 2;
		}
	);

	// find all paths starting at vertices of degree != 2
	for (node u = 0; u < n; ++u) {
		if (t.get_degree(u) == 2) {
			continue;
		}

		if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
			for (node v : t.get_neighbors(u)) {
				expand_branchless_path(t, u, v, bfs, res, p);
			}
		}
		else if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
			for (node v : t.get_out_neighbors(u)) {
				expand_branchless_path(t, u, v, bfs, res, p);
			}
			for (node v : t.get_in_neighbors(u)) {
				expand_branchless_path(t, u, v, bfs, res, p);
			}
		}
	}

	return res;
}

} // namespace detail
} // namespace lal
