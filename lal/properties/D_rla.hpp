/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

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
 * @brief Expected sum of edge lengths of an undirected graph in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
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
 * @brief Expected sum of edge lengths of an undirected graph in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
 *
 * See @ref lal::properties::exp_sum_edge_lengths_rational for details.
 * @param g The input graph.
 * @returns The expected value of the sum of edge lengths as a floating point value.
 */
inline
double exp_sum_edge_lengths(const graphs::undirected_graph& g) noexcept {
	return exp_sum_edge_lengths_rational(g).to_double();
}

/**
 * @brief Expected sum of edge lengths of a free tree in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
 *
 * This function uses the formulae derived in \cite Ferrer2019a.
 *
 * Returns the value \f$E[D]\f$ as a rational value.
 * @param t The input rooted tree.
 * @returns The expected value of the sum of edge lengths as a rational value.
 */
inline
numeric::rational exp_sum_edge_lengths_rational(const graphs::free_tree& t)
noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	const uint64_t n = t.get_num_nodes();
	return numeric::rational(n*n - 1, 3);
}

/**
 * @brief Expected sum of edge lengths of a free tree in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
 *
 * See @ref lal::properties::exp_sum_edge_lengths_rational for details.
 * @param t The input free tree.
 * @returns The expected value of the sum of edge lengths as a floating point value.
 */
inline
double exp_sum_edge_lengths(const graphs::free_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return exp_sum_edge_lengths_rational(t).to_double();
}

/**
 * @brief Expected sum of edge lengths of a rooted tree in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
 *
 * This function uses the formulae derived in \cite Ferrer2019a.
 *
 * Returns the value \f$E[D]\f$ as a rational value.
 * @param t The input rooted tree.
 * @returns The expected value of the sum of edge lengths as a rational value.
 */
inline
numeric::rational exp_sum_edge_lengths_rational(const graphs::rooted_tree& t)
noexcept {
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	const uint64_t n = t.get_num_nodes();
	return numeric::rational(n*n - 1, 3);
}

/**
 * @brief Expected sum of edge lengths of a rooted tree in unconstrained arrangments,
 * \f$\mathbb{E}[D]\f$.
 *
 * See @ref lal::properties::exp_sum_edge_lengths_rational for details.
 * @param t The input rooted tree.
 * @returns The expected value of the sum of edge lengths as a floating point value.
 */
inline
double exp_sum_edge_lengths(const graphs::rooted_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	return exp_sum_edge_lengths_rational(t).to_double();
}


/* ------------------------- */
/* EXPECTATION OF D: E_pr[D] */
/* (projective arrangements) */

/**
 * @brief Expected sum of edge lengths of a tree constrained to projective arrangments,
 * \f$\mathbb{E}_{\mathrm{pr}}[D]\f$.
 *
 * This function uses the formulae derived in \cite Alemany2021b.
 *
 * Returns the value \f$E_{\mathrm{pr}}[D]\f$ as a rational value.
 * @param rt The input rooted tree.
 * @returns The expected value of the sum of edge lengths constrained to projective
 * arrangements as an exact rational value.
 * @pre @e rt must be a valid rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
numeric::rational exp_sum_edge_lengths_projective_rational
(const graphs::rooted_tree& rt) noexcept;

/**
 * @brief Expected sum of edge lengths of a tree constrained to projective arrangments,
 * \f$\mathbb{E}_{\mathrm{pr}}[D]\f$.
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


/* ------------------------- */
/* EXPECTATION OF D: E_pl[D] */
/*   (planar arrangements)   */

/**
 * @brief Expected sum of edge lengths of a tree constrained to planar arrangments,
 * \f$\mathbb{E}_{\mathrm{pl}}[D]\f$.
 *
 * This function uses the formulae derived in \cite Alemany2021c.
 *
 * Returns the value \f$E_{\mathrm{pl}}[D]\f$ as a rational value.
 * @param t The input free tree.
 * @returns The expected value of the sum of edge lengths constrained to planar
 * arrangements as an exact rational value.
 * @pre @e rt must be a valid free tree (see @ref lal::graphs::free_tree::is_tree).
 */
numeric::rational exp_sum_edge_lengths_planar_rational
(const graphs::free_tree& t) noexcept;
/**
 * @brief Expected sum of edge lengths of a tree constrained to planar arrangments,
 * \f$\mathbb{E}_{\mathrm{pl}}[D]\f$.
 *
 * This function uses the formulae derived in \cite Alemany2021c.
 *
 * Returns the value \f$E_{\mathrm{pl}}[D]\f$ as a rational value.
 * @param rt The input rooted tree.
 * @returns The expected value of the sum of edge lengths constrained to planar
 * arrangements as an exact rational value.
 * @pre @e rt must be a valid rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
inline
numeric::rational exp_sum_edge_lengths_planar_rational
(const graphs::rooted_tree& rt) noexcept
{ return exp_sum_edge_lengths_planar_rational(rt.to_undirected()); }

/**
 * @brief Expected sum of edge lengths of a tree constrained to planar arrangments,
 * \f$\mathbb{E}_{\mathrm{pl}}[D]\f$.
 *
 * This function uses the formulae derived in \cite Alemany2021c.
 *
 * Returns the value \f$E_{\mathrm{pl}}[D]\f$ as a rational value.
 * @param t The input free tree.
 * @returns The expected value of the sum of edge lengths constrained to planar
 * arrangements as a floating point value.
 * @pre @e rt must be a valid free tree (see @ref lal::graphs::free_tree::is_tree).
 */
inline
double exp_sum_edge_lengths_planar
(const graphs::free_tree& t) noexcept
{ return exp_sum_edge_lengths_planar_rational(t).to_double(); }
/**
 * @brief Expected sum of edge lengths of a tree constrained to planar arrangments,
 * \f$\mathbb{E}_{\mathrm{pl}}[D]\f$.
 *
 * This function uses the formulae derived in \cite Alemany2021c.
 *
 * Returns the value \f$E_{\mathrm{pl}}[D]\f$ as a rational value.
 * @param rt The input rooted tree.
 * @returns The expected value of the sum of edge lengths constrained to planar
 * arrangements as a floating point value.
 * @pre @e rt must be a valid rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
inline
double exp_sum_edge_lengths_planar
(const graphs::rooted_tree& rt) noexcept
{ return exp_sum_edge_lengths_planar_rational(rt).to_double(); }


/* ---------------------------- */
/*    VARIANCE OF D: V_rla[D]   */
/* (unconstrained arrangements) */

/**
 * @brief Computes the variance of the sum of the length of edges of a graph,
 * \f$\mathbb{V}[D]\f$.
 *
 * Computes the variance of the sum of edge lengths over all \f$n!\f$ arrangements.
 *
 * This function uses the formula derived in \cite Ferrer2019a.
 * @param g Input graph.
 * @returns The exact value of \f$V[D]\f$ as a rational value.
 */
numeric::rational var_sum_edge_lengths_rational(const graphs::undirected_graph& g)
noexcept;
/**
 * @brief Computes the variance of the sum of the length of edges of a graph,
 * \f$\mathbb{V}[D]\f$.
 *
 * See @ref lal::properties::var_sum_edge_lengths_rational for details.
 * @param g The input graph.
 * @returns The exact value of \f$V[D]\f$ as a floating point value.
 */
inline
double var_sum_edge_lengths(const graphs::undirected_graph& g) noexcept
{ return var_sum_edge_lengths_rational(g).to_double(); }

} // -- namespace properties
} // -- namespace lal
