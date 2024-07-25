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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
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
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <string_view>
#endif
#include <cstdint>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

/**
 * @brief The many different reasons to not assign a vertex to the arrangement.
 *
 * This is used in @ref lal::detail::DMax::unconstrained::AEF_BnB before deciding
 * whether or not a vertex should be arranged at the next empty position.
 */
enum class reason_discard : int8_t {
	/// No reason to discard. Use the vertex.
	none,
	/// Placing the vertex will produce a bipartite arrangement
	will_produce_bipartite_arrangement,

	/* -- paths -- */
	/// Placing this vertex will produce a thistle located at an antenna.
	node_of_antenna_as_thistle,
	/**
	 * This vertex wants to be a thistle (and could be one) but it is not
	 * the lowest vertex (in the lexicographic order) among the vertices of
	 * the bridge.
	 */
	thistle_in_bridge_is_not_the_lowest,
	/**
	 * A hub vertex has a level value that does not allow an optimal placement
	 * of some of the antennas incident to it.
	 */
	hub_disallows_placement_of_antennas,

	/* -- level values -- */
	/// The level prediction made will not be met.
	placement_is_in_conflict_with_level_prediction,
	/**
	 * If the vertex is placed then the level signature will not be
	 * non-increasing -- breaks Nurse's properties.
	 */
	level_signature_will_not_be_nonincreasing,
	/**
	 * If the vertex is placed then none of the vertices (of degree <= 2) of
	 * some path will be allowed to be placed in the arrangement.
	 */
	missing_entire_path,
	/**
	 * If the vertex is placed then some leaf will be misplaced (thus breaking
	 * the non-increasing order of level values).
	 */
	missing_degree1,
	/**
	 * If the vertex is placed then some degree-2 vertex will be misplaced
	 * (with level value +2), thus breaking the non-increasing order of level
	 * values.
	 */
	missing_degree2_lp2,
	/**
	 * If the vertex is placed then some degree-2 vertex will be misplaced
	 * (with level value -2), thus breaking the non-increasing order of level
	 * values.
	 */
	missing_degree2_lm2,
	/**
	 * If the vertex is placed then it will have the same level value as
	 * one of its neighbor vertices (in the graph) -- breaks Nurse's properties.
	 */
	adjacent_vertices_with_equal_level_value,
	/**
	 * Placing this vertex prevents the construction of a maximum arrangement
	 * since the placement of its remaining neighbors will not satisfy
	 * (1) non-increasing level sequence,
	 * (2) neighbors may have the same level value.
	 */
	node_disallows_placement_of_neighbors,
	/**
	 * Placing this vertex (of |level|=2) will surely fail level value
	 * propagation, thus eventually breaking one of Nurse's properties.
	 */
	placement_fails_level_propagation,

	/* -- optimality -- */
	/// The largest cut is below the lower bound for the maximum cut value.
	largest_cut_below_minimum,

	/* -- symmetry breaking -- */
	/**
	 * The vertices in the same level value interval in the arrangement are
	 * not sorted by lexicographic order.
	 */
	nodes_of_equal_level_disobey_lexicographic_order,
	/**
	 * The leaves attached to the same vertex are not arranged so that they
	 * appear (from left to right) in the lexicographic order.
	 */
	node_leaves_disobey_lexicographic_order,
	/**
	 * The vertices that are root of isomorphic subtrees are not arranged so
	 * that they appear (from left to right) in the lexicographic order.
	 */
	roots_of_isomorphic_subtrees_disobey_lexicographic_order,

	// Simply to get the amount of items in this enum safely.
	/// Null value
	__last_item
};

#if defined __LAL_DEBUG_DMax_Unc_BnB
/// Converts a value of @ref lal::detail::DMax::unconstrained::reason_discard
/// to a long string.
[[nodiscard]] static constexpr std::string_view reason_discard_to_string
(const reason_discard at) noexcept
{
	switch (at) {
	case reason_discard::none:
		return "Success";
	case reason_discard::will_produce_bipartite_arrangement:
		return "Placing this vertex will produce a bipartite arrangement";

	/* -- paths -- */
	case reason_discard::node_of_antenna_as_thistle:
		return "Vertex of antenna as thistle";
	case reason_discard::thistle_in_bridge_is_not_the_lowest:
		return "The vertex is not adequate to be thistle in its bridge";
	case reason_discard::hub_disallows_placement_of_antennas:
		return "The level of the hub of an antenna is not Ok for the placement of the antenna";
	case reason_discard::placement_fails_level_propagation:
		return "Placing this vertex will eventually prevent the placement of some other vertex in the same path";

	/* -- level values -- */
	case reason_discard::placement_is_in_conflict_with_level_prediction:
		return "Conflict in the prediction of level value";
	case reason_discard::level_signature_will_not_be_nonincreasing:
		return "The level signature will not be non-increasing";
	case reason_discard::missing_entire_path:
		return "None of the vertices of a path (degree <= 2) has been assigned";
	case reason_discard::missing_degree1:
		return "Some leaf will be misplaced";
	case reason_discard::missing_degree2_lp2:
		return "Some degree-2 vertex of a path (of level +2) will be misplaced";
	case reason_discard::missing_degree2_lm2:
		return "Some degree-2 vertex of a path (of level -2) will be misplaced";
	case reason_discard::adjacent_vertices_with_equal_level_value:
		return "There will be connected vertices (by an edge) with the same level value";
	case reason_discard::node_disallows_placement_of_neighbors:
		return "Placing will eventually lead to breaking two of Nurse's properties";

	/* -- optimality -- */
	case reason_discard::largest_cut_below_minimum:
		return "The largest cut is not large enough";

	/* -- symmetry breaking -- */
	case reason_discard::nodes_of_equal_level_disobey_lexicographic_order:
		return "Vertices of equal level value are not arranged in lexicographic order";
	case reason_discard::node_leaves_disobey_lexicographic_order:
		return "Leaves attached to the same vertex are not arranged in lexicographic order";
	case reason_discard::roots_of_isomorphic_subtrees_disobey_lexicographic_order:
		return "Roots of isomorphic subtrees are not arranged in lexicographic order";

	/* -- */
	case reason_discard::__last_item:
		return "?";
	}
	return "";
}

/// Converts a value of @ref lal::detail::DMax::unconstrained::reason_discard
/// to an index value.
[[nodiscard]] static inline constexpr std::size_t reason_discard_to_index
(reason_discard rd) noexcept
{ return static_cast<std::size_t>(rd); }

/// Converts an index value to a value of
/// @ref lal::detail::DMax::unconstrained::reason_discard.
[[nodiscard]] static inline constexpr reason_discard index_to_reason_discard
(std::size_t i) noexcept
{ return static_cast<reason_discard>(i); }
#endif

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
