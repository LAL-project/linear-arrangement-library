/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

namespace lal {
namespace linarr {

/**
 * @brief The different types of syntactic dependency tree structures.
 *
 * Any tree with its nodes linearly arranged can be classified into several
 * different classes.
 *
 * We can currently identify the following structures:
 * - Projective structures (see @ref syntactic_dependency_structure::projective),
 * - Planar structures (see @ref syntactic_dependency_structure::planar),
 * - Well nested trees with maximum degree gap 1 (see @ref syntactic_dependency_structure::WG1),
 * - 1-Endpoint Crossing (see @ref syntactic_dependency_structure::EC1),
 */
enum class syntactic_dependency_structure {
	/**
	 * @brief 1-Endpoint Crossing.
	 *
	 * A structure is 1-endpoint crossing if, given any dependency, all other
	 * dependencies crossing it are incident to a common node. See
	 * \cite Pitler2013a for further details.
	 */
	EC1,
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
	/**
	 * @brief Projective structures.
	 *
	 * A structure is projective if it is @ref syntactic_dependency_structure::planar
	 * and the root is not covered by any dependency.
	 */
	projective,
	/**
	 * @brief Well nested trees with maximum gap-degree 1.
	 *
	 * For further details and a thorough discussion, see \cite Gomez2011a.
	 */
	WG1,
	// This value must always be the last one.
	/// The structure could not be classified.
	unknown
};

// *DEVELOPER NOTE*
// Swig does not like multilines for this declaration! Use only one line!
/// Number of elements within enumeration @ref syntactic_dependency_structure.
inline constexpr std::size_t __syntactic_dependency_structure_size =
	1 + static_cast<std::size_t>(lal::linarr::syntactic_dependency_structure::unknown);

} // -- namespace linarr
} // -- namespace lal
