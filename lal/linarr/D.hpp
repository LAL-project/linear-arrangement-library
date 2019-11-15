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
uint64_t sum_length_edges
(const graphs::ugraph& g, const LINARR& pi = {});

/**
 * @brief Computes the mean edge length.
 *
 * Given a graph and a linear arrangement of its vertices, computes the average
 * edge length, or the mean dependency distance (see \cite Jing2015).
 *
 * It simply computes the sum of the length of the edges of the graph given the
 * arrangement \f$\pi\f$ and divides the sum by the number of edges.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If @e \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns Jing's and Liu's \f$MDD\f$ for a single tree.
 */
numeric::rational MDD_rational
(const graphs::ugraph& g, const LINARR& pi = {});

/**
 * @brief Computes the mean edge length as an exact rational value.
 *
 * See @ref MDD_rational for details.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If @e \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return The return value is a floating point value.
 */
double MDD(const graphs::ugraph& g, const LINARR& pi = {});

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
 * If \f$\pi_i[u]=p\f$ then node @e u is placed in position @e p of the \f$i\f$-th
 * arrangement.
 * @return Returns Jing's and Liu's 1-level \f$MDD\f$ for an ensemble of graphs.
 */
numeric::rational MDD_1level_rational
(const std::vector<graphs::ugraph>& Gs, const std::vector<LINARR>& pis = {});

/**
 * @brief Computes the 1-level Mean Dependency Distance over an ensemble of graphs.
 *
 * See @ref MDD_1level_rational for details.
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the vertices \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * If \f$\pi_i[u]=p\f$ then node @e u is placed in position @e p of the \f$i\f$-th
 * arrangement.
 * @return The return value is a floating point value.
 */
double MDD_1level
(const std::vector<graphs::ugraph>& Gs, const std::vector<LINARR>& pis = {});

/* 2-LEVEL METRICS */

/**
 * @brief Computes the 2-level Mean Dependency Distance over an ensemble of graphs.
 *
 * Given a list of graphs and a linear arrangement of the vertices for each of
 * them, computes the 2-level Mean Dependency Distance, i.e., it computes the
 * average Mean Dependency Distance of the graphs in the list.
 *
 * Formally, given a list of linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$
 * and a list of graphs \f$G = \{G_i\}_{i=1}^k\f$, computes \f$S_{<d>}/k\f$, where
 * \f$S_{<d>} = \sum_{i=1}^k MDD(G_i, \pi_i)\f$ is the sum of the mean dependency
 * distances of every graph (see @ref MDD_rational for details on the definition
 * of the Mean Dependency Distance).
 *
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the vertices \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * If \f$\pi_i[u]=p\f$ then node @e u is placed in position @e p of the \f$i\f$-th
 * arrangement.
 * @return Returns Jing's and Liu's 1-level \f$MDD\f$ for an ensemble of graphs.
 */
numeric::rational MDD_2level_rational
(const std::vector<graphs::ugraph>& Gs, const std::vector<LINARR>& pis = {});

/**
 * @brief Computes the 1-level Mean Dependency Distance over an ensemble of graphs.
 *
 * See @ref MDD_2level_rational for details.
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the vertices \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * If \f$\pi_i[u]=p\f$ then node @e u is placed in position @e p of the \f$i\f$-th
 * arrangement.
 * @return The return value is a floating point value.
 */
double MDD_2level
(const std::vector<graphs::ugraph>& Gs, const std::vector<LINARR>& pis = {});

} // -- namespace linarr
} // -- namespace lal
