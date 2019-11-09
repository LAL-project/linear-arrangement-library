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

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/iterators/edge_iterator.hpp>

typedef uint64_t bigint;

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace properties {

// ------------------------------
// Variance of C in using formula
// TREES

inline void compute_data_tree
(
	const utree& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& n_paths_4, bigint& n_paths_5, bigint& KG,
	bigint& ks_p_kt__x__ku_p_kv, bigint& ks_x_kt__p__ku_x_kv,
	bigint& sum_adjs__x__sum_degs, bigint& sum_prod_pair_adj_deg
)
{
	// -----------------------------------------
	// auxiliary memory and additional variables

	// Auxiliar memory. Stores sum of degrees and sum of products of degrees.
	bigint *all_memory = static_cast<bigint *>(malloc(n*sizeof(bigint)));

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	bigint *nds = &all_memory[0];
	// n<k^2>: second moment of degree about zero multiplied by n
	bigint nk2 = 0;
	// n<k^3>: third moment of degree about zero multiplied by n
	bigint nk3 = 0;
	// sum_{st in E} k_s*k_t = sum_{s in V} ndp_s
	bigint Lg = 0;

	// ----------------------
	// precompute useful data

	for (node s = 0; s < n; ++s) {
		const uint32_t ks = g.degree(s);
		nk2 += ks*ks;
		nk3 += ks*ks*ks;

		nds[s] = 0;
		for (const node& t : g.get_neighbours(s)) {
			const uint32_t kt = g.degree(t);
			Lg += ks*kt;
			nds[s] += kt;
		}
	}
	Lg /= 2;

	// ------------------------
	// start computing variance

	Qs = (n*(n - 1) - nk2)/2;
	KG = (m + 1)*nk2 - nk3 - 2*Lg;
	ks_x_kt__p__ku_x_kv = (m + 1)*Lg;

	edge_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge st = it.get_edge();
		const node s = st.first;
		const bigint ks = g.degree(s);
		const node t = st.second;
		const bigint kt = g.degree(t);

		n_paths_4 += (ks - 1)*(kt - 1);
		n_paths_5 += (kt - 1)*(nds[s] - kt - ks + 1) +
					 (ks - 1)*(nds[t] - kt - ks + 1);

		const bigint eps1 = nds[s] - kt;
		const bigint eps2 = nds[t] - ks;

		sum_adjs__x__sum_degs +=
			(ks - 1)*(kt - 1)*(ks + kt) + (kt - 1)*eps1 + (ks - 1)*eps2;

		sum_prod_pair_adj_deg +=
			(ks - 1)*eps2 + (kt - 1)*eps1;

		ks_x_kt__p__ku_x_kv -= ks*kt*(ks + kt);
		ks_p_kt__x__ku_p_kv +=
			(ks + kt)*(nk2 - nds[s] - nds[t] - kt*(kt - 1) - ks*(ks - 1));
	}

	// we counted the amount of 5-paths twice
	n_paths_5 /= 2;
	// similarly, some things were counted twice
	//ks_x_kt__p__ku_x_kv /= 2;
	ks_p_kt__x__ku_p_kv /= 2;

	free(all_memory);
}

rational variance_C_tree_rational(const utree& g) {
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

	compute_data_tree
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

double variance_C_tree(const utree& g) {
	rational V = variance_C_tree_rational(g);
	return V.to_double();
}

} // -- namespace properties
} // -- namespace lal
