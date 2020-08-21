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
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace utilities {

/**
 * @brief Isomorphism test for rooted trees.
 *
 * Decides whether the input trees are isomorphic or not. Two trees \f$t_1\f$
 * and \f$t_2\f$ (or graphs in general) are isomorphic if there exists a
 * mapping \f$\phi \;:\; V(t_1) \longrightarrow V(t_2)\f$ such that
 *
 * \f$
 * \forall u,v\in V(t_1) \; (u,v)\in E(t_1) \longleftrightarrow (\phi(u),\phi(v))\in E(t_2)
 * \f$
 *
 * and \f$\phi(r_1)=r_2\f$ where \f$r_1\f$ and \f$r_2\f$ are, respectively,
 * the roots of \f$t_1\f$ and \f$t_2\f$. Note that \f$(u,v)\f$ denotes a directed
 * edge.
 *
 * The algorithm implemented can be found in \cite Aho1974a.
 * @param t1 Input rooted tree.
 * @param t2 Input rooted tree.
 * @return Returns whether the input trees are isomorphic or not. If the rooted
 * trees are not of the same type (see @ref graphs::rooted_tree::rooted_tree_type)
 * the trees are considered to be non-isomorphic.
 */
bool are_trees_isomorphic(const graphs::rooted_tree& t1, const graphs::rooted_tree& t2);

/**
 * @brief Isomorphism test for free trees.
 *
 * Decides whether the input trees are isomorphic or not. Two trees \f$t_1\f$
 * and \f$t_2\f$ (or graphs in general) are isomorphic if there exists a
 * mapping \f$\phi \;:\; V(t_1) \longrightarrow V(t_2)\f$ such that
 *
 * \f$
 * \forall u,v\in V(t_1) \; uv\in E(t_1) \longleftrightarrow \phi(u)\phi(v)\in E(t_2)
 * \f$
 *
 * The algorithm implemented can be found in \cite Aho1974a.
 * @param t1 Input free tree.
 * @param t2 Input free tree.
 * @return Returns whether the input trees are isomorphic or not.
 */
bool are_trees_isomorphic(const graphs::free_tree& t1, const graphs::free_tree& t2);

} // -- namespace utilities
} // -- namespace lal
