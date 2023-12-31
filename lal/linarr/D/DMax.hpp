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
#include <lal/linear_arrangement.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace linarr {

/* -------------------------- BIPARTITE CONSTRAINT -------------------------- */

/**
 * @brief Calculates the solution to Bipartite MaxLA as defined in
 * \cite Alemany2024a.
 *
 * It computes a maximum bipartite arrangement of a bipartite graph. This function
 * implements the algorithm described in \cite Alemany2023a.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 * @param g Input undirected graph.
 * @param c Coloring of the input graph.
 * @returns A maximum bipartite arrangement.
 * @pre The input graph @e g is a bipartite graph.
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::undirected_graph& g, const properties::bipartite_graph_coloring& c)
noexcept;
/**
 * @brief Calculates the solution to Bipartite MaxLA as defined in
 * \cite Alemany2024a.
 *
 * It computes a maximum bipartite arrangement of a bipartite graph. This function
 * implements the algorithm described in \cite Alemany2023a.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 * @param g Input undirected graph.
 * @returns A maximum bipartite arrangement.
 * @pre The input graph @e g is a bipartite graph.
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::undirected_graph& g)
noexcept;
/**
 * @brief Calculates the solution to Bipartite MaxLA as defined in
 * \cite Alemany2024a.
 *
 * It computes a maximum bipartite arrangement of a bipartite graph. This function
 * implements the algorithm described in \cite Alemany2023a.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 *
 * This function converts the input directed graph into an undirected graph (see
 * @ref lal::graphs::directed_graph::to_undirected()).
 * @param g Input directed graph.
 * @param c Coloring of the input graph.
 * @returns A maximum bipartite arrangement.
 * @pre The input graph @e g is a bipartite graph (ignoring orientation of edges).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::directed_graph& g, const properties::bipartite_graph_coloring& c)
noexcept;
/**
 * @brief Calculates the solution to Bipartite MaxLA as defined in
 * \cite Alemany2024a.
 *
 * It computes a maximum bipartite arrangement of a bipartite graph. This function
 * implements the algorithm described in \cite Alemany2023a.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of bipartite
 * arrangement.
 *
 * This function converts the input rooted into a free tree (see @ref
 * lal::graphs::directed_graph::to_undirected())
 * @param g Input directed graph.
 * @returns A maximum bipartite arrangement.
 * @pre The input graph @e g is a bipartite graph (ignoring orientation of edges).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::directed_graph& g)
noexcept;

/* ------------------- PROJECTIVE AND PLANAR CONSTRAINTS -------------------- */

/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all planar arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of planar
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2024a.
 * @param t Input free tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::free_tree& t)
noexcept;
/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the planarity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all planar arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of planar
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2024a.
 *
 * This function converts the input rooted tree into a free tree (see @ref
 * lal::graphs::rooted_tree::to_free_tree())
 * @param t Input rooted tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
inline
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::rooted_tree& t)
noexcept
{
	return max_sum_edge_lengths_planar(t.to_free_tree());
}

/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the projectivity
 * constraint at every vertex of the tree.
 *
 * Calculates the maximum sum of edge lengths under the projectivity constraint
 * at every vertex of the tree, that is, the result returned is a list of values
 * \f$\{M_1,M_2,\dots,M_n\}\f$ where \f$M_i\f$ is the maximum sum of edge lengths
 * under projectivity for the tree rooted at the \f$i\f$-th vertex.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of projective
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2024a.
 * @param t Input free tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<std::vector<uint64_t>, node> max_sum_edge_lengths_projective_roots
(const graphs::free_tree& t)
noexcept;
/**
 * @brief Computes the maximum value of \f$D\f$ in trees under the projectivity
 * constraint at every vertex of the tree.
 *
 * Calculates the maximum sum of edge lengths under the projectivity constraint
 * at every vertex of the tree, that is, the result returned is a list of values
 * \f$\{M_1,M_2,\dots,M_n\}\f$ where \f$M_i\f$ is the maximum sum of edge lengths
 * under projectivity for the tree rooted at the \f$i\f$-th vertex.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of projective
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2024a.
 *
 * This function converts the input rooted tree into a free tree (see @ref
 * lal::graphs::rooted_tree::to_free_tree()). Therefore, the root is ignored.
 * @param t Input free tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
inline
std::pair<std::vector<uint64_t>, node> max_sum_edge_lengths_projective_roots
(const graphs::rooted_tree& t)
noexcept
{
	return max_sum_edge_lengths_projective_roots(t.to_free_tree());
}

/**
 * @brief Computes the maximum value of \f$D\f$ in rooted trees under the projectivity
 * constraint.
 *
 * Calculates the maximum value of \f$D\f$ over all projective arrangements of
 * the input tree. This function also returns the linear arrangement that yields
 * the maximum value. The caller can choose the algorithm to calculate such
 * maximum value.
 *
 * See @ref LAL_concepts__linear_arrangement__types for the definition of projective
 * arrangement.
 *
 * This function implements the algorithm described in \cite Alemany2024a.
 * @param t Input rooted tree.
 * @returns The maximum value of \f$D\f$ and a maximum arrangement.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree()).
 */
std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_projective
(const graphs::rooted_tree& t)
noexcept;

} // -- namespace linarr
} // -- namespace lal
