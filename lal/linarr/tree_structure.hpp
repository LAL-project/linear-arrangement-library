/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <cinttypes>
#include <string>

namespace lal {
namespace linarr {

/**
 * @brief The different types of syntactic dependency tree structures.
 *
 * Any tree can be classified when its nodes are linearly arranged into
 * several different classes.
 *
 * We can currently identify the following structures:
 * - Projective structures (see @ref tree_structure::projective),
 * - Planar structures (see @ref tree_structure::planar),
 * - Well nested trees with maximum degree gap 1 (see @ref tree_structure::WG1),
 * - 1-Endpoint Crossing (see @ref tree_structure::EC1),
 * - Multi-headed 4 (see @ref tree_structure::MH4),
 * - Multi-headed 5 (see @ref tree_structure::MH5),
 */
enum class tree_structure {
	// Projective structures

	/**
	 * @brief Projective structures.
	 *
	 * A structure is projective if it is planar and the root is not covered by
	 * any dependency.
	 */
	projective,

	/**
	 * @brief Planar structures.
	 *
	 * A structure is
	 * planar if none of its dependencies cross. Two dependencies \f$(s,t), (u,v)\f$
	 * cross if, and only if, their positions in the arrangement are interleaved,
	 * i.e., if \f$\pi(s) < \pi(u) < \pi(t) < \pi(v)\f$, assuming that \f$s\f$
	 * precedes \f$t\f$ and \f$u\f$ precedes \f$v\f$ in the arrangement.
	 */
	planar,

	// Mildly non-projective

	/// Well nested trees with maximum gap-degree 1.
	WG1,

	/**
	 * @brief 1-Endpoint Crossing.
	 *
	 * A structure has the property of being 1-endpoint crossing if, given any
	 * dependency, all other dependencies crossing it are incident to a common
	 * node.
	 */
	EC1,

	/// MH-4
	MH4,

	/// MH-5
	MH5,

	/// The structure could not be classified.
	none
};

inline
std::string tree_structure_to_string(const tree_structure& tt) {
	switch (tt) {
		case tree_structure::projective: return "projective";
		case tree_structure::planar: return "planar";
		case tree_structure::WG1: return "WG1";
		case tree_structure::EC1: return "EC1";
		case tree_structure::MH4: return "MH4";
		case tree_structure::MH5: return "MH5";
		default: return "none";
	}
}

/// Number of elements within enumeration @ref tree_structure.
static const std::size_t __tree_structure_size = 7;

// This assertion ensures that the value of '__tree_structure_size' is correct.
static_assert(
	__tree_structure_size == 1 + static_cast<std::size_t>(tree_structure::none),
"Constant '__tree_structure_size' does not contain the correct number of \
elements within enumeration 'tree_structure'."
);

} // -- namespace linarr
} // -- namespace lal
