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
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

// -----------------------------------------------------------------------------
// tree -> head vector

/**
 * @brief Converts a rooted tree into a head vector.
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
 * and (2) not being valid due to containing two '0' (i.e., two roots).
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in
 * disk.
 * @param t A lal::graphs::rooted_tree.
 * @returns The head vector representation of this tree.
 * @pre Rooted tree @e t must be a rooted tree (see @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
head_vector from_tree_to_head_vector(const rooted_tree& t) noexcept;

/**
 * @brief Converts a rooted tree to a head vector.
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
 * and (2) not being valid due to containing two '0' (i.e., two roots).
 *
 * Methods @ref lal::io::read_head_vector read a head vector from a file in disk.
 * @param t A lal::graphs::free_tree.
 * @param r A fictional root to be used to calculate the head vector.
 * @returns The head vector representation of this tree.
 */
head_vector from_tree_to_head_vector(const free_tree& t, node r = 0) noexcept;

// -----------------------------------------------------------------------------
// head vector -> graph

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
 * and (2) not being valid due to containing two '0' (i.e., two roots).
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
 * and (2) not being valid due to containing two '0' (i.e., two roots).
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

} // -- namespace graphs
} // -- namespace lal
