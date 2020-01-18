/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <string>

namespace lal {
namespace linarr {

/**
 * @brief The different types of syntactic dependency tree structures.
 *
 * Given a tree (not necessarily directed) and a linear arrangement of
 * its vertices, we can classify the tree in several different classes.
 *
 * We can currently identify the following structures:
 * - Projective structures (see @ref tree_structure_type::projective). A
 * structure is projective if it is planar and the root is not covered by any
 * dependency.
 * - Planar structures (see @ref tree_structure_type::planar). A structure is
 * planar if none of its dependencies cross. Two dependencies \f$(s,t), (u,v)\f$
 * cross if, and only if, their positions in the arrangement are interleaved,
 * i.e., if \f$\pi(s) < \pi(u) < \pi(t) < \pi(v)\f$, assuming that \f$s\f$
 * precedes \f$t\f$ and \f$u\f$ precedes \f$v\f$ in the arrangement.
 * - Well nested trees with maximum degree gap \f$k\f$, usually denoted as
 * \f$WG_k\f$. We support:
 *		- \f$WG_i\f$ (for \f$1\le i \le 10\f$, see
 *		@ref tree_structure_type::WG_1, ..., @ref tree_structure_type::WG_10).
 *		- \f$WG_k\f$ (for \f$k > 10\f$, see @ref tree_structure_type::WG_k).
 * - 1-Endpoint Crossing (see @ref tree_structure_type::EC_1). A structure has
 * the property of being 1-endpoint crossing if, given any dependency, all
 * other dependencies crossing it are incident to a common node.
 */
enum class tree_structure_type {
	// Projective structures

	/// Projective structures.
	projective,
	/// Planar structures.
	planar,

	// Mildly non-projective

	/// Well nested trees with maximum degree gap 1.
	WG_1,
	/// Well nested trees with maximum degree gap 2.
	WG_2,
	/// Well nested trees with maximum degree gap 3.
	WG_3,
	/// Well nested trees with maximum degree gap 4.
	WG_4,
	/// Well nested trees with maximum degree gap 5.
	WG_5,
	/// Well nested trees with maximum degree gap 6.
	WG_6,
	/// Well nested trees with maximum degree gap 7.
	WG_7,
	/// Well nested trees with maximum degree gap 8.
	WG_8,
	/// Well nested trees with maximum degree gap 9.
	WG_9,
	/// Well nested trees with maximum degree gap 10.
	WG_10,
	/// Well nested trees with maximum degree gap \f$k>10\f$.
	WG_k,

	/// 1-Endpoint Crossing.
	EC_1,

	/// The structure could not be classified.
	none
};

} // -- namespace linarr
} // -- namespace lal
