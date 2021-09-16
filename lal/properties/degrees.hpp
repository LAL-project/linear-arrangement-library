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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

/* -------------------------------------------------------------------------- */

/**
 * @brief Generic template function for the sum of degrees.
 *
 * Each degree is raised to a certain power according to the parameter.
 *
 * @tparam G Type of graph.
 * @tparam return_type Type of function's result (uint64_t, @ref lal::numeric::integer)
 * @param g Input graph
 * @param p Power to which degrees must be raised
 * @param degree_function The function that returns the specific degree
 * (in+out, out, in, or other).
 * @return The sum of degrees raised to the input power @e p.
 */
template<class G, class return_type>
inline return_type sum_powers_degrees
(const G& g, uint64_t p, uint64_t (G::*degree_function)(node) const noexcept)
noexcept
{
	static_assert(
		std::is_same_v<return_type, uint64_t> ||
		std::is_same_v<return_type, numeric::integer>
	);

	if constexpr (std::is_same_v<G, graphs::rooted_tree>) {
		// the sum of powers of the in-degree of the vertices in a rooted
		// tree is always equal to the number of edges!
		if (degree_function == &graphs::rooted_tree::get_in_degree) {
			return g.get_num_edges();
		}
	}

	// sum of powers
	return_type S(0);
	// variable used to calculate powers
	return_type du(0);

	for (node u = 0; u < g.get_num_nodes(); ++u) {
		const uint64_t deg = (g.*degree_function)(u);
		// calculate the power of the degree 'deg'
		if constexpr (std::is_same_v<return_type, numeric::integer>) {
			du.set_number(deg);
			du.powt(p);
		}
		else {
			du = 1;
			for (uint64_t i = 0; i < p; ++i) {
				du *= deg;
			}
		}
		// accumulate power
		S += du;
	}
	return S;
}

/**
 * @brief Computes the sum of degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_i^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of degrees raised to the \f$p\f$-th power.
 */
inline numeric::integer
sum_powers_degrees_integer(const graphs::undirected_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::undirected_graph, numeric::integer>
	(g, p, &graphs::undirected_graph::get_degree);
}
/**
 * @brief Computes the sum of degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_i^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of degrees raised to the \f$p\f$-th power.
 */
inline uint64_t
sum_powers_degrees(const graphs::undirected_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::undirected_graph, uint64_t>
	(g, p, &graphs::undirected_graph::get_degree);
}

/**
 * @brief Computes the sum of degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_i^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of degrees raised to the \f$p\f$-th power.
 */
inline numeric::integer
sum_powers_degrees_integer(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, numeric::integer>
	(g, p, &graphs::directed_graph::get_degree);
}
/**
 * @brief Computes the sum of degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_i^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline uint64_t
sum_powers_degrees(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, uint64_t>
	(g, p, &graphs::directed_graph::get_degree);
}

/**
 * @brief Computes the sum of in-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of in-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{in, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the in-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline numeric::integer
sum_powers_in_degrees_integer(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, numeric::integer>
	(g, p, &graphs::directed_graph::get_in_degree);
}
/**
 * @brief Computes the sum of in-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of in-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{in, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the in-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline uint64_t
sum_powers_in_degrees(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, uint64_t>
	(g, p, &graphs::directed_graph::get_in_degree);
}

/**
 * @brief Computes the sum of in-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of in-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{in, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the in-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline numeric::integer
sum_powers_in_degrees_integer(const graphs::rooted_tree& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::rooted_tree, numeric::integer>
	(g, p, &graphs::rooted_tree::get_in_degree);
}
/**
 * @brief Computes the sum of in-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of in-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{in, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the in-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline uint64_t
sum_powers_in_degrees(const graphs::rooted_tree& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::rooted_tree, uint64_t>
	(g, p, &graphs::rooted_tree::get_in_degree);
}

/**
 * @brief Computes the sum of out-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of out-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{out, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the out-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of out-degrees raised to the \f$p\f$-th power.
 */
inline numeric::integer
sum_powers_out_degrees_integer(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, numeric::integer>
	(g, p, &graphs::directed_graph::get_out_degree);
}
/**
 * @brief Computes the sum of out-degrees raised to the \f$p\f$-th power.
 *
 * Computes the sum of out-degrees raised to the \f$p\f$-th power using
 *
 * \f$K^p = \sum_{i=1}^n k_{out, i}^p\f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{out, i}\f$ is
 * the out-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 */
inline uint64_t
sum_powers_out_degrees(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	sum_powers_degrees<graphs::directed_graph, uint64_t>
	(g, p, &graphs::directed_graph::get_out_degree);
}


/* -------------------------------------------------------------------------- */

/**
 * @brief Generic template function for the moment of degree about 0.
 *
 * Calculates the \f$p\f$-th moment of degree about 0, where the degree is given
 * by some function.
 *
 * @tparam G Type of graph
 * @tparam return_type Type of function's result (double, @ref lal::numeric::rational)
 * @param g Input graph
 * @param p Power to which degrees must be raised
 * @param degree_function The function that returns the specific degree (full,
 * out, in, or other).
 * @return The \f$p\f$-th moment of degree about 0.
 */
template<class G, class return_type>
inline return_type moment_degree
(const G& g, uint64_t p, uint64_t (G::*degree_function)(node) const noexcept)
noexcept
{
	static_assert(
		std::is_same_v<return_type, double> ||
		std::is_same_v<return_type, numeric::rational>
	);

	if constexpr (std::is_same_v<return_type, double>) {
		const uint64_t S =
			sum_powers_degrees<G,uint64_t>(g,p,degree_function);
		return static_cast<double>(S)/static_cast<double>(g.get_num_nodes());
	}
	else {
		const numeric::integer S =
			sum_powers_degrees<G,numeric::integer>(g,p,degree_function);
		return numeric::rational(S,g.get_num_nodes());
	}
}


/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a graph as
 * an exact rational value.
 *
 * Computes the \f$p\f$-th moment of degree about zero,
 * \f$\langle k^p \rangle\f$, of a graph using:
 *
 * \f$\langle k^p \rangle = \frac{1}{n} \sum_{i=1}^n k_i^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 */
inline numeric::rational
moment_degree_rational(const graphs::undirected_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::undirected_graph, numeric::rational>
	(g, p, &graphs::undirected_graph::get_degree);
}
/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the degree about 0 as a floating point value.
 */
inline double
moment_degree(const graphs::undirected_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::undirected_graph, double>
	(g, p, &graphs::undirected_graph::get_degree);
}

/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of degree about zero,
 * \f$\langle k^p \rangle\f$, of a graph using:
 *
 * \f$\langle k^p \rangle = \frac{1}{n} \sum_{i=1}^n k_i^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_i\f$ is the
 * in-degree plus the out-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 */
inline numeric::rational
moment_degree_rational(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, numeric::rational>
	(g, p, &graphs::directed_graph::get_degree);
}
/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the degree about 0 as a floating point value.
 */
inline double
moment_degree(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, double>
	(g, p, &graphs::directed_graph::get_degree);
}

/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of in-degree about zero,
 * \f$\langle k_{in}^p \rangle\f$, of a directed graph using:
 *
 * \f$\langle k_{in}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{in, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 */
inline numeric::rational
moment_in_degree_rational(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, numeric::rational>
	(g, p, &graphs::directed_graph::get_in_degree);
}
/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_in_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a floating point value.
 */
inline double
moment_in_degree(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, double>
	(g, p, &graphs::directed_graph::get_in_degree);
}

/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of in-degree about zero,
 * \f$\langle k_{in}^p \rangle\f$, of a directed graph using:
 *
 * \f$\langle k_{in}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{in, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 */
inline numeric::rational
moment_in_degree_rational(const graphs::rooted_tree& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::rooted_tree, numeric::rational>
	(g, p, &graphs::rooted_tree::get_in_degree);
}
/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_in_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a floating point value.
 */
inline double
moment_in_degree(const graphs::rooted_tree& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::rooted_tree, double>
	(g, p, &graphs::rooted_tree::get_in_degree);
}

/**
 * @brief Computes the \f$p\f$-th moment of out-degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of out-degree about zero,
 * \f$\langle k_{out}^p \rangle\f$, of a directed graph using:
 *
 * \f$\langle k_{out}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{out, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The \f$p\f$-th moment of the out-degree about 0 as a rational value.
 */
inline numeric::rational
moment_out_degree_rational(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, numeric::rational>
	(g, p, &graphs::directed_graph::get_out_degree);
}
/**
 * @brief Computes the \f$p\f$-th moment of out-degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_out_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the out-degree about 0 as a floating point
 * value.
 */
inline double
moment_out_degree(const graphs::directed_graph& g, uint64_t p) noexcept {
	return
	moment_degree<graphs::directed_graph, double>
	(g, p, &graphs::directed_graph::get_out_degree);
}


/* -------------------------------------------------------------------------- */


/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * The hubiness coefficient is defined as.
 *
 * \f$h = \frac{ \langle k^2 \rangle - \langle k^2 \rangle_{linear} }
 *				{ \langle k^2 \rangle_{star} - \langle k^2 \rangle_{linear} }\f$,
 *
 * where \f$\langle k^2 \rangle_{star}\f$ and \f$\langle k^2 \rangle_{linear}\f$
 * are the second moment of degree about 0 (see
 * @ref lal::properties::moment_degree_rational) of a star and linear tree
 * respectively.
 *
 * See \cite Ferrer2018a for further details.
 * @param t Input free tree.
 * @returns The hubiness coefficient as a rational value.
 * @pre The tree @e t is a valid tree. Method graphs::free_tree::is_tree returns true.
 * @pre \f$n > 3\f$.
 */
inline numeric::rational hubiness_rational(const graphs::free_tree& t) noexcept {
	const uint64_t n = t.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(t.is_tree());
	assert(n > 3);
#endif

	const numeric::rational k2_tree = moment_degree_rational(t, 2);
	const numeric::rational k2_linear = numeric::rational(4*n - 6, n);
	const numeric::rational k2_star = numeric::rational(n - 1);
	return (k2_tree - k2_linear)/(k2_star - k2_linear);
}

/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * The hubiness coefficient is defined as.
 *
 * \f$h = \frac{ \langle k^2 \rangle - \langle k^2 \rangle_{linear} }
 *				{ \langle k^2 \rangle_{star} - \langle k^2 \rangle_{linear} }\f$,
 *
 * where \f$\langle k^2 \rangle_{star}\f$ and \f$\langle k^2 \rangle_{linear}\f$
 * are the second moment of degree about 0 (see
 * @ref lal::properties::moment_degree_rational) of a star and linear tree respectively.
 *
 * See \cite Ferrer2018a for further details.
 * @param t Input rooted tree.
 * @returns The hubiness coefficient as a rational value.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 * @pre \f$n > 3\f$.
 */
inline numeric::rational hubiness_rational(const graphs::rooted_tree& t) noexcept {
	const uint64_t n = t.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(t.is_rooted_tree());
	assert(n > 3);
#endif

	const numeric::rational k2_tree = moment_degree_rational(t, 2);
	const numeric::rational k2_linear = numeric::rational(4*n - 6, n);
	const numeric::rational k2_star = numeric::rational(n - 1);
	return (k2_tree - k2_linear)/(k2_star - k2_linear);
}

/**
 * @brief Computes the hubiness coefficient as a floating point value.
 *
 * See @ref lal::properties::hubiness_rational for details.
 * @param t Input free tree.
 * @returns The hubiness coefficient as a floating point value.
 * @pre The tree @e t is a valid tree. Method graphs::free_tree::is_tree returns true.
 * @pre \f$n > 3\f$.
 */
inline double hubiness(const graphs::free_tree& t) noexcept {
	const uint64_t n = t.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(t.is_tree());
	assert(n > 3);
#endif

	const double k2_tree = moment_degree(t, 2);
	const double k2_linear = static_cast<double>(4*n - 6)/static_cast<double>(n);
	const double k2_star = static_cast<double>(n - 1);
	return (k2_tree - k2_linear)/(k2_star - k2_linear);
}

/**
 * @brief Computes the hubiness coefficient as a floating point value.
 *
 * See @ref lal::properties::hubiness_rational for details.
 * @param t Input rooted tree.
 * @returns The hubiness coefficient as a floating point value.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 * @pre \f$n > 3\f$.
 */
inline double hubiness(const graphs::rooted_tree& t) noexcept {
	const uint64_t n = t.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(t.is_rooted_tree());
	assert(n > 3);
#endif

	const double k2_tree = moment_degree(t, 2);
	const double k2_linear = static_cast<double>(4*n - 6)/static_cast<double>(n);
	const double k2_star = static_cast<double>(n - 1);
	return (k2_tree - k2_linear)/(k2_star - k2_linear);
}

} // -- namespace properties
} // -- namespace lal
