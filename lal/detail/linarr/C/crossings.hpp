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
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace detail {

/*
 * This file is a summary of the functions that implement the different
 * algorithms to calculate the number of edge crossings.
 */

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
template <class graph_t>
uint64_t n_C_brute_force
(const graph_t& g, const linear_arrangement& arr)
noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
template <class graph_t>
std::vector<uint64_t> n_C_brute_force
(const graph_t& g, const std::vector<linear_arrangement>& arrs)
noexcept;

/**
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
uint64_t is_n_C_brute_force_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. The upper bound @e upper_bound
 * is applied to all linear arrangements.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. To each arrangement
 * corresponds only one upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds List of constants (each an upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
template <class graph_t>
uint64_t n_C_dynamic_programming
(const graph_t& g, const linear_arrangement& arr)
noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
template <class graph_t>
std::vector<uint64_t> n_C_dynamic_programming(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. The upper bound @e upper_bound
 * is applied to all linear arrangements.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
uint64_t is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. To each arrangement
 * corresponds only one upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. To each arrangement
 * corresponds only one upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds List of constants (each an upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
template <class graph_t>
uint64_t n_C_ladder
(const graph_t& g, const linear_arrangement& arr)
noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
template <class graph_t>
std::vector<uint64_t> n_C_ladder(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
uint64_t is_n_C_ladder_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. The upper bound @e upper_bound
 * is applied to all linear arrangements.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. To each arrangement
 * corresponds only one upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds List of constants (each an upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
template <class graph_t>
uint64_t n_C_stack_based
(const graph_t& g, const linear_arrangement& arr)
noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
template <class graph_t>
std::vector<uint64_t> n_C_stack_based(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
uint64_t is_n_C_stack_based_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. The upper bound @e upper_bound
 * is applied to all linear arrangements.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	uint64_t upper_bound
) noexcept;

/**
 * @brief Fast calculation of \f$C\f$ if it is less than or equal to an upper bound.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns a value strictly larger than the upper bound.
 *
 * This is applied to every linear arrangement in @e pi. To each arrangement
 * corresponds only one upper bound.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arrs List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When one is omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds List of constants (each an upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than the upper
 * bound if otherwise.
 */
template <class graph_t>
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

} // -- namespace detail
} // -- namespace lal
