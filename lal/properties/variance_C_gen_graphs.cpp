/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <iostream>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/data_array.hpp>

#define sorted_edge(u,v) (u < v ? edge(u,v) : edge(v,u))
#define has_key(MAP, K, it) ((it = MAP.find(K)) != MAP.end())

/* This macro has two local variables: __i, __j
 * The first variable, __i, iterates over Ni.
 * The first variable, __j, iterates over Nj.
 */
#define iterate(Ni, Nj, JOB)						\
{													\
	std::size_t __i = 0;							\
	std::size_t __j = 0;							\
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

#define USE_HASH
//#define USE_MAP

#if defined USE_HASH

#include <unordered_map>

/*
// stackoverflow hash

template<typename T>
void hash_combine(std::size_t& seed, const T& key) {
	std::hash<T> hasher;
	seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct hash_edge {
	std::size_t operator()(const lal::edge& p) const {
		std::size_t seed1(0);
		hash_combine(seed1, p.first);
		hash_combine(seed1, p.second);

		std::size_t seed2(0);
		hash_combine(seed2, p.second);
		hash_combine(seed2, p.first);

		return std::min(seed1, seed2);
	}
};
*/

struct hash_edge {
	std::size_t operator()(const lal::edge& p) const {
		// Cantor hash
		// source: https://cran.r-project.org/web/packages/clustAnalytics/index.html
		// From the Ph.D. thesis of Martí Renedo Mirambell (Polytechnic University of Catalonia)
		return (p.first + p.second) * (p.first + p.second + 1) / 2 + p.second;
	}
};

typedef std::unordered_map<lal::edge, useful_info_pairs, hash_edge> hash_algorithm;
typedef hash_algorithm::const_iterator CIT;

#endif

#if defined USE_MAP

#include <map>

typedef std::map<lal::edge, useful_info_pairs> hash_algorithm;
typedef hash_algorithm::const_iterator CIT;

#endif

namespace lal {
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
template <bool reuse, bool is_normalised>
void compute_data_gen_graphs
(
	const graphs::undirected_graph& g, const uint64_t& n, const uint64_t& m,
	uint64_t& Qs, uint64_t& Kg,
	uint64_t& n_paths_4, uint64_t& n_cycles_4, uint64_t& paw,
	uint64_t& n_paths_5, uint64_t& pair_C3_L2,
	uint64_t& Phi_1, uint64_t& Phi_2,
	uint64_t& Lambda_1, uint64_t& Lambda_2
)
noexcept
{
	// ----------------------------------------------------------
	// DATA USED TO DEAL WITH THE NON-NORMALISED CASE

	// this vector is used to store the sorted adjacency lists of
	// a non-normalised graph
	std::vector<neighbourhood> sorted_neighbourhoods;

	// if the graph is not normalised, dump all the vector here and normalise it
	if constexpr (not is_normalised) {
		detail::data_array<char> mem(n, 0);

		sorted_neighbourhoods.resize(n);
		for (node u = 0; u < n; ++u) {
			sorted_neighbourhoods[u] = g.get_neighbours(u);

			detail::sorting::bit_sort_mem<node>(
				sorted_neighbourhoods[u].begin(),
				sorted_neighbourhoods[u].end(),
				sorted_neighbourhoods[u].size(),
				mem.begin()
			);
		}
	}

	// ------------------------------------------------
	// local variables (some store precomputed data)

	// neighbour's degree sum: nds_s = sum_{st in E} k_t
	detail::data_array<uint64_t> xi(n);
	// sum of degrees squared
	uint64_t nk2 = 0;
	// sum of degrees cubed
	uint64_t nk3 = 0;
	// sum_{st in E} k_s*k_t
	uint64_t psi = 0;

	// ------------------------------------------------
	// precompute data

	for (node s = 0; s < n; ++s) {
		const uint64_t ks = g.get_degree(s);
		nk2 += ks*ks;
		nk3 += ks*ks*ks;

		xi[s] = 0;
		for (node t : g.get_neighbours(s)) {
			const uint64_t kt = g.get_degree(t);
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
	hash_algorithm H;
#if defined USE_HASH
	if constexpr (reuse) {
		H.reserve(g.get_num_edges() + g.get_num_nodes());
	}
#endif

	// ------------------------------------------------
	// compute the variance

	Qs = (m*(m + 1) - nk2)/2;
	Kg = (m + 1)*nk2 - nk3 - 2*psi;
	Phi_1 = (m + 1)*psi;
	Phi_2 = 0;

	uint64_t mu = 0;

	for (iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [s,t] = e_it.get_edge();

		const uint64_t ks = g.get_degree(s);
		const neighbourhood& Ns =
			(is_normalised ? g.get_neighbours(s) : sorted_neighbourhoods[s]);

		const uint64_t kt = g.get_degree(t);
		const neighbourhood& Nt =
			(is_normalised ? g.get_neighbours(t) : sorted_neighbourhoods[t]);

		// for each neighbour of 's' different from 't'
		for (node u : Ns) {
			if (u == t) { continue; }

			const uint64_t ku = g.get_degree(u);
			const neighbourhood& Nu =
				(is_normalised ? g.get_neighbours(u) : sorted_neighbourhoods[u]);

			uint64_t common_ut = 0;
			if constexpr (reuse) {
				if (CIT it_ut; has_key(H, sorted_edge(u,t), it_ut)) {
					common_ut = it_ut->second.common;
				}
				else {
					// compute values and store them
					uint64_t deg_sum = 0;
					iterate(Nt, Nu,
						++common_ut;
						deg_sum += g.get_degree(Nu[__j]);
					);
					H.insert(
						make_pair(
							sorted_edge(u,t),
							useful_info_pairs(common_ut, deg_sum)
						));
				}
			}
			else {
				iterate(Nu, Nt,
					++common_ut;
				);
			}

			const bool ut_is_edge = g.has_edge(u,t);
			n_paths_5 += (kt - 1 - ut_is_edge)*(ku - 1 - ut_is_edge) + 1 - common_ut;
		}
		// for each neighbour of 't' different from 's'
		for (node u : Nt) {
			if (u == s) { continue; }

			const uint64_t ku = g.get_degree(u);
			const neighbourhood& Nu =
				(is_normalised ? g.get_neighbours(u) : sorted_neighbourhoods[u]);

			uint64_t common_us = 0;
			if constexpr (reuse) {
				if (CIT it_su; has_key(H, sorted_edge(u,s), it_su)) {
					common_us = it_su->second.common;
				}
				else {
					// compute values and store them
					uint64_t deg_sum = 0;
					iterate(Ns, Nu,
						++common_us;
						deg_sum += g.get_degree(Nu[__j]);
					);
					H.insert(
						make_pair(
							sorted_edge(u,s),
							useful_info_pairs(common_us, deg_sum)
						));
				}
			}
			else {
				iterate(Nu, Ns,
					++common_us;
				);
			}

			const bool is_us_edge = g.has_edge(u,s);
			n_paths_5 += (ks - 1 - is_us_edge)*(ku - 1 - is_us_edge) + 1 - common_us;
			n_cycles_4 += common_us;
		}

		n_cycles_4 -= kt - 1;

		uint64_t common_st = 0;
		uint64_t deg_sum_st = 0;
		if constexpr (reuse) {
			if (CIT it_st; has_key(H, sorted_edge(s,t), it_st)) {
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
				);
				H.insert(
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
			);
		}

		paw += deg_sum_st - 2*common_st;
		pair_C3_L2 += common_st*(m - ks - kt + 3) - deg_sum_st;

		Phi_1 -= ks*kt*(ks + kt);
		Phi_2 += (ks + kt)*(nk2 - (ks*(ks - 1) + kt*(kt - 1)) - xi[s] - xi[t]);

		mu += common_st;

		Lambda_1 += (kt - 1)*(xi[s] - kt) + (ks - 1)*(xi[t] - ks);
		Lambda_1 -= 2*deg_sum_st;

		Lambda_2 += (ks + kt)*( (ks - 1)*(kt - 1) - common_st );
	}

#if defined DEBUG
	assert(Phi_2%2 == 0);
	assert(n_cycles_4%4 == 0);
	assert(n_paths_5%2 == 0);
	assert(pair_C3_L2%3 == 0);
#endif

	Lambda_2 += Lambda_1;
	Phi_2 /= 2;
	n_paths_4 = m - nk2 + psi - mu;
	n_cycles_4 /= 4;
	n_paths_5 /= 2;
	pair_C3_L2 /= 3;
}

numeric::rational var_num_crossings_rational
(const graphs::undirected_graph& g, bool reuse)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	const uint64_t m = g.get_num_edges();

	// ----------------------------
	// compute terms dependent of Q

	// size of set Q
	uint64_t Qs = 0;

	// n_G(L_4)
	uint64_t n_paths_4 = 0;
	// n_G(L_5)
	uint64_t n_paths_5 = 0;
	// n_G(C_4)
	uint64_t n_cycles_4 = 0;

	// (a_{tu} + a_{sv})(a_{tv} + a_{su})
	uint64_t paw = 0;
	// the amount of pairs of disjoint
	// triangle and edge in the graph.
	uint64_t pair_C3_L2 = 0;

	// k_s + k_t + k_u + k_v
	uint64_t Kg = 0;
	// (k_s*k_t + k_u*k_v)
	uint64_t Phi_1 = 0;
	// (k_s + k_t)(k_u + k_v)
	uint64_t Phi_2 = 0;

	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	uint64_t Lambda_1 = 0;
	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	uint64_t Lambda_2 = 0;

#define parameters_of_compute_data	\
	(								\
		g, n, m,					\
		Qs, Kg,						\
		n_paths_4, n_cycles_4, paw,	\
		n_paths_5, pair_C3_L2,		\
		Phi_1, Phi_2,				\
		Lambda_1, Lambda_2			\
	)

	if (reuse) {
		if (g.is_normalised()) {
			compute_data_gen_graphs<true, true>
				parameters_of_compute_data;
		}
		else {
			compute_data_gen_graphs<true, false>
				parameters_of_compute_data;
		}
	}
	else {
		if (g.is_normalised()) {
			compute_data_gen_graphs<false, true>
				parameters_of_compute_data;
		}
		else {
			compute_data_gen_graphs<false, false>
				parameters_of_compute_data;
		}
	}

	// V[C]
	numeric::rational V(0);
	V += numeric::rational(2,45)*(m + 2)*Qs;
	V -= numeric::rational(1,180)*(2*m + 7)*n_paths_4;
	V -= numeric::rational(1,180)*n_paths_5;
	V += numeric::rational(1,90)*Kg;
	V -= numeric::rational(3,45)*n_cycles_4;
	V -= numeric::rational(1,60)*Lambda_1;
	V += numeric::rational(1,180)*Lambda_2;
	V += numeric::rational(1,180)*Phi_2;
	V -= numeric::rational(1,90)*Phi_1;
	V += numeric::rational(1,30)*paw;
	V += numeric::rational(1,30)*pair_C3_L2;
	return V;
}

} // -- namespace properties
} // -- namespace lal
