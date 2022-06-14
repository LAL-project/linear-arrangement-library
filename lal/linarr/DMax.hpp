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

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all planar arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of planar
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2022b.
 * @param t Input free tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::free_tree& t)
noexcept;
/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all planar arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of planar
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2022b.
 *
 * This function converts the input rooted into a free tree (see @ref
 * lal::graphs::rooted_tree::to_free_tree())
 * @param t Input rooted tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
inline
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::rooted_tree& t)
noexcept
{
	return max_sum_edge_lengths_planar(t.to_free_tree());
}

/**
 * @brief Computes the maximum value of \f$D\f$ in rooted trees under the projectivity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all projective arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of projective
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2022b.
 * @param t Input rooted tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_projective
(const graphs::rooted_tree& t)
noexcept;

} // -- namespace linarr
} // -- namespace lal
