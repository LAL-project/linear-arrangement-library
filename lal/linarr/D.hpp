/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          LOGPROG: Logics and Programming Research Group
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/linarr/algorithms_Dmin.hpp>

namespace lal {
namespace linarr {

/* D */

/**
 * @brief Computes the sum of the length of the edges in a linear arrangement.
 *
 * Given a graph and a linear arrangement of its nodes, computes the sum of
 * the length of the graph's edges in the arrangement. If the arrangement is
 * not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return Returns \f$D\f$.
 */
uint32_t sum_length_edges
(const graphs::graph& g, const linearrgmnt& pi = {});

/**
 * @brief Computes the minimum value of \f$D\f$.
 *
 * Calculates the minimum value of the sum of the length of the edges \f$D\f$
 * and returns a linear arrangement yielding this value.
 * @param t Input tree.
 * @param a The algorithm to be chosen.
 * @return Returns the minimum value of \f$D\f$ and an optimum arrangement.
 */
std::pair<uint32_t, linearrgmnt> compute_Dmin
(const graphs::tree& t, const algorithms_Dmin& a = algorithms_Dmin::Shiloach);

/* MDD */

/**
 * @brief Computes the mean edge length.
 *
 * Given a graph and a linear arrangement of its nodes, computes the average
 * edge length, or the mean dependency distance (see \cite Jing2015a).
 *
 * It simply computes the sum of the length of the edges of the graph given the
 * arrangement \f$\pi\f$ and divides the sum by the number of edges.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return Returns Jing's and Liu's \f$MDD\f$.
 */
numeric::rational MDD_rational
(const graphs::graph& g, const linearrgmnt& pi = {});

/**
 * @brief Computes the mean edge length as an exact rational value.
 *
 * See @ref MDD_rational for details.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return The return value is a floating point value.
 */
double MDD(const graphs::graph& g, const linearrgmnt& pi = {});

} // -- namespace linarr
} // -- namespace lal
