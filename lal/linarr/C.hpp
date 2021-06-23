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
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/linarr/algorithms_C.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$, computes the number of edge crossings using the
 * identity arrangement \f$\pi_I\f$, i.e., computes \f$C_{\pi_I}(G)\f$ using
 * the algorithm specified by the parameter @e A.
 * @param G Input graph.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t num_crossings(
	const graphs::directed_graph& G,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$, computes the number of edge crossings using the
 * identity arrangement \f$\pi_I\f$, i.e., computes \f$C_{\pi_I}(G)\f$ using
 * the algorithm specified by the parameter @e A.
 * @param G Input graph.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t num_crossings(
	const graphs::undirected_graph& G,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes,
 * computes the number of edge crossings \f$C_{\pi}(G)\f$ using the algorithm
 * specified by the parameter @e A.
 * @param G Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t num_crossings(
	const graphs::directed_graph& G,
	const linear_arrangement& pi,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes,
 * computes the number of edge crossings \f$C_{\pi}(G)\f$ using the algorithm
 * specified by the parameter @e A.
 * @param G Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t num_crossings(
	const graphs::undirected_graph& G,
	const linear_arrangement& pi,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes, computes the number of edge crossings for each of the linear
 * arrangements \f$\pi_i\f$, i.e., computes \f$\{C_{\pi_i}(G)\}_{i=1}^k\f$,
 * using the algorithm specified by the parameter @e A.
 * @param G Input graph.
 * @param pis A list of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(G)\f$.
 * @pre None of the arrangements in @e pis can be empty.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> num_crossings_list(
	const graphs::directed_graph& G,
	const std::vector<linear_arrangement>& pis,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Computes the number of edge crossings in a linear arrangement.
 *
 * Given a graph \f$G\f$ and a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes, computes the number of edge crossings for each of the linear
 * arrangements \f$\pi_i\f$, i.e., computes \f$\{C_{\pi_i}(G)\}_{i=1}^k\f$,
 * using the algorithm specified by the parameter @e A.
 * @param G Input graph.
 * @param pis A list of \f$k\f$ linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns A list \f$L\f$ where \f$L_i = C_{\pi_i}(G)\f$.
 * @pre None of the arrangements in @e pis can be empty.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> num_crossings_list(
	const graphs::undirected_graph& G,
	const std::vector<linear_arrangement>& pis,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ computes the number of edge crossings using the identity
 * arrangement \f$\pi_I\f$, \f$C_{\pi_I}(G)\f$, if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns \f$m^2\f$, where \f$m\f$ is the number of edges of the graph. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& G,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ computes the number of edge crossings using the identity
 * arrangement \f$\pi_I\f$, \f$C_{\pi_I}(G)\f$, if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns \f$m^2\f$, where \f$m\f$ is the number of edges of the graph. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& G,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if it is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns \f$m^2\f$, where \f$m\f$ is the number of edges of the graph. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& G,
	const linear_arrangement& pi,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ and a linear arrangements \f$\pi\f$ of its nodes, returns
 * the number of edge crossings \f$C_{\pi}(G)\f$ if is less than or equal to the
 * given upper bound constant \f$u\f$. In case the number of crossings is greater,
 * returns \f$m^2\f$, where \f$m\f$ is the number of edges of the graph. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& G,
	const linear_arrangement& pi,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ and a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes, computes the number of edge crossings for each of the linear
 * arrangements \f$\pi_i\f$ if that amount is less than or equal to the given
 * upper bound \f$u\f$, i.e., computes \f$\{f_i\}_{i=1}^k\f$, where
 * \f$f_i=C_{\pi_i}(G)\f$ if \f$C_{\pi_i}(G)\le u\f$, or \f$f_i>m^2\f$ if
 * \f$C_{\pi_i}(G)>u\f$. This function uses a modified version of the algorithm
 * specified by the parameter @e A.
 * @param G Input graph.
 * @param pis A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& G,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$ and a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes, computes the number of edge crossings for each of the linear
 * arrangements \f$\pi_i\f$ if that amount is less than or equal to the given
 * upper bound \f$u\f$, i.e., computes \f$\{f_i\}_{i=1}^k\f$, where
 * \f$f_i=C_{\pi_i}(G)\f$ if \f$C_{\pi_i}(G)\le u\f$, or \f$f_i>m^2\f$ if
 * \f$C_{\pi_i}(G)>u\f$. This function uses a modified version of the algorithm
 * specified by the parameter @e A.
 * @param G Input graph.
 * @param pis A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bound Upper bound on the number of crossings.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& G,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$, a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes and a list of upper bounds \f$\{u_i\}_{i=1}^k\f$, computes the
 * number of edge crossings for each of the linear arrangements \f$\pi_i\f$ if
 * that amount is less than or equal to the given upper bound \f$u_i\f$, i.e.,
 * computes \f$\{ f_i \}_{i=1}^k\f$, where \f$f_i=C_{\pi_i}(G)\f$ if
 * \f$C_{\pi_i}(G)\le u_i\f$, or \f$f_i>m^2\f$ if \f$C_{\pi_i}(G)>u_i\f$. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param pis A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds A list of upper bounds on the number of crossings for
 * each linear arrangement.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre There must be as many linear arrangements as upper bounds.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& G,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;
/**
 * @brief Is the number of crossings in the linear arrangement less than a constant?
 *
 * Given a graph \f$G\f$, a list of linear arrangements \f$L=\{\pi_i\}_{i=1}^k\f$
 * of its nodes and a list of upper bounds \f$\{u_i\}_{i=1}^k\f$, computes the
 * number of edge crossings for each of the linear arrangements \f$\pi_i\f$ if
 * that amount is less than or equal to the given upper bound \f$u_i\f$, i.e.,
 * computes \f$\{ f_i \}_{i=1}^k\f$, where \f$f_i=C_{\pi_i}(G)\f$ if
 * \f$C_{\pi_i}(G)\le u_i\f$, or \f$f_i>m^2\f$ if \f$C_{\pi_i}(G)>u_i\f$. This
 * function uses a modified version of the algorithm specified by the parameter
 * @e A.
 * @param G Input graph.
 * @param pis A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @param upper_bounds A list of upper bounds on the number of crossings for
 * each linear arrangement.
 * @param A Algorithm to use to compute the number of crossings.
 * @returns The number of crossings \f$C\f$ if said number is less than or equal
 * to the upper bound. The function returns a value strictly larger than \f$m^2\f$
 * if otherwise.
 * @pre There must be as many linear arrangements as upper bounds.
 * @pre The preconditions of this function depend on the choice of algorithm.
 * See the preconditions of each algorithm in @ref lal::linarr::algorithms_C.
 */
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& G,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A = algorithms_C::ladder
) noexcept;

/* ---------------------------------------- */
/* APPROXIMATION OF THE NUMBER OF CROSSINGS */

/**
 * @brief Predicts the number of crossings.
 *
 * Given a linear arrangement, which determines the length of the edges, predict
 * the number of crossings conditioned by the length of the edges in the
 * linear arrangement. Implementation of \cite Ferrer2014a. If the arrangement
 * is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return Approximation of the number of crossings \f$E_s[C_G\;|\;\delta]\f$.
 */
numeric::rational predicted_num_crossings_rational
(const graphs::undirected_graph& g, const linear_arrangement& pi = {}) noexcept;

/**
 * @brief Predicts the number of crossings.
 *
 * Given a linear arrangement, which determines the length of the edges, predict
 * the number of crossings conditioned by the length of the edges in the
 * linear arrangement. Implementation of \cite Ferrer2014a. If the arrangement
 * is not specified, the identity arrangement is used.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @return Approximation of the number of crossings \f$E_s[C_G\;|\;\delta]\f$.
 */
numeric::rational predicted_num_crossings_rational
(const graphs::directed_graph& g, const linear_arrangement& pi = {}) noexcept;

/**
 * @brief Approximates the number of crossings.
 *
 * See @ref lal::linarr::predicted_num_crossings_rational for details.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The return value is a floating point value.
 */
double predicted_num_crossings
(const graphs::undirected_graph& g, const linear_arrangement& pi = {}) noexcept;

/**
 * @brief Approximates the number of crossings.
 *
 * See @ref lal::linarr::predicted_num_crossings_rational for details.
 * @param g Input graph.
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The return value is a floating point value.
 */
double predicted_num_crossings
(const graphs::directed_graph& g, const linear_arrangement& pi = {}) noexcept;

} // -- namespace linarr
} // -- namespace lal
