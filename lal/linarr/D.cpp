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
 
// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/identity_arrangement.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace linarr {

// -----------------------------------------------------------------------------
/* D */

template <class graph_t, detail::linarr_type arr_type>
uint64_t __sum_length_edges(
	const graph_t& g,
	const detail::linarr_wrapper<arr_type>& arr
)
noexcept
{
	// sum of lengths
	uint64_t l = 0;

	iterators::E_iterator<graph_t> e_it(g);
	for (; not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge_t();

		const auto pu = arr[u];
		const auto pv = arr[v];

		// accumulate length of edge
		l += (pu < pv ? pv - pu : pu - pv);
	}
	return l;
}

uint64_t sum_edge_lengths
(const graphs::directed_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__sum_length_edges(g, detail::linarr_wrapper<detail::identity>(arr))
		:
			__sum_length_edges(g, detail::linarr_wrapper<detail::nonident>(arr))
		);
}

uint64_t sum_edge_lengths
(const graphs::undirected_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__sum_length_edges(g, detail::linarr_wrapper<detail::identity>(arr))
		:
			__sum_length_edges(g, detail::linarr_wrapper<detail::nonident>(arr))
		);
}

// -----------------------------------------------------------------------------
/* MDD */

template <detail::linarr_type arr_type, class graph_t, typename result>
result __MDD(const graph_t& g, const linear_arrangement& arr) noexcept
{
#if defined DEBUG
assert(g.get_num_edges() > 0);
#endif

	const uint64_t D =
		__sum_length_edges(g, detail::linarr_wrapper<arr_type>(arr));

	if constexpr (std::is_same_v<result, numeric::rational>) {
		return numeric::rational(D, g.get_num_edges());
	}
	else {
		return detail::to_double(D)/detail::to_double(g.get_num_edges());
	}
}

numeric::rational mean_dependency_distance_rational
(const graphs::directed_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__MDD<detail::identity, graphs::directed_graph, numeric::rational>
			(g, arr)
		:
			__MDD<detail::nonident, graphs::directed_graph, numeric::rational>
			(g, arr)
		);
}

numeric::rational mean_dependency_distance_rational
(const graphs::undirected_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__MDD<detail::identity, graphs::undirected_graph, numeric::rational>
			(g, arr)
		:
			__MDD<detail::nonident, graphs::undirected_graph, numeric::rational>
			(g, arr)
		);
}

// -----------------------------------------------------------------------------

double mean_dependency_distance
(const graphs::directed_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__MDD<detail::identity, graphs::directed_graph, double>(g, arr)
		:
			__MDD<detail::nonident, graphs::directed_graph, double>(g, arr)
		);
}
double mean_dependency_distance
(const graphs::undirected_graph& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif

	return
		(arr.size() == 0 ?
			__MDD<detail::identity, graphs::undirected_graph, double>(g, arr)
		:
			__MDD<detail::nonident, graphs::undirected_graph, double>(g, arr)
		);
}

} // -- namespace linarr
} // -- namespace lal
