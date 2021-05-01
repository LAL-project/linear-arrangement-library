/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 ********************************************************************/

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/properties/tree_centroid.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/data_array.hpp>

#define LEFT_ANCHOR -1
#define RIGHT_ANCHOR 1
#define NO_ANCHOR 0
#define ANCHOR 1

#define to_uint32(x) static_cast<uint32_t>(x)

typedef pair<uint32_t,lal::node> size_node;
typedef lal::internal::data_array<size_node> ordering;

namespace lal {
using namespace graphs;

namespace internal {
namespace dmin_Shiloach {

template<char anchored>
uint32_t calculate_p_alpha(
	const uint32_t n, const ordering& ord,
	uint32_t& s_0, uint32_t& s_1
)
noexcept
{
	// anchored is ANCHOR or NO_ANCHOR
	// left or right anchored is not important for the cost
#if defined DEBUG
	assert(anchored == NO_ANCHOR or anchored == ANCHOR);
#endif

	// number of subtrees
	const uint32_t k = to_uint32(ord.size() - 1);

	uint32_t n_0 = ord[0].first;
	uint32_t max_p = 0;

	if constexpr (anchored == NO_ANCHOR) {
		// -- not anchored

		// Maximum possible p_alpha
		max_p = k/2;
		if (max_p == 0) { return 0; }

		uint32_t sum = 0;
		for (uint32_t i = 0; i <= 2*max_p; ++i) { sum += ord[i].first; }

		uint32_t n_star = n - sum;
		uint32_t tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

		// n_0 >= n_1 >= ... >= n_k
		uint32_t n_p = ord[2*max_p].first;
		while (max_p > 0 and n_p <= tricky_formula) {
			sum -= ord[2*max_p].first + ord[2*max_p - 1].first;

			--max_p;
			n_star = n - sum;
			tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

			// --
			//if (max_p > 0) { n_p = ord[2*max_p].first; }
			n_p = (max_p > 0 ? ord[2*max_p].first : n_p);
			// --
		}
		s_0 = max_p*(n_star + 1 + n_0);
		s_1 = 0;
		for (uint32_t i = 1; i < max_p; ++i) {
			s_0 += i*(ord[2*i + 1].first + ord[2*i + 2].first);
		}
	}
	else {
		// -- anchored

		// Maximum possible p_alpha
		max_p = (k + 1)/2;
		if (max_p == 0) { return 0; }

		uint32_t sum = 0;
		for (uint32_t i = 0; i <= 2*max_p - 1; ++i) { sum += ord[i].first; }
		uint32_t n_star = n - sum;
		uint32_t tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;
		uint32_t n_p = ord[2*max_p - 1].first;
		while (max_p > 0 and n_p <= tricky_formula) {
			sum -= ord[2*max_p - 1].first;
			sum -= ord[2*max_p - 2].first;
			--max_p;
			n_star = n - sum;
			tricky_formula = (n_0 + 2)/2 + (n_star + 2)/2;

			// --
			//if (max_p > 0) { n_p = ord[2*max_p - 1].first; }
			n_p = (max_p > 0 ? ord[2*max_p - 1].first : n_p);
			// --
		}
		s_0 = 0;
		s_1 = max_p*(n_star + 1 + n_0) - 1;
		for (uint32_t i = 1; i < max_p; ++i) {
			s_1 += i*(ord[2*i].first + ord[2*i + 1].first);
		}
	}
	return max_p;
}

// t: input forest a single connected component of which has to be arranged.
// alpha: indicates whether the connected component of 't' is rooted or anchored.
// root_or_anchor: node used as a reference to the said connected component.
//     Its value is within [1,n]
// start: position where to start placing the vertices (the leftmost position
//     in the mla for the subtree).
// end: position where to end placing the vertices (the rightmost position
//     int the mla for the subtree).
template<char alpha>
void calculate_mla(
	free_tree& t,
	node root_or_anchor, position start, position end,
	linear_arrangement& mla, uint32_t& cost
)
noexcept
{
	static_assert(alpha == NO_ANCHOR or alpha == RIGHT_ANCHOR or alpha == LEFT_ANCHOR);

	vector<node> reachable(t.get_num_nodes_component(root_or_anchor - 1));
	{
	auto it = reachable.begin();
	internal::BFS<free_tree> bfs(t);
	bfs.set_process_current(
		// add '1' to vertices so that they range in [1,n]
		[&](const auto&, node u) { *it = u + 1; ++it; }
	);
	bfs.start_at(root_or_anchor - 1);
	}

	// Size of the tree
	const uint32_t size_tree = t.get_num_nodes_component(root_or_anchor - 1);

#if defined DEBUG
	assert(size_tree > 0);
#endif

	// Base case
	if (size_tree == 1) {
		cost = 0;
		mla[root_or_anchor - 1] = start;
		return;
	}

	// Recursion for COST A
	const node v_star = (
		alpha == NO_ANCHOR ?
			internal::retrieve_centroid(t, root_or_anchor - 1).first + 1 :
			root_or_anchor
	);

	// Let 'T_v' to be a tree rooted at vertex 'v'.
	// Order subtrees of 'T_v' by size.
	ordering ord(t.get_degree(v_star - 1));
	{
	// Retrieve size of every subtree. Let 'T_v[u]' be the subtree
	// of 'T_v' rooted at vertex 'u'. Now,
	//     s[u] := the size of the subtree 'T_v[u]'
	data_array<uint32_t> s(t.get_num_nodes());
	internal::get_size_subtrees(t, v_star - 1, s.data);

	uint32_t M = 0; // maximum of the sizes (needed for the counting sort algorithm)
	const neighbourhood& v_star_neighs = t.get_neighbours(v_star - 1);
	for (size_t i = 0; i < v_star_neighs.size(); ++i) {
		// i-th child of v_star
		const node ui = v_star_neighs[i];
		// size of subtree rooted at 'ui'
		const uint32_t s_ui = s[ui];

		ord[i].first = s_ui;
		M = std::max(M, s_ui);
		ord[i].second = ui + 1;
	}
	internal::counting_sort<size_node, size_node*, countingsort::decreasing_t>
	(
		ord.begin(), ord.end(), M, ord.size(),
		[](const size_node& p) { return p.first; }
	);
	}

	const node v_0 = ord[0].second;		// Root of biggest subtree
	const uint32_t n_0 = ord[0].first;	// Size of biggest subtree

	// remove edge connecting v_star and its largest subtree
	t.remove_edge(v_star - 1, v_0 - 1, false, false);

	uint32_t c1, c2;
	c1 = c2 = 0;

	// t -t0 : t0  if t has a LEFT_ANCHOR
	if constexpr (alpha == LEFT_ANCHOR) {
		calculate_mla<NO_ANCHOR>
			(t, v_star, start, end - n_0, mla, c2);

		calculate_mla<LEFT_ANCHOR>
			(t, v_0, end - n_0 + 1, end, mla, c1);
	}
	// t0 : t- t0 if t has NO_ANCHOR or RIGHT_ANCHOR
	else {
		calculate_mla<RIGHT_ANCHOR>
			(t, v_0, start, start + n_0 - 1, mla, c1);

		calculate_mla<(alpha == NO_ANCHOR ? LEFT_ANCHOR : NO_ANCHOR)>
			(t, v_star, start + n_0, end, mla, c2);
	}

	// Cost for recursion A
	if constexpr (alpha == NO_ANCHOR) { cost = c1 + c2 + 1; }
	else							  { cost = c1 + c2 + size_tree - n_0; }

	// reconstruct t
	t.add_edge(v_star - 1, v_0 - 1, false, false);

	// Recursion B

	// Left or right anchored is not important for the cost.
	// Note that the result returned is either 0 or 1.
	constexpr unsigned char anchored =
		(alpha == RIGHT_ANCHOR or alpha == LEFT_ANCHOR ? ANCHOR : NO_ANCHOR);

	uint32_t s_0 = 0;
	uint32_t s_1 = 0;
	const uint32_t p_alpha = calculate_p_alpha<anchored>(size_tree, ord, s_0, s_1);

	uint32_t cost_B = 0;
	linear_arrangement mla_B(mla);

	if (p_alpha > 0) {
		vector<edge> edges(2*p_alpha - anchored);

		// number of nodes not in the central tree
		for (uint32_t i = 1; i <= 2*p_alpha - anchored; ++i) {
			const node r = ord[i].second;
			edges[i - 1].first = v_star - 1;
			edges[i - 1].second = r - 1;
		}
		t.remove_edges(edges, false, false);

		// t1 : t3 : ... : t* : ... : t4 : t2 if t has NO_ANCHOR or RIGHT_ANCHOR
		// t2 : t4 : ... : t* : ... : t3 : t1 ig t has LEFT_ANCHOR
		for(uint32_t i = 1; i <= 2*p_alpha - anchored; ++i) {
			uint32_t c_aux = 0;

			const node r = ord[i].second;
			const uint32_t n_i = ord[i].first;
			if ((alpha == LEFT_ANCHOR and i%2 == 0) or (alpha != LEFT_ANCHOR and i%2 == 1)) {
				calculate_mla<RIGHT_ANCHOR>(t, r, start, start + n_i - 1, mla_B, c_aux);
				cost_B += c_aux;
				start += n_i;
			}
			else {
				calculate_mla<LEFT_ANCHOR>(t, r, end - n_i + 1, end, mla_B, c_aux);
				cost_B += c_aux;
				end -= n_i;
			}
		}

		// t*
		uint32_t c_aux = 0;
		calculate_mla<NO_ANCHOR>(t, v_star, start, end, mla_B, c_aux);
		cost_B += c_aux;

		// reconstruct t
		t.add_edges(edges, false, false);

		// We add the anchors part not previously added
		if constexpr (alpha == NO_ANCHOR) {
			cost_B += s_0;
		}
		else {
			cost_B += s_1;
		}

	}

	// We choose B-recursion only if it is better
	if (p_alpha != 0) {
		if (cost_B < cost) {
			mla.swap(mla_B);
			cost = cost_B;
		}
	}
}

} // -- namespace dmin_shiloach

pair<uint32_t, linear_arrangement> Dmin_Unconstrained_YS(const free_tree& t)
noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	uint32_t c = 0;
	linear_arrangement arrangement(t.get_num_nodes(),0);

	free_tree T = t;
	// Positions 0, 1, ..., t.get_num_nodes() - 1
	dmin_Shiloach::calculate_mla<NO_ANCHOR>
		(T, 1, 0, t.get_num_nodes() -1, arrangement, c);

	return make_pair(c, arrangement);
}

} // -- namespace internal
} // -- namespace lal
