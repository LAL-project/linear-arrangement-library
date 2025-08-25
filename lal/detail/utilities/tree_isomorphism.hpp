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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/properties/tree_centre.hpp>
#include <lal/detail/utilities/tree_isomorphism_tuple_small.hpp>
#include <lal/detail/utilities/tree_isomorphism_tuple_large.hpp>
#include <lal/detail/utilities/tree_isomorphism_string.hpp>
#include <lal/detail/utilities/tree_isomorphism_fast_noniso.hpp>

namespace lal {
namespace detail {
namespace isomorphism {

/// The different isomorphism algorithms implemented.
enum class algorithm {
	/**
	 * @brief A string-based algorithm.
	 *
	 * This algorithm builds an identifier string for each vertex over the
	 * alphabet {0, 1}. The size of the strings is proportional to the size
	 * of the subtree rooted at a specific vertex.
	 *
	 * This is based on \cite Aho1974a.
	 *
	 * See function @ref lal::detail::are_rooted_trees_isomorphic_string.
	 */
	string,
	/**
	 * @brief A tuple-based algorithm.
	 *
	 * This implementation uses tuples to build an identifier of every vertex.
	 * The tuple is constructed using integer values, which can be safely
	 * assumed to be under 2^64. This algorithm is more memory friendly.
	 *
	 * This algorithm implements the algorithm described in \cite Aho1974a.
	 * The implementation is more suitable for small tress.
	 *
	 * See function @ref lal::detail::are_rooted_trees_isomorphic_tuple.
	 */
	tuple_small,
	/**
	 * @brief A tuple-based algorithm.
	 *
	 * This implementation uses tuples to build an identifier of every vertex.
	 * The tuple is constructed using integer values, which can be safely
	 * assumed to be under 2^64. This algorithm is more memory friendly.
	 *
	 * This algorithm implements the algorithm described in \cite Aho1974a.
	 * The implementation is more suitable for large tress.
	 *
	 * See function @ref lal::detail::are_rooted_trees_isomorphic_tuple.
	 */
	tuple_large
};

/**
 * @brief Calls one of the isomorphism algorithms.
 * @tparam algo The algorithm to call.
 * @param T1 Input rooted tree as a @ref lal::graphs::free_tree object.
 * @param r1 Root of the first tree.
 * @param T2 Input rooted tree as a @ref lal::graphs::free_tree object.
 * @param r2 Root of the second tree.
 * @returns Whether or not the rooted trees are isomorphic.
 */
template <isomorphism::algorithm algo>
[[nodiscard]] inline bool iso_func(
	const graphs::free_tree& t1,
	const node r1,
	const graphs::free_tree& t2,
	const node r2
) noexcept
{
	if constexpr (algo == isomorphism::algorithm::string) {
		return are_rooted_trees_isomorphic_string(t1, r1, t2, r2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_small) {
		return are_rooted_trees_isomorphic_tuple_small(t1, r1, t2, r2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_large) {
		return are_rooted_trees_isomorphic_tuple_large(t1, r1, t2, r2);
	}
	else {
		static_assert(false);
	}
}

} // namespace isomorphism

/**
 * @brief Calls one of the isomorphism algorithms.
 * @tparam algo The algorithm to call.
 * @tparam check_fast_noniso Use a fast algorithm to discard isomorphism.
 * @param t1 Input rooted tree.
 * @param t2 Input rooted tree.
 * @returns Whether or not the rooted trees are isomorphic.
 */
template <isomorphism::algorithm algo, bool check_fast_noniso = true>
[[nodiscard]] inline bool are_trees_isomorphic(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{
#if defined DEBUG
	assert(t1.is_rooted_tree());
	assert(t2.is_rooted_tree());
#endif

	if constexpr (check_fast_noniso) {
		const auto discard = fast_non_iso(t1, t2);
		if (discard == 0) {
			return true;
		}
		if (discard == 1) {
			return false;
		}
	}

	if constexpr (algo == isomorphism::algorithm::string) {
		return are_rooted_trees_isomorphic_string(t1, t2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_small) {
		return are_rooted_trees_isomorphic_tuple_small(t1, t2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_large) {
		return are_rooted_trees_isomorphic_tuple_large(t1, t2);
	}
	else {
		static_assert(false);
	}
}

/**
 * @brief Calls one of the isomorphism algorithms.
 * @tparam algo The algorithm to call.
 * @tparam check_fast_noniso Use a fast algorithm to discard isomorphism.
 * @param t1 Input rooted tree as a @ref lal::graphs::free_tree object.
 * @param r1 Root of the first tree.
 * @param t2 Input rooted tree as a @ref lal::graphs::free_tree object.
 * @param r2 Root of the second tree.
 * @returns Whether or not the rooted trees are isomorphic.
 */
template <isomorphism::algorithm algo, bool check_fast_noniso = true>
[[nodiscard]] inline bool are_trees_isomorphic(
	const graphs::free_tree& t1,
	const node r1,
	const graphs::free_tree& t2,
	const node r2
) noexcept
{
#if defined DEBUG
	assert(t1.is_tree());
	assert(t2.is_tree());
#endif

	if constexpr (check_fast_noniso) {
		const auto discard = fast_non_iso(t1, t2);
		if (discard == 0) {
			return true;
		}
		if (discard == 1) {
			return false;
		}
	}

	if constexpr (algo == isomorphism::algorithm::string) {
		return are_rooted_trees_isomorphic_string(t1, r1, t2, r2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_small) {
		return are_rooted_trees_isomorphic_tuple_small(t1, r1, t2, r2);
	}
	else if constexpr (algo == isomorphism::algorithm::tuple_large) {
		return are_rooted_trees_isomorphic_tuple_large(t1, r1, t2, r2);
	}
	else {
		static_assert(false);
	}
}

/**
 * @brief Calls one of the isomorphism algorithms.
 * @tparam algo The algorithm to call.
 * @tparam check_fast_noniso Use a fast algorithm to discard isomorphism.
 * @param t1 Input free tree.
 * @param t2 Input free tree.
 * @returns Whether or not the rooted trees are isomorphic.
 */
template <isomorphism::algorithm algo, bool check_fast_noniso = true>
[[nodiscard]] inline bool are_trees_isomorphic(
	const graphs::free_tree& t1, const graphs::free_tree& t2
) noexcept
{
#if defined DEBUG
	assert(t1.is_tree());
	assert(t2.is_tree());
#endif

	if constexpr (check_fast_noniso) {
		const auto discard = fast_non_iso(t1, t2);
		if (discard == 0) {
			return true;
		}
		if (discard == 1) {
			return false;
		}
	}

	const uint64_t n = t1.get_num_nodes();
	if (n <= 3) {
		return true;
	}

	// find centres of the trees
	const auto c1 = retrieve_centre(t1, 0);
	const auto c2 = retrieve_centre(t2, 0);

	// check centre sizes
	const uint64_t size1 = (c1.second < n ? 2 : 1);
	const uint64_t size2 = (c2.second < n ? 2 : 1);
	if (size1 != size2) {
		return false;
	}

	// the centres have only one vertex
	if (size1 == 1) {
		return isomorphism::iso_func<algo>(t1, c1.first, t2, c2.first);
	}

	// the centres have two vertices

	// try with the first centre of the second tree
	const bool iso1 = isomorphism::iso_func<algo>(t1, c1.first, t2, c2.first);
	if (iso1) {
		return true;
	}

	// try with the second centre of the second tree
	return isomorphism::iso_func<algo>(t1, c1.first, t2, c2.second);
}

} // namespace detail
} // namespace lal
