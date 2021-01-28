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

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Dependency flux.
 *
 * The dependency flux of a dependency tree, as defined in \cite Kahane2017a.
 */
struct dependency_flux {
	/// Number of words to the left of this position.
	uint32_t left_span = 0;
	/// Number of words to the right of this position.
	uint32_t right_span = 0;
	/**
	 * @brief Dependencies in this flux.
	 *
	 * The set of concomitant dependencies.
	 */
	std::vector<edge> dependencies;
	/// Size of this flux. Equal to the size of @ref dependency_flux::dependencies.
	uint32_t size = 0;
	/**
	 * @brief Weight of this flux.
	 *
	 * This is the size of the largest subset of disjoint dependencies in
	 * @ref dependency_flux::dependencies.
	 */
	uint32_t weight = 0;

	/**
	 * @brief Is this flux a left-branching bouquet?
	 *
	 * A bouquet is a set of dependencies sharing the same vertex. It is
	 * left-branching if the common vertex is on the right.
	 */
	bool is_left_bouquet = false;
	/**
	 * @brief Is this flux a right-branching bouquet?
	 *
	 * A bouquet is a set of dependencies sharing the same vertex. It is
	 * right-branching if the common vertex is on the left.
	 */
	bool is_right_bouquet = false;
};

/**
 * @brief Computes the flux of a dependency tree.
 *
 * @param t Input rooted tree (or dependency tree).
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 */
std::vector<dependency_flux>
compute_flux(const graphs::free_tree& t, const linear_arrangement& pi = {});

} // -- namespace linarr
} // -- namespace lal
