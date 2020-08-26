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

namespace lal {
namespace linarr {

/**
 * @brief The different algorithms for computing the minimum sum of the
 * length of the edges \f$D\f$.
 *
 * This enumeration's values are used to choose the algorithm the functions
 * @ref Dmin(const graphs::rooted_tree&, const algorithms_Dmin&) and
 * @ref Dmin(const graphs::free_tree&, const algorithms_Dmin&)
 * use to compute the minimum value of the sum of the length the edges \f$D\f$.
 */
enum class algorithms_Dmin {
	/* ALGORITHMS FOR ROOTED TREES */

    /**
     * @brief Algorithm to calculate optimal projective arrangements of rooted trees.
     *
     * Computes an optimal projective linear arrangement for rooted trees.
     * A projective linear arrangement is an arrangement in which there are
     * no edge crossings and the root is not covered by any edge.
     *
	 * This option uses Gildea and Temperley's algorithm published in \cite Gildea2007a.
     *
	 * This algorithm is allowed only in function
	 * @ref Dmin(const graphs::rooted_tree&, const algorithms_Dmin& a),
	 * i.e., in the function that accepts rooted trees. Alternatively, one can
	 * call directly function @ref Dmin_Projective.
     */
    Projective,

	/* ALGORITHMS FOR FREE TREES */

	/**
	 * @brief Algorithm to calculate optimal planar arrangements of free trees.
	 *
	 * Computes an optimal planar linear arrangement for free trees.
	 * A planar linear arrangement is an arrangement in which there are
	 * no edge crossings.
	 *
	 * This option uses an algorithm first published by Iordanskii in
	 * \cite Iordanskii1987a and later by Hochberg and Stallmann's in
	 * \cite Hochberg2003a.
	 *
	 * This algorithm is allowed only in function
	 * @ref Dmin(const graphs::free_tree&, const algorithms_Dmin& a),
	 * i.e., in the function that accepts free trees. Alternatively, one can
	 * call directly function @ref Dmin_Planar.
	 */
	Planar,

    /**
     * @brief Algorithm to calculate unconstrained an optimal linearization of free trees.
     *
     * Computes an unconstrained optimal linear arrangement of a free tree and
     * the value of its cost, i.e., the sum of the lengths of the edges.
     *
     * This option uses Yossi Shiloach's algorithm published in \cite Shiloach1979a.
     * The implementation of this algorithm uses the corrections published
     * in \cite Esteban2017a.
	 *
	 * This algorithm is allowed only in function
	 * @ref Dmin(const graphs::free_tree&, const algorithms_Dmin&),
	 * i.e., in the function that accepts free trees. Alternatively, one can
	 * call directly function @ref Dmin_Unconstrained_YS.
     */
    Unconstrained_YS,
    /**
     * @brief Algorithm to calculate unconstrained an optimal linearization of free trees.
     *
     * Computes an unconstrained optimal linear arrangement of a free tree and
     * the value of its cost, i.e., the sum of the lengths of the edges.
     *
     * This option uses Fan Chung's algorithm published in \cite Chung1984a.
	 *
	 * This algorithm is allowed only in function
	 * @ref Dmin(const graphs::free_tree&, const algorithms_Dmin&),
	 * i.e., in the function that accepts free trees. Alternatively, one can
	 * call directly function @ref Dmin_Unconstrained_FC.
     */
    Unconstrained_FC
};

} // -- namespace linarr
} // -- namespace lal
