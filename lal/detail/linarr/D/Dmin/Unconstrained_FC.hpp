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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 ********************************************************************/

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

#include <optional>
#include <limits>
#include <vector>

#include <lal/linear_arrangement.hpp>
#include <lal/detail/pairs_utils.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/properties/tree_centroid.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>

namespace lal {
namespace detail {
namespace Dmin {
namespace unconstrained {

/**
 * @brief Functions for Chung's minimum linear arrangement algorithm.
 *
 * Namespace that holds function for Chung's algorithm for the minimum linear
 * arrangement problem. See \cite Chung1984a for further details.
 */
namespace Chung {

/// Typedef for a useful type.
typedef data_array<node_size> ordering;

/*
int calculate_q(uint64_t n, const ordering& ord) {
	const uint64_t k = to_uint64(ord.size()) - 1;
	const uint64_t t_0 = ord[0].size;

	// Maximum possible p_alpha
	int64_t q = to_int64(k)/2;
	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*to_uint64(q); ++i) {
		sum += ord[i].size;
	}

	uint64_t z = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint64_t t_2q = ord[2*q].size;
	while (q >= 0 and t_2q <= tricky_formula) {
		z += ord[2*q].size;
		if (q > 0) {
			z += ord[2*q - 1].size;
		}
		--q;
		tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
		if (q >= 0) {
			t_2q = ord[2*q].size;
		}
	}
	return q;
}
*/

/**
 * @brief Calculate \f$q\f$
 *
 * See \cite Chung1984a for details.
 * @param n Number of vertices
 * @param ord Ordering of the children with respect to a node.
 * @returns Returns the value of \f$q\f$.
 */
std::optional<uint64_t> calculate_q(uint64_t n, const ordering& ord) noexcept {
#if defined DEBUG
	assert(ord.size() > 0);
#endif

	const uint64_t k = ord.size() - 1;
	const uint64_t t_0 = ord[0].size;

	// Maximum possible p_alpha
	uint64_t q = k/2;
	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*q; ++i) {
		sum += ord[i].size;
	}

	uint64_t z = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint64_t t_2q = ord[2*q].size;

	while (t_2q <= tricky_formula) {
		z += ord[2*q].size;

		if (q > 0) { z += ord[2*q - 1].size; }
		tricky_formula = (t_0 + 2)/2 + (z + 2)/2;

		if (q == 0) { return {}; }
		--q;
		t_2q = ord[2*q].size;
	}
	return q;
}

/*
int calculate_p(uint64_t n, const ordering& ord) {
	if (ord.size() < 2) {
		return -1;
	}

	// number of subtrees (T_0, T_1, ..., T_k)
	const uint64_t k = to_uint64(ord.size() - 1);
	const uint64_t t_0 = ord[0].size;

	int p = (k - 1)/2;

	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*to_uint64(p) + 1; ++i) {
		sum += ord[i].size;
	}

	uint64_t y = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
	uint64_t t_2p_plus_1 = ord[2*p + 1].size;

	while (p >= 0 and t_2p_plus_1 <= tricky_formula) {
		y = y + ord[2*p + 1].size + ord[2*p].size;
		--p;
		tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
		if (p >= 0) {
			t_2p_plus_1 = ord[2*p + 1].size;
		}
	}
	return p;
}
*/

/**
 * @brief Calculate \f$p\f$
 *
 * See \cite Chung1984a for details.
 * @param n Number of vertices
 * @param ord Ordering of the children with respect to a node.
 * @returns Returns the value of \f$p\f$.
 */
std::optional<uint64_t> calculate_p(uint64_t n, const ordering& ord) noexcept {
	if (ord.size() < 2) { return {}; }

	// number of subtrees (T_0, T_1, ..., T_k)
	const uint64_t k = ord.size() - 1;
	const uint64_t t_0 = ord[0].size;

	uint64_t p = (k - 1)/2;

	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*p + 1; ++i) {
		sum += ord[i].size;
	}

	uint64_t y = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
	uint64_t t_2p_plus_1 = ord[2*p + 1].size;

	while (t_2p_plus_1 <= tricky_formula) {
		y = y + ord[2*p + 1].size + ord[2*p].size;
		tricky_formula = (t_0 + 2)/2 + (y + 2)/2;

		if (p == 0) { return {}; }
		--p;
		t_2p_plus_1 = ord[2*p + 1].size;
	}
	return p;
}

/**
 * @brief Calculate \f$P\f$
 *
 * See \cite Chung1984a for details.
 * @param p See @ref lal::detail::Dmin::unconstrained::Chung::calculate_p.
 * @param i Index of the i-th children in the ordering.
 * @returns Returns the value of \f$P\f$.
 */
data_array<uint64_t> get_P(uint64_t p, uint64_t i) noexcept {
	data_array<uint64_t> v(2*p + 1 + 1);
	uint64_t pos = v.size() - 1;
	uint64_t right_pos = pos;
	uint64_t left_pos = 1;

	uint64_t j = 0;
	while (j <= 2*p + 1) {
		if (j == i) {
			++j;
		}
		else {
			v[pos] = j;
			if (pos > left_pos) {
				--right_pos;
				pos = left_pos;
			}
			else {
				++left_pos;
				pos = right_pos;
			}
			++j;
		}
	}

	return v;
}

/**
 * @brief Calculate \f$Q\f$
 *
 * See \cite Chung1984a for details.
 * @param q See @ref lal::detail::Dmin::unconstrained::Chung::calculate_q.
 * @param i Index of the i-th children in the ordering.
 * @returns Returns the value of \f$Q\f$.
 */
data_array<uint64_t> get_Q(uint64_t q, uint64_t i) noexcept {
	data_array<uint64_t> v(2*q + 1);
	uint64_t pos = v.size() - 1;
	uint64_t right_pos = pos;
	uint64_t left_pos = 1;

	uint64_t j = 0;
	while (j <= 2*q) {
		if (j == i) {
			++j;
		}
		else {
			v[pos] = j;
			if (pos > left_pos) {
				--right_pos;
				pos = left_pos;
			}
			else {
				++left_pos;
				pos = right_pos;
			}
			++j;
		}
	}
	return v;
}

/**
 * @brief Sort the children of @e u in the rooted tree \f$T^u\f$.
 *
 * See \cite Chung1984a for further details.
 * @param t Input free tree.
 * @param u Vertex.
 * @returns Returns the children of @e u sorted in non-increasing order.
 */
ordering get_ordering(const graphs::free_tree& t, node u) noexcept {
	// Let 'T_u' to be a tree rooted at vertex 'u'.
	// Order subtrees of 'T_u' by size.
	ordering ord(t.get_degree(u));

	// Retrieve size of every subtree. Let 'T_u[v]' be the subtree
	// of 'T_u' rooted at vertex 'v'. Now,
	//     s[v] := the size of the subtree 'T_u[v]'
	data_array<uint64_t> s(t.get_num_nodes());
	detail::get_size_subtrees(t, u, s.begin());

	uint64_t M = 0; // maximum of the sizes (needed for the counting sort algorithm)
	const neighbourhood& u_neighs = t.get_neighbours(u);
	for (std::size_t i = 0; i < u_neighs.size(); ++i) {
		// i-th child of v_star
		const node ui = u_neighs[i];
		// size of subtree rooted at 'ui'
		const uint64_t s_ui = s[ui];

		ord[i].size = s_ui;
		ord[i].v = ui;

		M = std::max(M, s_ui);
	}
	detail::sorting::counting_sort
	<node_size, sorting::non_increasing_t>
	(
		ord.begin(), ord.end(), M, ord.size(),
		[](const node_size& p) { return p.size; }
	);

	return ord;
}

/**
 * @brief Calculate a minimum linear arrangment using Fan Chung's algorithm.
 *
 * For further details, see \cite Chung1984a.
 * @tparam root Is the call to this function with a root?
 * @tparam make_arrangement Whether or not the arrangement should be constructed.
 * @param t Input free tree.
 * @param one_node Root or anchor.
 * @param start Starting position of the minLA of the current tree.
 * @param[out] mla A minimum arrangement.
 * @param[out] cost The cost of the minimum arrangement.
 */
template <char root, bool make_arrangement>
void calculate_mla(
	graphs::free_tree& t,
	node one_node, position start,
	linear_arrangement& mla, uint64_t& cost
)
noexcept
{
	std::vector<node> reachable(t.get_num_nodes_component(one_node));
	{
	auto it = reachable.begin();
	detail::BFS bfs(t);
	bfs.set_process_current([&](const auto&, node u) { *it++ = u; });
	bfs.start_at(one_node);
	}
	const uint64_t size_tree = t.get_num_nodes_component(one_node);

	static_assert(
		root == Dopt_utils::NO_ANCHOR or
		root == Dopt_utils::RIGHT_ANCHOR or
		root == Dopt_utils::LEFT_ANCHOR
	);

#if defined DEBUG
	assert(size_tree > 0);
#endif

	// Base case
	if (size_tree == 1) {
#if defined DEBUG
		assert(one_node == reachable[0]);
		assert(start <= t.get_num_nodes());
#endif
		if constexpr (make_arrangement) {
		mla.assign(one_node, start);
		}
		cost = 0;
		return;
	}

	if constexpr (root == Dopt_utils::NO_ANCHOR) {
		const node u = detail::retrieve_centroid(t, one_node).first;

		const ordering ord = get_ordering(t, u);

		const auto q = calculate_q(size_tree, ord);
		if (not q) {
			const uint64_t n_0 = ord[0].size;
			const node t_0 = ord[0].v;

			t.remove_edge(u, t_0, false, false);

			uint64_t c1 = 0;
			calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
				(t, t_0, start, mla, c1);

			uint64_t c2 = 0;
			calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
				(t, u, start + n_0, mla, c2);

			cost = c1 + c2 + 1;

			t.add_edge(u, t_0, false, false);
		}
		else {
			// uq: unsigned 'q'
			const uint64_t uq = *q;
			cost = std::numeric_limits<uint64_t>::max();

			std::vector<edge> edges(2*uq + 1);
			for (uint64_t i = 0; i <= 2*uq; ++i) {
				edges[i] = {u, ord[i].v};
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint64_t size_rest_of_trees = 0;
			for (uint64_t i = 2*uq + 1; i < ord.size(); ++i) {
				size_rest_of_trees += ord[i].size;
			}

			for (uint64_t i = 0; i <= 2*uq; ++i) {
				const auto Q_i = get_Q(uq, i);

				t.add_edge(u, ord[i].v);

				uint64_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint64_t start_aux = start;

				// Left part of the arrangement
				for (uint64_t j = 1; j <= uq; ++j) {
					const position pos_in_ord = Q_i[j];

					uint64_t c_i_j = 0;
					calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
					(
						t,
						ord[pos_in_ord].v, start_aux,
						arr_aux, c_i_j
					);
					start_aux += ord[pos_in_ord].size;
					c_i += c_i_j;
				}

				// Central part of the arrangement
				uint64_t c_i_j = 0;
				calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>(t, u, start_aux, arr_aux, c_i_j);
				c_i += c_i_j;

				// Right part of the arrangement
				start_aux += ord[i].size + 1 + size_rest_of_trees;
				for (uint64_t j = uq + 1; j <= 2*uq; ++j) {
					const position pos_in_ord = Q_i[j];

					uint64_t c_i_j_in = 0;
					calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
					(
						t,
						ord[pos_in_ord].v, start_aux,
						arr_aux, c_i_j_in
					);
					start_aux += ord[pos_in_ord].size;
					c_i += c_i_j_in;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*uq;

				uint64_t subs = 0;
				for (uint64_t j = 1; j <= uq; ++j) {
					subs += (uq - j + 1)*(ord[Q_i[j]].size + ord[Q_i[2*uq - j + 1]].size);
				}
				c_i -= subs;
				c_i += uq; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					if constexpr (make_arrangement) {
					mla = std::move(arr_aux);
					}
				}
#if defined DEBUG
				assert(u != ord[i].v);
#endif
				t.remove_edge(u, ord[i].v, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);
		}
	}
	else { // root == ANCHOR
		const ordering ord = get_ordering(t, one_node);

		const auto p = calculate_p(size_tree, ord);
		if (not p) {
			const uint64_t n_0 = ord[0].size;
			const node t_0 = ord[0].v;
#if defined DEBUG
			assert(one_node != t_0);
#endif

			t.remove_edge(one_node, t_0, false, false);

			uint64_t c1 = 0;
			uint64_t c2 = 0;

			if constexpr (root == Dopt_utils::LEFT_ANCHOR) {
				calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
					(t, one_node, start, mla, c1);

				calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
					(t, t_0, start + size_tree - ord[0].size, mla, c2);
			}
			else {
				calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
					(t, t_0, start, mla, c1);

				calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
					(t, one_node, start + n_0, mla, c2);
			}

			cost = c1 + c2 + size_tree - ord[0].size;
			t.add_edge(one_node, t_0, false, false);

			/*
			if constexpr (root == LEFT_ANCHOR) {
				//if (2*mla[one_node - 1] - 2*start > size_tree - 1) {

					// Left anchor and the root is too much to the right
					for (uint64_t i = 0; i < size_tree; ++i) {
						const uint64_t aux = start + size_tree - 1 - mla[reachable[i]] + start;
						mla[reachable[i]] = aux;
					}
				//}
			}
			*/
		}
		else {
			// up: unsigned 'p'
			const uint64_t up = *p;
			cost = std::numeric_limits<uint64_t>::max();

			std::vector<edge> edges(2*up + 2);
			for (uint64_t i = 0; i <= 2*up + 1; ++i) {
				edges[i] = {one_node, ord[i].v};
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint64_t size_rest_of_trees= 0;
			for (uint64_t i = 2*up + 2; i < ord.size() ;++i) {
				size_rest_of_trees += ord[i].size;
			}

			for (uint64_t i = 0; i <= 2*up + 1; ++i) {
				const auto P_i = get_P(up, i);
				t.add_edge(one_node, ord[i].v, false, false);

				uint64_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint64_t start_aux = start;

				if constexpr (root == Dopt_utils::LEFT_ANCHOR) {
					// Left part of the arrangement
					for (uint64_t j = 1; j <= up; ++j) {
						const position pos_in_ord = P_i[j];

						uint64_t c_i_j_in = 0;
						calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
						(
							t,
							ord[pos_in_ord].v, start_aux,
							arr_aux, c_i_j_in
						);
						start_aux += ord[pos_in_ord].size;
						c_i += c_i_j_in;
					}

					// Central part of the arrangement
					uint64_t c_i_j = 0;
					calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
						(t, one_node, start_aux, arr_aux, c_i_j);

					start_aux += ord[i].size + 1 + size_rest_of_trees;
					c_i += c_i_j;

					// Right part of the arrangement
					for (uint64_t j = up + 1; j <= 2*up + 1; ++j) {
						const position pos_in_ord = P_i[j];

						uint64_t c_i_j_in = 0;
						calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
						(
							t,
							ord[pos_in_ord].v, start_aux,
							arr_aux, c_i_j_in
						);
						start_aux += ord[pos_in_ord].size;
						c_i += c_i_j_in;
					}
				}
				else { // root == RIGHT_ANCHOR

					// Right part of the arrangement
					for (uint64_t j = 2*up + 1; j >= up + 1; --j) {
						const position pos_in_ord = P_i[j];

						uint64_t c_i_j_in = 0;
						calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
						(
							t,
							ord[pos_in_ord].v, start_aux,
							arr_aux, c_i_j_in
						);
						start_aux += ord[pos_in_ord].size;
						c_i += c_i_j_in;
					}

					// Central part of the arrangement
					uint64_t c_i_j = 0;
					calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
						(t, one_node, start_aux, arr_aux, c_i_j);

					start_aux += ord[i].size + 1 + size_rest_of_trees;
					c_i += c_i_j;

					// Left part of the arrangement
					for (uint64_t j = up; j >= 1; --j) {
						const position pos_in_ord = P_i[j];

						uint64_t c_i_j_in = 0;
						calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
						(
							t,
							ord[pos_in_ord].v, start_aux,
							arr_aux, c_i_j_in
						);
						start_aux += ord[pos_in_ord].size;
						c_i += c_i_j_in;
					}
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*(up + 1);
				c_i -= (up + 1)*ord[P_i[P_i.size()-1]].size;

				uint64_t subs = 0;
				for (uint64_t j = 1; j <= up; ++j) {
					subs += (up - j + 1)*(ord[P_i[j]].size + ord[P_i[2*up - j + 1]].size);
				}
				c_i -= subs;
				c_i += up; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					mla = arr_aux;
				}
#if defined DEBUG
				assert(one_node != ord[i].v);
#endif
				t.remove_edge(one_node, ord[i].v, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);

		}
	}
}

} // -- namespace dmin_Chung

/**
 * @brief Calculates a minimum linear arrangment using Fan Chung's algorithm.
 *
 * See \cite Chung1984a for further details.
 * @tparam make_arrangement Whether or not the arrangement should be constructed.
 * @param t Input free tree.
 * @returns Either a pair of <cost, linear arrangement> or just the cost depending
 * on the value of the template parameter @e make_arrangement.
 */
template <bool make_arrangement>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
FanChung_2(const graphs::free_tree& t) noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	graphs::free_tree T = t;

	uint64_t Dmin = 0;
	linear_arrangement arr(make_arrangement ? t.get_num_nodes() : 0);
	Chung::calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>(T, 0, 0, arr, Dmin);

	if constexpr (make_arrangement) {
		return {Dmin, std::move(arr)};
	}
	else {
		return Dmin;
	}
}

} // -- namespace unconstrained
} // -- namespace Dmin
} // -- namespace detail
} // -- namespace lal
