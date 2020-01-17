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

namespace lal {
namespace linarr {

/* 1-LEVEL METRICS */

/**
 * @brief Computes the 1-level Mean Dependency Distance over an ensemble of graphs.
 *
 * Given a list of graphs and a linear arrangement of the vertices for each of
 * them, computes the 1-level Mean Dependency Distance as the quotient of
 * \f$D\f$, the sum of all the \f$D_i\f$, where \f$D_i\f$ is the sum of the
 * edge lengths of tree \f$i\f$, and of \f$M\f$ the sum of the number of edges
 * of all the trees.
 *
 * Formally, given a list of linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$
 * and a list of graphs \f$G = \{G_i\}_{i=1}^k\f$, computes \f$D/M\f$, where
 * - \f$D = \sum_{i=1}^k D(G_i, \pi_i)\f$ is the sum of edge lengths of every
 * graph.
 * - \f$M = \sum_{i=1}^k |E(G_i)|\f$ is the sum of the number of edges of every
 * graph.
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the vertices \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for all graphs.
 * @return Returns Jing's and Liu's 1-level \f$MDD\f$ for an ensemble of graphs.
 */
template<class G>
numeric::rational MDD_1level_rational
(const std::vector<G>& Gs, const std::vector<LINARR>& pis = {});

/**
 * @brief Computes the 1-level Mean Dependency Distance over an ensemble of graphs.
 *
 * See @ref MDD_1level_rational for details.
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the vertices \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for all graphs.
 * @return The return value is a floating point value.
 */
template<class G>
double MDD_1level
(const std::vector<G>& Gs, const std::vector<LINARR>& pis = {});

} // -- namespace linarr
} // -- namespace lal

#include <lal/linarr/1level_impl.hpp>