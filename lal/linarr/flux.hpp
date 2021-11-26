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
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/linarr/dependency_flux.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the flux of a dependency tree.
 *
 * This function is implemented based on the explanations given in \cite Kahane2017a.
 * @param t Input free tree (or dependency tree).
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @pre The tree @e t is a valid free tree. Method graphs::free_tree::is_tree
 * returns true.
 */
std::vector<dependency_flux>
compute_flux(const graphs::free_tree& t, const linear_arrangement& pi = {}) noexcept;

/**
 * @brief Computes the flux of a dependency tree.
 *
 * This function is implemented based on the explanations given in \cite Kahane2017a.
 * @param t Input rooted tree (or dependency tree).
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 */
inline
std::vector<dependency_flux> compute_flux
(const graphs::rooted_tree& t, const linear_arrangement& pi = {}) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	return compute_flux(t.to_free_tree(), pi);
}

} // -- namespace linarr
} // -- namespace lal
