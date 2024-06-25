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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <array>

// lal includes
#include <lal/graphs/tree_type.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Classify a tree into one of the types @ref lal::graphs::tree_type.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param[out] tree_types A set of bits (or flags) each indicating whether or not @e t
 * is of a certain tree type.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
void classify_tree(const tree_t& t, std::array<bool, graphs::__tree_type_size>& tree_types)
noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif
	// -------------------------------------------------------------------------
	// utilities

	bool is_some = false; // the type of the tree is different from 'unknown'

	const auto set_type =
	[&](const graphs::tree_type& tt) {
		tree_types[static_cast<std::size_t>(tt)] = true;
		is_some = true;
	};

	// only neighbour of a vertex of a tree in its underlying UNDIRECTED structure
	const auto get_only_neighbour =
	[&](lal::node u) -> lal::node {
		if constexpr (std::is_base_of_v<lal::graphs::free_tree, tree_t>) {
			return t.get_neighbors(u)[0];
		}
		else {
			return (t.get_out_degree(u) == 0 ?
				t.get_in_neighbors(u)[0] :
				t.get_out_neighbors(u)[0]
			);
		}
	};

	// -------------------------------------------------------------------------

	// number of vertices
	const uint64_t n = t.get_num_nodes();
	if (n == 0) {
		set_type(graphs::tree_type::empty);
		tree_types[static_cast<std::size_t>(graphs::tree_type::unknown)] = false;
		return;
	}
	if (n == 1) {
		set_type(graphs::tree_type::singleton);
		set_type(graphs::tree_type::caterpillar);
		tree_types[static_cast<std::size_t>(graphs::tree_type::unknown)] = false;
		return;
	}
	if (n == 2) {
		set_type(graphs::tree_type::linear);
		set_type(graphs::tree_type::star);
		set_type(graphs::tree_type::bistar);
		set_type(graphs::tree_type::caterpillar);
		tree_types[static_cast<std::size_t>(graphs::tree_type::unknown)] = false;
		return;
	}
	if (n == 3) {
		set_type(graphs::tree_type::linear);
		set_type(graphs::tree_type::star);
		set_type(graphs::tree_type::bistar);
		set_type(graphs::tree_type::caterpillar);
		tree_types[static_cast<std::size_t>(graphs::tree_type::unknown)] = false;
		return;
	}

	// N >= 4

	bool is_linear = false;
	bool is_star = false;
	bool is_quasistar = false;
	bool is_bistar = false;
	bool is_caterpillar = false;
	bool is_spider = false;
	bool is_two_linear = false;

	// number of vertices
	uint64_t n_deg_eq_1 = 0; // of degree = 1
	uint64_t n_deg_eq_2 = 0; // of degree = 2
	uint64_t n_deg_ge_2 = 0; // of degree >= 2
	uint64_t n_deg_ge_3 = 0; // of degree >= 3

	// degree of the internal vertices
	array<int64_t> deg_internal(n, 0);

	// fill in data
	for (lal::node u = 0; u < n; ++u) {
		// 'du' is the degree of the vertex in the underlying undirected graph
		const int64_t du = static_cast<int64_t>(t.get_degree(u));
		deg_internal[u] += (du > 1)*du;

		n_deg_eq_1 += du == 1;
		n_deg_eq_2 += du == 2;
		n_deg_ge_2 += du > 1;
		n_deg_ge_3 += du > 2;

		// this reduces the degree of the internal vertices
		// as many times as leaves are connected to them
		if (du == 1) {
			deg_internal[ get_only_neighbour(u) ] -= 1;
		}
	}

	// LINEAR
	if (n_deg_eq_1 == 2) {
		// if there are only two leaves then we must
		// have that the remaining vertices are of degree 2.
#if defined DEBUG
		assert(n_deg_ge_2 == n - 2);
#endif
		is_linear = true;
		is_caterpillar = true;
	}

	// BISTAR
	if (n_deg_ge_2 == 2 and n - n_deg_ge_2 == n_deg_eq_1) {
		is_bistar = true;
		is_caterpillar = true;
	}

	// QUASISTAR
	if (n - n_deg_ge_2 == n_deg_eq_1 and
		(
		(n_deg_eq_2 == 2 and n_deg_ge_3 == 0) or
		(n_deg_ge_3 == 1 and n_deg_eq_2 == 1)
		)
	)
	{
		is_quasistar = true;
		is_caterpillar = true;
	}

	// STAR
	if (n_deg_ge_2 == 1 and n_deg_eq_1 == n - 1) {
		is_star = true;
		is_caterpillar = true;
	}

	// SPIDER
	if (n_deg_ge_3 == 1 and n_deg_eq_1 + n_deg_eq_2 == n - 1) {
		is_spider = true;
	}

	// 2-LINEAR
	if (n_deg_ge_3 == 2 and n_deg_eq_1 + n_deg_eq_2 == n - 2) {
		is_two_linear  = true;
	}

	if (not is_caterpillar) {
		// If we are left with 2, or 0, vertices with degree 1,
		// it means that after removing the leaves of the tree
		// all vertices are internal (degree 2), i.e., they are
		// part of a linear tree. Needless to say that these
		// two vertices of degree 1 are the endpoints of the
		// linear tree.
		uint64_t n1 = 0;
		for (lal::node u = 0; u < n; ++u) {
			n1 += deg_internal[u] == 1;
		}

		is_caterpillar = n1 == 2 or n1 == 0;
	}

	if (is_linear) { set_type(graphs::tree_type::linear); }
	if (is_star) { set_type(graphs::tree_type::star); }
	if (is_quasistar) { set_type(graphs::tree_type::quasistar); }
	if (is_bistar) { set_type(graphs::tree_type::bistar); }
	if (is_caterpillar) { set_type(graphs::tree_type::caterpillar); }
	if (is_spider) { set_type(graphs::tree_type::spider); }
	if (is_two_linear) { set_type(graphs::tree_type::two_linear); }

	if (is_some) {
		tree_types[static_cast<std::size_t>(graphs::tree_type::unknown)] = false;
	}
}

} // -- namespace detail
} // -- namespace lal
