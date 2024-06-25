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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace properties {

// ------------------------------
// FORESTS

inline void compute_data_forest
(
	const graphs::undirected_graph& g, const uint64_t& n, const uint64_t& m,
	uint64_t& Qs, uint64_t& n_paths_4, uint64_t& n_paths_5, uint64_t& KG,
	uint64_t& Phi_1, uint64_t& Phi_2,
	uint64_t& Lambda_1, uint64_t& Lambda_2
)
noexcept
{
	// -----------------------------------------
	// auxiliary memory and additional variables

	// neighbour's degree sum: nds[s] = sum_{st in E} k_t
	detail::array<uint64_t> xi(n);

	// in the paper: n<k^2>
	uint64_t sum_squared_degrees = 0;
	// in the paper: n<k^3>
	uint64_t sum_cubed_degrees = 0;
	// sum_{st in E} k_s*k_t = sum_{s in V} ndp_s
	uint64_t psi = 0;

	// ----------------------
	// precompute data

	for (node s = 0; s < n; ++s) {
		const uint64_t ks = g.get_degree(s);
		// calculate n*<k^2>, n*<k^3>
		sum_squared_degrees += ks*ks;
		sum_cubed_degrees += ks*ks*ks;

		xi[s] = 0;
		for (node t : g.get_neighbors(s)) {
			const uint64_t kt = g.get_degree(t);

			// calculate sum_{st in E} k_s*k_t
			psi += ks*kt;
			// calculate for each s in V: sum_{t in Gamma(s)} k_t
			xi[s] += kt;
		}
	}
#if defined DEBUG
	assert(psi%2 == 0);
#endif
	psi /= 2;

	// ------------------------
	// start computing variance

	Qs = (m*(m + 1) - sum_squared_degrees)/2;
	KG = (m + 1)*sum_squared_degrees - sum_cubed_degrees - 2*psi;
	Phi_1 = (m + 1)*psi;
	Phi_2 = 0;

	for (iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [s,t] = e_it.get_edge();

		const uint64_t ks = g.get_degree(s);
		const uint64_t kt = g.get_degree(t);

		n_paths_4 += (ks - 1)*(kt - 1);
		n_paths_5 += (kt - 1)*(xi[s] - kt - ks + 1) +
					 (ks - 1)*(xi[t] - kt - ks + 1);

		Lambda_1 += (ks - 1)*(xi[t] - ks) + (kt - 1)*(xi[s] - kt);
		Lambda_2 += (ks - 1)*(kt - 1)*(ks + kt);

		Phi_1 -= ks*kt*(ks + kt);
		Phi_2 += (ks + kt)*(sum_squared_degrees - xi[s] - xi[t] - kt*(kt - 1) - ks*(ks - 1));
	}

	Lambda_2 += Lambda_1;

#if defined DEBUG
	assert(n_paths_5%2 == 0);
	assert(Phi_2%2 == 0);
#endif

	n_paths_5 /= 2;
	Phi_2 /= 2;
}

numeric::rational var_num_crossings_forest_rational
(const graphs::undirected_graph& g) noexcept
{
	const uint64_t n = g.get_num_nodes();
	const uint64_t m = g.get_num_edges();

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

	compute_data_forest
	(
		g, n, m,
		Qs, n_paths_4, n_paths_5, KG,
		Phi_1, Phi_2,
		Lambda_1, Lambda_2
	);

	// V[C]
	numeric::rational V(0);
	V += numeric::rational(2,45)*(m + 2)*Qs;
	V -= numeric::rational(1,180)*(2*m + 7)*n_paths_4;
	V -= numeric::rational(1,180)*n_paths_5;
	V += numeric::rational(1,90)*KG;
	V -= numeric::rational(1,60)*Lambda_1;
	V += numeric::rational(1,180)*Lambda_2;
	V += numeric::rational(1,180)*Phi_2;
	V -= numeric::rational(1,90)*Phi_1;
	return V;
}

} // -- namespace properties
} // -- namespace lal
