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
#include <lal/graphs/ugraph.hpp>

namespace lal {
namespace linarr {

/* -------------------------------------- */
/* COMPUTATION OF THE NUMBER OF CROSSINGS */

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given the set \f$Q\f$ of a graph, and a linear arrangement of its vertices,
 * computes by brute force the number of edges that cross in such linear
 * arrangement. If the arrangement is not specified, the identity arrangement
 * is used.
 * @param Q The set \f$Q(G)\f$ of a graph.
 * @param pi Linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$C\f$.
 */
uint32_t n_crossings_Q(const std::vector<edge_pair>& Q, const std::vector<node>& pi = {});

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangement of its vertices, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$C\f$.
 */
uint32_t n_crossings_brute_force(const graphs::ugraph& g, const std::vector<node>& pi = {});

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangement of its vertices, computes using
 * dynamic programming the number of edges that cross in such linear
 * arrangement. If the arrangement is not specified, the identity arrangement
 * is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$C\f$.
 */
uint32_t n_crossings_dyn_prog(const graphs::ugraph& g, const std::vector<node>& pi = {});
/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its vertices, computes
 * using dynamic programming the number of edges that cross in every linear
 * arrangement.
 * @param g Input graph.
 * @param pis List of linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * If \f$\pi[u]=p\f$ then node @e u is placed in position @e p of the arrangement.
 * @return Returns a list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 */
std::vector<uint32_t> n_crossings_dyn_prog_list
(const graphs::ugraph& g, const std::vector<std::vector<node> >& pis);

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a permutation of its vertices, computes using
 * dynamic programming the number of edges that cross in such linear
 * arrangement. If the arrangement is not specified, the identity arrangement
 * is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$C\f$.
 */
uint32_t n_crossings_ladder(const graphs::ugraph& g, const std::vector<node>& pi = {});

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its vertices, computes
 * using dynamic programming the number of edges that cross in such linear
 * arrangement.
 * @param g Input graph.
 * @param pis @param pis List of linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * If \f$\pi[u]=p\f$ then node @e u is placed in position @e p of the arrangement.
 * @return Returns a list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 */
std::vector<uint32_t> n_crossings_ladder_list
(const graphs::ugraph& g, const std::vector<std::vector<node> >& pis);

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its vertices, computes
 * using the algorithm by K. Palios and G. Pitsiladis the number of edges that
 * cross in such linear arrangement. If the arrangement is not specified, the
 * identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the vertices.If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns \f$C\f$.
 */
uint32_t n_crossings_stack_based
(const graphs::ugraph& g, const std::vector<node>& pi = {});

/* ---------------------------------------- */
/* APPROXIMATION OF THE NUMBER OF CROSSINGS */

/**
 * @brief Approximate the number of crossings.
 *
 * Given a linear arrangement, which determines the length of the edges, predict
 * the number of crossings conditioned by the length of the edges in the
 * linear arrangement. Implementation of \cite Ferrer2014a. If the arrangement
 * is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Approximation of the number of crossings \f$E_s[C_G\;|\;\delta]\f$.
 */
numeric::rational approximate_C_2_rational
(const graphs::ugraph& g, const std::vector<node>& pi = {});
/**
 * @brief Approximate the number of crossings.
 *
 * Given a linear arrangement, which determines the length of the edges, predict
 * the number of crossings conditioned by the length of the edges in the
 * linear arrangement. Implementation of \cite Ferrer2014a.  If the arrangement
 * is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Approximation of the number of crossings \f$E_s[C_G\;|\;\delta]\f$.
 */
double approximate_C_2
(const graphs::ugraph& g, const std::vector<node>& pi = {});

} // -- namespace linarr
} // -- namespace lal
