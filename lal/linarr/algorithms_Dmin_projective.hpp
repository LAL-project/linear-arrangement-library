/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

namespace lal {
namespace linarr {

// **DEVELOPER NOTE**
// This enumeration's documentation has to be updated manually in the
// algorithms_Dmin_projective.py python file.

/**
 * @brief The different algorithms for computing the minimum sum of the
 * length of the edges \f$D\f$ in projective arrangements of rooted trees.
 *
 * Recall that a projective arrangement is one in which there are no edge
 * crossings and the root is not covered by any edge.
 *
 * This enumeration's values are used to choose the algorithm which the functions
 * @ref lal::linarr::min_sum_edge_lengths_projective use to compute the minimum
 * value of the sum of the length of the edges \f$D\f$.
 */
enum class algorithms_Dmin_projective {
	/**
	 * @brief Alemany-Esteban-Ferrer's algorithm.
	 *
	 * Interval-based approach to the calculation of minimum projective arrangements.
	 * Algorithm published in \cite Alemany2022a.
	 *
	 * This algorithm's complexity is \f$O(n)\f$.
	 */
	AlemanyEstebanFerrer,

	/**
	 * @brief Hochberg-Stallmann's algorithm.
	 *
	 * Displacement-based approach to the calculation of minimum projective
	 * arrangements. The algorithm was originally published in \cite Hochberg2003a,
	 * however, the implementation uses the correction in \cite Alemany2022a.
	 *
	 * This algorithm's complexity is \f$O(n)\f$.
	 */
	HochbergStallmann
};

} // -- namespace linarr
} // -- namespace lal
