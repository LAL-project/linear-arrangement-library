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
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

// -----------------------------------------------------------------------------
// head vector -> graph

/**
 * @brief Converts a head vector into an undirected graph.
 *
 * See @ref LAL_concepts__head_vector for the definition of head vector.
 *
 * The difference with methods @ref lal::graphs::from_head_vector_to_free_tree and
 * @ref lal::graphs::from_head_vector_to_rooted_tree is that these methods require
 * the head vector to be that of a (free or rooted) tree. This method does not
 * impose any requirement on the head vector.
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::undirected_graph obtained from the head vector.
 */
undirected_graph from_head_vector_to_undirected_graph
(const head_vector& hv, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts a head vector into a directed graph.
 *
 * See @ref LAL_concepts__head_vector for the definition of head vector.
 *
 * The difference with methods @ref lal::graphs::from_head_vector_to_free_tree and
 * @ref lal::graphs::from_head_vector_to_rooted_tree is that these methods require
 * the head vector to be that of a (free or rooted) tree. This method does not
 * impose any requirement on the head vector.
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::directed_graph obtained from the head vector.
 */
directed_graph from_head_vector_to_directed_graph
(const head_vector& hv, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts a head vector into a rooted tree.
 *
 * See @ref LAL_concepts__head_vector for the definition of head vector.
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre The head vector must be of a valid rooted tree.
 */
std::pair<free_tree,node> from_head_vector_to_free_tree
(const head_vector& hv, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts a head vector into a rooted tree.
 *
 * See @ref LAL_concepts__head_vector for the definition of head vector.
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre The head vector must be of a valid rooted tree.
 */
rooted_tree from_head_vector_to_rooted_tree
(const head_vector& hv, bool normalise = true, bool check = true)
noexcept;

// -----------------------------------------------------------------------------
// edge list -> graph

/**
 * @brief Converts an edge list into a rooted tree.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param el An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 * @pre The maximum index in the list must be equal to the number of edges in the list.
 */
rooted_tree from_edge_list_to_rooted_tree
(const edge_list& el, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into a rooted tree.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param el An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 * @pre The maximum index in the list must be equal to the number of edges in the list.
 */
free_tree from_edge_list_to_free_tree
(const edge_list& el, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into a directed graph.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param el An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 */
directed_graph from_edge_list_to_directed_graph
(const edge_list& el, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into an undirected graph.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param el An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 */
undirected_graph from_edge_list_to_undirected_graph
(const edge_list& el, bool normalise = true, bool check = true)
noexcept;

} // -- namespace graphs
} // -- namespace lal
