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

#include <lal/generate/rand_ulab_rooted_trees.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>

#define to_int32(x) static_cast<int32_t>(x)
#define to_uint32(x) static_cast<uint32_t>(x)

namespace lal {
using namespace numeric;
using namespace graphs;

namespace generate {

// -----------------------------------------------------------------------------
// ACTUAL GENERATOR

__rand_ulab_rooted_trees::__rand_ulab_rooted_trees(uint32_t _n, uint32_t seed) {
	init_rn();
	__rand_ulab_rooted_trees::init(_n, seed);
}
__rand_ulab_rooted_trees::~__rand_ulab_rooted_trees() {
	delete[] m_tree;
}

void __rand_ulab_rooted_trees::init(uint32_t _n, uint32_t seed) {
	m_n = _n;
	if (m_n <= 1) { return; }

	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}
	m_unif = uniform_real_distribution<double>(0, 1);

	init_rn();
	m_tree = new uint32_t[m_n];
}

rooted_tree __rand_ulab_rooted_trees::get_tree() {
	if (m_n <= 1) {
		rooted_tree r(m_n, 0);
		r.set_root(0);
		r.set_valid_orientation(true);
		return r;
	}

	// call with an invalid index for the 'root of the last tree added'
	// so as to indicate that there is no such thing at this moment.
	ranrut(m_n, 0, 0);

	rooted_tree rT(m_n);
	for (node u = 1; u < m_n; ++u) {
		// in order to construct an arborescence
		// orient edges away from the root (node 0).
		rT.add_edge_bulk(m_tree[u], u);
	}
	rT.finish_bulk_add(false, false);
	rT.set_root(0);
	rT.set_valid_orientation(true);
	return rT;
}

void __rand_ulab_rooted_trees::clear() {
	m_rn.clear();
	init_rn();
}

/* PROTECTED */

pair<uint32_t,uint32_t>
__rand_ulab_rooted_trees::ranrut(uint32_t n, uint32_t lr, uint32_t nt)
{
	if (n == 0) {
		// The new tree has no nodes.
		// The returned indices are:
		//    lr: because we haven't generated any new tree (so, no new root),
		//    nt: because the place where to store the next tree to be generated
		//        is still 'nt'
		return make_pair(lr,nt);
	}
	if (n == 1) {
		// The new tree has a single node, to be stored in 'nt'.
		// This node has to point to root of the last tree that was generated.
		m_tree[nt] = lr;

		// The returned indices are:
		//    nt: the root of the last tree generated is placed at 'nt'
		//    nt+1: the place where to store the next tree that will be generated
		//          has to be placed at nt+1
		return make_pair(nt, nt + 1);
	}
	if (n == 2) {
		// The new tree has two nodes.
		// The root, placed at 'nt', points to root of the last tree generated.
		m_tree[nt] = lr;

		// The second node, placed at 'nt+1', points to this tree's root,
		// placed at 'nt'.
		m_tree[nt + 1] = nt;
		// The returned indices are:
		//    nt: the root of the last tree generated is placed at 'nt'
		//    nt+1: the place where to store the next tree that will be generated
		//          has to be placed at nt+1
		return make_pair(nt, nt + 2);
	}

	const auto [j, d] = choose_jd_from_T(n);

#if defined DEBUG
	// Both 'j' and 'd' must be strictly positive.
	// Use two assertions to spot which fails more easily.
	assert(j > 0);
	assert(d > 0);
#endif

	// -------------------------------------------------------------
	// Generate T' (a random rooted tree of n - j*d nodes)
	const auto [root_Tp, store_Tpp] = ranrut(n - j*d, lr,nt);

	// -------------------------------------------------------------
	// Generate T'' (a random rooted tree of d nodes)
	// NOTE:
	//    1. We have to have j copies of T''.
	//	  2. One of the copies has already been made, and is
	//		 already connected to the root of T'.
	//	  3. root_Tpp is the position of the root of the first copy of T''.
	const auto [root_Tpp, nt2] = ranrut(d, root_Tp,store_Tpp);

	// -------------------------------------------------------------
	// Make j - 1 copies of T'' and connect them to T'.
	// The nodes of T'' are placed in TREE[root_Tpp:(root_Tpp + d - 1)]

	nt = nt2;
	for (uint32_t c = 1; c < j; ++c) {
		// Each copy of T'' is a child of T'.
		// Therefore, the root of each copy of T''
		// must be connected to the root of T'.
		m_tree[nt] = root_Tp;
		// make a copy of T''
		for (uint32_t v = nt + 1; v < nt + d; ++v) {
			// 'v - c*d' is the position of 'v' relative to
			// the root of the first copy (the first T'').

			// 'TREE[v - c*d] - root_Tpp' is the increment with
			// respect to the new root ('nt') so that the
			// node in 'v' eventually connects with 'nt'.
			m_tree[v] = nt + m_tree[v - c*d] - root_Tpp;
		}
		nt += d;
	}

	// The root of this tree generated is placed at the
	// position at which the root of T' was stored: root_Tp.
	// The variable 'nt' contains the position where the next
	// tree must be generated at.
#if defined DEBUG
	assert(nt <= m_n);
#endif
	return make_pair(root_Tp, nt);
}

void __rand_ulab_rooted_trees::init_rn() {
	// from the OEIS: https://oeis.org/A000081
	m_rn = vector<integer>(31);
	m_rn[0] = 0;
	m_rn[1] = 1;
	m_rn[2] = 1;
	m_rn[3] = 2;
	m_rn[4] = 4;
	m_rn[5] = 9;
	m_rn[6] = 20;
	m_rn[7] = 48;
	m_rn[8] = 115;
	m_rn[9] = 286;
	m_rn[10] = 719;
	m_rn[11] = 1842;
	m_rn[12] = 4766;
	m_rn[13] = 12486;
	m_rn[14] = 32973;
	m_rn[15] = 87811;
	m_rn[16] = 235381;
	m_rn[17] = 634847;
	m_rn[18] = 1721159;
	m_rn[19] = 4688676;
	m_rn[20] = 12826228;
	m_rn[21] = 35221832;
	m_rn[22] = 97055181;
	m_rn[23] = 268282855;
	m_rn[24] = 743724984;
	m_rn[25] = integer("2067174645");
	m_rn[26] = integer("5759636510");
	m_rn[27] = integer("16083734329");
	m_rn[28] = integer("45007066269");
	m_rn[29] = integer("126186554308");
	m_rn[30] = integer("354426847597");
}

const integer& __rand_ulab_rooted_trees::get_rn(uint32_t n) {
	if (m_rn.size() >= n + 1) {
		// value already computed
		return m_rn[n];
	}

	// the algorithm in the book (\cite Nijenhuis1978a)

	uint32_t k = to_uint32(m_rn.size()) - 1;
	while (k <= n + 1) {
		integer s = 0;
		for (uint32_t d = 1; d <= k; ++d) {
			const integer td = m_rn[d]*d;

			int32_t i = to_int32(k) + 1;
			int32_t j = 1;
			while (j <= to_int32(k) and i > 0) {
				i -= to_int32(d);

				// --
				//if (i > 0) { s += m_rn[utils::to_uint32(i)]*td; }
				s += (i > 0 ? m_rn[to_uint32(i)]*td : 0);
				// --

				++j;
			}
		}
		s /= k;

		m_rn.push_back(std::move(s));
		++k;
	}
	return m_rn[n];
}

pair<uint32_t, uint32_t> __rand_ulab_rooted_trees::choose_jd_from_T(uint32_t n)
{
	// Weight of the pair to choose. It will be decreased
	// at every iteration, and we will have found our pair
	// when it reaches a value below or equal to 0

	const double r = m_unif(m_gen);
	double weight = (get_rn(n)*(n - 1)).to_double()*r;

	// Generate all possible pairs. For each pair calculate
	// the weight and substract it from z. As soon as 'z'
	// reaches 0 or less, we found a pair with its probability.
	uint32_t j = 1;
	uint32_t d = 1;

	while (weight > 0) {

		if (n <= j*d) {
			// we need to "start a next pair"
			++d;
			j = 1;
		}
		else {
			// substract weight of current pair
			weight -= (get_rn(n - j*d)*get_rn(d)*d).to_double();
			// if 'z' has not reached 0 then generate next pair

			// --
			//if (weight > 0) { ++j; }
			j += (weight > 0 ? 1 : 0);
			// --
		}
	}

	return make_pair(j, d);
}

// -----------------------------------------------------------------------------
// WRAPPER CLASS

rand_ulab_rooted_trees::rand_ulab_rooted_trees(uint32_t n, uint32_t seed)
: m_Gen(n, seed)
{
}

void rand_ulab_rooted_trees::init(uint32_t n, uint32_t seed) {
	m_Gen.init(n, seed);
}

/* PROTECTED */

graphs::rooted_tree rand_ulab_rooted_trees::__get_tree() {
	return m_Gen.get_tree();
}

} // -- namespace generate
} // -- namespace lal
