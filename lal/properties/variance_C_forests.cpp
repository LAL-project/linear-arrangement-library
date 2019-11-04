/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#include <lal/properties/C_rla.hpp>

// C includes
#include <assert.h>
#include <string.h>

// C++ includes
#include <vector>
#include <queue>
using namespace std;

typedef uint64_t bigint;

namespace lal {
using namespace numeric;

namespace properties {

/*
 * Starting at vertex 'u', perform a BFS search to find
 * the connected components of the tree that vertex 'u'
 * belongs to.
 *
 * While finding the vertices in the connected component:
 *	- accumulates the sum of squares and cubes of the degrees
 *	- accumulates the value Lg = sum_{st in E} k_s*k_t
 *	- accumulates the neighbour degree product and neighbour degree sum
 */
inline void find_tree_vertices(
	const ugraph& g, const node u,
	vector<node>& tree_nodes, vector<bool>& vis,
	bigint& nk2, bigint& this_nk2, bigint& nk3, bigint& Lg,
	bigint *nds
)
{
	this_nk2 = 0;

	queue<node> q;
	q.push(u);
	vis[u] = true;
	tree_nodes.push_back(u);

	const bigint ku = g.degree(u);
	this_nk2 += ku*ku;
	nk3 += ku*ku*ku;

	while (not q.empty()) {
		node s = q.front();
		q.pop();
		bigint ks = g.degree(s);

		for (const node& t : g.get_neighbours(s)) {
			if (not vis[t]) {
				q.push(t);
				tree_nodes.push_back(t);
				vis[t] = true;

				bigint kt = g.degree(t);
				this_nk2 += kt*kt;
				nk3 += kt*kt*kt;
				Lg += kt*ks;

				nds[s] += kt;
				nds[t] += ks;
			}
		}
	}

	nk2 += this_nk2;
}

// m: number of edges of the WHOLE TREE
inline void compute_data_tree
(
	const ugraph& g, const vector<node>& tree_nodes,
	const bigint *nds, const bigint& nk2,
	bigint& n_paths_4, bigint& n_paths_5,
	bigint& ks_p_kt__x__ku_p_kv, bigint& inner_ks_x_kt__p__ku_x_kv,
	bigint& sum_adjs__x__sum_degs, bigint& sum_prod_pair_adj_deg
)
{
	bigint n = tree_nodes.size();

	// ------------------------
	// start computing terms

	// st
	for (node ps = 0; ps < n; ++ps) {
	const node s = tree_nodes[ps];
	const bigint ks = g.degree(s);
	for (node t : g.get_neighbours(s)) {
	const bigint kt = g.degree(t);
	// no repetitions from this point on
	if (s > t) { continue; }

		n_paths_4 += (ks - 1)*(kt - 1);
		n_paths_5 += (kt - 1)*(nds[s] - kt - ks + 1) +
					 (ks - 1)*(nds[t] - kt - ks + 1);

		const bigint eps1 = nds[s] - kt;
		const bigint eps2 = nds[t] - ks;

		sum_adjs__x__sum_degs +=
			(ks - 1)*(kt - 1)*(ks + kt) + (kt - 1)*eps1 + (ks - 1)*eps2;

		sum_prod_pair_adj_deg +=
			(ks - 1)*eps2 + (kt - 1)*eps1;

		inner_ks_x_kt__p__ku_x_kv -= ks*kt*(ks + kt);
		ks_p_kt__x__ku_p_kv +=
			(ks + kt)*(nk2 - nds[s] - nds[t] - kt*(kt - 1) - ks*(ks - 1));
	}
	}
}

inline void compute_data_forest
(
	const ugraph& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& n_paths_4, bigint& n_paths_5, bigint& KG,
	bigint& ks_p_kt__x__ku_p_kv, bigint& ks_x_kt__p__ku_x_kv,
	bigint& sum_adjs__x__sum_degs, bigint& sum_prod_pair_adj_deg
)
{
	// Auxiliar memory. Stores sum of degrees and sum of products of degrees.
	const bigint bytes = n*sizeof(bigint);
	bigint *all_memory = static_cast<bigint *>(malloc(bytes));

	// neighbour's degree sum: nds_s = sum_{st in E} k_t*(k_t - 1)
	bigint *nds = &all_memory[0];
	memset(all_memory, 0, bytes);

	bigint n_vis = 0;
	vector<bool> vis(n, false);
	// nodes of each tree
	vector<vector<node> > trees;
	// first non-visited node in vis
	node first_non_vis = 0;
	// sum of squares and cubes of degrees per tree
	bigint nk2, nk3;
	// sum_{st in E} k_s*k_t = sum_{s in V} ndp_s
	bigint Lg;

	nk2 = nk3 = Lg = 0;

	// -----------------------------
	// retrieve connected components

	while (n_vis < n) {
		while (vis[first_non_vis]) {
			++first_non_vis;
		}

		bigint tnk2;
		trees.push_back(vector<node>());
		find_tree_vertices(
			g, first_non_vis, trees.back(), vis,
			nk2, tnk2, nk3, Lg, nds
		);

		n_vis += trees.back().size();
	}

	// ------------------------
	// start computing variance

	// the size of Q of a forest can be calculated
	// just like the size of Q of any simple graph
	Qs = (m*(m + 1) - nk2)/2;
	KG = (m + 1)*nk2 - nk3 - 2*Lg;
	ks_x_kt__p__ku_x_kv = (m + 1)*Lg;

	for (const vector<node>& T : trees) {
		compute_data_tree(
			g, T, nds, nk2,
			n_paths_4, n_paths_5,
			ks_p_kt__x__ku_p_kv, ks_x_kt__p__ku_x_kv,
			sum_adjs__x__sum_degs, sum_prod_pair_adj_deg
		);
	}

	// we counted the amount of 5-paths twice
	n_paths_5 /= 2;
	// similarly, some things were counted twice
	//ks_x_kt__p__ku_x_kv /= 2;
	ks_p_kt__x__ku_p_kv /= 2;

	free(all_memory);
}

rational variance_C_forest_rational(const ugraph& g) {
	const bigint n = g.n_nodes();
	const bigint m = g.n_edges();

	// ----------------------------
	// compute terms dependent of Q

	// bigint of set Q
	bigint Qs = 0;

	// n_G(L_4)
	bigint n_paths_4 = 0;
	// n_G(L_5)
	bigint n_paths_5 = 0;

	// k_s + k_t + k_u + k_v
	bigint KG = 0;
	// (k_s + k_t)(k_u + k_v)
	bigint ks_p_kt__x__ku_p_kv = 0;
	// (k_s*k_t + k_u*k_v)
	bigint ks_x_kt__p__ku_x_kv = 0;

	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	bigint sum_adjs__x__sum_degs = 0;
	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	bigint sum_prod_pair_adj_deg = 0;

	compute_data_forest
	(
		g, n, m,
		Qs, n_paths_4, n_paths_5, KG,
		ks_p_kt__x__ku_p_kv, ks_x_kt__p__ku_x_kv,
		sum_adjs__x__sum_degs, sum_prod_pair_adj_deg
	);

	integer J(0);

	// V[C]
	rational V(0);
	J.init_ui((m + 2)*Qs);
	V += rational(2,45)*J;

	J.init_ui((2*m + 7)*n_paths_4);
	V -= rational(1,180)*J;

	J.init_ui(n_paths_5);
	V -= rational(1,180)*J;

	J.init_ui(KG);
	V += rational(1,90)*J;

	J.init_ui(sum_prod_pair_adj_deg);
	V -= rational(1,60)*J;

	J.init_ui(sum_adjs__x__sum_degs);
	V += rational(1,180)*J;

	J.init_ui(ks_p_kt__x__ku_p_kv);
	V += rational(1,180)*J;

	J.init_ui(ks_x_kt__p__ku_x_kv);
	V -= rational(1,90)*J;
	return V;
}

double variance_C_forest(const ugraph& g) {
	rational V = variance_C_forest_rational(g);
	return V.to_double();
}

} // -- namespace properties
} // -- namespace lal
