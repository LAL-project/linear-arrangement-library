/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

// C: the number of crossings

/* -------------------------- */
/* EXPECTATION OF C: E_rla[C] */

/**
 * @brief Computes the the expected number of crossings in unconstrained arrangements,
 * \f$\mathbb{E}[C]\f$.
 *
 * Returns \f$\mathbb{E}[C]\f$ as a rational value.
 * @param g The input graph.
 * @returns The expected value of the number of crossings as a rational value.
 */
numeric::rational exp_num_crossings_rational(const graphs::undirected_graph& g)
noexcept;
/**
 * @brief Computes the the expected number of crossings in unconstrained arrangements,
 * \f$\mathbb{E}[C]\f$.
 *
 * See @ref lal::properties::exp_num_crossings_rational for details.
 * @param g The input graph.
 * @returns The expected value of the number of crossings as a floating point value.
 */
inline
double exp_num_crossings(const graphs::undirected_graph& g) noexcept
{ return exp_num_crossings_rational(g).to_double(); }

/* ----------------------- */
/* VARIANCE OF C: V_rla[C] */

// ---------------------------
// VARIANCE FOR GENERAL GRAPHS

/**
 * @brief Computes the variance of the number of crossings of a graph in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * Computes \f$\mathbb{V}[C]\f$ on the given graph. This function implements the
 * algorithm in \cite Alemany2020b for general graphs, which stems from the
 * study in \cite Alemany2020a.
 *
 * Since there are many computations that can be resued, setting @e reuse to
 * 'true' can help speed up the algorithm. Warning: reusing memory might be too
 * memory-consuming for large graphs (handle with care).
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
numeric::rational var_num_crossings_rational
(const graphs::undirected_graph& g, bool reuse = true) noexcept;
/**
 * @brief Computes the variance of the number of crossings of a graph in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * See @ref lal::properties::var_num_crossings_rational for details.
 * @param g Input graph.
 * @param reuse The algorithm will reuse computations in order to compute
 * the variance faster. Note: this might be too memory-consuming.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a rationafloating point value.
 */
inline
double var_num_crossings(const graphs::undirected_graph& g, bool reuse = true)
noexcept
{ return var_num_crossings_rational(g, reuse).to_double(); }

// --------------------
// VARIANCE FOR FORESTS

/**
 * @brief Computes the variance of the number of crossings of a forest in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * Computes \f$\mathbb{V}[C]\f$ on the given forest. This function implements the
 * algorithm in \cite Alemany2020b for forests, which stems from the study in
 * \cite Alemany2020a.
 *
 * Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
 * @param g Input forest.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a rational value.
 * @pre The input graph @e g is a forest.
 */
numeric::rational var_num_crossings_forest_rational(const graphs::undirected_graph& g)
noexcept;
/**
 * @brief Computes the variance of the number of crossings of a forest in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * See @ref lal::properties::var_num_crossings_forest_rational for details.
 * @param g Input forest.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a floating point value.
 */
inline
double var_num_crossings_forest(const graphs::undirected_graph& g) noexcept
{ return var_num_crossings_forest_rational(g).to_double(); }

// ------------------
// VARIANCE FOR TREES

/**
 * @brief Computes the variance of the number of crossings of a tree in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * Computes \f$\mathbb{V}[C]\f$ on the given tree. This function computes the
 * simplified formula of \f$V_{rla}[C]\f$ on general graphs for the case of
 * trees. Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
 * @param t Input free tree.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
inline
numeric::rational var_num_crossings_tree_rational(const graphs::free_tree& t)
noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return var_num_crossings_forest_rational(t);
}
/**
 * @brief Computes the variance of the number of crossings of a tree in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * See @ref lal::properties::var_num_crossings_tree_rational for details.
 * @param t Input free tree.
 * @returns The return value is a floating point value.
 */
inline
double var_num_crossings_tree(const graphs::free_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return var_num_crossings_forest_rational(t).to_double();
}

/**
 * @brief Computes the variance of the number of crossings of a tree in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * This function converts the input rooted tree into a free tree.
 *
 * Computes \f$\mathbb{V}[C]\f$ on the given tree. This function computes the
 * simplified formula of \f$V_{rla}[C]\f$ on general graphs for the case of
 * trees. Complexity: time \f$O(n)\f$, space \f$O(n)\f$.
 * @param t Input rooted tree.
 * @returns The exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
inline
numeric::rational var_num_crossings_tree_rational(const graphs::rooted_tree& t)
noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return var_num_crossings_forest_rational(t.to_free_tree());
}
/**
 * @brief Computes the variance of the number of crossings of a tree in unconstrained
 * arrangements, \f$\mathbb{V}[C]\f$.
 *
 * This function converts the input rooted tree into a free tree.
 *
 * See @ref lal::properties::var_num_crossings_tree_rational for details.
 * @param t Input rooted tree.
 * @returns The return value is a floating point value.
 */
inline
double var_num_crossings_tree(const graphs::rooted_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return var_num_crossings_forest_rational(t.to_free_tree()).to_double();
}

} // -- namespace properties
} // -- namespace lal
