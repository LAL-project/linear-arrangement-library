/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
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
#include <algorithm>
#include <vector>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

// -----------------------------------------------------------------------------
// head vector -> tree

/**
 * @brief Converts a head vector into a rooted tree.
 *
 * A head vector of an @e n-vertex tree is a list of non-negative integer
 * numbers. The number at position @e i denotes the parent node of the vertex
 * at said position. Value '0' denotes the root. In this case, the vertex
 * corresponding to the value '0' is not labelled as a root.
 *
 * Each tree is formatted as a list of whole, positive numbers (including
 * zero), each representing a node of the tree. The number 0 denotes the root
 * of the tree, and a number at a certain position indicates its parent node.
 * For example, when number 4 is at position 9 it means that node 9 has parent
 * node 4. Therefore, if number 0 is at position 1 it means that node 1 is
 * the root of the tree. A complete example of such a tree's representation
 * is the following
 *
 *       0 3 4 1 6 3
 *
 *
 * which should be interpreted as
 *
 *		(a) predecessor:       0 3 4 1 6 3
 *		(b) node of the tree:  1 2 3 4 5 6
 *
 *
 * Note that lines like these are not valid:
 *
 *		(1) 0 2 2 2 2 2
 *		(2) 2 0 0
 *
 *
 * Line (1) is not valid due to a self-reference in the second position,
 * and (2) is not valid since it contains two '0' (i.e., two roots).
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 */
std::pair<free_tree,node> from_head_vector_to_free_tree
(const head_vector& hv, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts a head vector into a rooted tree.
 *
 * A head vector of an @e n-vertex tree is a list of non-negative integer
 * numbers. The number at position @e i denotes the parent node of the vertex
 * at said position. Value '0' denotes the root. In this case, the vertex
 * corresponding to the value '0' is not labelled as a root.
 *
 * Each tree is formatted as a list of whole, positive numbers (including
 * zero), each representing a node of the tree. The number 0 denotes the root
 * of the tree, and a number at a certain position indicates its parent node.
 * For example, when number 4 is at position 9 it means that node 9 has parent
 * node 4. Therefore, if number 0 is at position 1 it means that node 1 is
 * the root of the tree. A complete example of such a tree's representation
 * is the following
 *
 *       0 3 4 1 6 3
 *
 *
 * which should be interpreted as
 *
 *		(a) predecessor:       0 3 4 1 6 3
 *		(b) node of the tree:  1 2 3 4 5 6
 *
 *
 * Note that lines like these are not valid:
 *
 *		(1) 0 2 2 2 2 2
 *		(2) 2 0 0
 *
 *
 * Line (1) is not valid due to a self-reference in the second position,
 * and (2) is not valid since it contains two '0' (i.e., two roots).
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param hv A head vector as specified above.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
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
 * @param edge_list An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 * @pre The maximum index in the list must be equal to the number of edges in the list.
 */
rooted_tree from_edge_list_to_rooted_tree
(const std::vector<edge>& edge_list, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into a rooted tree.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param edge_list An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 * @pre The maximum index in the list must be equal to the number of edges in the list.
 */
free_tree from_edge_list_to_free_tree
(const std::vector<edge>& edge_list, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into a directed graph.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param edge_list An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 */
directed_graph from_edge_list_to_directed_graph
(const std::vector<edge>& edge_list, bool normalise = true, bool check = true)
noexcept;

/**
 * @brief Converts an edge list into an undirected graph.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param edge_list An edge list.
 * @param normalise Should the graph be normalised?
 * @param check In case the graph is not to be normalised, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 */
undirected_graph from_edge_list_to_undirected_graph
(const std::vector<edge>& edge_list, bool normalise = true, bool check = true)
noexcept;

} // -- namespace graphs
} // -- namespace lal
