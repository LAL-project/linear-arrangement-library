/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/properties/connected_components.hpp>
#include <lal/detail/graphs/traversal.hpp>

namespace lal {
namespace detail {

template <bool full_structure, class graph_t>
[[nodiscard]] std::conditional_t<
	full_structure,
	properties::connected_components<graph_t>,
	std::vector<graph_t>
>
connected_components(const graph_t& g)
noexcept
{
	const auto n = g.get_num_nodes();
	BFS bfs(g);
	bfs.set_use_rev_edges(true);

	std::unordered_map<node, node> map_nodes_to_current_cc;
	std::vector<node> nodes_current_cc;
	std::size_t num_ccs = 0;

	properties::connected_components<graph_t> all_ccs_full;
	std::vector<graph_t> all_ccs_simple;

	if constexpr (full_structure) {
		all_ccs_full.init(n);
	}

	bfs.set_process_current(
	[&](const auto&, node u) {
		map_nodes_to_current_cc.insert({u, map_nodes_to_current_cc.size()});
		nodes_current_cc.push_back(u);
		if constexpr (full_structure) {
			all_ccs_full.set_node_cc(u, num_ccs);
		}
	}
	);

	for (node u = 0; u < n; ++u) {
		if (bfs.node_was_visited(u)) { continue; }

		nodes_current_cc.clear();
		nodes_current_cc.reserve(n);
		map_nodes_to_current_cc.clear();
		map_nodes_to_current_cc.reserve(n);

		bfs.start_at(u);
		graph_t cc(nodes_current_cc.size());
		for (node v : nodes_current_cc) {

			if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
				for (node w : g.get_out_neighbors(v)) {
					if (v < w) {
						cc.add_edge_bulk(
							map_nodes_to_current_cc[v],
							map_nodes_to_current_cc[w]
						);
					}
				}
				for (node w : g.get_in_neighbors(v)) {
					if (v < w) {
						cc.add_edge_bulk(
							map_nodes_to_current_cc[w],
							map_nodes_to_current_cc[v]
						);
					}
				}
			}
			else if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_t>) {
				for (node w : g.get_neighbors(v)) {
					if (v < w) {
						cc.add_edge_bulk(
							map_nodes_to_current_cc[v],
							map_nodes_to_current_cc[w]
						);
					}
				}
			}
		}

		if constexpr (full_structure) {
			all_ccs_full.add_graph( std::move(cc) );

			for (node w : nodes_current_cc) {
				all_ccs_full.set_label_graph_node_to_cc_node
					(w, map_nodes_to_current_cc[w]);
				all_ccs_full.set_label_cc_node_to_graph_node
					(num_ccs, map_nodes_to_current_cc[w], w);
			}
		}
		else {
			all_ccs_simple.push_back( std::move(cc) );
		}

		++num_ccs;
	}

	if constexpr (full_structure) {
		return all_ccs_full;
	}
	else {
		return all_ccs_simple;
	}
}

} // -- namespace detail
} // -- namespace lal
