/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
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

namespace lal {
namespace linarr {

// **DEVELOPER NOTE**
// This enumeration's documentation has to be updated manually in the
// algorithms_C.py python file.

/**
 * @brief The different algorithms for computing the number of crossings.
 *
 * Two edges \f$\{s,t\},\{u,v\}\f$ of a graph \f$G\f$ cross in a linear arrangement
 * \f$\pi\f$ of its nodes if, and only if, their positions interleave in the
 * linear arrangement. More formally, given an arrangement \f$\pi\f$ of a graph
 * \f$G\f$, the edges \f$\{s,t\},\{u,v\}\f$ cros iff \f$\pi(s) < \pi(u) < \pi(t) < \pi(v)\f$ .
 */
enum class algorithms_C {
	/**
	 * @brief Brute force computation of the number of crossings.
	 *
	 * Complexity: time \f$O(m^2)\f$, space \f$O(1)\f$.
	 */
	brute_force,
	/**
	 * @brief Dynamic programming algorithm \cite Alemany2019a.
	 *
	 * Complexity: time \f$O(n^2)\f$, space \f$O(n^2)\f$.
	 */
	dynamic_programming,
	/**
	 * @brief Dynamic programming algorithm \cite Alemany2019a.
	 *
	 * Complexity: time \f$O(n^2)\f$, space \f$O(n)\f$.
	 */
	ladder,
	/**
	 * @brief Algorithm based on sorting \cite Alemany2019a.
	 *
	 * Complexity: time \f$O(m\log n)\f$, space \f$O(m)\f$.
	 */
	stack_based
};

} // -- namespace linarr
} // -- namespace lal
