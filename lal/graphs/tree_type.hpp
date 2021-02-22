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
#include <cinttypes>
#ifdef SWIG
#include <string>
#else
#include <string_view>
#endif

namespace lal {
namespace graphs {

/**
 * @brief Enumeration of several types of trees.
 *
 * Classifies a tree into not necessarily disjoint classes of trees.
 */
enum class tree_type {
	/**
	 * @brief Linear trees.
	 *
	 * A linear tree has only two leaves, and the rest of the vertices have degree
	 * exactly two. This is, precisely, a path graph.
	 */
	linear,

	/**
	 * @brief Star trees.
	 *
	 * Also star graphs, trees where all vertices but one have degree 1.
	 */
	star,

	/**
	 * @brief Quasi star trees.
	 *
	 * Also quasi star graphs, trees where all vertices but two have degree 1.
	 * One of these two vertices has degree exactly two, the other has degree
	 * at least two.
	 */
	quasistar,

	/**
	 * @brief Bi-star trees.
	 *
	 * These trees are made of two star trees joined by an edge at their centers.
	 */
	bistar,

	/// Caterpillar trees.
	caterpillar,

	/**
	 * @brief Spider trees.
	 *
	 * A spider tree has a unique vertex of degree greater than or equal to 3.
	 * The other vertices have degree 2 or 1 (the leaves).
	 */
	spider,

	// This value must always be the last one.
	/// The tree could not be classified
	none
};

// since SWIG does not wrap string_view, we need a
// different return type for this "to_string" function
/// Converts to a string a value of the enumeration @ref tree_type.
inline
#ifdef SWIG
std::string
#else
constexpr std::string_view
#endif
tree_type_to_string
(const tree_type& tt)
{
	switch (tt) {
		case tree_type::linear: return "linear";
		case tree_type::star: return "star";
		case tree_type::quasistar: return "quasistar";
		case tree_type::bistar: return "bistar";
		case tree_type::caterpillar: return "caterpillar";
		case tree_type::spider: return "spider";
		default: return "none";
	}
}

/// Number of elements within enumeration @ref tree_type.
constexpr std::size_t __tree_type_size =
	1 + static_cast<std::size_t>(tree_type::none);

} // -- namespace graphs
} // -- namespace lal
