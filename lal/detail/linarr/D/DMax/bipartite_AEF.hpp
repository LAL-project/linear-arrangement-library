/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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

// C++ includes
#include <type_traits>
#include <cstdint>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace bipartite {

/**
 * @brief Maximal Bipartite Arrangement.
 *
 * Algorithm by Alemany, Ferrer-i-Cancho and Esteban \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @tparam graph_t Type of graph. Any subclass of @ref lal::graphs::graph.
 * @param g Input (bipartite) graph.
 * @param c Coloring of the input graph.
 * @returns The cost of a maximal bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement, class graph_t>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const graph_t& g, const properties::bipartite_graph_coloring& c) {
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);

	const auto n = g.get_num_nodes();

	// annoying corner case
	if (n == 1) {
		if constexpr (make_arrangement) {
			return {0, lal::linear_arrangement::identity(n)};
		}
		else {
			return 0;
		}
	}

	data_array<node> vertices_color_1(n - 1);
	std::size_t size_1 = 0;
	data_array<node> vertices_color_2(n - 1);
	std::size_t size_2 = 0;

	{
	const auto first_color = c[0];
	for (lal::node u = 0; u < n; ++u) {
		if (c[u] == first_color) {
			vertices_color_1[size_1++] = u;
		}
		else {
			vertices_color_2[size_2++] = u;
		}
	}

	const auto sort_nodes =
		[&](data_array<node>& nodes, std::size_t s) {
		lal::detail::sorting::counting_sort
		<lal::node, lal::detail::sorting::non_decreasing_t>
		(
			nodes.begin(), nodes.begin() + s,
			n - 1,
			s,
			[&](const lal::node u) -> std::size_t {
				// in directed graphs, this function returns the sum of the
				// in-degree plus the out-degree of the vertex.
				return g.get_degree(u);
			}
		);
	};
	sort_nodes(vertices_color_1, size_1);
	sort_nodes(vertices_color_2, size_2);
	}

	uint64_t D = 0;
	lal::linear_arrangement arr;

	if constexpr (make_arrangement) {
		arr.resize(n);
	}

	lal::position p = 0;
	for (std::size_t i = size_1 - 1; i > 0; --i) {
		const node u = vertices_color_1[i];
		if constexpr (make_arrangement) {
			arr.assign(u, p);
			++p;
		}
		D += (n - p)*g.get_degree(u);
	}

	{
	const node u = vertices_color_1[0];
	if constexpr (make_arrangement) {
		arr.assign(vertices_color_1[0], p);
		++p;
	}
	D += (n - p)*g.get_degree(u);
	}

	for (std::size_t i = 0; i < size_2; ++i) {
		const node u = vertices_color_2[i];
		if constexpr (make_arrangement) {
			arr.assign(u, p);
			++p;
		}
		D -= (n - p)*g.get_degree(u);
	}

	if constexpr (make_arrangement) {
		return {D, std::move(arr)};
	}
	else {
		return D;
	}
}

/**
 * @brief Maximal Bipartite Arrangement.
 *
 * Algorithm by Alemany, Ferrer-i-Cancho and Esteban \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @tparam graph_t Type of graph. Any subclass of @ref lal::graphs::graph.
 * @param g Input (bipartite) graph.
 * @returns The cost of a maximal bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement, class graph_t>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const graph_t& g) {
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);
	const auto c = color_vertices_graph(g);
	return AEF<make_arrangement>(g, c);
}

} // -- namespace bipartite
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
