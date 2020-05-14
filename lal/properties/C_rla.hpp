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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/ftree.hpp>

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
numeric::rational expectation_C_rational(const graphs::ugraph& g);
/**
 * @brief Computes the the expectation of the first moment of C.
 *
 * See @ref expectation_C_rational for details.
 * @param g The input graph.
 * @return The return value is a floating point value.
 */
double expectation_C(const graphs::ugraph& g);

/* ----------------------- */
/* VARIANCE OF C: V_rla[C] */

// ---------------------------
// VARIANCE FOR GENERAL GRAPHS

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V_{rla}[C]\f$ on the given graph. This function implements the
 * algorithm in \cite Alemany2020b for general graphs, which stems from the
 * study in \cite Alemany2020a.
 *
 * Since there are many computations that can be resued, setting @e reuse to
 * 'true' can help speed up the algorithm. Warning: reusing memory might be too
 * memory-consuming for large graphs (handle with care).
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
 * Computes \f$V_{rla}[C]\f$ on the given forest. This function implements the
 * algorithm in \cite Alemany2020b for forests, which stems from the study in
 * \cite Alemany2020a.
 *
 * Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
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
numeric::rational variance_C_tree_rational(const graphs::ftree& g);
/**
 * @brief Computes the variance of the number of crossings of a tree.
 *
 * See @ref variance_C_tree_rational for details.
 * @param g Input tree.
 * @return The return value is a floating point value.
 */
double variance_C_tree(const graphs::ftree& g);

} // -- namespace properties
} // -- namespace lal
