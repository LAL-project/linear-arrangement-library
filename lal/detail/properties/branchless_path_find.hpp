/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/detail/data_array.hpp>
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
 * @param label Label per (internal) vertex so as to identify paths.
 * @param max_label Next label to be used.
 * @param res Vector containing all branchless paths.
 * @param p Current branchless path.
 */
template <class tree_t>
void expand_branchless_path(
	const tree_t& t,
	const node u, const node v,
	detail::BFS<tree_t>& bfs,
	detail::data_array<std::size_t>& label,
	std::size_t& max_label,
	std::vector<properties::branchless_path>& res,
	properties::branchless_path& p
)
noexcept
{
	const uint64_t n = t.get_num_nodes();

	const bool visited_u = bfs.node_was_visited(u);
	const bool visited_v = bfs.node_was_visited(v);

	if (visited_u and visited_v) { return; }
	if (not visited_u and visited_v) {
		bfs.set_visited(u, 1);
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

	// remaining cases:
	// visited_u and not visited_v
	// not visited_u and not visited_v

	if (label[v] == 0) {
		label[v] = ++max_label;
#if defined DEBUG
		assert(max_label <= n);
#endif

		// initialize the path
		p.init(n);
		// set the first non-internal vertex and add it
		p.add_node(u);
		p.set_h1(u);

		// expand the new path
		bfs.set_visited(u, 1);
		bfs.start_at(v);

		// find the lowest *internal* vertex in lexicographic order
		const auto& seq = p.get_vertex_sequence();
		node lowest_lexicographic = n + 1;
		for (std::size_t i = 1; i < seq.size() - 1; ++i) {
			lowest_lexicographic = std::min(lowest_lexicographic, seq[i]);
		}
		p.set_lowest_lexicographic(lowest_lexicographic);

		// push the new path
		res.push_back(std::move(p));
	}
}

/**
 * @brief Finds all branchless paths in a tree.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @returns The list of all branchless paths.
 */
template <class tree_t>
std::vector<properties::branchless_path>
find_all_branchless_paths(const tree_t& t)
noexcept
{
	const uint64_t n = t.get_num_nodes();

	// result of the function (to be returned)
	std::vector<properties::branchless_path> res;

	// label of each internal vertex
	detail::data_array<std::size_t> label(n, 0);
	std::size_t max_label = 0;

	// path to be filled
	properties::branchless_path p;

	detail::BFS bfs(t);

	// detect the last hub
	bfs.set_process_current(
		[&](const auto&, node u) {
			const auto d = t.get_degree(u);
			if (d == 1 or d > 2) {
				// The exploration will finish in the
				// next call to the 'terminate' function.
				p.add_node(u);
				p.set_h2(u);
			}
		}
	);
	// stop the traversal as soon as we find a
	// vertex of degree different from 2
	bfs.set_terminate(
		[&](const auto&, node u) { return t.get_degree(u) != 2; }
	);
	// propagate the labels
	bfs.set_process_neighbour(
		[&](const auto&, node u, node v, bool) {
			label[v] = label[u];
			p.add_node(u);
		}
	);

	// find all paths starting at vertices of degree != 2
	for (node u = 0; u < n; ++u) {
		if (t.get_degree(u) == 2) { continue; }

		if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
			for (node v : t.get_neighbours(u)) {
				expand_branchless_path(t, u, v, bfs, label, max_label, res, p);
			}
		}
		else if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
			for (node v : t.get_out_neighbours(u)) {
				expand_branchless_path(t, u, v, bfs, label, max_label, res, p);
			}
			for (node v : t.get_in_neighbours(u)) {
				expand_branchless_path(t, u, v, bfs, label, max_label, res, p);
			}
		}
	}

	return res;
}

} // -- namespace detail
} // -- namespace lal
