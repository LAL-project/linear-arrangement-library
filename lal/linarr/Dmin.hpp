/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/algorithms_Dmin.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the minimum value of \f$D\f$ in rooted trees under the projectivity
 * constraint.
 *
 * Computes an optimal projective linear arrangement for rooted trees. A
 * projective linear arrangement is an arrangement in which there are no edge
 * crossings and the root is not covered by any edge.
 *
 * This function implements the algorithm in \cite Alemany2021a. A non-linear
 * time algorithm to solve this problem was oulined in \cite Gildea2007a.
 *
 * @param t Input tree.
 * @returns The minimum value of \f$D\f$ and an optimum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 * @pre The input rooted tree has to have the size of its subtrees calculated
 * (see @ref lal::graphs::rooted_tree::calculate_size_subtrees and
 * @ref lal::graphs::rooted_tree::are_size_subtrees_valid).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Projective
(const graphs::rooted_tree& t) noexcept;

/**
 * @brief Computes the minimum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * This function implements the algorithm published in \cite Alemany2021a.
 *
 * Computes an optimal planar linear arrangement for free trees. A planar linear
 * arrangement is an arrangement in which there are no edge crossings. This problem
 * was originally tackled by Iordanskii \cite Iordanskii1987a and later by Hochberg
 * and Stallmann \cite Hochberg2003a. See \cite Alemany2021a for a review.
 * @param t Input tree.
 * @returns The minimum value of \f$D\f$ and an optimum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Planar
(const graphs::free_tree& t) noexcept;
/**
 * @brief Computes the minimum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * This function implements the algorithm published in \cite Alemany2021a.
 *
 * Computes an optimal planar linear arrangement for free trees. A planar linear
 * arrangement is an arrangement in which there are no edge crossings. This problem
 * was originally tackled by Iordanskii \cite Iordanskii1987a and later by Hochberg
 * and Stallmann \cite Hochberg2003a. See \cite Alemany2021a for a review.
 * @param t Input tree.
 * @returns The minimum value of \f$D\f$ and an optimum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
inline std::pair<uint32_t, linear_arrangement> Dmin_Planar
(const graphs::rooted_tree& t) noexcept
{ return Dmin_Planar(t.to_undirected()); }

/**
 * @brief Computes the minimum value of \f$D\f$ in free trees.
 *
 * Calculates the minimum value of \f$D\f$ and returns a linear arrangement
 * yielding this value. Such optimal value of \f$D\f$ depends on the choice
 * of algorithm for its calculation.
 * @param t Input tree.
 * @param a The algorithm to be chosen.
 * @returns The minimum value of \f$D\f$ and an optimum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 * @pre This function has as extra preconditions those specified in the enumeration
 * passed as parameter.
 */
std::pair<uint32_t, linear_arrangement> Dmin(
	const graphs::free_tree& t,
	const algorithms_Dmin& a = algorithms_Dmin::Unconstrained_YS
) noexcept;
/**
 * @brief Computes the minimum value of \f$D\f$ in trees.
 *
 * Calculates the minimum value of \f$D\f$ and returns a linear arrangement
 * yielding this value. Such optimal value of \f$D\f$ depends on the choice
 * of algorithm for its calculation.
 * @param t Input tree.
 * @param a The algorithm to be chosen.
 * @returns The minimum value of \f$D\f$ and an optimum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 * @pre This function has as extra preconditions those specified in the enumeration
 * passed as parameter.
 */
inline std::pair<uint32_t, linear_arrangement> Dmin(
	const graphs::rooted_tree& t,
	const algorithms_Dmin& a = algorithms_Dmin::Unconstrained_YS
) noexcept
{
	return Dmin(t.to_undirected(), a);
}

} // -- namespace linarr
} // -- namespace lal
