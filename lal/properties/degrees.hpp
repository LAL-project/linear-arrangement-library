/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

/* -------------------------------------------------------------------------- */

#if !defined LAL_INTERFACE_PYTHON

/**
 * @brief Generic template function for the sum of degrees.
 *
 * Each degree is raised to a certain power according to the parameter.
 *
 * @tparam value_t Type of function's result (uint64_t, @ref lal::numeric::integer)
 * @tparam graph_t Type of graph.
 * @param g Input graph
 * @param p Power to which degrees must be raised
 * @param degree_function The function that returns the specific degree
 * (in+out, out, in, or other).
 * @return The sum of degrees raised to the input power @e p.
 */
template <class value_t, graphs::Graph graph_t>
[[nodiscard]] value_t sum_powers_degrees(
	const graph_t& g,
	const uint64_t p,
	uint64_t (graph_t::*degree_function)(node) const noexcept
) noexcept
{
	static_assert(
		std::is_same_v<value_t, uint64_t> ||
		std::is_same_v<value_t, numeric::integer>
	);

	// sum of powers
	value_t S(0);
	// variable used to calculate powers
	value_t du(0);

	for (node u = 0; u < g.get_num_nodes(); ++u) {
		const uint64_t deg = (g.*degree_function)(u);
		// calculate the power of the degree 'deg'
		if constexpr (std::is_same_v<value_t, numeric::integer>) {
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

#endif

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
[[nodiscard]] inline numeric::integer sum_powers_degrees_integer(
	const graphs::undirected_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<numeric::integer>(
		g, p, &graphs::undirected_graph::get_degree
	);
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
[[nodiscard]] inline uint64_t
sum_powers_degrees(const graphs::undirected_graph& g, const uint64_t p) noexcept
{
	return sum_powers_degrees<uint64_t>(
		g, p, &graphs::undirected_graph::get_degree
	);
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
[[nodiscard]] inline numeric::integer sum_powers_degrees_integer(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<numeric::integer>(
		g, p, &graphs::directed_graph::get_degree
	);
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
[[nodiscard]] inline uint64_t
sum_powers_degrees(const graphs::directed_graph& g, const uint64_t p) noexcept
{
	return sum_powers_degrees<uint64_t>(
		g, p, &graphs::directed_graph::get_degree
	);
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
[[nodiscard]] inline numeric::integer sum_powers_in_degrees_integer(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<numeric::integer>(
		g, p, &graphs::directed_graph::get_in_degree
	);
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
[[nodiscard]] inline uint64_t sum_powers_in_degrees(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<uint64_t>(
		g, p, &graphs::directed_graph::get_in_degree
	);
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
 * @param t Input tree.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 * @pre Tree @e t is a valid rooted tree.
 */
[[nodiscard]] inline numeric::integer sum_powers_in_degrees_integer(
	const graphs::rooted_tree& t, const uint64_t p
) noexcept
{
	(void(p));

#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	return t.get_num_edges();
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
 * @param t Input tree.
 * @param p Power of degree.
 * @return The sum of in-degrees raised to the \f$p\f$-th power.
 * @pre Tree @e t is a valid rooted tree.
 */
[[nodiscard]] inline uint64_t sum_powers_in_degrees(
	const graphs::rooted_tree& t, [[maybe_unused]] const uint64_t p
) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	return t.get_num_edges();
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
 * @return The sum of out-degrees raised to the \f$p\f$-th power.
 */
[[nodiscard]] inline numeric::integer sum_powers_out_degrees_integer(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<numeric::integer>(
		g, p, &graphs::directed_graph::get_out_degree
	);
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
[[nodiscard]] inline uint64_t sum_powers_out_degrees(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return sum_powers_degrees<uint64_t>(
		g, p, &graphs::directed_graph::get_out_degree
	);
}

/* -------------------------------------------------------------------------- */

#if !defined LAL_INTERFACE_PYTHON

/**
 * @brief Generic template function for the moment of degree about 0.
 *
 * Calculates the \f$p\f$-th moment of degree about 0, where the degree is given
 * by some function.
 *
 * @tparam value_t Type of function's result (double, @ref lal::numeric::rational)
 * @tparam graph_t Type of graph
 * @param g Input graph
 * @param p Power to which degrees must be raised
 * @param degree_function The function that returns the specific degree (full,
 * out, in, or other).
 * @return The \f$p\f$-th moment of degree about 0.
 */
template <class value_t, graphs::Graph graph_t>
[[nodiscard]] value_t moment_degree(
	const graph_t& g,
	const uint64_t p,
	uint64_t (graph_t::*degree_function)(node) const noexcept
) noexcept
{
	static_assert(
		std::is_floating_point_v<value_t> ||
		std::is_same_v<value_t, numeric::rational>
	);

	if constexpr (std::is_floating_point_v<value_t>) {
		const uint64_t S = sum_powers_degrees<uint64_t>(g, p, degree_function);
		return static_cast<value_t>(S) /
			   static_cast<value_t>(g.get_num_nodes());
	}
	else {
		const numeric::integer S =
			sum_powers_degrees<numeric::integer>(g, p, degree_function);
		return numeric::rational(S, g.get_num_nodes());
	}
}

#endif

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
[[nodiscard]] inline numeric::rational moment_degree_rational(
	const graphs::undirected_graph& g, const uint64_t p
) noexcept
{
	return moment_degree<numeric::rational>(
		g, p, &graphs::undirected_graph::get_degree
	);
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
[[nodiscard]] inline double
moment_degree(const graphs::undirected_graph& g, const uint64_t p) noexcept
{
	return moment_degree<double>(g, p, &graphs::undirected_graph::get_degree);
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
[[nodiscard]] inline numeric::rational moment_degree_rational(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return moment_degree<numeric::rational>(
		g, p, &graphs::directed_graph::get_degree
	);
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
[[nodiscard]] inline double
moment_degree(const graphs::directed_graph& g, const uint64_t p) noexcept
{
	return moment_degree<double>(g, p, &graphs::directed_graph::get_degree);
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
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{in, i}\f$ is
 * the in-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 */
[[nodiscard]] inline numeric::rational moment_in_degree_rational(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return moment_degree<numeric::rational>(
		g, p, &graphs::directed_graph::get_in_degree
	);
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
[[nodiscard]] inline double
moment_in_degree(const graphs::directed_graph& g, const uint64_t p) noexcept
{
	return moment_degree<double>(g, p, &graphs::directed_graph::get_in_degree);
}

/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a rooted
 * tree as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of in-degree about zero,
 * \f$\langle k_{in}^p \rangle\f$, of a rooted tree using:
 *
 * \f$\langle k_{in}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{in, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the tree. In this case, this
 * value is trivially independent of the moment \f$p\f$,
 *
 * \f$\langle k_{in}^p \rangle = \frac{n - 1}{n}\f$.
 * @param t Input rooted tree.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a rational value.
 * @pre Tree @e t is a valid rooted tree.
 */
[[nodiscard]] inline numeric::rational moment_in_degree_rational(
	const graphs::rooted_tree& t, [[maybe_unused]] const uint64_t p
) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	const auto n = t.get_num_nodes();
	return numeric::rational(n - 1, n);
}
/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as a floating point value.
 *
 * See @ref lal::properties::moment_in_degree_rational for details.
 * @param t Input rooted tree.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a floating point value.
 * @pre Tree @e t is a valid rooted tree.
 */
inline double moment_in_degree(
	const graphs::rooted_tree& t, [[maybe_unused]] const uint64_t p
) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	const auto n = t.get_num_nodes();
	return static_cast<double>(n - 1) / static_cast<double>(n);
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
 * where \f$n\f$ denotes the number of nodes of the graph and \f$k_{out, i}\f$ is
 * the out-degree of vertex \f$i\f$.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The \f$p\f$-th moment of the out-degree about 0 as a rational value.
 */
[[nodiscard]] inline numeric::rational moment_out_degree_rational(
	const graphs::directed_graph& g, const uint64_t p
) noexcept
{
	return moment_degree<numeric::rational>(
		g, p, &graphs::directed_graph::get_out_degree
	);
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
[[nodiscard]] inline double
moment_out_degree(const graphs::directed_graph& g, const uint64_t p) noexcept
{
	return moment_degree<double>(g, p, &graphs::directed_graph::get_out_degree);
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
[[nodiscard]] numeric::rational
hubiness_rational(const graphs::free_tree& t) noexcept;

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
[[nodiscard]] numeric::rational
hubiness_rational(const graphs::rooted_tree& t) noexcept;

/**
 * @brief Computes the hubiness coefficient as a floating point value.
 *
 * See @ref lal::properties::hubiness_rational for details.
 * @param t Input free tree.
 * @returns The hubiness coefficient as a floating point value.
 * @pre The tree @e t is a valid tree. Method graphs::free_tree::is_tree returns true.
 * @pre \f$n > 3\f$.
 */
[[nodiscard]] double hubiness(const graphs::free_tree& t) noexcept;

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
[[nodiscard]] double hubiness(const graphs::rooted_tree& t) noexcept;

} // namespace properties
} // namespace lal
