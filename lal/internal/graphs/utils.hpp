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

// C++ includes
#include <algorithm>

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
	std::enable_if_t<
		std::is_same_v<graphs::directed_graph, G> ||
		std::is_same_v<graphs::undirected_graph, G>,
	bool> = true
>
inline void get_bool_neighbours(
	const G& g, node u, char *neighs
)
{
	if constexpr (std::is_base_of_v<graphs::directed_graph, G>) {
		const auto& in_u = g.get_in_neighbours(u);
		std::for_each(in_u.begin(), in_u.end(), [&](node v) { neighs[v] = 1; });
		const auto& out_u = g.get_out_neighbours(u);
		std::for_each(out_u.begin(), out_u.end(), [&](node v) { neighs[v] = 1; });
	}
	else if constexpr (std::is_base_of_v<graphs::undirected_graph, G>) {
		const auto& neighs_u = g.get_neighbours(u);
		std::for_each(neighs_u.begin(), neighs_u.end(), [&](node v) { neighs[v] = 1; });
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
		const auto& in_u = g.get_in_neighbours(u);
		std::for_each(in_u.begin(), in_u.end(), [&](node v) { neighs[v] = 1; });
		const auto& out_u = g.get_out_neighbours(u);
		std::for_each(out_u.begin(), out_u.end(), [&](node v) { neighs[v] = 1; });
	}
	else {
		const auto& neighs_u = g.get_neighbours(u);
		std::for_each(neighs_u.begin(), neighs_u.end(), [&](node v) { neighs[v] = 1; });
	}
}

} // -- namespace internal
} // -- namespace lal
