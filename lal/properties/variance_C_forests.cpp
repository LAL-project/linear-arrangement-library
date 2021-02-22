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
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/data_array.hpp>

typedef uint64_t bigint;

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace properties {

// ------------------------------
// FORESTS

inline void compute_data_forest
(
	const undirected_graph& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& n_paths_4, bigint& n_paths_5, bigint& KG,
	bigint& Phi_1, bigint& Phi_2,
	bigint& Lambda_1, bigint& Lambda_2
)
{
	// -----------------------------------------
	// auxiliary memory and additional variables

	// neighbour's degree sum: nds[s] = sum_{st in E} k_t
	internal::data_array<bigint> xi(n);

	// n<k^2>: second moment of degree about zero multiplied by n
	bigint nk2 = 0;
	// n<k^3>: third moment of degree about zero multiplied by n
	bigint nk3 = 0;
	// sum_{st in E} k_s*k_t = sum_{s in V} ndp_s
	bigint psi = 0;

	// ----------------------
	// precompute data

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.degree(s);
		// calculate n*<k^2>, n*<k^3>
		nk2 += ks*ks;
		nk3 += ks*ks*ks;

		xi[s] = 0;
		for (node t : g.get_neighbours(s)) {
			const bigint kt = g.degree(t);

			// calculate sum_{st in E} k_s*k_t
			psi += ks*kt;
			// calculate for each s in V: sum_{t in Gamma(s)} k_t
			xi[s] += kt;
		}
	}
	psi /= 2;

	// ------------------------
	// start computing variance

	Qs = (m*(m + 1) - nk2)/2;
	KG = (m + 1)*nk2 - nk3 - 2*psi;
	Phi_1 = (m + 1)*psi;
	Phi_2 = 0;

	E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const auto [s,t] = it.get_edge();

		const bigint ks = g.degree(s);
		const bigint kt = g.degree(t);

		n_paths_4 += (ks - 1)*(kt - 1);
		n_paths_5 += (kt - 1)*(xi[s] - kt - ks + 1) +
					 (ks - 1)*(xi[t] - kt - ks + 1);

		Lambda_1 += (ks - 1)*(xi[t] - ks) + (kt - 1)*(xi[s] - kt);
		Lambda_2 += (ks - 1)*(kt - 1)*(ks + kt);

		Phi_1 -= ks*kt*(ks + kt);
		Phi_2 += (ks + kt)*(nk2 - xi[s] - xi[t] - kt*(kt - 1) - ks*(ks - 1));
	}

	Lambda_2 += Lambda_1;
	n_paths_5 /= 2;
	Phi_2 /= 2;
}

rational variance_C_forest_rational(const undirected_graph& g) {
	const bigint n = g.num_nodes();
	const bigint m = g.num_edges();

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
	// (k_s*k_t + k_u*k_v)
	bigint Phi_1 = 0;
	// (k_s + k_t)(k_u + k_v)
	bigint Phi_2 = 0;

	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	bigint Lambda_1 = 0;
	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	bigint Lambda_2 = 0;

	compute_data_forest
	(
		g, n, m,
		Qs, n_paths_4, n_paths_5, KG,
		Phi_1, Phi_2,
		Lambda_1, Lambda_2
	);

	integer J(0);

	// V[C]
	rational V(0);
	J.set_ui((m + 2)*Qs);			V += rational(2,45)*J;
	J.set_ui((2*m + 7)*n_paths_4);	V -= rational(1,180)*J;
	J.set_ui(n_paths_5);			V -= rational(1,180)*J;
	J.set_ui(KG);					V += rational(1,90)*J;
	J.set_ui(Lambda_1);				V -= rational(1,60)*J;
	J.set_ui(Lambda_2);				V += rational(1,180)*J;
	J.set_ui(Phi_2);				V += rational(1,180)*J;
	J.set_ui(Phi_1);				V -= rational(1,90)*J;
	return V;
}

double variance_C_forest(const undirected_graph& g) {
	return variance_C_forest_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
