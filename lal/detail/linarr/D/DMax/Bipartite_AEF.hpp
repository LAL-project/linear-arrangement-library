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

// C++ includes
#include <type_traits>
#include <cstdint>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/linarr/D/bipartite_opt_utils.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace bipartite {

/**
 * @brief Maximum Bipartite Arrangement.
 *
 * Algorithm by Alemany, Ferrer-i-Cancho and Esteban \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @tparam graph_t Type of graph. Any subclass of @ref lal::graphs::graph.
 * @param g Input (bipartite) graph.
 * @param c Bipartite coloring of the input graph.
 * @returns The cost of a maximum bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement, class graph_t>
[[nodiscard]] std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const graph_t& g, const properties::bipartite_graph_coloring& c) noexcept {
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);
	return
		bipartite_opt_utils::optimal_bipartite_arrangement_AEF
		<make_arrangement, detail::sorting::non_decreasing_t>
		(g, c);
}

/**
 * @brief Maximum Bipartite Arrangement.
 *
 * Algorithm by Alemany, Ferrer-i-Cancho and Esteban \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @tparam graph_t Type of graph. Any subclass of @ref lal::graphs::graph.
 * @param g Input (bipartite) graph.
 * @returns The cost of a maximum bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement, class graph_t>
[[nodiscard]] std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const graph_t& g) noexcept {
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);
	const auto c = color_vertices_graph(g);
	return AEF(g, c);
}

} // -- namespace bipartite
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
