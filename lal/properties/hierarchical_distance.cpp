/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

// C++ includ
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/macros/integer_convert.hpp>

namespace lal {
namespace properties {

uint64_t sum_hierarchical_distances(const graphs::rooted_tree& t) noexcept {
	const uint64_t n = t.get_num_nodes();

#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	uint64_t sum_distances = 0;
	detail::data_array<uint64_t> distances(n, 0);

	detail::BFS<graphs::rooted_tree> bfs(t);
	bfs.set_process_neighbour(
	[&](const auto&, const node u, const node v, bool) -> void {
		distances[v] = distances[u] + 1;
		sum_distances += distances[v];
	}
	);
	bfs.start_at(t.get_root());

	return sum_distances;
}

template<typename result>
result MHD(const graphs::rooted_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_rooted_tree());
	assert(t.get_num_nodes() > 1);
#endif

	const auto sum_distances = sum_hierarchical_distances(t);
	if constexpr (std::is_same_v<numeric::rational, result>) {
		return numeric::rational(sum_distances, t.get_num_edges());
	}
	else {
		return detail::to_double(sum_distances)/detail::to_double(t.get_num_edges());
	}
}

numeric::rational mean_hierarchical_distance_rational
(const graphs::rooted_tree& tree)
noexcept
{
	return MHD<numeric::rational>(tree);
}

double mean_hierarchical_distance(const graphs::rooted_tree& tree) noexcept {
	return MHD<double>(tree);
}

} // -- namespace properties
} // -- namespace lal
