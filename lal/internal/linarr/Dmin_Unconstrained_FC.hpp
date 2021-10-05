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
#include <optional>
#include <vector>

#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/properties/tree_centroid.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/data_array.hpp>

#define LEFT_ANCHOR -1
#define RIGHT_ANCHOR 1
#define NO_ANCHOR 0

#define to_int64(x) static_cast<int64_t>(x)

typedef std::pair<uint64_t,lal::node> size_node;
typedef lal::internal::data_array<size_node> ordering;

namespace lal {
using namespace graphs;

namespace internal {
namespace dmin_Chung {

/*
int calculate_q(uint64_t n, const ordering& ord) {
	const uint64_t k = to_uint64(ord.size()) - 1;
	const uint64_t t_0 = ord[0].first;

	// Maximum possible p_alpha
	int64_t q = to_int64(k)/2;
	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*to_uint64(q); ++i) {
		sum += ord[i].first;
	}

	uint64_t z = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint64_t t_2q = ord[2*q].first;
	while (q >= 0 and t_2q <= tricky_formula) {
		z += ord[2*q].first;
		if (q > 0) {
			z += ord[2*q - 1].first;
		}
		--q;
		tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
		if (q >= 0) {
			t_2q = ord[2*q].first;
		}
	}
	return q;
}
*/

inline
std::optional<uint64_t> calculate_q(uint64_t n, const ordering& ord) noexcept {
#if defined DEBUG
	assert(ord.size() > 0);
#endif

	const uint64_t k = ord.size() - 1;
	const uint64_t t_0 = ord[0].first;

	// Maximum possible p_alpha
	uint64_t q = k/2;
	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*q; ++i) {
		sum += ord[i].first;
	}

	uint64_t z = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint64_t t_2q = ord[2*q].first;

	while (t_2q <= tricky_formula) {
		z += ord[2*q].first;

		if (q > 0) { z += ord[2*q - 1].first; }
		tricky_formula = (t_0 + 2)/2 + (z + 2)/2;

		if (q == 0) { return {}; }
		--q;
		t_2q = ord[2*q].first;
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
	const uint64_t t_0 = ord[0].first;

	int p = (k - 1)/2;

	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*to_uint64(p) + 1; ++i) {
		sum += ord[i].first;
	}

	uint64_t y = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
	uint64_t t_2p_plus_1 = ord[2*p + 1].first;

	while (p >= 0 and t_2p_plus_1 <= tricky_formula) {
		y = y + ord[2*p + 1].first + ord[2*p].first;
		--p;
		tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
		if (p >= 0) {
			t_2p_plus_1 = ord[2*p + 1].first;
		}
	}
	return p;
}
*/

inline
std::optional<uint64_t> calculate_p(uint64_t n, const ordering& ord) noexcept {
	if (ord.size() < 2) { return {}; }

	// number of subtrees (T_0, T_1, ..., T_k)
	const uint64_t k = ord.size() - 1;
	const uint64_t t_0 = ord[0].first;

	uint64_t p = (k - 1)/2;

	uint64_t sum = 0;
	for (uint64_t i = 0; i <= 2*p + 1; ++i) {
		sum += ord[i].first;
	}

	uint64_t y = n - sum;
	uint64_t tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
	uint64_t t_2p_plus_1 = ord[2*p + 1].first;

	while (t_2p_plus_1 <= tricky_formula) {
		y = y + ord[2*p + 1].first + ord[2*p].first;
		tricky_formula = (t_0 + 2)/2 + (y + 2)/2;

		if (p == 0) { return {}; }
		--p;
		t_2p_plus_1 = ord[2*p + 1].first;
	}
	return p;
}

std::vector<uint64_t> get_P(uint64_t p, uint64_t i) {
	std::vector<uint64_t> v(2*p + 1 + 1);
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

inline std::vector<uint64_t> get_Q(uint64_t q, uint64_t i) noexcept {
	std::vector<uint64_t> v(2*q + 1);
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

inline void get_ordering(const free_tree& t, node u, ordering& ord) noexcept {
	// Let 'T_v' to be a tree rooted at vertex 'v'.
	// Order subtrees of 'T_v' by size.
#if defined DEBUG
	assert(ord.size() == t.get_degree(u - 1));
#endif

	// Retrieve size of every subtree. Let 'T_v[u]' be the subtree
	// of 'T_v' rooted at vertex 'u'. Now,
	//     s[u] := the size of the subtree 'T_v[u]'
	data_array<uint64_t> s(t.get_num_nodes());
	internal::get_size_subtrees(t, u - 1, s.data());

	uint64_t M = 0; // maximum of the sizes (needed for the counting sort algorithm)
	const neighbourhood& u_neighs = t.get_neighbours(u - 1);
	for (size_t i = 0; i < u_neighs.size(); ++i) {
		// i-th child of v_star
		const node ui = u_neighs[i];
		// size of subtree rooted at 'ui'
		const uint64_t s_ui = s[ui];

		ord[i].first = s_ui;
		ord[i].second = ui + 1;

		M = std::max(M, s_ui);
	}
	internal::counting_sort<size_node, size_node*, countingsort::decreasing_t>
	(
		ord.begin(), ord.end(), M, ord.size(),
		[](const size_node& p) { return p.first; }
	);
}

// t: input forest a single connected component of which has to be arranged.
// root: indicates whether the connected component of 't' is rooted or anchored.
// one_node: node used as a reference to the said connected component.
//     Its value is within [1,n]
// start: position where to start placing the vertices (the leftmost position
//     in the mla for the subtree). We could also have an anologous finish
//     (rightmost position) but it is not needed.
template<char root>
void calculate_mla(
	free_tree& t,
	node one_node, 
	position start,
	position end,
	linear_arrangement& mla, 
	uint64_t& cost
)
noexcept
{
	std::vector<node> reachable(t.get_num_nodes_component(one_node - 1));
	{
	auto it = reachable.begin();
	internal::BFS<free_tree> bfs(t);
	bfs.set_process_current(
		// add '1' to vertices so that they range in [1,n]
		[&](const auto&, node u) { *it++ = u + 1; }
	);
	bfs.start_at(one_node - 1);
	}
	const uint64_t size_tree = t.get_num_nodes_component(one_node - 1);

	static_assert(root == NO_ANCHOR or root == RIGHT_ANCHOR or root == LEFT_ANCHOR);

#if defined DEBUG
	assert(size_tree > 0);
#endif

	// Base case
	if (size_tree == 1) {
#if defined DEBUG
		assert(one_node == reachable[0]);
		assert(start <= t.get_num_nodes());
#endif
		mla[one_node - 1] = start;
		cost = 0;
		return;
	}

	if constexpr (root == NO_ANCHOR) {
		const node u = internal::retrieve_centroid(t, one_node - 1).first + 1;

		ordering ord(t.get_degree(u - 1));
		get_ordering(t, u, ord);

		const auto q = calculate_q(size_tree, ord);
		if (not q) {
			const uint64_t n_0 = ord[0].first;
			const node t_0 = ord[0].second;

			t.remove_edge(u - 1, t_0 - 1, false, false);

			uint64_t c1 = 0;
			uint64_t c2 = 0;
			calculate_mla<RIGHT_ANCHOR>(t, t_0, start, start + n_0 - 1, mla, c1);
			calculate_mla<LEFT_ANCHOR>(t, u, start + n_0, end, mla, c2);
			cost = c1 + c2 + 1;

			t.add_edge(u - 1, t_0 - 1, false, false);
		}
		else {
			// uq: unsigned 'q'
			const uint64_t uq = *q;
			cost = std::numeric_limits<uint64_t>::max();

			std::vector<edge> edges(2*uq + 1);
			for (uint64_t i = 0; i <= 2*uq; ++i) {
				edges[i].first = u - 1;
				edges[i].second = ord[i].second - 1;
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint64_t size_rest_of_trees = 0;
			for (uint64_t i = 2*uq + 1; i < ord.size(); ++i) {
				size_rest_of_trees += ord[i].first;
			}

			for (uint64_t i = 0; i <= 2*uq; ++i) {
				const std::vector<uint64_t> Q_i = get_Q(uq, i);

				t.add_edge(u - 1, ord[i].second - 1);

				uint64_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint64_t start_aux = start;
				//uint64_t end_aux = end;
				
				// Left part of the arrangement
				for (uint64_t j = 1; j <= uq; ++j) {
					const position pos_in_ord = Q_i[j];
					uint64_t n_i = ord[pos_in_ord].first;
					
					uint64_t c_i_j = 0;
					calculate_mla<RIGHT_ANCHOR>(
						t,
						ord[pos_in_ord].second, start_aux,
						start_aux + n_i -1,
						arr_aux, c_i_j
					);
					start_aux += n_i; // ord[pos_in_ord].first;
					c_i += c_i_j;
				}

				// Central part of the arrangement
				uint64_t c_i_j = 0;
				uint64_t end_for_here = start_aux+ord[i].first+size_rest_of_trees;
				calculate_mla<NO_ANCHOR>(t, u, start_aux, end_for_here, arr_aux, c_i_j);
				c_i += c_i_j;

				// Right part of the arrangement
				//start_aux += ord[i].first + 1 + size_rest_of_trees;
				start_aux=end_for_here+1;
				for (uint64_t j = uq + 1; j <= 2*uq; ++j) {
					const position pos_in_ord = Q_i[j];
					uint64_t n_i = ord[pos_in_ord].first;
					uint64_t c_i_j_in = 0;
					calculate_mla<LEFT_ANCHOR>(
						t,
						ord[pos_in_ord].second, start_aux,
						start_aux + n_i -1,
						arr_aux, c_i_j_in
					);
					start_aux += n_i; // ord[pos_in_ord].first;
					c_i += c_i_j_in;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*uq;

				uint64_t subs = 0;
				for (uint64_t j = 1; j <= uq; ++j) {
					subs += (uq - j + 1)*(ord[Q_i[j]].first + ord[Q_i[2*uq - j + 1]].first);
				}
				c_i -= subs;
				c_i += uq; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					mla = arr_aux;
				}
#if defined DEBUG
				assert(u != ord[i].second);
#endif
				t.remove_edge(u - 1, ord[i].second - 1, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);
		}
	}
	else {
		ordering ord(t.get_degree(one_node - 1));
		get_ordering(t, one_node, ord);

		const auto p = calculate_p(size_tree, ord);
		if (not p) {
			const uint64_t n_0 = ord[0].first;
			const node t_0 = ord[0].second;
#if defined DEBUG
			assert(one_node != t_0);
#endif

			t.remove_edge(one_node - 1, t_0 - 1, false, false);

			uint64_t c1 = 0;
			uint64_t c2 = 0;
			
			
			if constexpr (root == LEFT_ANCHOR) {
				calculate_mla<NO_ANCHOR>(t, one_node, start, end - n_0, mla, c1);
				calculate_mla<LEFT_ANCHOR>(t, t_0, end-n_0+1, end, mla, c2);
			}
			else
			{
				calculate_mla<RIGHT_ANCHOR>(t, t_0, start, start + n_0 -1, mla, c1);
				calculate_mla<NO_ANCHOR>(t, one_node, start + n_0, end, mla, c2);
			}
			cost = c1 + c2 + size_tree - ord[0].first;

			t.add_edge(one_node - 1, t_0 - 1, false, false);
		}
		else {
			// up: unsigned 'p'
			const uint64_t up = *p;
			cost = std::numeric_limits<uint64_t>::max();

			std::vector<edge> edges(2*up + 2);
			for (uint64_t i = 0; i <= 2*up + 1; ++i) {
				edges[i].first = one_node - 1;
				edges[i].second = ord[i].second - 1;
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint64_t size_rest_of_trees= 0;
			for (uint64_t i = 2*up + 2; i < ord.size() ;++i) {
				size_rest_of_trees += ord[i].first;
			}

			for (uint64_t i = 0; i <= 2*up + 1; ++i) {
				const std::vector<uint64_t> P_i = get_P(up, i);
				t.add_edge(one_node - 1, ord[i].second - 1, false, false);

				uint64_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint64_t start_aux = start;
				uint64_t end_aux=end;
				
				if constexpr (root == LEFT_ANCHOR) {

					// Left part of the arrangement
					for (uint64_t j = 1; j <= up; ++j) {
						const position pos_in_ord = P_i[j];
						uint64_t r = ord[pos_in_ord].second;
						uint64_t n_i = ord[pos_in_ord].first;
	
						uint64_t c_i_j_in = 0;
						calculate_mla<RIGHT_ANCHOR>(
							t, r,
							//ord[pos_in_ord].second, 
							start_aux,
							start_aux + n_i - 1,
							arr_aux, c_i_j_in
						);
						start_aux += n_i; //ord[pos_in_ord].first;
						c_i += c_i_j_in;
					}
	
					// Central part of the arrangement
					uint64_t c_i_j = 0;
					calculate_mla<NO_ANCHOR>(t, one_node, start_aux, 
						start_aux + ord[i].first + 1 + size_rest_of_trees - 1, 
						arr_aux, c_i_j);
	
					start_aux += ord[i].first + 1 + size_rest_of_trees;
					c_i += c_i_j;
	
					// Right part of the arrangement
					for (uint64_t j = up + 1; j <= 2*up + 1; ++j) {
						const position pos_in_ord = P_i[j];
						uint64_t r = ord[pos_in_ord].second;
						uint64_t n_i = ord[pos_in_ord].first;
						uint64_t c_i_j_in = 0;
						calculate_mla<LEFT_ANCHOR>(
							t, r,
							//ord[pos_in_ord].second, 
							start_aux,
							start_aux + n_i -1,
							arr_aux, c_i_j_in
						);
						start_aux += n_i; // ord[pos_in_ord].first;
						c_i += c_i_j_in;
					}
				}
				else{ // RIGHT ANCHOR
					// Right part of the arrangement
					for (uint64_t j = 1; j <= up; ++j) {
						const position pos_in_ord = P_i[j];
						uint64_t r = ord[pos_in_ord].second;
						uint64_t n_i = ord[pos_in_ord].first;
	
						uint64_t c_i_j_in = 0;
						calculate_mla<LEFT_ANCHOR>(
							t, r,
							//ord[pos_in_ord].second,
							end_aux-n_i+1,
							end_aux,
							//start_aux,
							//start_aux + n_i - 1,
							arr_aux, c_i_j_in
						);
						start_aux -= n_i; //ord[pos_in_ord].first;
						c_i += c_i_j_in;
					}
	
					// Central part of the arrangement
					uint64_t c_i_j = 0;
					calculate_mla<NO_ANCHOR>(t, one_node, 
						end_aux-ord[i].first-1-size_rest_of_trees+1, end_aux,
						//start_aux, 
						//start_aux + ord[i].first + 1 + size_rest_of_trees - 1, 
						arr_aux, c_i_j);
					end_aux-=ord[i].first+1+size_rest_of_trees;
					//start_aux += ord[i].first + 1 + size_rest_of_trees;
					c_i += c_i_j;
	
					// Right part of the arrangement
					for (uint64_t j = up + 1; j <= 2*up + 1; ++j) {
						const position pos_in_ord = P_i[j];
						uint64_t r = ord[pos_in_ord].second;
						uint64_t n_i = ord[pos_in_ord].first;
						uint64_t c_i_j_in = 0;
						calculate_mla<RIGHT_ANCHOR>(
							t, r,
							//ord[pos_in_ord].second,
							end_aux-n_i+1, end_aux,
							//start_aux,
							//start_aux + n_i -1,
							arr_aux, c_i_j_in
						);
						start_aux -= n_i; // ord[pos_in_ord].first;
						c_i += c_i_j_in;
					}
				}
				
				
				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*(up + 1);
				c_i -= (up + 1)*ord[P_i[P_i.size()-1]].first;

				uint64_t subs = 0;
				for (uint64_t j = 1; j <= up; ++j) {
					subs += (up - j + 1)*(ord[P_i[j]].first + ord[P_i[2*up - j + 1]].first);
				}
				c_i -= subs;
				c_i += up; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					mla = arr_aux;
				}
#if defined DEBUG
				assert(one_node != ord[i].second);
#endif
				t.remove_edge(one_node - 1, ord[i].second - 1, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);
		}
	}

	// Flipping arrangement if needed
	/*if constexpr (root == RIGHT_ANCHOR) {
		if (2*mla[one_node - 1] - 2*start < size_tree - 1) {
			// Right anchor and the root is too much to the left
			for (uint64_t i = 0; i < size_tree; ++i) {
				const uint64_t aux = start + size_tree - 1 - mla[reachable[i] - 1] + start;
				mla[reachable[i] - 1] = aux;
			}
		}
	}
	else if constexpr (root == LEFT_ANCHOR) {
		if (2*mla[one_node - 1] - 2*start > size_tree - 1) {
			// Left anchor and the root is too much to the right
			for (uint64_t i = 0; i < size_tree; ++i) {
				const uint64_t aux = start + size_tree - 1 - mla[reachable[i] - 1] + start;
				mla[reachable[i] - 1] = aux;
			}
		}
	}*/
}

} // -- namespaec dmin_chung

std::pair<uint64_t, linear_arrangement> Dmin_Unconstrained_FC(const free_tree& t)
noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	uint64_t c = 0;
	linear_arrangement arr(t.get_num_nodes(),0);

	free_tree T = t;
	dmin_Chung::calculate_mla<NO_ANCHOR>(T, 1, 0, t.get_num_nodes() - 1, arr, c);

	return std::make_pair(c, arr);
}

} // -- namespace internal
} // -- namespace lal
