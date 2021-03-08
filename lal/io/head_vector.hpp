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
#include <optional>
#include <string>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace io {

// **DEVELOPER NOTE**
// These functions were not implemented as a templated function in this header
// file so as to avoid including <filesystem> and <ifstream> in this header.

// **DEVELOPER NOTE**
// These function's documentation has to be updated manually in the io.i
// interface file.

/**
 * @brief Reads a free tree in head vector format.
 *
 * A head vector of an @e n-vertex tree is a list of non-negative integer numbers.
 * The number at position @e i denotes the parent node of the vertex at said
 * position. Value '0' denotes the root. In this case, the vertex corresponding
 * to the value '0' is not labelled as a root. See method @ref lal::io::read_head_vector
 * for further details.
 *
 * The current contents of the graph will be cleared and replaced by the contents
 * of the file.
 * @param filename Name of the file.
 * @param norm Should the tree be normalised?
 * See @ref lal::graphs::free_tree::is_normalised()
 * @param check_norm If the graph is not to be normalised check whether or
 * not the graph read is normalised.
 * @returns Nothing in case the file does not exist. A free tree if otherwise.
 */
std::optional<graphs::free_tree> read_head_vector_free_tree
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;
/**
 * @brief Reads a rooted tree in head vector format.
 *
 * A head vector of an @e n-vertex tree is a list of non-negative integer numbers.
 * The number at position @e i denotes the parent node of the vertex at said
 * position. Value '0' denotes the root. See method @ref lal::io::read_head_vector
 * for further details.
 *
 * The current contents of the graph will be cleared and replaced by
 * the contents of the file.
 * @param filename Name of the file.
 * @param norm Should the graph be normalised? See @ref graphs::graph::is_normalised()
 * @param check_norm If the graph is not to be normalised check whether or
 * not the graph read is normalised.
 * @returns Nothing in case the file does not exist. A free tree if otherwise.
 */
std::optional<graphs::rooted_tree> read_head_vector_rooted_tree
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;

/**
 * @brief Reads a rooted tree in head vector format.
 *
 * A head vector of an @e n-vertex tree is a list of non-negative integer numbers.
 * The number at position @e i denotes the parent node of the vertex at said
 * position. Value '0' denotes the root. In this case, the vertex corresponding
 * to the value '0' is not labelled as a root.
 *
 * Each tree is formatted as a list of whole, positive numbers (including zero),
 * each representing a node of the tree. The number 0 denotes the root of the
 * tree, and a number at a certain position indicates its parent node. For example,
 * when number 4 is at position 9 it means that node 9 has parent node 4.
 * Therefore, if number 0 is at position 1 it means that node 1 is the root of
 * the tree. A complete example of such a tree's representation is the following
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
 * Line (1) is not valid due to a self-reference in the second position, and (2)
 * not being valid due to containing two '0' (i.e., two roots).
 *
 * The current contents of the graph will be cleared and replaced by
 * the contents of the file.
 * @param filename Name of the file.
 * @param norm Should the graph be normalised? See @ref graphs::graph::is_normalised()
 * @param check_norm If the graph is not to be normalised check whether or
 * not the graph read is normalised.
 * @returns Nothing in case the file does not exist. A free tree if otherwise.
 */
template<
	class T,
	std::enable_if_t<
		std::is_base_of_v<graphs::free_tree, T> ||
		std::is_base_of_v<graphs::rooted_tree, T>,
		bool
	> = true
>
std::optional<T> read_head_vector
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept
{
	if constexpr (std::is_same_v<graphs::free_tree, T>) {
		return read_head_vector_free_tree(filename, norm, check_norm);
	}
	else if constexpr (std::is_same_v<graphs::rooted_tree, T>) {
		return read_head_vector_rooted_tree(filename, norm, check_norm);
	}
}

} // -- namespace io
} // -- namespace lal
