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
	const utree& g, const uint64_t& n, const uint64_t& m,
	uint64_t& Qs, uint64_t& n_paths_4, uint64_t& n_paths_5, uint64_t& KG,
	uint64_t& Phi_1, uint64_t& Phi_2,
	uint64_t& Lambda_1, uint64_t& Lambda_2
)
{
	// -----------------------------------------
	// auxiliary memory and additional variables

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	uint64_t *nds = static_cast<uint64_t *>(malloc(n*sizeof(uint64_t)));

	// n<k^2>: second moment of degree about zero multiplied by n
	uint64_t nk2 = 0;
	// n<k^3>: third moment of degree about zero multiplied by n
	uint64_t nk3 = 0;
	// sum_{st in E} k_s*k_t = sum_{s in V} ndp_s
	uint64_t Lg = 0;

	// ----------------------
	// precompute data

	for (node s = 0; s < n; ++s) {
		const uint64_t ks = g.degree(s);
		// calculate n*<k^2>, n*<k^3>
		nk2 += ks*ks;
		nk3 += ks*ks*ks;

		nds[s] = 0;
		for (node t : g.get_neighbours(s)) {
			const uint64_t kt = g.degree(t);

			// calculate sum_{st in E} k_s*k_t
			Lg += ks*kt;
			// calculate for each s in V: sum_{t in Gamma(s)} k_t
			nds[s] += kt;
		}
	}
	Lg /= 2;

	// ------------------------
	// start computing variance

	Qs = (n*(n - 1) - nk2)/2;
	KG = (m + 1)*nk2 - nk3 - 2*Lg;
	Phi_1 = (m + 1)*Lg;

	edge_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge st = it.get_edge();
		const node s = st.first;
		const uint64_t ks = g.degree(s);
		const node t = st.second;
		const uint64_t kt = g.degree(t);

		n_paths_4 += (ks - 1)*(kt - 1);
		n_paths_5 += (kt - 1)*(nds[s] - kt - ks + 1) +
					 (ks - 1)*(nds[t] - kt - ks + 1);

		const uint64_t eps1 = nds[s] - kt;
		const uint64_t eps2 = nds[t] - ks;

		Lambda_1 += (ks - 1)*eps2 + (kt - 1)*eps1;
		Lambda_2 += (ks - 1)*(kt - 1)*(ks + kt);

		Phi_1 -= ks*kt*(ks + kt);
		Phi_2 +=
			(ks + kt)*(nk2 - nds[s] - nds[t] - kt*(kt - 1) - ks*(ks - 1));
	}
	// complete calculating Lambda_2
	Lambda_2 += Lambda_1;

	// we counted the amount of 5-paths twice
	n_paths_5 /= 2;
	// similarly, some things were counted twice
	Phi_2 /= 2;

	free(nds);
}

rational variance_C_tree_rational(const utree& g) {
	const uint64_t n = g.n_nodes();
	const uint64_t m = g.n_edges();

	// ----------------------------
	// compute terms dependent of Q

	// uint64_t of set Q
	uint64_t Qs = 0;

	// n_G(L_4)
	uint64_t n_paths_4 = 0;
	// n_G(L_5)
	uint64_t n_paths_5 = 0;

	// k_s + k_t + k_u + k_v
	uint64_t KG = 0;
	// (k_s*k_t + k_u*k_v)
	uint64_t Phi_1 = 0;
	// (k_s + k_t)(k_u + k_v)
	uint64_t Phi_2 = 0;

	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	uint64_t Lambda_1 = 0;
	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	uint64_t Lambda_2 = 0;

	compute_data_tree
	(
		g, n, m,
		Qs, n_paths_4, n_paths_5, KG,
		Phi_1, Phi_2,
		Lambda_1, Lambda_2
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

	J.init_ui(Lambda_1);
	V -= rational(1,60)*J;

	J.init_ui(Lambda_2);
	V += rational(1,180)*J;

	J.init_ui(Phi_2);
	V += rational(1,180)*J;

	J.init_ui(Phi_1);
	V -= rational(1,90)*J;
	return V;
}

double variance_C_tree(const utree& g) {
	return variance_C_tree_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
