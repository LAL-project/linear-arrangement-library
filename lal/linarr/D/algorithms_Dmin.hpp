/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

namespace lal {
namespace linarr {

// **DEVELOPER NOTE**
// This enumeration's documentation has to be updated manually in the
// algorithms_Dmin.py python file.
/**
 * @brief The different algorithms for computing the minimum sum of the
 * length of the edges \f$D\f$.
 *
 * This enumeration's values are used to choose the algorithm which the functions
 * @ref lal::linarr::min_sum_edge_lengths use to compute the minimum value of
 * the sum of the length of the edges \f$D\f$.
 */
enum class algorithms_Dmin {
    /**
	 * @brief Yossi Shiloach's algorithm.
	 *
	 * Shiloach's quadratic algorithm \f$O(n^{2.2})\f$. Said algorithm was
	 * published in \cite Shiloach1979a, but the implementation applies the
	 * correction published in \cite Esteban2017a.
	 */
	Shiloach,
    /**
	 * @brief Fan Chung's quadratic algorithm.
     *
	 * Fan Chung's quadratic algorithm \f$O(n^2)\f$. Said algorithm was published
	 * in \cite Chung1984a. In particular, this implements Fan Chung's quadratic
	 * algorithm (Section 3).
	 */
	Chung_2
};

} // -- namespace linarr
} // -- namespace lal
