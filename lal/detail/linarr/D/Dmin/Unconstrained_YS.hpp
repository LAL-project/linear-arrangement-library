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
#include <vector>

#include <lal/linear_arrangement.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/properties/tree_centroid.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>

namespace lal {
namespace detail {
namespace Dmin {
namespace unconstrained {

/**
 * @brief Functions for Shiloach's minimum linear arrangement algorithm.
 *
 * Namespace that holds function for Shiloach's algorithm for the minimum linear
 * arrangement problem. See \cite Shiloach1979a for further details.
 */
namespace Shiloach {

/// Typedef for a useful type.
typedef data_array<node_size> ordering;

/**
 * @brief Calculate \f$p_{\alpha}\f$.
 *
 * See \cite Shiloach1979a for details.
 * @tparam anchored Is the tree anchored?
 * @param n Number of vertices of the tree.
 * @param ord Ordering of the children of a vertex in non-increasing order.
 * @param s_0 Value \f$s_0\f$.
 * @param s_1 Value \f$s_1\f$.
 * @returns Maximum \f$p_\alpha\f$.
 */
template <char anchored>
uint64_t calculate_p_alpha
(const uint64_t n, const ordering& ord, uint64_t& s_0, uint64_t& s_1)
noexcept
{
	// anchored is ANCHOR or NO_ANCHOR
	// left or right anchored is not important for the cost
	static_assert(
		anchored == Dopt_utils::NO_ANCHOR or
		anchored == Dopt_utils::ANCHOR
	);

	// number of subtrees
	const uint64_t k = ord.size() - 1;

	uint64_t n_0 = ord[0].size;
	uint64_t max_p = 0;

	if constexpr (anchored == Dopt_utils::NO_ANCHOR) {
		// -- not anchored

		// Maximum possible p_alpha
		max_p = k/2;
		if (max_p == 0) { return 0; }

		uint64_t sum = 0;
		for (uint64_t i = 0; i <= 2*max_p; ++i) { sum += ord[i].size; }

		uint64_t n_star = n - sum;
		uint64_t tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

		// n_0 >= n_1 >= ... >= n_k
		uint64_t n_p = ord[2*max_p].size;
		while (max_p > 0 and n_p <= tricky_formula) {
			sum -= ord[2*max_p].size + ord[2*max_p - 1].size;

			--max_p;
			n_star = n - sum;
			tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

			// --
			//if (max_p > 0) { n_p = ord[2*max_p].first; }
			n_p = (max_p > 0 ? ord[2*max_p].size : n_p);
			// --
		}
		s_0 = max_p*(n_star + 1 + n_0);
		s_1 = 0;
		for (uint64_t i = 1; i < max_p; ++i) {
			s_0 += i*(ord[2*i + 1].size + ord[2*i + 2].size);
		}
	}
	else {
		// -- anchored

		// Maximum possible p_alpha
		max_p = (k + 1)/2;
		if (max_p == 0) { return 0; }

		uint64_t sum = 0;
		for (uint64_t i = 0; i <= 2*max_p - 1; ++i) { sum += ord[i].size; }
		uint64_t n_star = n - sum;
		uint64_t tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;
		uint64_t n_p = ord[2*max_p - 1].size;
		while (max_p > 0 and n_p <= tricky_formula) {
			sum -= ord[2*max_p - 1].size;
			sum -= ord[2*max_p - 2].size;
			--max_p;
			n_star = n - sum;
			tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

			// --
			//if (max_p > 0) { n_p = ord[2*max_p - 1].first; }
			n_p = (max_p > 0 ? ord[2*max_p - 1].size : n_p);
			// --
		}
		s_0 = 0;
		s_1 = max_p*(n_star + 1 + n_0) - 1;
		for (uint64_t i = 1; i < max_p; ++i) {
			s_1 += i*(ord[2*i].size + ord[2*i + 1].size);
		}
	}
	return max_p;
}

/**
 * @brief Calculate a minimum linear arrangment using Shiloach's algorithm.
 *
 * For further details, see \cite Shiloach1979a.
 * @tparam alpha indicates whether the connected component of 't' is rooted or anchored.
 * @tparam make_arrangement Whether or not the arrangement should be constructed.
 * @param t input forest a single connected component of which has to be arranged.
 * @param root_or_anchor: node used as a reference to the said connected component.
 * Its value is within [1,n]
 * @param start position where to start placing the vertices (the leftmost position
 * in the mla for the subtree).
 * @param end position where to end placing the vertices (the rightmost position
 * int the mla for the subtree).
 * @param[out] mla A minimum arrangement.
 * @param[out] cost The cost of the minimum arrangement.
 */
template <char alpha, bool make_arrangement>
void calculate_mla(
	graphs::free_tree& t,
	const node root_or_anchor,
	position start, position end,
	linear_arrangement& mla, uint64_t& cost
)
noexcept
{
	static_assert(
		alpha == Dopt_utils::NO_ANCHOR or
		alpha == Dopt_utils::RIGHT_ANCHOR or
		alpha == Dopt_utils::LEFT_ANCHOR
	);

	// Size of the tree
	const uint64_t size_tree = t.get_num_nodes_component(root_or_anchor);

#if defined DEBUG
	assert(size_tree > 0);
#endif

	// Base case
	if (size_tree == 1) {
		cost = 0;
		if constexpr (make_arrangement) {
		mla.assign(root_or_anchor, start);
		}
		return;
	}

	// Recursion for COST A
	const node v_star =
		alpha == Dopt_utils::NO_ANCHOR ?
			retrieve_centroid(t, root_or_anchor).first :
			root_or_anchor;

	// Let 'T_v' to be a tree rooted at vertex 'v'.
	// Order subtrees of 'T_v' by size.
	ordering ord(t.get_degree(v_star));
	{
	// Retrieve size of every subtree. Let 'T_v[u]' be the subtree
	// of 'T_v' rooted at vertex 'u'. Now,
	//     s[u] := the size of the subtree 'T_v[u]'
	data_array<uint64_t> s(t.get_num_nodes());
	get_size_subtrees(t, v_star, s.begin());

	uint64_t M = 0; // maximum of the sizes (needed for the counting sort algorithm)
	const neighbourhood& v_star_neighs = t.get_neighbours(v_star);
	for (std::size_t i = 0; i < v_star_neighs.size(); ++i) {
		// i-th child of v_star
		const node ui = v_star_neighs[i];
		// size of subtree rooted at 'ui'
		const uint64_t s_ui = s[ui];

		ord[i].size = s_ui;
		M = std::max(M, s_ui);
		ord[i].v = ui;
	}
	sorting::counting_sort
		<node_size, sorting::non_increasing_t>
		(
			ord.begin(), ord.end(), M, ord.size(),
			[](const node_size& p) { return p.size; }
		);
	}

	const node v_0 = ord[0].v;		  // Root of biggest subtree
	const uint64_t n_0 = ord[0].size; // Size of biggest subtree

	// remove edge connecting v_star and its largest subtree
	t.remove_edge(v_star, v_0, false, false);

	uint64_t c1, c2;
	c1 = c2 = 0;

	// t -t0 : t0  if t has a LEFT_ANCHOR
	if constexpr (alpha == Dopt_utils::LEFT_ANCHOR) {
		calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
			(t, v_star, start, end - n_0, mla, c2);

		calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
			(t, v_0, end - n_0 + 1, end, mla, c1);
	}
	// t0 : t- t0 if t has NO_ANCHOR or RIGHT_ANCHOR
	else {
		calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
			(t, v_0, start, start + n_0 - 1, mla, c1);

		constexpr auto new_alpha =
			alpha == Dopt_utils::NO_ANCHOR ?
				Dopt_utils::LEFT_ANCHOR :
				Dopt_utils::NO_ANCHOR;

		calculate_mla<new_alpha, make_arrangement>
			(t, v_star, start + n_0, end, mla, c2);
	}

	// Cost for recursion A
	if constexpr (alpha == Dopt_utils::NO_ANCHOR) { cost = c1 + c2 + 1; }
	else							              { cost = c1 + c2 + size_tree - n_0; }

	// reconstruct t
	t.add_edge(v_star, v_0, false, false);

	// Recursion B

	// Left or right anchored is not important for the cost.
	// Note that the result returned is either 0 or 1.
	constexpr auto anchored =
		alpha == Dopt_utils::RIGHT_ANCHOR or alpha == Dopt_utils::LEFT_ANCHOR ?
			Dopt_utils::ANCHOR :
			Dopt_utils::NO_ANCHOR;

	uint64_t s_0 = 0;
	uint64_t s_1 = 0;
	const uint64_t p_alpha =
		calculate_p_alpha<anchored>(size_tree, ord, s_0, s_1);

	uint64_t cost_B = 0;
	linear_arrangement mla_B(make_arrangement ? mla : 0);

	if (p_alpha > 0) {
		std::vector<edge> edges(2*p_alpha - anchored);

		// number of nodes not in the central tree
		for (uint64_t i = 1; i <= 2*p_alpha - anchored; ++i) {
			edges[i - 1] = {v_star, ord[i].v};
		}
		t.remove_edges(edges, false, false);

		// t1 : t3 : ... : t* : ... : t4 : t2 if t has NO_ANCHOR or RIGHT_ANCHOR
		// t2 : t4 : ... : t* : ... : t3 : t1 ig t has LEFT_ANCHOR
		for(uint64_t i = 1; i <= 2*p_alpha - anchored; ++i) {
			uint64_t c_aux = 0;

			const node r = ord[i].v;
			const uint64_t n_i = ord[i].size;
			if (
				(alpha == Dopt_utils::LEFT_ANCHOR and i%2 == 0) or
				(alpha != Dopt_utils::LEFT_ANCHOR and i%2 == 1)
			)
			{
				calculate_mla<Dopt_utils::RIGHT_ANCHOR, make_arrangement>
					(t, r, start, start + n_i - 1, mla_B, c_aux);

				cost_B += c_aux;
				start += n_i;
			}
			else {
				calculate_mla<Dopt_utils::LEFT_ANCHOR, make_arrangement>
					(t, r, end - n_i + 1, end, mla_B, c_aux);

				cost_B += c_aux;
				end -= n_i;
			}
		}

		// t*
		uint64_t c_aux = 0;
		calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
			(t, v_star, start, end, mla_B, c_aux);

		cost_B += c_aux;

		// reconstruct t
		t.add_edges(edges, false, false);

		// We add the anchors part not previously added
		if constexpr (alpha == Dopt_utils::NO_ANCHOR) {
			cost_B += s_0;
		}
		else {
			cost_B += s_1;
		}

	}

	// We choose B-recursion only if it is better
	if (p_alpha != 0) {
		if (cost_B < cost) {
			if constexpr (make_arrangement) {
			//mla.swap(mla_B);
			mla = std::move(mla_B);
			}

			cost = cost_B;
		}
	}
}

} // -- namespace Shiloach

/**
 * @brief Calculates a minimum linear arrangment using Shiloach's algorithm.
 *
 * See \cite Shiloach1979a for further details.
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
YossiShiloach(const graphs::free_tree& t)
noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	uint64_t Dmin = 0;
	linear_arrangement arrangement(make_arrangement ? t.get_num_nodes() : 0);

	graphs::free_tree T = t;
	// Positions 0, 1, ..., t.get_num_nodes() - 1
	Shiloach::calculate_mla<Dopt_utils::NO_ANCHOR, make_arrangement>
		(T, 0, 0, t.get_num_nodes() - 1, arrangement, Dmin);

	if constexpr (make_arrangement) {
		return {Dmin, std::move(arrangement)};
	}
	else {
		return Dmin;
	}
}

} // -- namespace unconstrained
} // -- namespace Dmin
} // -- namespace detail
} // -- namespace lal
