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
#include <cstdlib>

namespace lal {
namespace linarr {

/**
 * @brief The different types of syntactic dependency tree structures.
 *
 * Any tree with its nodes linearly arranged can be classified into several
 * different classes.
 */
enum class syntactic_dependency_tree_type {
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
	 * A structure is planar if none of its dependencies cross. Two dependencies
	 * \f$(s,t), (u,v)\f$ cross if, and only if, their positions in the arrangement
	 * are interleaved, i.e., if \f$\pi(s) < \pi(u) < \pi(t) < \pi(v)\f$, assuming
	 * that \f$s\f$ precedes \f$t\f$ and \f$u\f$ precedes \f$v\f$ in the arrangement.
	 */
	planar,
	/**
	 * @brief Projective structures.
	 *
	 * A structure is projective if it is
	 * @ref lal::linarr::syntactic_dependency_tree_type::planar
	 * and the root is not covered by any dependency.
	 */
	projective,
	/**
	 * @brief Well nested trees with maximum gap-degree 1.
	 *
	 * All trees classified (by this library) as \f$WG_1\f$ are not in \f$WG_0\f$
	 * (notice that \f$WG_0\f$ is the class of projective trees).
	 *
	 * For further details and thorough definitions of \f$WG_k\f$, see \cite Gomez2011a.
	 */
	WG1,
	// This value must always be the last one.
	/// The structure could not be classified.
	unknown
};

/// Number of elements within enumeration @ref syntactic_dependency_tree_type.
constexpr inline std::size_t _syntactic_dependency_tree_size =
	1 + static_cast<std::size_t>(
			lal::linarr::syntactic_dependency_tree_type::unknown
		);

} // namespace linarr
} // namespace lal
