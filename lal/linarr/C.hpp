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

// C++ includes
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/linarr/algorithms_C.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph and a linear arrangements of its nodes, computes the number
 * of edge crossings using the algorithm specified by the parameter @e A.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param A Algorithm to use to compute the number of crossings. Default:
 * @ref algorithms_C::stack_based.
 * @return Returns \f$C\f$.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref algorithms_C.
 */
uint32_t n_crossings(
	const graphs::graph& g, const linear_arrangement& pi = {},
	const algorithms_C& A = algorithms_C::stack_based
);
/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph and a list of linear arrangements of its nodes, computes
 * the number of edge crossings for each of the linear arrangements in the
 * list using the algorithm specified by the parameter @e A.
 * @param g Input graph.
 * @param pis List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @param A Algorithm to use to compute the number of crossings. Default:
 * @ref algorithms_C::stack_based.
 * @return Returns a list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements in @e pis can be empty.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref algorithms_C.
 */
std::vector<uint32_t> n_crossings_list
(const graphs::graph& g, const std::vector<linear_arrangement>& pis,
 const algorithms_C& A = algorithms_C::stack_based);

/* ---------------------------------------- */
/* APPROXIMATION OF THE NUMBER OF CROSSINGS */

/**
 * @brief Approximates the number of crossings.
 *
 * Given a linear arrangement, which determines the length of the edges, predict
 * the number of crossings conditioned by the length of the edges in the
 * linear arrangement. Implementation of \cite Ferrer2014a. If the arrangement
 * is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return Approximation of the number of crossings \f$E_s[C_G\;|\;\delta]\f$.
 */
numeric::rational approximate_C_rational
(const graphs::graph& g, const linear_arrangement& pi = {});

/**
 * @brief Approximates the number of crossings.
 *
 * See @ref approximate_C_rational for details.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return The return value is a floating point value.
 */
double approximate_C(const graphs::graph& g, const linear_arrangement& pi = {});

} // -- namespace linarr
} // -- namespace lal
