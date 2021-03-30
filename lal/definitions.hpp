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
#include <cstdint>
#include <vector>

namespace lal {

/// Node type.
typedef uint32_t node;
/// Node's position type.
typedef uint32_t position;

/**
 * @brief A linear arrangement of the nodes of a graph.
 *
 * If @e pi is a linear arrangement of @e n nodes:
 * @code
	lal::linearrgmnt pi(n);
 * @endcode
 * then the @e u-th position gives the position of node @e u
 * in the arrangement:
 * @code
	position pu = pi[u];
 * @endcode
 * 
 * For the sake of simplicity, we refer to the arrangement \f$ \pi[i]=i \f$,
 * where \f$i\f$ denotes both the nodes of the graph and a position in the
 * linear arrangement, as the identity arrangement and is denoted by \f$\pi_I\f$.
 */
typedef std::vector<position> linear_arrangement;

/// Edge type
typedef std::pair<node, node> edge;
/// Edge pair type.
typedef std::pair<edge,edge> edge_pair;
/// List of nodes.
typedef std::vector<node> neighbourhood;
/**
 * @brief A head vector representation of a (usually rooted) tree.
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
 */
typedef std::vector<uint32_t> head_vector;

} // -- namespace lal
