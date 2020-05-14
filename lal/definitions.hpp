/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
#include <string>

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
typedef std::vector<position> linearrgmnt;

/// Edge type
typedef std::pair<node, node> edge;
/// Edge pair type.
typedef std::pair<edge,edge> edge_pair;
/// List of nodes.
typedef std::vector<node> neighbourhood;

/// Major version number of the library's current state.
static const std::string __lal_major_verno = "2020";
/// Minor version number of the library's current state.
static const std::string __lal_minor_verno = "03";
/// Patch version number of the library's current state.
static const std::string __lal_patch_verno = "00";

} // -- namespace lal
