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
namespace graphs {

/**
 * @brief Enumeration of several types of trees.
 *
 * Classifies a tree into not necessarily disjoint classes of trees.
 *
 * The description of each class can be found in @ref LAL_concepts__tree_types.
 */
enum class tree_type {
	/**
	 * @brief Empty tree.
	 *
	 * This is a tree with no vertices.
	 */
	empty,

	/**
	 * @brief Singleton tree.
	 *
	 * This is a tree with a single vertex.
	 */
	singleton,

	/**
	 * @brief Caterpillar trees.
	 *
	 * These are the trees which, after removing their leaves, only a path graph
	 * remains \cite Harary1973a.
	 */
	caterpillar,

	/**
	 * @brief Linear trees.
	 *
	 * Also known as path graphs, a tree where all vertices have degree \f$\le 2\f$.
	 */
	linear,

	/**
	 * @brief Bi-star trees.
	 *
	 * These trees have two vertex hubs, \f$h_1\f$ and \f$h_2\f$, which are the
	 * only vertices that can have degree \f$\ge 2\f$.
	 * - If \f$deg(h_1) = 1\f$ then the tree is an instance of a @ref lal::graphs::tree_type::star tree.
	 * - If \f$deg(h_1) = 2\f$ then the tree is an instance of a @ref lal::graphs::tree_type::quasistar
	 * tree.
	 */
	bistar,

	/**
	 * @brief Quasi star trees.
	 *
	 * A @ref lal::graphs::tree_type::bistar tree where \f$deg(h_1) = 2\f$. These
	 * are a specific instance of @ref lal::graphs::tree_type::caterpillar trees
	 * and @ref lal::graphs::tree_type::spider trees.
	 */
	quasistar,

	/**
	 * @brief Star trees.
	 *
	 * A @ref lal::graphs::tree_type::bistar tree where \f$deg(h_1) = 1\f$. These
	 * are a specific instance of @ref lal::graphs::tree_type::quasistar trees,
	 * @ref lal::graphs::tree_type::caterpillar trees and
	 * @ref lal::graphs::tree_type::spider trees.
	 */
	star,

	/**
	 * @brief Spider trees.
	 *
	 * A tree where one vertex (the hub) has degree \f$\ge 3\f$ and the rest have
	 * degree \f$\le 2\f$ \cite English2019a. This type of trees have as particular
	 * instances:
	 * - @ref lal::graphs::tree_type::star tree
	 * - @ref lal::graphs::tree_type::quasistar tree
	 */
	spider,

	/**
	 * @brief 2-linear trees.
	 *
	 * Trees that have exactly two vertices of degree \f$\ge 3\f$ \cite Johnson2020a.
	 * Equivalently, these trees are two @ref lal::graphs::tree_type::spider trees
	 * whose hubs are joined with a @ref lal::graphs::tree_type::linear tree by
	 * the hubs.
	 *
	 * These trees have as specific instance @ref lal::graphs::tree_type::bistar
	 * trees, but not @ref lal::graphs::tree_type::star or
	 * @ref lal::graphs::tree_type::quasistar trees.
	 */
	two_linear,

	// *DEVELOPER NOTE*
	// This value must always be the last one.
	/// The tree could not be classified
	unknown
};

/// Number of elements within enumeration @ref lal::graphs::tree_type.
constexpr inline std::size_t __tree_type_size =
	1 + static_cast<std::size_t>(tree_type::unknown);

} // namespace graphs
} // namespace lal
