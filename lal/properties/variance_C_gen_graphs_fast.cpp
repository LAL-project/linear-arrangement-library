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
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

typedef uint64_t bigint;

// lal includes
#include <lal/iterators/edge_iterator.hpp>

#define sorted_edge(u,v) (u < v ? edge(u,v) : edge(v,u))
#define map_has_key(MAP, K, it) ((it = MAP.find(K)) != MAP.end())

#define iterate(i,Ni, j,Nj, JOB)				\
	i=0; j=0;									\
	while (i < Ni.size() and j < Nj.size())	{	\
		if (Ni[i] == Nj[j]) {					\
			JOB;								\
			++i; ++j;							\
		}										\
		else {									\
			auto __Ni_i = Ni[i];				\
			auto __Nj_j = Nj[j];				\
			i += (__Ni_i < __Nj_j);				\
			j += (__Ni_i > __Nj_j);				\
		}										\
	}

struct useful_info_pairs {
	// number of common neighbours
	uint64_t common;
	// sum of the degrees of the common vertices
	uint64_t sum_deg_common;

	useful_info_pairs() {
		common = sum_deg_common = 0;
	}
	useful_info_pairs(uint64_t _common, uint64_t _sdc)
		: common(_common), sum_deg_common(_sdc)
	{
	}
};

typedef map<lal::edge, useful_info_pairs>::iterator IT;
typedef map<lal::edge, useful_info_pairs>::const_iterator CIT;

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace properties {

// ------------------------------
// Variance of C (using formula)
// GENERAL GRAPHS

inline void compute_data_gen_graphs
(
	const ugraph& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& Kg,
	bigint& n_paths_4, bigint& n_cycles_4, bigint& graphlet,
	bigint& n_paths_5, bigint& pair_C3_L2,
	bigint& ks_p_kt__x__ku_p_kv, bigint& ks_x_kt__p__ku_x_kv,
	bigint& sum_adjs__x__sum_degs, bigint& sum__pair__adj_x_deg
)
{

	// ------------------------------------------------
	// local variables (some store precomputed data)

	// allocate memory for all arrays
	const bigint n_bytes = (n + n)*sizeof(bigint);
	bigint *all_memory = static_cast<bigint *>(malloc(n_bytes));
	memset(all_memory, 0, n_bytes);

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	bigint *nds = &all_memory[0];
	// number of 4-paths through each vertex
	bigint *npaths = &all_memory[n];
	// sum of degrees squared
	bigint nk2 = 0;
	// sum of degrees cubed
	bigint nk3 = 0;
	// sum_{st in E} k_s*k_t
	bigint Lg = 0;

	// ------------------------------------------------
	// precompute useful data

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.degree(s);
		nk2 += ks*ks;
		nk3 += ks*ks*ks;
		for (node t : g.get_neighbours(s)) {
			const bigint kt = g.degree(t);
			Lg += ks*kt;
			nds[s] += kt;
		}
	}
	Lg /= 2;

	// ------------------------------------------------
	// compute the variance

	Qs = (m*(m + 1) - nk2)/2;
	Kg = (m + 1)*nk2 - nk3 - 2*Lg;
	ks_x_kt__p__ku_x_kv = (m + 1)*Lg;
	ks_p_kt__x__ku_p_kv = nk2*nk2;

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.degree(s);
		npaths[s] = (nds[s] + 2 - 2*ks)*ks;
	}

	size_t idx1, idx2;
	bigint npaths4_c1 = 0;
	bigint npaths4_c2 = 0;

	edge_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		// first node
		const node s = e.first;
		const bigint ks = g.degree(s);
		const neighbourhood& Ns = g.get_neighbours(s);
		// second node
		const node t = e.second;
		const bigint kt = g.degree(t);
		const neighbourhood& Nt = g.get_neighbours(t);

		// for each neighbour of 's' different from 't'
		for (node u : Ns) {
			if (u == t) { continue; }

			const neighbourhood& Nu = g.get_neighbours(u);
			const bigint ku = g.degree(u);

			bool ut_neighs = std::binary_search(Nt.begin(), Nt.end(), u);
			bigint common_ut = 0;
			iterate(idx1,Nu, idx2,Nt,
				++common_ut;
			)

			n_paths_5 += (kt - 1 - ut_neighs)*(ku - 1 - ut_neighs) + 1;
			n_paths_5 -= common_ut;
		}
		// for each neighbour of 't' different from 's'
		for (node u : Nt) {
			if (u == s) { continue; }

			const neighbourhood& Nu = g.get_neighbours(u);
			const bigint ku = g.degree(u);

			bool us_neighs = std::binary_search(Ns.begin(), Ns.end(), u);
			bigint common_us = 0;
			iterate(idx1,Nu, idx2,Ns,
				++common_us;
			)

			n_paths_5 += (ks - 1 - us_neighs)*(ku - 1 - us_neighs) + 1;
			n_paths_5 -= common_us;
			n_cycles_4 += common_us - 1;
		}

		bigint common_st = 0;
		iterate(idx1,Nt, idx2,Ns,
			++common_st;
			graphlet += g.degree(Ns[idx2]) - 2;
			pair_C3_L2 += m - ks - kt - g.degree(Ns[idx2]) + 3;
		)

		ks_x_kt__p__ku_x_kv -= ks*kt*(ks + kt);
		ks_p_kt__x__ku_p_kv -=
			(ks + kt)*(nds[s] + nds[t] + kt*(kt - 1) + ks*(ks - 1));

		sum__pair__adj_x_deg += ks*(nds[t] - ks - kt + 1) + (kt - 1)*(nds[s] - kt);
		sum__pair__adj_x_deg += kt*(nds[s] - ks - kt + 1) + (ks - 1)*(nds[t] - ks);
		sum__pair__adj_x_deg -= 2*common_st*(ks + kt);

		npaths4_c1 += nds[t] + nds[s];
		npaths4_c2 += common_st;

		npaths[s] += nds[t] - 2*(kt + common_st);
		npaths[t] += nds[s] - 2*(ks + common_st);
	}

	for (node s = 0; s < n; ++s) {
		sum_adjs__x__sum_degs += g.degree(s)*npaths[s];
	}

	ks_p_kt__x__ku_p_kv /= 2;
	n_paths_4 = m - nk2 + npaths4_c1/2 - npaths4_c2;
	n_cycles_4 /= 4;
	n_paths_5 /= 2;
	pair_C3_L2 /= 3;
	sum__pair__adj_x_deg /= 2;

	// deallocate memory
	free(all_memory);
}

/* Warning:
 * 'pair_C3_L2' is, by 3 times, larger than the actual amount of
 * pairs of disjoint cycle of 3 vertices and edge. This is done
 * so that we don't have to change the rational numbers used to
 * compute the variance.
 */
inline void compute_data_gen_graphs_reuse
(
	const ugraph& g, const bigint& n, const bigint& m,
	bigint& Qs, bigint& Kg,
	bigint& n_paths_4, bigint& n_cycles_4, bigint& graphlet,
	bigint& n_paths_5, bigint& pair_C3_L2,
	bigint& ks_p_kt__x__ku_p_kv, bigint& ks_x_kt__p__ku_x_kv,
	bigint& sum_adjs__x__sum_degs, bigint& sum__pair__adj_x_deg
)
{

	// ------------------------------------------------
	// local variables (some store precomputed data)

	// allocate memory for all arrays
	const bigint n_bytes = (n + n)*sizeof(bigint);
	bigint *all_memory = static_cast<bigint *>(malloc(n_bytes));
	memset(all_memory, 0, n_bytes);

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	bigint *nds = &all_memory[0];
	// number of 4-paths through each vertex
	bigint *npaths = &all_memory[n];
	// sum of degrees squared
	bigint nk2 = 0;
	// sum of degrees cubed
	bigint nk3 = 0;
	// sum_{st in E} k_s*k_t
	bigint Lg = 0;

	// ------------------------------------------------
	// precompute useful data

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.degree(s);
		nk2 += ks*ks;
		nk3 += ks*ks*ks;
		for (node t : g.get_neighbours(s)) {
			const bigint kt = g.degree(t);
			Lg += ks*kt;
			nds[s] += kt;
		}
	}
	Lg /= 2;

	// hash table to reuse computations
	map<edge, useful_info_pairs> saving_comps;
	// indices to iterate over (sorted) adjacency lists
	size_t idx1, idx2;

	// ------------------------------------------------
	// compute the variance

	Qs = (m*(m + 1) - nk2)/2;
	Kg = (m + 1)*nk2 - nk3 - 2*Lg;
	ks_x_kt__p__ku_x_kv = (m + 1)*Lg;
	ks_p_kt__x__ku_p_kv = nk2*nk2;

	for (node s = 0; s < n; ++s) {
		const bigint ks = g.degree(s);
		npaths[s] = (nds[s] + 2 - 2*ks)*ks;
	}

	bigint npaths4_c1 = 0;
	bigint npaths4_c2 = 0;

	edge_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		// first node
		const node s = e.first;
		const bigint ks = g.degree(s);
		const neighbourhood& Ns = g.get_neighbours(s);
		// second node
		const node t = e.second;
		const bigint kt = g.degree(t);
		const neighbourhood& Nt = g.get_neighbours(t);

		for (node u : Ns) {
			if (u == t) { continue; }

			bigint common_ut = 0;
			CIT it_ut;
			if (map_has_key(saving_comps, sorted_edge(u,t), it_ut)) {
				common_ut = it_ut->second.common;
			}
			else {
				const neighbourhood& Nu = g.get_neighbours(u);
				bigint deg_sum = 0;
				iterate(idx1,Nt, idx2,Nu,
					++common_ut;
					deg_sum += g.degree(Nu[idx2]);
				)
				saving_comps.insert(
					make_pair(
						sorted_edge(u,t),
						useful_info_pairs(common_ut, deg_sum)
					));
			}

			bool ut_neighs = std::binary_search(Nt.begin(), Nt.end(), u);
			bigint ku = g.degree(u);
			n_paths_5 += (kt - 1 - ut_neighs)*(ku - 1 - ut_neighs) + 1 - common_ut;
		}

		for (node u : Nt) {
			if (u == s) { continue; }

			// precompute data
			bigint common_us = 0;
			CIT it_su;
			if (map_has_key(saving_comps, sorted_edge(u,s), it_su)) {
				common_us = it_su->second.common;
			}
			else {
				// compute values and store them
				const neighbourhood& Nu = g.get_neighbours(u);
				bigint deg_sum = 0;
				iterate(idx1,Ns, idx2,Nu,
					++common_us;
					deg_sum += g.degree(Nu[idx2]);
				)
				saving_comps.insert(
					make_pair(
						sorted_edge(u,s),
						useful_info_pairs(common_us, deg_sum)
					));
			}

			bool us_neighs = std::binary_search(Ns.begin(), Ns.end(), u);
			const bigint ku = g.degree(u);
			n_paths_5 += (ks - 1 - us_neighs)*(ku - 1 - us_neighs) + 1 - common_us;
			n_cycles_4 += common_us;
		}

		// number of common vertices
		bigint common_st = 0;
		// sum of the degrees of the common vertices
		bigint deg_sum_st = 0;
		auto it_st = saving_comps.begin();
		if (map_has_key(saving_comps, sorted_edge(s,t), it_st)) {
			// if the neighbours were not searched then the variables
			// 'sum_deg_common_st' and 'common_st' are equal to '0',
			// so we must initialise them now
			deg_sum_st = it_st->second.sum_deg_common;
			common_st = it_st->second.common;
		}
		else {
			// compute values and store them
			// common neighbours of 's' and 't'

			iterate(idx1,Ns, idx2,Nt,
					++common_st;
					deg_sum_st += g.degree(Nt[idx2]);
			)

			saving_comps.insert(
				make_pair(
					sorted_edge(s,t),
					useful_info_pairs(common_st, deg_sum_st)
				));
		}

		n_cycles_4 -= kt - 1;

		graphlet += deg_sum_st - 2*common_st;
		pair_C3_L2 += common_st*(m - ks - kt + 3) - deg_sum_st;

		ks_x_kt__p__ku_x_kv -= ks*kt*(ks + kt);
		ks_p_kt__x__ku_p_kv -=
			(ks + kt)*(nds[s] + nds[t] + kt*(kt - 1) + ks*(ks - 1));

		sum__pair__adj_x_deg += ks*(nds[t] - ks - kt + 1) + (kt - 1)*(nds[s] - kt);
		sum__pair__adj_x_deg += kt*(nds[s] - ks - kt + 1) + (ks - 1)*(nds[t] - ks);
		sum__pair__adj_x_deg -= 2*common_st*(ks + kt);

		npaths4_c1 += nds[t] + nds[s];
		npaths4_c2 += common_st;

		npaths[s] += nds[t] - 2*(kt + common_st);
		npaths[t] += nds[s] - 2*(ks + common_st);
	}

	for (node s = 0; s < n; ++s) {
		sum_adjs__x__sum_degs += g.degree(s)*npaths[s];
	}

	ks_p_kt__x__ku_p_kv /= 2;
	n_paths_4 = m - nk2 + npaths4_c1/2 - npaths4_c2;
	n_cycles_4 /= 4;
	n_paths_5 /= 2;
	pair_C3_L2 /= 3;
	sum__pair__adj_x_deg /= 2;

	// deallocate memory
	free(all_memory);
}

rational variance_C_rational(const ugraph& g, bool reuse) {
	assert(g.is_normalised());

	const bigint n = g.n_nodes();
	const bigint m = g.n_edges();

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
	bigint graphlet = 0;
	// the amount of pairs of disjoint
	// triangle and edge in the graph.
	bigint pair_C3_L2 = 0;

	// k_s + k_t + k_u + k_v
	bigint Kg = 0;
	// (k_s + k_t)(k_u + k_v)
	bigint ks_p_kt__x__ku_p_kv = 0;
	// (k_s*k_t + k_u*k_v)
	bigint ks_x_kt__p__ku_x_kv = 0;

	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	bigint sum_adjs__x__sum_degs = 0;
	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	bigint sum__pair__adj_x_deg = 0;

	if (reuse) {
		compute_data_gen_graphs_reuse
		(
			g, n, m,
			Qs, Kg,
			n_paths_4, n_cycles_4, graphlet,
			n_paths_5, pair_C3_L2,
			ks_p_kt__x__ku_p_kv, ks_x_kt__p__ku_x_kv,
			sum_adjs__x__sum_degs, sum__pair__adj_x_deg
		);
	}
	else {
		compute_data_gen_graphs
		(
			g, n, m,
			Qs, Kg,
			n_paths_4, n_cycles_4, graphlet,
			n_paths_5, pair_C3_L2,
			ks_p_kt__x__ku_p_kv, ks_x_kt__p__ku_x_kv,
			sum_adjs__x__sum_degs, sum__pair__adj_x_deg
		);
	}

	integer J(0);

	// V[C]
	rational V(0);
	J.init_ui((m + 2)*Qs);				V += rational(2,45)*J;
	J.init_ui((2*m + 7)*n_paths_4);		V -= rational(1,180)*J;
	J.init_ui(n_paths_5);				V -= rational(1,180)*J;
	J.init_ui(Kg);						V += rational(1,90)*J;
	J.init_ui(n_cycles_4);				V -= rational(3,45)*J;
	J.init_ui(sum__pair__adj_x_deg);	V -= rational(1,60)*J;
	J.init_ui(sum_adjs__x__sum_degs);	V += rational(1,180)*J;
	J.init_ui(ks_p_kt__x__ku_p_kv);		V += rational(1,180)*J;
	J.init_ui(ks_x_kt__p__ku_x_kv);		V -= rational(1,90)*J;
	J.init_ui(graphlet);				V += rational(1,30)*J;
	J.init_ui(pair_C3_L2);				V += rational(1,30)*J;
	return V;
}

double variance_C(const ugraph& g, bool reuse) {
	assert(g.is_normalised());
	rational V = variance_C_rational(g, reuse);
	return V.to_double();
}

} // -- namespace properties
} // -- namespace lal
