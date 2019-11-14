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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/utree.hpp>

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
numeric::rational expectation_C_first_rational(const graphs::ugraph& g);
/**
 * @brief Computes the the expectation of the first moment of C.
 *
 * See @ref expectation_C_first_rational for details.
 * @param g The input graph.
 * @return The return value is a floating point value.
 */
double expectation_C_first(const graphs::ugraph& g);

/* ----------------------- */
/* VARIANCE OF C: V_rla[C] */

// ---------------------------
// VARIANCE FOR GENERAL GRAPHS

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Compute the variance of the number of crossings \f$V_{rla}[C]\f$ over the space
 * of random linear arrangements.
 *
 * This function enumerates all elements of \f$Q\f$, classifies all elements of
 * \f$Q\times Q\f$ into their corresponding frequency type (see \cite Alemany2019a)
 * and then computes the variance.
 * @param g Input graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V_{rla}[C]\f$.
 */
numeric::rational variance_C_freqs_rational(const graphs::ugraph& g, uint32_t nthreads = 8);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * See @ref variance_C_freqs_rational for details.
 * @param g Input graph.
 * @param nthreads Number of threads. Default: 8.
 * @return The return value is a floating point value.
 */
double variance_C_freqs(const graphs::ugraph& g, uint32_t nthreads = 8);

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Given the set \f$Q(G)\f$ of a graph \f$G\f$, classifies all elements of
 * \f$Q\times Q\f$ into their corresponding frequency type (see \cite Alemany2019a)
 * and then computes the variance
 * @param Q Input set \f$Q\f$ of a graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V_{rla}[C]\f$.
 */
numeric::rational variance_C_freqs_Q_rational(const std::vector<edge_pair>& Q, uint32_t nthreads = 8);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * See @ref variance_C_freqs_Q_rational for details.
 * @param Q Input set \f$Q\f$ of a graph.
 * @param nthreads Number of threads. Default: 8.
 * @return The return value is a floating point value.
 */
double variance_C_freqs_Q(const std::vector<edge_pair>& Q, uint32_t nthreads = 8);

// -----------------------------------------------------------------------------

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V_{rla}[C]\f$ on the given graph. Applies a more efficient method than
 * the function @ref variance_C_freqs_Q_rational applies.
 * @param g Input graph \f$G\f$.
 * @param Q Set of pairs of independent edges \f$Q(G)\f$.
 * @return Returns the exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
numeric::rational variance_C_rational_Q
(const graphs::ugraph& g, const std::vector<edge_pair>& Q);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * See @ref variance_C_rational_Q for details.
 * @return The return value is a floating point value.
 */
double variance_C_Q(const graphs::ugraph& g, const std::vector<edge_pair>& Q);

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V_{rla}[C]\f$ on the given graph. This function implements the
 * most efficient method to compute \f$V_{rla}[C]\f$ to our knowledge. Since
 * there are many computations that can be resued, setting @e reuse to 'true'
 * can help speed up the algorithm. Warning: reusing memory might be too
 * memory consumption for large graphs.
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @return Returns the exact value of \f$V_{rla}[C]\f$ as a rational value.
 * @pre Method @ref graphs::graph::is_normalised() must evaluate to true.
 */
numeric::rational variance_C_rational(const graphs::ugraph& g, bool reuse = true);
/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * See @ref variance_C_rational for details.
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @return The return value is a floating point value.
 */
double variance_C(const graphs::ugraph& g, bool reuse = true);

// --------------------
// VARIANCE FOR FORESTS

/**
 * @brief Computes the variance of the number of crossings of a forest.
 *
 * Computes \f$V_{rla}[C]\f$ on the given forest. This function computes the
 * simplified formula of \f$V_{rla}[C]\f$ on general graphs for the case of
 * forests. Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
 * @param g Input forest.
 * @return Returns the exact value of \f$V_{rla}[C]\f$ as a rational value.
 * @pre The input graph @e g is a forest.
 */
numeric::rational variance_C_forest_rational(const graphs::ugraph& g);
/**
 * @brief Computes the variance of the number of crossings of a forest.
 *
 * See @ref variance_C_forest_rational for details.
 * @param g Input forest.
 * @return The return value is a floating point value.
 */
double variance_C_forest(const graphs::ugraph& g);

// ------------------
// VARIANCE FOR TREES

/**
 * @brief Computes the variance of the number of crossings of a tree.
 *
 * Computes \f$V_{rla}[C]\f$ on the given tree. This function computes the
 * simplified formula of \f$V_{rla}[C]\f$ on general graphs for the case of
 * trees. Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
 * @param g Input tree.
 * @return Returns the exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
numeric::rational variance_C_tree_rational(const graphs::utree& g);
/**
 * @brief Computes the variance of the number of crossings of a tree.
 *
 * See @ref variance_C_tree_rational for details.
 * @param g Input tree.
 * @return The return value is a floating point value.
 */
double variance_C_tree(const graphs::utree& g);

} // -- namespace properties
} // -- namespace lal
