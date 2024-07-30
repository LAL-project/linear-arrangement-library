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

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace detail {

/**
 * @brief Is the root of a rooted tree covered in a given arrangement?
 *
 * See @ref LAL_concepts__linear_arrangement__properties for details on vertex
 * covering in an arrangement.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @tparam arrangement_t Type of arrangement.
 * @param rt Input rooted tree
 * @param arr Input linear arrangement.
 * @returns Whether or not the root is covered in the given arrangement.
 * @pre The input rooted tree must be a valid rooted tree
 * (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
template <class arrangement_t>
[[nodiscard]] bool is_root_covered
(const graphs::rooted_tree& rt, const arrangement_t& arr)
noexcept
{
#if defined DEBUG
	assert(rt.is_rooted_tree());
#endif

	// the linear arrangement is given
	const position pr = arr[node_t{rt.get_root()}];

	iterators::E_iterator e_it(rt);
	while (not e_it.end()) {
		const auto [s,t] = e_it.yield_edge_t();
		const position ps = arr[s];
		const position pt = arr[t];

		const bool r_covered_st = ps < pr and pr < pt;
		const bool r_covered_ts = pt < pr and pr < ps;
		if (r_covered_st or r_covered_ts) { return true; }
	}
	return false;
}

/**
 * @brief Is a given arrangement projective?
 *
 * See @ref LAL_concepts__linear_arrangement__types for details on projective
 * arrangements.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @tparam arrangement_t Type of arrangement.
 * @param rt Input rooted tree
 * @param arr Input linear arrangement.
 * @returns Whether or not the input rooted tree arranged with the input arrangement
 * is projective.
 * @pre The input rooted tree must be a valid rooted tree
 * (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
template <class arrangement_t>
[[nodiscard]] bool is_projective
(const graphs::rooted_tree& rt, const arrangement_t& arr)
noexcept
{
#if defined DEBUG
	assert(rt.is_rooted_tree());
#endif

	// check for planarity
	// this function already checks that an arrangement must be valid
	if (not is_planar(rt, arr)) { return false; }
	return not is_root_covered(rt, arr);
}

/**
 * @brief Is a given arrangement bipartite?
 *
 * See @ref LAL_concepts__linear_arrangement__types for details on bipartite
 * arrangements.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @tparam arrangement_t Type of arrangement.
 * @param c Coloring of a bipartite graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangement is bipartite.
 * @pre Input @e arr is an arrangement of a connected bipartite graph.
 */
template <class arrangement_t>
[[nodiscard]] bool is_bipartite__connected
(const properties::bipartite_graph_coloring& c, const arrangement_t& arr)
noexcept
{
	const auto n = c.size();
	int num_changes = 0;
	position_t p = 1ull;
	while (p < n and num_changes <= 1) {
		const node u = arr[p - 1ull];
		const auto color_u = c.get_color_of(u);
		const node v = arr[p];
		const auto color_v = c.get_color_of(v);
		num_changes += color_v != color_u;
		++p;
	}
	return num_changes <= 1;
}

/**
 * @brief Is a given arrangement bipartite?
 *
 * See @ref LAL_concepts__linear_arrangement__types for details on bipartite
 * arrangements.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangement is bipartite.
 */
template <class graph_t, class arrangement_t>
[[nodiscard]] bool is_bipartite(const graph_t& g, const arrangement_t& arr)
noexcept
{
	typedef properties::bipartite_graph_coloring::color_t color_t;
	static constexpr color_t blue = properties::bipartite_graph_coloring::blue;
	static constexpr color_t red = properties::bipartite_graph_coloring::red;
	static constexpr color_t invalid = properties::bipartite_graph_coloring::invalid_color;

	const auto n = g.get_num_nodes();
	array<color_t> color_per_vertex(n, invalid);

	const auto color_a_vertex = [&](node u) {
		color_per_vertex[u] = blue;
		if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
			for (node v : g.get_out_neighbors(u)) { color_per_vertex[v] = red; }
			for (node v : g.get_in_neighbors(u)) { color_per_vertex[v] = red; }
		}
		else {
			for (node v : g.get_neighbors(u)) { color_per_vertex[v] = red; }
		}
	};

	color_a_vertex( arr[position_t{0ull}] );

	int num_changes = 0;
	position_t p = 1ull;
	while (p < n and num_changes <= 1) {
		const node u = arr[p];
		if (color_per_vertex[u] == invalid) {
			color_a_vertex(u);
		}
		const node v = arr[p - 1ull];
		num_changes += color_per_vertex[v] != color_per_vertex[u];
		++p;
	}
	return num_changes <= 1;
}

} // -- namespace detail
} // -- namespace lal
