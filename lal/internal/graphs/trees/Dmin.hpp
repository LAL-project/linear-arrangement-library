/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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

#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace internal {

/*
 * t: the rooted tree.
 * M: adjacency matrix of the tree with extra information: for each vertex,
 *		attach an integer that represents the size of the subtree rooted
 *		at that vertex. Each adjacency list is sorted INCREASINGLY by that size.
 * r: the vertex root of the subtree whose interval is to be made
 * data: the interval of every vertex.
 * 		data[v][p] = u <-> vertex 'u' is at position 'p' of vertex 'v's interval
 *
 * Returns the sum of the length of the edges incident to vertex 'r' plus
 * the length of the anchor of the edge from 'r' to its parent. Such length
 * is defined as the number of vertices to the left of 'r' if 'r_place'
 * is RIGHT_PLACE, or as the number of vertices to the right of 'r' if
 * 'r_place' is LEFT_PLACE.
 *
 * See implementation for all details of this method.
 */
uint32_t Dmin_Pr__optimal_interval_of(
	const graphs::rooted_tree& t,
	const std::vector<std::vector<std::pair<lal::node,uint32_t>>>& M,
	node r,
	std::vector<std::vector<node>>& data
);

/*
 * @brief Algorithm to calculate optimal projective arrangements of rooted trees.
 *
 * Computes an optimal projective linear arrangement for rooted trees.
 * A projective linear arrangement is an arrangement in which there are
 * no edge crossings and the root is not covered by any edge.
 *
 * This function implements Gildea and Temperley's algorithm published in \cite Gildea2007a.
 *
 * @param t Input rooted tree.
 * @pre The object @e t must be a valid rooted tree (see @ref graphs::rooted_tree::is_rooted_tree).
 * @pre The tree must have the size of its subtrees calculated (see
 * @ref graphs::rooted_tree::size_subtrees_valid).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Projective
(const graphs::rooted_tree& t);

/*
 * @brief Algorithm to calculate optimal planar arrangements of free trees.
 *
 * Computes an optimal planar linear arrangement for free trees.
 * A planar linear arrangement is an arrangement in which there are
 * no edge crossings.
 *
 * This function implements an algorithm first published by Iordanskii in
 * \cite Iordanskii1987a and later by Hochberg and Stallmann's in \cite Hochberg2003a.
 *
 * @param t Input tree.
 * @pre The object @e t must be a tree (see @ref graphs::free_tree::is_tree).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Planar
(const graphs::free_tree& t);

/*
 * @brief Algorithm to calculate unconstrained optimal linearization of free trees.
 *
 * Computes an unconstrained optimal linear arrangement of a free tree and
 * the value of its cost, i.e., the sum of the lengths of the edges.
 *
 * This function implements Yossi Shiloach's algorithm published in \cite Shiloach1979a.
 * The implementation of this algorithm uses the corrections published
 * in \cite Esteban2017a.
 *
 * @pre The object must be a tree (see @ref graphs::rooted_tree::is_tree).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Unconstrained_YS
(const graphs::free_tree& t);

/*
 * @brief Algorithm to calculate unconstrained optimal linearization of free trees.
 *
 * Computes an unconstrained optimal linear arrangement of a free tree and
 * the value of its cost, i.e., the sum of the lengths of the edges.
 *
 * This function implements Fan Chung's algorithm published in \cite Chung1984a.
 *
 * @pre The object must be a tree (see @ref graphs::rooted_tree::is_tree).
 */
std::pair<uint32_t, linear_arrangement> Dmin_Unconstrained_FC
(const graphs::free_tree& t);

} // -- namespace internal
} // -- namespace lal
