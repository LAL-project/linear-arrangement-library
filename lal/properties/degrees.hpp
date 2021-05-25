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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

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
numeric::rational
moment_degree_rational(const graphs::undirected_graph& g, uint32_t p) noexcept;
/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref lal::properties::moment_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the degree about 0 as a floating point value.
 */
double moment_degree(const graphs::undirected_graph& g, uint32_t p) noexcept;

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
numeric::rational
moment_degree_rational(const graphs::directed_graph& g, uint32_t p) noexcept;
/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref lal::properties::moment_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the degree about 0 as a floating point value.
 */
double moment_degree(const graphs::directed_graph& g, uint32_t p) noexcept;

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
numeric::rational
moment_degree_in_rational(const graphs::directed_graph& g, uint32_t p) noexcept;
/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref lal::properties::moment_degree_in_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the in-degree about 0 as a floating point value.
 */
double moment_degree_in(const graphs::directed_graph& g, uint32_t p) noexcept;

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
numeric::rational
moment_degree_out_rational(const graphs::directed_graph& g, uint32_t p) noexcept;
/**
 * @brief Computes the \f$p\f$-th moment of out-degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref lal::properties::moment_degree_out_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @returns The \f$p\f$-th moment of the out-degree about 0 as a floating point
 * value.
 */
double moment_degree_out(const graphs::directed_graph& g, uint32_t p)noexcept;

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
numeric::rational hubiness_rational(const graphs::free_tree& t) noexcept;

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
numeric::rational hubiness_rational(const graphs::rooted_tree& t) noexcept;

/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * See @ref lal::properties::hubiness_rational for details.
 * @param t Input free tree.
 * @returns The hubiness coefficient as a floating point value.
 * @pre The tree @e t is a valid tree. Method graphs::free_tree::is_tree returns true.
 * @pre \f$n > 3\f$.
 */
double hubiness(const graphs::free_tree& t) noexcept;

/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * See @ref lal::properties::hubiness_rational for details.
 * @param t Input rooted tree.
 * @returns The hubiness coefficient as a floating point value.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 * @pre \f$n > 3\f$.
 */
double hubiness(const graphs::rooted_tree& t) noexcept;

} // -- namespace properties
} // -- namespace lal
