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
#if defined DEBUG
#include <cassert>
#endif
#include <map>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/data_array.hpp>

typedef uint64_t bigint;

#define sorted_edge(u,v) (u < v ? edge(u,v) : edge(v,u))
#define map_has_key(MAP, K, it) ((it = MAP.find(K)) != MAP.end())

/* This macro has two local variables: __i, __j
 * The first variable, __i, iterates over Ni.
 * The first variable, __j, iterates over Nj.
 */
#define iterate(Ni, Nj, JOB)						\
{													\
	size_t __i = 0;									\
	size_t __j = 0;									\
	while (__i < Ni.size() and __j < Nj.size())	{	\
		if (Ni[__i] == Nj[__j]) {					\
			JOB;									\
			++__i; ++__j;							\
		}											\
		else {										\
			const auto __Ni_i = Ni[__i];			\
			const auto __Nj_j = Nj[__j];			\
			__i += (__Ni_i < __Nj_j);				\
			__j += (__Ni_i > __Nj_j);				\
		}											\
	}												\
}

struct useful_info_pairs {
	// number of common neighbours
	uint64_t common = 0;
	// sum of the degrees of the common nodes
	uint64_t sum_deg_common = 0;

	useful_info_pairs(uint64_t _common, uint64_t _sdc)
		: common(_common), sum_deg_common(_sdc)
	{
	}
};

typedef map<lal::edge, useful_info_pairs>::const_iterator CIT;

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace properties {

// ------------------------------
// Variance of C (using formula)
// GENERAL GRAPHS

/* Warning:
 * 'pair_C3_L2' is 3 times larger than the actual amount of
 * pairs of disjoint cycle of 3 nodes and edge. This is done
 * so that we don't have to change the rational numbers used to
 * compute the variance.
 */
template<bool reuse>
void compute_data_gen_graphs
(
	const undirected_graph& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& Kg,
	bigint& n_paths_4, bigint& n_cycles_4, bigint& paw,
	bigint& n_paths_5, bigint& pair_C3_L2,
	bigint& Phi_1, bigint& Phi_2,
	bigint& Lambda_1, bigint& Lambda_2
)
{

	// ------------------------------------------------
	// local variables (some store precomputed data)

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	internal::data_array<bigint> xi(n);
	// sum of degrees squared
	bigint nk2 = 0;
	// sum of degrees cubed
	bigint nk3 = 0;
	// sum_{st in E} k_s*k_t
	bigint psi = 0;

	// ------------------------------------------------
	// precompute data

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.get_degree(s);
		nk2 += ks*ks;
		nk3 += ks*ks*ks;

		xi[s] = 0;
		for (node t : g.get_neighbours(s)) {
			const bigint kt = g.get_degree(t);
			psi += ks*kt;
			xi[s] += kt;
		}
	}
	// note: at this point 'psi' is equal to
	// 2*sum_{st in E} k_s*k_t
#if defined DEBUG
	assert(psi%2 == 0);
#endif
	psi /= 2;

	// hash table to reuse computations
	map<edge, useful_info_pairs> saving_comps;

	// ------------------------------------------------
	// compute the variance

	Qs = (m*(m + 1) - nk2)/2;
	Kg = (m + 1)*nk2 - nk3 - 2*psi;
	Phi_1 = (m + 1)*psi;
	Phi_2 = 0;

	bigint mu1 = 0;
	bigint mu2 = 0;

	E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const auto [s,t] = it.get_edge();

		const bigint ks = g.get_degree(s);
		const neighbourhood& Ns = g.get_neighbours(s);

		const bigint kt = g.get_degree(t);
		const neighbourhood& Nt = g.get_neighbours(t);

		// for each neighbour of 's' different from 't'
		for (node u : Ns) {
			if (u == t) { continue; }

			const neighbourhood& Nu = g.get_neighbours(u);
			const bigint ku = g.get_degree(u);

			bigint common_ut = 0;
			if constexpr (reuse) {
				if (CIT it_ut; map_has_key(saving_comps, sorted_edge(u,t), it_ut)) {
					common_ut = it_ut->second.common;
				}
				else {
					// compute values and store them
					bigint deg_sum = 0;
					iterate(Nt, Nu,
						++common_ut;
						deg_sum += g.get_degree(Nu[__j]);
					)
					saving_comps.insert(
						make_pair(
							sorted_edge(u,t),
							useful_info_pairs(common_ut, deg_sum)
						));
				}
			}
			else {
				iterate(Nu, Nt,
					++common_ut;
				)
			}

			const bool ut_is_edge = g.has_edge(u,t);
			n_paths_5 += (kt - 1 - ut_is_edge)*(ku - 1 - ut_is_edge) + 1 - common_ut;
		}
		// for each neighbour of 't' different from 's'
		for (node u : Nt) {
			if (u == s) { continue; }

			const neighbourhood& Nu = g.get_neighbours(u);
			const bigint ku = g.get_degree(u);

			bigint common_us = 0;
			if constexpr (reuse) {
				if (CIT it_su; map_has_key(saving_comps, sorted_edge(u,s), it_su)) {
					common_us = it_su->second.common;
				}
				else {
					// compute values and store them
					bigint deg_sum = 0;
					iterate(Ns, Nu,
						++common_us;
						deg_sum += g.get_degree(Nu[__j]);
					)
					saving_comps.insert(
						make_pair(
							sorted_edge(u,s),
							useful_info_pairs(common_us, deg_sum)
						));
				}
			}
			else {
				iterate(Nu, Ns,
					++common_us;
				)
			}

			const bool is_us_edge = g.has_edge(u,s);
			n_paths_5 += (ks - 1 - is_us_edge)*(ku - 1 - is_us_edge) + 1 - common_us;
			n_cycles_4 += common_us;
		}

		n_cycles_4 -= kt - 1;

		bigint common_st = 0;
		bigint deg_sum_st = 0;
		if constexpr (reuse) {
			if (CIT it_st; map_has_key(saving_comps, sorted_edge(s,t), it_st)) {
				// if the neighbours were not searched then the variables
				// 'sum_deg_common_st' and 'common_st' are equal to '0',
				// so we must initialise them now
				deg_sum_st = it_st->second.sum_deg_common;
				common_st = it_st->second.common;
			}
			else {
				// compute values and store them
				iterate(Ns, Nt,
					++common_st;
					deg_sum_st += g.get_degree(Nt[__j]);
				)
				saving_comps.insert(
					make_pair(
						sorted_edge(s,t),
						useful_info_pairs(common_st, deg_sum_st)
					));
			}
		}
		else {
			iterate(Ns, Nt,
				++common_st;
				deg_sum_st += g.get_degree(Ns[__i]);
			)
		}

		paw += deg_sum_st - 2*common_st;
		pair_C3_L2 += common_st*(m - ks - kt + 3) - deg_sum_st;

		Phi_1 -= ks*kt*(ks + kt);
		Phi_2 += (ks + kt)*(nk2 - (ks*(ks - 1) + kt*(kt - 1)) - xi[s] - xi[t]);

		mu1 += xi[t] + xi[s];
		mu2 += common_st;

		Lambda_1 += (kt - 1)*(xi[s] - kt) + (ks - 1)*(xi[t] - ks);
		Lambda_1 -= 2*deg_sum_st;

		Lambda_2 += (ks + kt)*( (ks - 1)*(kt - 1) - common_st );
	}

#if defined DEBUG
	assert(Phi_2%2 == 0);
	assert(mu1%2 == 0);
	assert(n_cycles_4%4 == 0);
	assert(n_paths_5%2 == 0);
	assert(pair_C3_L2%3 == 0);
#endif

	Lambda_2 += Lambda_1;
	Phi_2 /= 2;
	n_paths_4 = m - nk2 + mu1/2 - mu2;
	n_cycles_4 /= 4;
	n_paths_5 /= 2;
	pair_C3_L2 /= 3;
}

rational variance_C_rational(const undirected_graph& g, bool reuse) {
#if defined DEBUG
	assert(g.is_normalised());
#endif

	const bigint n = g.get_num_nodes();
	const bigint m = g.get_num_edges();

	// ----------------------------
	// compute terms dependent of Q

	// size of set Q
	bigint Qs = 0;

	// n_G(L_4)
	bigint n_paths_4 = 0;
	// n_G(L_5)
	bigint n_paths_5 = 0;
	// n_G(C_4)
	bigint n_cycles_4 = 0;

	// (a_{tu} + a_{sv})(a_{tv} + a_{su})
	bigint paw = 0;
	// the amount of pairs of disjoint
	// triangle and edge in the graph.
	bigint pair_C3_L2 = 0;

	// k_s + k_t + k_u + k_v
	bigint Kg = 0;
	// (k_s*k_t + k_u*k_v)
	bigint Phi_1 = 0;
	// (k_s + k_t)(k_u + k_v)
	bigint Phi_2 = 0;

	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	bigint Lambda_1 = 0;
	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	bigint Lambda_2 = 0;

	if (reuse) {
		compute_data_gen_graphs<true>
		(
			g, n, m,
			Qs, Kg,
			n_paths_4, n_cycles_4, paw,
			n_paths_5, pair_C3_L2,
			Phi_1, Phi_2,
			Lambda_1, Lambda_2
		);
	}
	else {
		compute_data_gen_graphs<false>
		(
			g, n, m,
			Qs, Kg,
			n_paths_4, n_cycles_4, paw,
			n_paths_5, pair_C3_L2,
			Phi_1, Phi_2,
			Lambda_1, Lambda_2
		);
	}

	integer J(0);

	// V[C]
	rational V(0);
	J.set_ui((m + 2)*Qs);			V += rational(2,45)*J;
	J.set_ui((2*m + 7)*n_paths_4);	V -= rational(1,180)*J;
	J.set_ui(n_paths_5);			V -= rational(1,180)*J;
	J.set_ui(Kg);					V += rational(1,90)*J;
	J.set_ui(n_cycles_4);			V -= rational(3,45)*J;
	J.set_ui(Lambda_1);				V -= rational(1,60)*J;
	J.set_ui(Lambda_2);				V += rational(1,180)*J;
	J.set_ui(Phi_2);				V += rational(1,180)*J;
	J.set_ui(Phi_1);				V -= rational(1,90)*J;
	J.set_ui(paw);					V += rational(1,30)*J;
	J.set_ui(pair_C3_L2);			V += rational(1,30)*J;
	return V;
}

double variance_C(const undirected_graph& g, bool reuse) {
	return variance_C_rational(g, reuse).to_double();
}

} // -- namespace properties
} // -- namespace lal
