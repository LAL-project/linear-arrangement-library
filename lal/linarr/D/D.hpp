/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the sum of the length of the edges in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a linear arrangement of its nodes \f$\pi\f$, computes
 * the sum of the length of the graph's edges in the arrangement. Formally,
 * this function computes the value \f$D_{\pi}(G)=\sum_{uv\in E(G)} |\pi(u) - \pi(v)|\f$.
 *
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The sum of edge lengths \f$D\f$.
 */
uint64_t sum_edge_lengths
(const graphs::directed_graph& g, const linear_arrangement& pi = {})
noexcept;
/**
 * @brief Computes the sum of the length of the edges in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a linear arrangement of its nodes \f$\pi\f$, computes
 * the sum of the length of the graph's edges in the arrangement. Formally,
 * this function computes the value \f$D_{\pi}(G)=\sum_{uv\in E(G)} |\pi(u) - \pi(v)|\f$.
 *
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The sum of edge lengths \f$D\f$.
 */
uint64_t sum_edge_lengths
(const graphs::undirected_graph& g, const linear_arrangement& pi = {})
noexcept;

/* MDD */

/**
 * @brief Computes the mean dependency distance \f$MDD\f$ as an exact rational value.
 *
 * Given a graph \f$G\f$ and a linear arrangement of its nodes \f$\pi\f$, computes
 * the average edge length, or the mean dependency distance (see \cite Jing2015a).
 * Formally, it computes \f$\frac{D_{\pi}(G)}{|E(G)|}\f$. See function @ref
 * sum_edge_lengths for further details on \f$D_{\pi}(G)\f$.
 *
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns Jing's and Liu's \f$MDD\f$.
 * @pre \f$m > 0\f$.
 */
numeric::rational mean_dependency_distance_rational
(const graphs::directed_graph& g, const linear_arrangement& pi = {})
noexcept;
/**
 * @brief Computes the mean dependency distance \f$MDD\f$ as an exact rational value.
 *
 * Given a graph \f$G\f$ and a linear arrangement of its nodes \f$\pi\f$, computes
 * the average edge length, or the mean dependency distance (see \cite Jing2015a).
 * Formally, it computes \f$\frac{D_{\pi}(G)}{|E(G)|}\f$. See function @ref
 * sum_edge_lengths for further details on \f$D_{\pi}(G)\f$.
 *
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns Jing's and Liu's \f$MDD\f$.
 * @pre \f$m > 0\f$.
 */
numeric::rational mean_dependency_distance_rational
(const graphs::undirected_graph& g, const linear_arrangement& pi = {})
noexcept;

/**
 * @brief Computes the mean dependency distance \f$MDD\f$ as a floating point value.
 *
 * See @ref lal::linarr::mean_dependency_distance_rational for details.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The return value is a floating point value.
 * @pre \f$m > 0\f$.
 */
double mean_dependency_distance
(const graphs::directed_graph& g, const linear_arrangement& pi = {})
noexcept;
/**
 * @brief Computes the mean dependency distance \f$MDD\f$ as a floating point value.
 *
 * See @ref lal::linarr::mean_dependency_distance_rational for details.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The return value is a floating point value.
 * @pre \f$m > 0\f$.
 */
double mean_dependency_distance
(const graphs::undirected_graph& g, const linear_arrangement& pi = {})
noexcept;

} // -- namespace linarr
} // -- namespace lal
