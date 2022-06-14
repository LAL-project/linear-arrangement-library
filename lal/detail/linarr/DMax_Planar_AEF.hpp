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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
#include <queue>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/linarr/DMax_utils.hpp>
#include <lal/detail/linarr/Dopt_utils.hpp>
#include <lal/detail/linarr/DMax_Projective_AEF.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace planar {

/// Choose a starting vertex to calculate maximum planar sum of edge lengths.
inline lal::node choose_starting_vertex(const lal::graphs::free_tree& t)
noexcept
{
	const uint64_t n = t.get_num_nodes();

	for (lal::node u = 0; u < n; ++u) {
		if (t.get_degree(u) == 1) {
			return t.get_neighbours(u)[0];
		}
	}

	return n + 1;
}

/// A piece of information within u's list
struct sorted_adjacency_list_info {
	/// The child of the parent. The parent node is the node that is
	/// the owner of the list.
	lal::node child; // v

	/// The number of nodes in the tree T^parent_child.
	uint64_t size;

	/// Index of child vertex within the parent's list.
	/// This one is (or should be!) trivial since this is the lal::position that
	/// this struct occupies within the list it belongs to.
	uint64_t index_of_child_within_parents_list; // sigma(u,v)

	/// The index of the parent within the list of 'u'.
	/// This is more difficult to calculate, see code below.
	uint64_t index_of_parent_within_childs_list; // sigma(v,u)

	/// The sum of this size plus all the sizes before it.
	uint64_t partial_sum;
};

/// A tuple used to construct the sorted adjacency list.
struct edge_size_sigma {
	/// Edge (u,v)
	lal::edge e;
	/// Directional size (u,v)
	uint64_t size;
	/// Index of 'v' within list of 'u'.
	std::size_t sigma;

	/// Constructor.
	edge_size_sigma() noexcept : e{}, size{0}, sigma{0} {}
	/// Constructor.
	edge_size_sigma(lal::edge _e, uint64_t _size, std::size_t _sigma) noexcept
		: e{_e}, size{_size}, sigma{_sigma}
	{}
};

/// Useful shorthand for a sorted adjacency list.
typedef std::vector<std::vector<sorted_adjacency_list_info>>
	sorted_adjacency_list;

/// Construct the sorted adjacency list needed to calculate DMax in every vertex.
inline
sorted_adjacency_list make_sorted_adjacency_list(const lal::graphs::free_tree& t)
noexcept
{
	typedef std::pair<lal::edge, uint64_t> edge_size;

	const uint64_t n = t.get_num_nodes();
	const uint64_t m = t.get_num_edges();

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	sorted_adjacency_list M(n);

	// bidirectional sizes
	lal::detail::data_array<edge_size> S(2*m);
	{
	lal::detail::calculate_bidirectional_sizes
		<lal::graphs::free_tree, edge_size*>
		(t, n, 0, S.begin());

	// sort all tuples in bidir_sizes using the size of the subtree
	lal::detail::sorting::counting_sort
		<edge_size, lal::detail::sorting::non_increasing_t>
		(
			S.begin(), S.end(), n, S.size(),
			[](const edge_size& T) -> std::size_t { return std::get<1>(T); }
		);
	}

	// put the sorted bidirectional sizes into an adjacency list
	lal::detail::data_array<edge_size_sigma> J(2*m);
	for (std::size_t idx = 0; idx < S.size(); ++idx) {
		const auto& T = S[idx];

		const auto [u, v] = T.first;
		const uint64_t nv = T.second;
		const uint64_t sigma_u_v = M[u].size();

		M[u].push_back({
			// node identifier
			v,
			// The size of the subtree T^u_v
			nv,
			// position_child_within_parents_list:
			//      The index of 'v' within the list of 'u'
			sigma_u_v,
			// position_parent_within_childs_list:
			//      calculated after M is filled
			0,
			// The sum of all the sizes strictly before this
			(M[u].size() > 0 ? M[u].back().size + M[u].back().partial_sum : 0)
		});

		J[idx] = {{v,u}, n - nv, sigma_u_v};

#if defined DEBUG
		assert(t.has_edge(u,v));
#endif
	}

#if defined DEBUG
	for (lal::node u = 0; u < n; ++u) {
		assert(M[u].size() == t.get_degree(u));
	}
#endif

	// sort all tuples in bidir_idxs using the size of the subtree
	lal::detail::sorting::counting_sort
		<edge_size_sigma, lal::detail::sorting::non_increasing_t>
		(
			J.begin(), J.end(), n, J.size(),
			[](const edge_size_sigma& T) -> std::size_t { return T.size; }
		);

	lal::detail::data_array<std::size_t> I(n, 0);
	for (const auto& [e, suv, sigma_v_u] : J) {
		const auto u = e.first;

		M[u][ I[u] ].index_of_parent_within_childs_list = sigma_v_u;
		++I[u];
	}

	return M;
}

/**
 * @brief Maximum planar arrangement of a free tree.
 *
 * This algorithm calculates DMax on every vertex and keeps track of the maximum.
 * The calculation of DMax on every vertex is done in \f$O(n)\f$ thanks to the
 * adjacency list calculated by function @ref make_sorted_adjacency_list.
 *
 * This function implements the algorithm in \cite Alemany2022d.
 * @tparam make_arrangement Construct a maximum arrangement.
 * @param t Input tree.
 * @returns A pair of cost and maximum linear arrangement.
 */
template <bool make_arrangement>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const lal::graphs::free_tree& t) noexcept
{
	const uint64_t n = t.get_num_nodes();

	if (n == 1) {
		if constexpr (make_arrangement) {
			return {0, linear_arrangement::identity(1)};
		}
		else {
			return 0;
		}
	}
	if (n == 2) {
		if constexpr (make_arrangement) {
			return {1, linear_arrangement::identity(2)};
		}
		else {
			return 1;
		}
	}

	const auto M = make_sorted_adjacency_list(t);

	// starting vertex for the algorithm
	const lal::node starting_vertex = choose_starting_vertex(t);
#if defined DEBUG
	assert(starting_vertex < n);
#endif

	// the value of DMax for all vertices
	lal::detail::data_array<uint64_t> DMax_node(n, 0);

	// calculate DMax for the starting vertex
	{
	lal::graphs::rooted_tree rt(t, starting_vertex);
	rt.calculate_size_subtrees();
	DMax_node[starting_vertex] = projective::AEF<false>(rt);
	}

	// the maximum value and the corresponding node
	uint64_t DMax = DMax_node[starting_vertex];
	lal::node max_node = starting_vertex;

	// calculate the value of DMax for all vertices
	lal::detail::data_array<char> visited(n, 0);
	visited[starting_vertex] = 1;
	std::queue<lal::node> Q;
	Q.push(starting_vertex);

	while (not Q.empty()) {
		const lal::node u = Q.front();
		Q.pop();

		const auto& Mu = M[u];
		for (const auto& [v, s_u_v, sigma_u_v, sigma_v_u, partial_sum_ui] : Mu) {
			if (visited[v] == 1) { continue; }

			const uint64_t s_v_u = n - s_u_v;
			const uint64_t partial_sum_vi = M[v][sigma_v_u].partial_sum;

			DMax_node[v] =
				  DMax_node[u]
				+ (partial_sum_vi + (t.get_degree(v) - (sigma_v_u + 1) + 1)*s_v_u)
				- (partial_sum_ui + (t.get_degree(u) - (sigma_u_v + 1) + 1)*s_u_v)
			;

			visited[v] = 1;
			Q.push(v);

			if (DMax < DMax_node[v]) {
				DMax = DMax_node[v];
				max_node = v;
			}
		}
	}

	// root the tree at a maximizing node
	lal::graphs::rooted_tree rt(t, max_node);
	rt.calculate_size_subtrees();
	return projective::AEF<make_arrangement>(rt);
}

} // -- namespace planar
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
