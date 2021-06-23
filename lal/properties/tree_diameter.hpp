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

#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace properties {

/**
 * @brief Calculate the diameter of a free tree.
 *
 * The diameter is defined as the longest shortest distance between every pair
 * of vertices. The distance is calculated in number of edges; two adjacent
 * vertices are at a distance 1 from each other. See \cite Harary1969a
 * (pages 24, 35) for further details.
 * @param t Input tree.
 * @returns The diameter of the input tree.
 */
uint64_t tree_diameter(const graphs::free_tree& t) noexcept;

/**
 * @brief Calculate the diameter of a free tree.
 *
 * The diameter is defined as the longest shortest distance between every pair
 * of vertices. The distance is calculated in number of edges; two adjacent
 * vertices are at a distance 1 from each other. See \cite Harary1969a
 * (pages 24, 35) for further details.
 * @param t Input tree.
 * @returns The diameter of the input tree.
 */
uint64_t tree_diameter(const graphs::rooted_tree& t) noexcept;

} // -- namespace properties
} // -- namespace lal
