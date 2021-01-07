/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace properties {

/**
 * @brief Calculate the centre of a rooted tree.
 *
 * Here, "centre" should not be confused with "centroid". The center is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. See \cite Harary1969a
 * for further details.
 * @param t Input tree.
 * @returns Returns a tuple of two values: the nodes in the centre. If the
 * tree has a single central node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 * @pre Method graphs::rooted_tree::is_tree returns true.
 */
std::pair<node, node> get_centre(const graphs::rooted_tree& t);

/**
 * @brief Calculate the centre of a free tree.
 *
 * Here, "centre" should not be confused with "centroid". The center is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. See \cite Harary1969a
 * for further details.
 * @param t Input tree.
 * @returns Returns a tuple of two values: the nodes in the centre. If the
 * tree has a single central node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 * @pre Method graphs::rooted_tree::is_tree returns true.
 */
std::pair<node, node> get_centre(const graphs::free_tree& t);

} // -- namespace properties
} // -- namespace lal
