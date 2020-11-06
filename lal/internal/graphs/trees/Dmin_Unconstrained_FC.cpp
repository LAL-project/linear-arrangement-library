/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <cassert>
#include <vector>
using namespace std;

#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/trees/tree_centroid.hpp>
#include <lal/internal/graphs/trees/size_subtrees.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

#define LEFT_ANCHOR -1
#define RIGHT_ANCHOR 1
#define NO_ANCHOR 0

#define to_uint32(x) static_cast<uint32_t>(x)

typedef pair<uint32_t,lal::node> size_node;
typedef vector<size_node> ordering;

namespace lal {
using namespace graphs;

namespace internal {

int calculate_q(uint32_t n, const ordering & ord) {
	const uint32_t k = to_uint32(ord.size() - 1);
	const uint32_t t_0 = ord[0].first;

	// Maximum possible p_alpha
	int q = k/2;
	uint32_t sum = 0;
	for (uint32_t i = 0; i <= 2*to_uint32(q); ++i) {
		sum += ord[i].first;
	}

	uint32_t z = n - sum;
	uint32_t tricky_formula = (t_0 + 2)/2 + (z + 2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint32_t t_2q = ord[2*q].first;
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

int calculate_p(uint32_t n, const ordering& ord) {
	if (ord.size() < 2) {
		uint32_t p = -1;
		return p;
	}

	// number of subtrees (T_0, T_1, ..., T_k)
	const uint32_t k = to_uint32(ord.size() - 1);
	const uint32_t t_0 = ord[0].first;

	int p = (k - 1)/2;

	uint32_t sum = 0;
	for (uint32_t i = 0; i <= 2*to_uint32(p) + 1; ++i) {
		sum += ord[i].first;
	}

	uint32_t y = n - sum;
	uint32_t tricky_formula = (t_0 + 2)/2 + (y + 2)/2;
	uint32_t t_2p_plus_1 = ord[2*p + 1].first;

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

void get_P(uint32_t p, uint32_t i, vector<uint32_t>& v) {
	v = vector<uint32_t>(2*p + 1 + 1, -1);
	uint32_t pos = to_uint32(v.size() - 1);
	uint32_t right_pos = pos;
	uint32_t left_pos = 1;

	uint32_t j = 0;
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
}

void get_Q(uint32_t q, uint32_t i, vector<uint32_t>& v) {
	v = vector<uint32_t>(2*q + 1, -1);
	uint32_t pos = to_uint32(v.size() - 1);
	uint32_t right_pos = pos;
	uint32_t left_pos = 1;

	uint32_t j = 0;
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
}

void get_ordering(const free_tree& t, node u, ordering& ord) {
	// Let 'T_v' to be a tree rooted at vertex 'v'.
	// Order subtrees of 'T_v' by size.
	ord = ordering(t.degree(u - 1));

	// Retrieve size of every subtree. Let 'T_v[u]' be the subtree
	// of 'T_v' rooted at vertex 'u'. Now,
	//     s[u] := the size of the subtree 'T_v[u]'
	uint32_t *s = new uint32_t[t.n_nodes()];
	internal::get_size_subtrees(t, u - 1, s);

	uint32_t M = 0; // maximum of the sizes (needed for the counting sort algorithm)
	const neighbourhood& u_neighs = t.get_neighbours(u - 1);
	for (size_t i = 0; i < u_neighs.size(); ++i) {
		// i-th child of v_star
		const node ui = u_neighs[i];
		// size of subtree rooted at 'ui'
		const uint32_t s_ui = s[ui];

		ord[i].first = s_ui;
		ord[i].second = ui + 1;

		M = std::max(M, s_ui);
	}
	internal::counting_sort<ordering::iterator, size_node, false>
	(
		ord.begin(), ord.end(), M, ord.size(),
		[](const size_node& p) { return p.first; }
	);
	delete[] s;
}

// t: input forest a single connected component of which has to be arranged.
// root: indicates whether the connected component of 't' is rooted or anchored.
// one_node: node used as a reference to the said connected component.
//     Its value is within [1,n]
// start: position where to start placing the vertices (the leftmost position
//     in the mla for the subtree). We could also have an anologous finish
//     (rightmost position) but it is not needed.
void calculate_mla_chung(
	free_tree& t,
	char root, node one_node, position start,
	linear_arrangement& mla, uint32_t& cost
)
{
	vector<node> reachable(t.n_nodes_component(one_node - 1));
	{
	auto it = reachable.begin();
	internal::BFS<free_tree> bfs(t);
	bfs.set_process_current(
		// add '1' to vertices so that they range in [1,n]
		[&](const auto&, node u) { *it = u + 1; ++it; }
	);
	bfs.start_at(one_node - 1);
	}
	const uint32_t size_tree = t.n_nodes_component(one_node - 1);

	assert(size_tree > 0);
	assert(root == NO_ANCHOR or root == RIGHT_ANCHOR or root == LEFT_ANCHOR);

	// Base case
	if (size_tree == 1) {
		assert(one_node == reachable[0]);
		assert(start <= t.n_nodes());
		mla[one_node - 1] = start;
		cost = 0;
		return;
	}

	if (root == NO_ANCHOR) {
		const node u = internal::retrieve_centroid(t, one_node - 1).first + 1;

		ordering ord;
		get_ordering(t, u, ord);

		const int q = calculate_q(size_tree, ord);
		if (q == -1) {
			const uint32_t n_0 = ord[0].first;
			const node t_0 = ord[0].second;

			t.remove_edge(u - 1, t_0 - 1, false, false);

			uint32_t c1 = 0;
			uint32_t c2 = 0;
			calculate_mla_chung(t, RIGHT_ANCHOR, t_0, start, mla, c1);
			calculate_mla_chung(t, LEFT_ANCHOR, u, start + n_0, mla, c2);
			cost = c1 + c2 + 1;

			t.add_edge(u - 1, t_0 - 1, false, false);
		}
		else {
			// uq: unsigned 'q'
			const uint32_t uq = to_uint32(q);
			cost = numeric_limits<uint32_t>::max();

			vector<edge> edges(2*uq + 1);
			for (uint32_t i = 0; i <= 2*uq; ++i) {
				edges[i].first = u - 1;
				edges[i].second = ord[i].second - 1;
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint32_t size_rest_of_trees = 0;
			for (uint32_t i = 2*q + 1; i < ord.size(); ++i) {
				size_rest_of_trees += ord[i].first;
			}

			for (uint32_t i = 0; i <= 2*uq; ++i) {
				vector<uint32_t> Q_i;
				get_Q(q, i, Q_i);
				t.add_edge(u - 1, ord[i].second - 1);

				uint32_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint32_t start_aux = start;

				// Left part of the arrangement
				for (uint32_t j = 1; j <= uq; ++j) {
					const position pos_in_ord = Q_i[j];

					uint32_t c_i_j = 0;
					calculate_mla_chung(
						t,
						RIGHT_ANCHOR, ord[pos_in_ord].second, start_aux,
						arr_aux, c_i_j
					);
					start_aux += ord[pos_in_ord].first;
					c_i += c_i_j;
				}

				// Central part of the arrangement
				uint32_t c_i_j = 0;
				calculate_mla_chung(t, NO_ANCHOR, u, start_aux, arr_aux, c_i_j);
				c_i += c_i_j;

				// Right part of the arrangement
				start_aux += ord[i].first + 1 + size_rest_of_trees;
				for (uint32_t j = q + 1; j <= 2*uq; ++j) {
					const position pos_in_ord = Q_i[j];

					uint32_t c_i_j_in = 0;
					calculate_mla_chung(
						t,
						LEFT_ANCHOR, ord[pos_in_ord].second, start_aux,
						arr_aux, c_i_j_in
					);
					start_aux += ord[pos_in_ord].first;
					c_i += c_i_j_in;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*q;

				uint32_t subs = 0;
				for (uint32_t j = 1; j <= uq; ++j) {
					subs += (q - j + 1)*(ord[Q_i[j]].first + ord[Q_i[2*q - j + 1]].first);
				}
				c_i -= subs;
				c_i += q; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					mla = arr_aux;
				}
				assert(u != ord[i].second);
				t.remove_edge(u - 1, ord[i].second - 1, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);
		}
	}
	else {
		ordering ord;
		get_ordering(t, one_node, ord);

		const int p = calculate_p(size_tree, ord);
		if (p == -1) {
			const uint32_t n_0 = ord[0].first;
			const node t_0 = ord[0].second;
			assert(one_node != t_0);

			t.remove_edge(one_node - 1, t_0 - 1, false, false);

			uint32_t c1 = 0;
			uint32_t c2 = 0;
			calculate_mla_chung(t, RIGHT_ANCHOR, t_0, start, mla, c1);
			calculate_mla_chung(t, NO_ANCHOR, one_node, start + n_0, mla, c2);
			cost = c1 + c2 + size_tree - ord[0].first;

			t.add_edge(one_node - 1, t_0 - 1, false, false);
		}
		else {
			// up: unsigned 'p'
			const uint32_t up = to_uint32(p);
			cost = numeric_limits<uint32_t>::max();

			vector<edge> edges(2*up + 2);
			for (uint32_t i = 0; i <= 2*up + 1; ++i) {
				edges[i].first = one_node - 1;
				edges[i].second = ord[i].second - 1;
			}

			// Transform g into Y
			t.remove_edges(edges, false, false);

			// Central tree size
			uint32_t size_rest_of_trees= 0;
			for (uint32_t i = 2*p + 2; i < ord.size() ;++i) {
				size_rest_of_trees += ord[i].first;
			}

			for (uint32_t i = 0; i <= 2*up + 1; ++i) {
				vector<uint32_t> P_i;
				get_P(p, i, P_i);
				t.add_edge(one_node - 1, ord[i].second - 1, false, false);

				uint32_t c_i = 0;
				linear_arrangement arr_aux = mla;
				uint32_t start_aux = start;

				// Left part of the arrangement
				for (uint32_t j = 1; j <= up; ++j) {
					const position pos_in_ord = P_i[j];

					uint32_t c_i_j_in = 0;
					calculate_mla_chung(
						t,
						RIGHT_ANCHOR, ord[pos_in_ord].second, start_aux,
						arr_aux, c_i_j_in
					);
					start_aux += ord[pos_in_ord].first;
					c_i += c_i_j_in;
				}

				// Central part of the arrangement
				uint32_t c_i_j = 0;
				calculate_mla_chung(
					t,
					NO_ANCHOR, one_node, start_aux,
					arr_aux, c_i_j
				);

				start_aux += ord[i].first + 1 + size_rest_of_trees;
				c_i += c_i_j;

				// Right part of the arrangement
				for (uint32_t j = p + 1; j <= 2*up + 1; ++j) {
					const position pos_in_ord = P_i[j];

					uint32_t c_i_j_in = 0;
					calculate_mla_chung(
						t,
						LEFT_ANCHOR, ord[pos_in_ord].second, start_aux,
						arr_aux, c_i_j_in
					);
					start_aux += ord[pos_in_ord].first;
					c_i += c_i_j_in;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i += size_tree*(p + 1);
				c_i -= (p + 1)*ord[P_i[P_i.size()-1]].first;

				uint32_t subs = 0;
				for (uint32_t j = 1; j <= up; ++j) {
					subs += (p - j + 1)*(ord[P_i[j]].first + ord[P_i[2*p - j + 1]].first);
				}
				c_i -= subs;
				c_i += p; // NOT IN CHUNG'S PAPER

				if (c_i < cost) {
					cost = c_i;
					mla = arr_aux;
				}
				assert(one_node != ord[i].second);
				t.remove_edge(one_node - 1, ord[i].second - 1, false, false);
			}

			// Transform g into its previous form
			t.add_edges(edges, false, false);
		}
	}

	// Flipping arrangement if needed
	if (root == RIGHT_ANCHOR) {
		if (2*mla[one_node - 1] - 2*start < size_tree - 1) {
			// Right anchor and the root is too much to the left
			for (uint32_t i = 0; i < size_tree; ++i) {
				const uint32_t aux = start + size_tree - 1 - mla[reachable[i] - 1] + start;
				mla[reachable[i] - 1] = aux;
			}
		}
	}
	else if (root == LEFT_ANCHOR) {
		if (2*mla[one_node - 1] - 2*start > size_tree - 1) {
			// Left anchor and the root is too much to the right
			for (uint32_t i = 0; i < size_tree; ++i) {
				const uint32_t aux = start + size_tree - 1 - mla[reachable[i] - 1] + start;
				mla[reachable[i] - 1] = aux;
			}
		}
	}
}

pair<uint32_t, linear_arrangement> Dmin_Unconstrained_FC(const free_tree& t) {
	assert(t.is_tree());

	uint32_t c = 0;
	linear_arrangement arr(t.n_nodes(),0);

	free_tree T = t;
	calculate_mla_chung(T, NO_ANCHOR, 1, 0, arr, c);

	return make_pair(c, arr);
}

} // -- namespace internal
} // -- namespace lal
