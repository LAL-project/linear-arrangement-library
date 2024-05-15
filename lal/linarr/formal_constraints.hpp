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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/C/C.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Is a given input arrangement a permutation?
 *
 * A linear arrangement is a permutation if all the positions are numbers in
 * \f$[0,n-1]\f$, where \f$n\f$ denotes the size of the arrangement and if
 * no two numbers appear twice in the arrangement.
 * @param arr Input linear arrangement
 * @returns Whether or not the input arrangement is a valid permutation.
 */
inline
bool is_permutation(const linear_arrangement& arr = {}) noexcept {
	// identity arrangement is always a permutation
	if (arr.size() == 0) { return true; }
	// an arrangement of a single element is a permutation
	// if its only element is within range
	if (arr.size() == 1) {
		return arr[lal::position_t{0ull}] == 0;
	}
	// ensure that no position has been used twice
	detail::array<char> d(arr.size(), 0);
	for (node_t u = 0ull; u < arr.size(); ++u) {
		const position p = arr[u];
		// ensure all elements are within range
		if (p >= arr.size()) { return false; }
		// if a value already exists, this is not a permutation
		if (d[p] > 0) { return false; }
		d[p] += 1;
	}
	return true;
}

/**
 * @brief Is a given arrangement valid?
 *
 * Checks that an input arrangement is valid for the corresponding input graph.
 * An arrangement is valid if it is a valid permutation of the vertices of the
 * graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns Whether or not the input arrangement is a valid permutation.
 */
template <class graph_t>
bool is_arrangement(const graph_t& g, const linear_arrangement& arr) noexcept
{
	if constexpr (std::is_base_of_v<graph_t, graphs::tree>) {
#if defined DEBUG
		assert(g.is_tree());
#endif
	}

	// identity arrangement is always a permutation
	if (arr.size() == 0) { return true; }
	// if sizes differ then the arrangement is not a permutation
	if (g.get_num_nodes() != arr.size()) { return false; }
	// ensure that the input arrangement is a permutation
	if (not is_permutation(arr)) { return false; }
	// the largest number must be exactly one less than the size
	const position max_pos =
		*std::max_element(arr.begin_direct(), arr.end_direct());

	return max_pos == arr.size() - 1;
}

/**
 * @brief Is a given arrangement bipartite?
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 *
 * @param g Input undirected graph.
 * @param c Coloring of the input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangment of @e g is bipartite.
 * @pre The input graph is bipartite.
 */
bool is_bipartite(
	const graphs::undirected_graph& g,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr = {}
)
noexcept;
/**
 * @brief Is a given arrangement bipartite?
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 *
 * @param g Input directed graph.
 * @param c Coloring of the input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangment of @e g is bipartite.
 * @pre The input graph is bipartite.
 */
bool is_bipartite(
	const graphs::directed_graph& g,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr = {}
)
noexcept;

/**
 * @brief Is a given arrangement bipartite?
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 *
 * @param g Input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangment of @e g is bipartite.
 * @pre The input graph is bipartite.
 */
template <class graph_t>
bool is_bipartite(const graph_t& g, const linear_arrangement& arr = {}) noexcept {
#if defined DEBUG
	assert(is_arrangement(g, arr));
#endif
	const auto c = properties::bipartite_coloring(g);
	return is_bipartite(g, c, arr);
}

/**
 * @brief Is a given arrangement planar?
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of planar
 * arrangement.
 *
 * @param g Input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangment of @e g is planar.
 */
template <class graph_t>
bool is_planar(const graph_t& g, const linear_arrangement& arr = {}) noexcept {
#if defined DEBUG
	assert(is_arrangement(g, arr));
#endif

	return is_num_crossings_lesseq_than(g, arr, 0) <= 0;
}

/**
 * @brief Is the root of a rooted tree covered in a given arrangement?
 *
 * See @ref LAL_concepts__linear_arrangement__properties for the definition of
 * vertex covering.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @param rt Input rooted tree
 * @param arr Input linear arrangement.
 * @returns Whether or not the root is covered in the given arrangement.
 * @pre The input rooted tree must be a valid rooted tree
 * (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
bool is_root_covered(const graphs::rooted_tree& rt, const linear_arrangement& arr)
noexcept;

/**
 * @brief Is a given arrangement projective?
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of projective
 * arrangement.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 * @param rt Input rooted tree
 * @param arr Input linear arrangement.
 * @returns Whether or not the input arrangment of @e rt is projective.
 * @pre The input rooted tree must be a valid rooted tree
 * (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
inline
bool is_projective(const graphs::rooted_tree& rt, const linear_arrangement& arr)
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

} // -- namespace linarr
} // -- namespace lal
