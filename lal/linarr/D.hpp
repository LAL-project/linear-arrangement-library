/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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

namespace lal {
namespace linarr {

/**
 * @brief Computes the sum of the length of the edges in a linear arrangement.
 *
 * Given a graph and a linear arrangement of its vertices, computes the sum of
 * the length of the graph's edges in the arrangement. If the arrangement is
 * not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If @e \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$D\f$.
 */
uint32_t sum_length_edges(const graphs::ugraph& g, const std::vector<node>& pi = {});

/**
 * @brief Computes the mean edge length as an exact rational value.
 *
 * Given a graph and a linear arrangement of its vertices, computes the mean
 * edge length, or the mean dependency distance (see \cite Jing2015).
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If @e \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns Jing's and Liu's \f$MDD\f$.
 */
numeric::rational MDD_rational(const graphs::ugraph& g, const std::vector<node>& pi = {});

/**
 * @brief Computes the mean edge length as an exact rational value.
 *
 * Given a graph and a linear arrangement of its vertices, computes the mean
 * edge length, or the mean dependency distance (see \cite Jing2015).
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If @e \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns Jing's and Liu's \f$MDD\f$.
 */
double MDD(const graphs::ugraph& g, const std::vector<node>& pi = {});

} // -- namespace linarr
} // -- namespace lal
