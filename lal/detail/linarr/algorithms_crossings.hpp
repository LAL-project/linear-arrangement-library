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
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace detail {

/*
 * This file is a summary of the functions that implement the different
 * algorithms to calculate the number of edge crossings.
 */

/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
uint64_t n_C_brute_force(
	const graphs::directed_graph& g,
	const linear_arrangement& pi
) noexcept;
uint64_t n_C_brute_force(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi
) noexcept;
/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangement of its nodes, computes by
 * brute force the number of edges that cross in such linear arrangement.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pis List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
std::vector<uint64_t> n_C_brute_force(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;
std::vector<uint64_t> n_C_brute_force(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
uint64_t is_n_C_brute_force_lesseq_than(
	const graphs::directed_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;
uint64_t is_n_C_brute_force_lesseq_than(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangement of its nodes, computes using
 * dynamic programming the number of edges that cross in such linear
 * arrangement. If the arrangement is not specified, the identity arrangement
 * is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
uint64_t n_C_dynamic_programming(
	const graphs::directed_graph& g,
	const linear_arrangement& pi
) noexcept;
uint64_t n_C_dynamic_programming(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi
) noexcept;
/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its nodes, computes
 * using dynamic programming the number of edges that cross in every linear
 * arrangement.
 * @param g Input graph.
 * @param pis List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
std::vector<uint64_t> n_C_dynamic_programming(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;
std::vector<uint64_t> n_C_dynamic_programming(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
uint64_t is_n_C_dynamic_programming_lesseq_than(
	const graphs::directed_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;
uint64_t is_n_C_dynamic_programming_lesseq_than(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a permutation of its nodes, computes using
 * dynamic programming the number of edges that cross in such linear
 * arrangement. If the arrangement is not specified, the identity arrangement
 * is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
uint64_t n_C_ladder(
	const graphs::directed_graph& g,
	const linear_arrangement& pi
) noexcept;
uint64_t n_C_ladder(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi
) noexcept;
/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its nodes, computes
 * using dynamic programming the number of edges that cross in such linear
 * arrangement.
 * @param g Input graph.
 * @param pis List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
std::vector<uint64_t> n_C_ladder(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;
std::vector<uint64_t> n_C_ladder(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
uint64_t is_n_C_ladder_lesseq_than(
	const graphs::directed_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;
uint64_t is_n_C_ladder_lesseq_than(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

// -----------------------------------------------------------------------------

/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a linear arrangements of its nodes, computes
 * using the algorithm by K. Palios and G. Pitsiladis the number of edges that
 * cross in such linear arrangement. If the arrangement is not specified, the
 * identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The number of crossings \f$C\f$.
 */
uint64_t n_C_stack_based(
	const graphs::graph& g,
	const linear_arrangement& pi
) noexcept;
/*
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph, and a list of linear arrangements of its nodes, computes
 * using the algorithm by K. Palios and G. Pitsiladis the number of edges that
 * cross in such linear arrangement. If the arrangement is not specified, the
 * identity arrangement is used.
 * @param g Input graph.
 * @param pis List of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(g)\f$.
 * @pre None of the arrangements is empty.
 */
std::vector<uint64_t> n_C_stack_based(
	const graphs::graph& g,
	const std::vector<linear_arrangement>& pis
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
uint64_t is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const std::vector<linear_arrangement>& pi,
	uint64_t upper_bound
) noexcept;

/*
 * @brief Returns whether the number of crossings is less than a given constant.
 *
 * Given a graph, and a linear arrangement of its nodes, tells by
 * brute force whether the number of edges that cross in such linear arrangement
 * is less than the given constant.
 * If the arrangement is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi Linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Constant (upper bound).
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns \f$m^2+1\f$ if otherwise.
 */
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const std::vector<linear_arrangement>& pi,
	const std::vector<uint64_t>& upper_bounds
) noexcept;

} // -- namespace detail
} // -- namespace lal
