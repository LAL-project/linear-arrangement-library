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

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/iterators/E_iterator.hpp>

namespace lal {
namespace properties {

template <typename graph_t>
bipartite_graph_coloring _coloring(const graph_t& g) noexcept
{
	return detail::color_vertices_graph(g);
}

bipartite_graph_coloring bipartite_coloring(const graphs::undirected_graph& g
) noexcept
{
	return _coloring(g);
}

bipartite_graph_coloring bipartite_coloring(const graphs::directed_graph& g
) noexcept
{
	return _coloring(g);
}

template <typename graph_t>
bool _is_graph_bipartite(const graph_t& g, const bipartite_graph_coloring& c)
	noexcept
{
	iterators::E_iterator it(g);
	while (not it.end()) {
		const auto [u, v] = it.yield_edge();
		if (c.get_color_of(u) == c.get_color_of(v)) {
			return false;
		}
	}
	return true;
}

bool is_graph_bipartite(
	const graphs::undirected_graph& g, const bipartite_graph_coloring& c
) noexcept
{
	return _is_graph_bipartite(g, c);
}

bool is_graph_bipartite(const graphs::undirected_graph& g) noexcept
{
	const auto c = bipartite_coloring(g);
	return _is_graph_bipartite(g, c);
}

bool is_graph_bipartite(
	const graphs::directed_graph& g, const bipartite_graph_coloring& c
) noexcept
{
	return _is_graph_bipartite(g, c);
}

bool is_graph_bipartite(const graphs::directed_graph& g) noexcept
{
	const auto c = bipartite_coloring(g);
	return _is_graph_bipartite(g, c);
}

} // namespace properties
} // namespace lal
