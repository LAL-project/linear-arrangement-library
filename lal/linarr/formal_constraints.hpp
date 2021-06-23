/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <numeric>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/C.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/data_array.hpp>

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
inline bool is_permutation(const linear_arrangement& arr = {}) noexcept {
	if (arr.size() == 0) { return true; }
	// ensure that no position has been used twice
	internal::data_array<position> d(arr.size(), 0);
	for (const position p : arr) {
		if (p >= arr.size()) { return false; }
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
template<class G>
inline bool is_arrangement(const G& g, const linear_arrangement& arr) noexcept
{
	if constexpr (std::is_base_of_v<G, graphs::tree>) {
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
	const position max_pos = *std::max_element(arr.begin(), arr.end());
	return max_pos == arr.size() - 1;
}

/**
 * @brief Is a given arrangement planar?
 *
 * A planar arrangement of a graph is an arrangement in which there are no
 * edge crossings. If the input arrangement is empty then the identity
 * arrangement \f$\pi_I\f$ is used.
 *
 * @param g Input graph.
 * @param arr Input linear arrangement.
 * @returns Whether or not the input graph arranged with the input arrangement
 * is planar.
 */
template<class G>
inline bool is_planar(const G& g, const linear_arrangement& arr = {}) noexcept {
#if defined DEBUG
	assert(is_arrangement(g, arr));
#endif

	const uint64_t invalid = g.get_num_edges()*g.get_num_edges() + 1;
	return is_num_crossings_lesseq_than(g, arr, 0) < invalid;
}

/**
 * @brief Is a given arrangement projective?
 *
 * A projective arrangement of a rooted tree is an arrangement that is planar
 * and the root is not covered by any edge. The root is covered if, for a given
 * input arrangement \f$\pi\f$, there exists an edge of the tree \f$\{s,t\}\f$
 * such that \f$\pi(s) < \pi(r) < \pi(t)\f$ or \f$\pi(t) < \pi(r) < \pi(s)\f$.
 *
 * If the input arrangement is empty then the identity arrangement \f$\pi_I\f$
 * is used.
 *
 * See method @ref is_planar for further details on the characterisation of planar
 * arrangements.
 * @param rt Input rooted tree
 * @param arr Input linear arrangement.
 * @returns Whether or not the input rooted tree arranged with the input arrangement
 * is projective.
 * @pre The input rooted tree must be a valid rooted tree
 * (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
inline bool is_projective
(const graphs::rooted_tree& rt, const linear_arrangement& arr = {})
noexcept
{
#if defined DEBUG
	assert(rt.is_rooted_tree());
#endif

	// check for planarity
	// this function already checks that an arrangement must be valid
	if (not is_planar(rt, arr)) { return false; }

	if (arr.size() == 0) {
		const node r = rt.get_root();
		iterators::E_iterator e_it(rt);
		while (not e_it.end()) {
			const auto [s,t] = e_it.get_edge();
			const bool r_covered_st = s < r and r < t;
			const bool r_covered_ts = t < r and r < s;
			if (r_covered_st or r_covered_ts) { return false; }
			e_it.next();
		}
		return true;
	}

	const node r = rt.get_root();
	iterators::E_iterator e_it(rt);
	while (not e_it.end()) {
		const auto [s,t] = e_it.get_edge();
		const bool r_covered_st = arr[s] < arr[r] and arr[r] < arr[t];
		const bool r_covered_ts = arr[t] < arr[r] and arr[r] < arr[s];
		if (r_covered_st or r_covered_ts) { return false; }
		e_it.next();
	}
	return true;
}

} // -- namespace linarr
} // -- namespace lal
