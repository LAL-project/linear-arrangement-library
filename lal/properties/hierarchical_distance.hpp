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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace properties {

/**
 * @brief Sum of hierarchical distances (SHD).
 *
 * The hierarchical distance \f$HD_u\f$ of a vertex \f$u\f$ to the root of the
 * tree is calculated as the number of edges between these two vertices. Therefore,
 * the hierarchical distance from a root's child and the root is exactly 1.
 *
 * The result of this function is the sum of such distances:
 * \f$SHD = \sum_{u\in V} HD_u\f$.
 *
 * For furhter details see \cite Jing2015a.
 * @param t Input rooted tree.
 * @returns The sum of hierarchical distances of a rooted tree.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 */
[[nodiscard]] uint64_t sum_hierarchical_distances(const graphs::rooted_tree& t)
noexcept;

/**
 * @brief Mean Hierarchical Distance (MHD).
 *
 * The hierarchical distance \f$HD_u\f$ of a vertex \f$u\f$ to the root of the
 * tree is calculated as the number of edges between these two vertices. Therefore,
 * the hierarchical distance from a root's child and the root is exactly 1.
 *
 * The result of this function is the average of such distances:
 * \f$MHD = \frac{1}{n-1} \sum_{u\in V} HD_u\f$.
 *
 * For furhter details see \cite Jing2015a.
 * @param t Input rooted tree.
 * @returns The Mean Hierarchical Distance of a rooted tree as a rational value.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 * @pre \f$n > 1\f$ (which is the same as \f$m > 0\f$.
 */
[[nodiscard]] numeric::rational mean_hierarchical_distance_rational
(const graphs::rooted_tree& t)
noexcept;

/**
 * @brief Mean Hierarchical Distance (MHD).
 *
 * See @ref lal::properties::mean_hierarchical_distance_rational for details.
 * @param t Input rooted tree.
 * @returns The Mean Hierarchical Distance of a rooted tree as a floating point
 * value.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 * @pre \f$n > 1\f$ (which is the same as \f$m > 0\f$.
 */
[[nodiscard]] double mean_hierarchical_distance(const graphs::rooted_tree& t)
noexcept;

} // -- namespace properties
} // -- namespace lal
