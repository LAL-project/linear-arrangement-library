/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

// D: sum of length of edges

/* ---------------------------- */
/*  EXPECTATION OF D: E_rla[D]  */
/* (unconstrained arrangements) */

/**
 * @brief Expected sum of edge lengths in uncosntrained arrangments.
 *
 * This function uses the formulae derived in \cite Ferrer2019a.
 *
 * Returns the value \f$E[D]\f$ as a rational value.
 * @param g The input graph.
 * @returns The expected value of the sum of edge lengths as a rational value.
 */
inline
numeric::rational exp_sum_edge_lengths_rational(const graphs::undirected_graph& g)
noexcept {
	return numeric::rational((g.get_num_nodes() + 1)*g.get_num_edges(), 3);
}

/**
 * @brief Expected sum of edge lengths in uncosntrained arrangments.
 *
 * See @ref lal::properties::exp_sum_edge_lengths_rational for details.
 * @param g The input graph.
 * @returns The expected value of the sum of edge lengths as a floating point value.
 */
inline
double exp_sum_edge_lengths(const graphs::undirected_graph& g) noexcept {
	return exp_sum_edge_lengths_rational(g).to_double();
}

/* ------------------------- */
/* EXPECTATION OF D: E_pr[D] */
/* (projective arrangements) */

/**
 * @brief Expected sum of edge lengths constrained to projective arrangments.
 *
 * This function uses the formulae derived in \cite Alemany2021b.
 *
 * Returns the value \f$E_{\mathrm{pr}}[D]\f$ as a rational value.
 * @param rt The input rooted tree.
 * @returns The expected value of the sum of edge lengths constrained to projective
 * arrangements as a floating point value.
 * @pre @e rt must be a valid rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
numeric::rational exp_sum_edge_lengths_projective_rational
(const graphs::rooted_tree& rt) noexcept;

/**
 * @brief Expected sum of edge lengths constrained to projective arrangments.
 *
 * See @ref lal::properties::exp_sum_edge_lengths_projective_rational for details.
 * @param rt The input rooted tree.
 * @returns The expected value of the sum of edge lengths constrained to projective
 * arrangements as a floating point value.
 * @pre @e rt must be a valid rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
inline
double exp_sum_edge_lengths_projective(const graphs::rooted_tree& rt) noexcept {
	return exp_sum_edge_lengths_projective_rational(rt).to_double();
}

/* ---------------------------- */
/*    VARIANCE OF D: V_rla[D]   */
/* (unconstrained arrangements) */

/**
 * @brief Computes the variance of the sum of the length of edges.
 *
 * Computes \f$V[D]\f$.
 *
 * This function uses the formula derived in \cite Ferrer2019a.
 * @param g Input graph.
 * @returns The exact value of \f$V[D]\f$ as a rational value.
 */
numeric::rational var_sum_edge_lengths_rational(const graphs::undirected_graph& g)
noexcept;
/**
 * @brief Computes the variance of the sum of the length of edges.
 *
 * See @ref lal::properties::var_sum_edge_lengths_rational for details.
 * @param g The input graph.
 * @returns The exact value of \f$V[D]\f$ as a floating point value.
 */
inline
double var_sum_edge_lengths(const graphs::undirected_graph& g) noexcept {
	return var_sum_edge_lengths_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
