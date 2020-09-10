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
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace internal {

/* @brief Retrieves the neighbours of a node in an undirected graph as a
 * list of 0-1 values.
 *
 * Sets to 1 the positions in @e neighs that correspond to the nodes
 * neighours of @e u.
 * @param g Input graph.
 * @param u Input node.
 * @param neighs 0-1 list of neighbours of @e u in @e g.
 * @pre The contents of @e neighs must be all 0 (or false).
 */
template<
	class G,
	// this method can't be used by objects of class 'graphs::graph'
	typename std::enable_if<!std::is_same<graphs::graph, G>::value, int>::type = 0
>
inline void get_bool_neighbours(
	const G& g, node u, char *neighs
)
{
	for (const node v : g.get_out_neighbours(u)) {
		neighs[v] = 1;
	}
	if constexpr (std::is_base_of<graphs::directed_graph, G>::value) {
	for (const node v : g.get_out_neighbours(u)) {
		neighs[v] = 1;
	}
	}
}

/* @brief Retrieves the neighbours of a node in an undirected graph as a
 * list of 0-1 values.
 *
 * Sets to 1 the positions in @e neighs that correspond to the nodes
 * neighours of @e u.
 * @param g Input graph.
 * @param u Input node.
 * @param neighs 0-1 list of neighbours of @e u in @e g.
 * @pre The contents of @e neighs must be all 0 (or false).
 */
inline void get_bool_neighbours(
	const graphs::graph& g, node u, char *neighs
)
{
	if (g.is_directed()) {
		for (const node v : g.get_in_neighbours(u)) {
			neighs[v] = 1;
		}
		for (const node v : g.get_out_neighbours(u)) {
			neighs[v] = 1;
		}
	}
	else {
		for (const node v : g.get_neighbours(u)) {
			neighs[v] = 1;
		}
	}
}

} // -- namespace internal
} // -- namespace lal
