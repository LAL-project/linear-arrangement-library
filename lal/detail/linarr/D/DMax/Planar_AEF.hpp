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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
#include <queue>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/linarr/D/DMax/utils.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>
#include <lal/detail/linarr/D/DMax/Projective_AEF.hpp>
#include <lal/detail/type_traits/conditional_list.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace planar {

/// A piece of information within u's list
struct sorted_adjacency_list_info {
	/// The child of the parent. The parent node is the node that is
	/// the owner of the list.
	node child; // v

	/// The number of nodes in the tree T^parent_child.
	uint64_t size;

	/// Index of child vertex within the parent's list.
	/// This one is (or should be!) trivial since this is the position that
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
	edge e;
	/// Directional size (u,v)
	uint64_t size;
	/// Index of 'v' within list of 'u'.
	std::size_t sigma;

	/// Constructor.
	edge_size_sigma() noexcept
		: e{},
		  size{0},
		  sigma{0}
	{ }
	/// Constructor.
	edge_size_sigma(
		const edge _e, const uint64_t _size, const std::size_t _sigma
	) noexcept
		: e{_e},
		  size{_size},
		  sigma{_sigma}
	{ }
};

/// Useful shorthand for a sorted adjacency list.
typedef std::vector<std::vector<sorted_adjacency_list_info>>
	sorted_adjacency_list;

/// Construct the  for every vertex in the tree. .
/**
 * @brief Sorted adjacency list needed to calculate the maximum sum of edge legnths.
 *
 * This adjacency list is needed in function @ref all_max_sum_lengths_values.
 * @param t Input free tree.
 * @returns The appropriate sorted adjacency list.
 */
[[nodiscard]] inline sorted_adjacency_list
make_sorted_adjacency_list(const graphs::free_tree& t) noexcept
{
	typedef std::pair<edge, uint64_t> edge_size;

	const uint64_t n = t.get_num_nodes();
	const uint64_t m = n - 1;

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	sorted_adjacency_list M(n);

	// bidirectional sizes
	array<edge_size> S(2 * m);
	{
		calculate_bidirectional_sizes(t, n, 0, S.begin());

		// sort all tuples in bidir_sizes using the size of the subtree
		sorting::counting_sort<edge_size, sorting::sort_type::non_increasing>(
			S.begin(),
			S.end(),
			n,
			S.size(),
			[](const edge_size& T) -> std::size_t
			{
				return std::get<1>(T);
			}
		);
	}

	// put the sorted bidirectional sizes into an adjacency list
	array<edge_size_sigma> J(2 * m);
	for (std::size_t idx = 0; idx < S.size(); ++idx) {
		const auto& T = S[idx];

		const auto [u, v] = T.first;
		const uint64_t nv = T.second;
		const uint64_t sigma_u_v = M[u].size();

		M[u].push_back({// node identifier
						v,
						// The size of the subtree T^u_v
						nv,
						// index_of_child_within_parents_list:
						//      The index of 'v' within the list of 'u'
						sigma_u_v,
						// index_of_parent_within_childs_list:
						//      calculated after M is filled
						0,
						// The sum of all the sizes including this size
						nv + (M[u].size() > 0 ? M[u].back().partial_sum : 0)
		});

		J[idx] = {{v, u}, n - nv, sigma_u_v};

#if defined DEBUG
		assert(t.has_edge(u, v));
#endif
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(M[u].size() == t.get_degree(u));
	}
#endif

	// sort all tuples in bidir_idxs using the size of the subtree
	sorting::counting_sort<edge_size_sigma, sorting::sort_type::non_increasing>(
		J.begin(),
		J.end(),
		n,
		J.size(),
		[](const edge_size_sigma& T) -> std::size_t
		{
			return T.size;
		}
	);

	array<std::size_t> I(n, 0);
	for (const auto& [e, suv, sigma_v_u] : J) {
		const auto u = e.first;

		M[u][I[u]].index_of_parent_within_childs_list = sigma_v_u;
		++I[u];
	}

	return M;
}

/**
 * @brief All return types as enumeration values.
 *
 * For function @ref all_max_sum_lengths_values.
 */
enum class return_type_all_maxs {
	/// Return both the set of max projective values at every vertex and the vertex
	/// that maximizes the maximum projective.
	DMax_value_vertex_and_max_root,
	/// Return only the max projective values for every vertex of the tree.
	DMax_value_vertex,
	/// Return only a vertex that maximizes the maximum projective.
	max_root
};

/**
 * @brief Maximum planar arrangement of a free tree.
 *
 * This algorithm calculates the maximum sum of edge lengths on every vertex and
 * keeps track of the maximum. The calculation of the maximum sum of edge lengths
 * on every vertex is done in \f$O(n)\f$ thanks to the adjacency list calculated
 * by function @ref make_sorted_adjacency_list.
 *
 * This function implements the algorithm in \cite Alemany2024a.
 * @tparam res_type The type of result to return. See @ref return_type_all_maxs
 * for details.
 * @param t Input tree.
 * @returns Depending of the value of @e res_type, a list of values, a p
 */
template <return_type_all_maxs res_type>
[[nodiscard]] conditional_list_t<
	bool_sequence<
		res_type == return_type_all_maxs::DMax_value_vertex_and_max_root,
		res_type == return_type_all_maxs::DMax_value_vertex,
		res_type == return_type_all_maxs::max_root>,
	type_sequence<
		std::pair<std::vector<uint64_t>, uint64_t>,
		std::vector<uint64_t>,
		uint64_t>>
all_max_sum_lengths_values(const graphs::free_tree& t) noexcept
{
	constexpr bool calculate_max_root =
		res_type == return_type_all_maxs::DMax_value_vertex_and_max_root or
		res_type == return_type_all_maxs::max_root;

	const uint64_t n = t.get_num_nodes();

	// the value of DMax for all vertices
	std::vector<uint64_t> DMax_per_vertex(n, 0);

	if (n <= 1) {
		if (n == 1) {
			DMax_per_vertex[0] = 0;
		}
		if constexpr (res_type ==
					  return_type_all_maxs::DMax_value_vertex_and_max_root) {
			return {std::move(DMax_per_vertex), 0};
		}
		else if constexpr (res_type ==
						   return_type_all_maxs::DMax_value_vertex) {
			return DMax_per_vertex;
		}
		else if constexpr (res_type == return_type_all_maxs::max_root) {
			return 0;
		}
	}
	if (n == 2) {
		DMax_per_vertex[0] = 1;
		DMax_per_vertex[1] = 1;
		if constexpr (res_type ==
					  return_type_all_maxs::DMax_value_vertex_and_max_root) {
			return {std::move(DMax_per_vertex), 0};
		}
		else if constexpr (res_type ==
						   return_type_all_maxs::DMax_value_vertex) {
			return DMax_per_vertex;
		}
		else if constexpr (res_type == return_type_all_maxs::max_root) {
			return 0;
		}
	}
	[[assume(n >= 3)]];

	const auto M = make_sorted_adjacency_list(t);

	// starting vertex for the algorithm
	const node starting_vertex = 0;
#if defined DEBUG
	assert(starting_vertex < n);
#endif

	// calculate DMax for the starting vertex
	{
		graphs::rooted_tree rt(t, starting_vertex);
		rt.calculate_size_subtrees();
		DMax_per_vertex[starting_vertex] = projective::AEF<false>(rt);
	}

	// the maximum value and the corresponding node
	uint64_t max_DMax = DMax_per_vertex[starting_vertex];
	node max_root = starting_vertex;

	// calculate the value of DMax for all vertices
	array<char> visited(n, 0);
	visited[starting_vertex] = 1;
	std::queue<node> Q;
	Q.push(starting_vertex);

	while (not Q.empty()) {
		const node u = Q.front();
		Q.pop();

		const auto& Mu = M[u];
		for (const auto& [v, s_u_v, sigma_u_v, sigma_v_u, partial_sum_ui] :
			 Mu) {
			if (visited[v] == 1) {
				continue;
			}

			const uint64_t s_v_u = n - s_u_v;
			const uint64_t partial_sum_vi = M[v][sigma_v_u].partial_sum;

			DMax_per_vertex[v] =
				DMax_per_vertex[u] +
				(partial_sum_vi + (t.get_degree(v) - (sigma_v_u + 1)) * s_v_u) -
				(partial_sum_ui + (t.get_degree(u) - (sigma_u_v + 1)) * s_u_v);

			visited[v] = 1;
			Q.push(v);

			if constexpr (calculate_max_root) {
				if (max_DMax < DMax_per_vertex[v]) {
					max_DMax = DMax_per_vertex[v];
					max_root = v;
				}
			}
		}
	}

	if constexpr (res_type ==
				  return_type_all_maxs::DMax_value_vertex_and_max_root) {
		return {std::move(DMax_per_vertex), max_root};
	}
	else if constexpr (res_type == return_type_all_maxs::DMax_value_vertex) {
		return DMax_per_vertex;
	}
	else if constexpr (res_type == return_type_all_maxs::max_root) {
		return max_root;
	}
}

/**
 * @brief Maximum planar arrangement of a free tree.
 *
 * This algorithm calculates the maximum sum of edge lengths on every vertex and
 * keeps track of the maximum. The calculation of DMax on every vertex is done
 * in \f$O(n)\f$ thanks to the adjacency list calculated by function
 * @ref make_sorted_adjacency_list.
 *
 * This function calls @ref all_max_sum_lengths_values, which implements the algorithm
 * described in \cite Alemany2024a.
 * @tparam make_arrangement Construct a maximum arrangement.
 * @param t Input tree.
 * @returns A pair of cost and maximum linear arrangement.
 */
template <bool make_arrangement>
[[nodiscard]] std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t>
AEF(const graphs::free_tree& t) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Alemany2024a);
#endif

	const uint64_t n = t.get_num_nodes();

	// in case the tree is caterpillar and the arrangement is
	// not required then simply apply the formula
	if constexpr (not make_arrangement) {
		if (t.is_tree_type_valid() and
			t.is_of_tree_type(graphs::tree_type::caterpillar)) {
			return (n * (n - 1)) / 2;
		}
	}

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

	const node max_root =
		all_max_sum_lengths_values<return_type_all_maxs::max_root>(t);

	// root the tree at a maximizing node
	graphs::rooted_tree rt(t, max_root);
	rt.calculate_size_subtrees();
	return projective::AEF<make_arrangement>(rt);
}

} // namespace planar
} // namespace DMax
} // namespace detail
} // namespace lal
