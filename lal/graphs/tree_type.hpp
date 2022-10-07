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
namespace graphs {

/**
 * @brief Enumeration of several types of trees.
 *
 * Classifies a tree into not necessarily disjoint classes of trees.
 *
 * The description of each class can be found in @ref LAL_concepts__tree_types.
 */
enum class tree_type {
	/// Empty tree./
	empty,

	/// Singleton tree.
	singleton,

	/// Caterpillar trees.
	caterpillar,

	/// Linear trees.
	linear,

	/// Spider trees.
	spider,

	/// 2-linear trees.
	two_linear,

	/// Star trees.
	star,

	/// Quasi star trees.
	quasistar,

	/// Bi-star trees.
	bistar,

	// *DEVELOPER NOTE*
	// This value must always be the last one.
	/// The tree could not be classified
	unknown
};

/// Number of elements within enumeration @ref lal::graphs::tree_type.
inline constexpr std::size_t __tree_type_size =
	1 + static_cast<std::size_t>(tree_type::unknown);

} // -- namespace graphs
} // -- namespace lal
