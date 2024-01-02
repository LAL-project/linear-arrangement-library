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

// lal includes
#include <lal/linarr/formal_constraints.hpp>
#include <lal/detail/linarr/formal_constraints.hpp>

namespace lal {
namespace linarr {

#define __ident(arr) detail::identity_arr(arr)
#define __nonident(arr) detail::nonident_arr(arr)

template <class graph_t, class arrangement_t>
bool is_bipartite(
	const graph_t& g,
	const properties::bipartite_graph_coloring& c,
	const arrangement_t& arr
)
noexcept
{
	const auto n = g.get_num_nodes();
	std::size_t num_changes = 0;
	lal::position_t p = 1ull;
	while (p < n and num_changes <= 1) {
		num_changes += c.get_color_of(arr[p]) != c.get_color_of(arr[p - 1ull]);
		++p;
	}
	return true;
}

bool is_bipartite(
	const graphs::undirected_graph& g,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(is_arrangement(g, arr));
#endif

	if (arr.size() == 0) { return is_bipartite(g, c, __ident(arr)); }
	return is_bipartite(g, c, __nonident(arr));
}

bool is_bipartite(
	const graphs::directed_graph& g,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(is_arrangement(g, arr));
#endif

	if (arr.size() == 0) { return is_bipartite(g, c, __ident(arr)); }
	return is_bipartite(g, c, __nonident(arr));
}

bool is_bipartite(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(is_arrangement(t, arr));
#endif

	if (arr.size() == 0) { return is_bipartite(t, c, __ident(arr)); }
	return is_bipartite(t, c, __nonident(arr));
}

bool is_bipartite(
	const graphs::rooted_tree& t,
	const properties::bipartite_graph_coloring& c,
	const linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(is_arrangement(t, arr));
#endif

	if (arr.size() == 0) { return is_bipartite(t, c, __ident(arr)); }
	return is_bipartite(t, c, __nonident(arr));
}

bool is_root_covered(const graphs::rooted_tree& rt, const linear_arrangement& arr)
noexcept
{
	return
		(arr.size() == 0 ?
			is_root_covered(rt, detail::identity_arr(arr))
		:
			is_root_covered(rt, detail::nonident_arr(arr))
		);
}

} // -- namespace linarr
} // -- namespace lal
