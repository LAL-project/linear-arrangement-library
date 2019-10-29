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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#pragma once

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/ugraph.hpp>

namespace lal {
namespace properties {

// C: the number of crossings

/* -------------------------- */
/* EXPECTATION OF C: E_rla[C] */

/**
 * @brief Computes the the expectation of the first moment of C.
 * @param g The input graph.
 * @return Returns \f$E[C]\f$ as a rational value.
 */
numeric::rational expectation_C_first_rational(const ugraph& g);
/**
 * @brief Computes the the expectation of the first moment of C.
 * @param g The input graph.
 * @return Returns \f$E[C]\f$.
 */
double expectation_C_first(const ugraph& g);

/* ----------------------- */
/* VARIANCE OF C: V_rla[C] */

// ---------------------------
// VARIANCE FOR GENERAL GRAPHS

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Compute the variance of the number of crossings \f$V[C]\f$ of a given graph
 * when its vertices are linearly arranged.
 *
 * This function enumerates all elements of Q.
 * @param g Input graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V[C]\f$.
 */
numeric::rational variance_C_freqs_rational(const ugraph& g, uint32_t nthreads = 8);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Compute the variance of the number of crossings \f$V[C]\f$ of a given graph
 * when its vertices are linearly arranged.
 *
 * This function enumerates all elements of Q.
 * @param g Input graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V[C]\f$.
 */
double variance_C_freqs(const ugraph& g, uint32_t nthreads = 8);

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Given the set \f$Q(G)\f$ of a graph \f$G\f$, compute the variance
 * of the number of crossings \f$V[C]\f$ when the vertices of such graph
 * placed in a linear arrangement.
 * @param Q Input set \f$Q\f$ of a graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V[C]\f$.
 */
numeric::rational variance_C_freqs_Q_rational(const std::vector<edge_pair>& Q, uint32_t nthreads = 8);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Given the set \f$Q(G)\f$ of a graph \f$G\f$, compute the variance
 * of the number of crossings \f$V[C]\f$ when the vertices of such graph
 * placed in a linear arrangement.
 * @param Q Input set \f$Q\f$ of a graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V[C]\f$.
 */
double variance_C_freqs_Q(const std::vector<edge_pair>& Q, uint32_t nthreads = 8);

// -----------------------------------------------------------------------------

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V[C]\f$ on the given graph.
 * @param g Input graph \f$G\f$.
 * @param Q Set of pairs of independent edges \f$Q(G)\f$.
 * @return Returns the exact value of \f$V[C]\f$ as a rational value.
 */
numeric::rational variance_C_rational_Q
(const ugraph& g, const std::vector<edge_pair>& Q);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V[C]\f$ on the given graph.
 * @param g Input graph \f$G\f$.
 * @param Q Set of pairs of independent edges \f$Q(G)\f$.
 * @return Returns the exact value of \f$V[C]\f$.
 */
double variance_C_Q(const ugraph& g, const std::vector<edge_pair>& Q);

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V[C]\f$ on the given graph.
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @return Returns the exact value of \f$V[C]\f$ as a rational value.
 * @pre Method @ref graph::is_normalised() must evaluate to true.
 */
numeric::rational variance_C_rational(const ugraph& g, bool reuse = true);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V[C]\f$ on the given graph.
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @return Returns the exact value of \f$V[C]\f$.
 * @pre Method @ref graph::is_normalised() must evaluate to true.
 */
double variance_C(const ugraph& g, bool reuse = true);

// --------------------
// VARIANCE FOR FORESTS

/**
 * @brief Computes the variance of the number of crossings of a forest.
 *
 * Computes \f$V[C]\f$ on the given forest. This function computes the
 * simplified formula of \f$V[C]\f$ on general graphs.
 * @param g Input forest.
 * @return Returns the exact value of \f$V[C]\f$ as a rational value.
 */
numeric::rational variance_C_forest_rational(const ugraph& g);
/**
 * @brief Computes the variance of the number of crossings of a forest.
 *
 * Computes \f$V[C]\f$ on the given forest. This function computes the
 * simplified formula of \f$V[C]\f$ on general graphs.
 * @param g Input forest.
 * @return Returns the exact value of \f$V[C]\f$.
 */
double variance_C_forest(const ugraph& g);

// ------------------
// VARIANCE FOR TREES

/**
 * @brief Computes the variance of the number of crossings of a tree.
 *
 * Computes \f$V[C]\f$ on the given tree. This function computes the
 * simplified formula of \f$V[C]\f$ on general graphs.
 * @param g Input tree.
 * @return Returns the exact value of \f$V[C]\f$ as a rational value.
 */
numeric::rational variance_C_tree_rational(const ugraph& g);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V[C]\f$ on the given tree. This function computes the
 * simplified formula of \f$V[C]\f$ on general graphs.
 * @param g Input tree.
 * @return Returns the exact value of \f$V[C]\f$.
 */
double variance_C_tree(const ugraph& g);

} // -- namespace properties
} // -- namespace lal
