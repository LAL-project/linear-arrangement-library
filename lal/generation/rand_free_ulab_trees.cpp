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
 
#include <lal/generation/rand_free_ulab_trees.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/conversions.hpp>

#define _to_int64(x) static_cast<int64_t>(x)
#define _to_uint64(x) static_cast<uint64_t>(x)
#define _to_double(x) static_cast<double>(x)

namespace lal {
using namespace graphs;
using namespace numeric;

namespace generate {

/* PUBLIC */

rand_free_ulab_trees::rand_free_ulab_trees() {
	init_T();
}
rand_free_ulab_trees::rand_free_ulab_trees(uint64_t _n, uint64_t seed) {
	init_T();
	init(_n, seed);
}
rand_free_ulab_trees::~rand_free_ulab_trees() { }

void rand_free_ulab_trees::init(uint64_t _n, uint64_t seed) {
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

	compute_T();
	compute_Amq();
	m_TREE = vector<uint64_t>(m_n + 1);
}

utree rand_free_ulab_trees::make_rand_tree() {
	if (m_n <= 1) { return utree(m_n); }
	if (m_n == 2) {
		utree t(2);
		t.add_edge(0,1);
		return t;
	}

	std::fill(m_TREE.begin(), m_TREE.end(), 0);

	double prob = 0;
	if (m_n%2 != 0) {
		// n is odd
		prob = 0;
	}
	else {
		// n is even...
		const double k = m_Tn[m_n/2].to_double() + 1;
		const double k_2 = k*(k - 1)*0.5;
		prob = k_2/m_Tn[m_n].to_double();
	}
	assert(prob <= 1.0);

	const double bicent_prob = m_unif(m_gen);
	if (bicent_prob <= prob) {
		// tree will have two centroids
		bicenter(m_n);
		return utils::linear_sequence_to_tree(m_TREE, m_n);
	}

	const uint64_t m = m_n - 1;
	const uint64_t q = (m_n - 1)/2;

	// compute forest
	vector<uint64_t> ROOTS(m_n - 1, 0);

	// make a forest on (n - 1) vertices
	const pair<uint64_t, uint64_t> nr_nt = forest(m,q, 0,1, ROOTS);
	const uint64_t nt = nr_nt.second;

	// join all roots of the connected components
	// into a new vertex, placed at the end of
	// the array TREE
	for (uint64_t r : ROOTS) {
		if (r != 0) {
			m_TREE[r] = nt;
		}
	}

#if defined DEBUG
	// make sure that the number of zeroes
	// is EXACTLY one
	uint64_t nz = 0;
	for (uint64_t i = 1; i < m_TREE.size(); ++i) {
		nz += (m_TREE[i] == 0);
	}
	assert(nz == 1);
#endif

	return utils::linear_sequence_to_tree(m_TREE, m_n);
}

void rand_free_ulab_trees::clear() {
	m_Tn.clear();
	m_Amq.clear();
	init_T();
}

/* PRIVATE */

void rand_free_ulab_trees::init_T() {
	// from the OEIS: https://oeis.org/A000081
	m_Tn = vector<integer>(31);
	m_Tn[0] = 0;
	m_Tn[1] = 1;
	m_Tn[2] = 1;
	m_Tn[3] = 2;
	m_Tn[4] = 4;
	m_Tn[5] = 9;
	m_Tn[6] = 20;
	m_Tn[7] = 48;
	m_Tn[8] = 115;
	m_Tn[9] = 286;
	m_Tn[10] = 719;
	m_Tn[11] = 1842;
	m_Tn[12] = 4766;
	m_Tn[13] = 12486;
	m_Tn[14] = 32973;
	m_Tn[15] = 87811;
	m_Tn[16] = 235381;
	m_Tn[17] = 634847;
	m_Tn[18] = 1721159;
	m_Tn[19] = 4688676;
	m_Tn[20] = 12826228;
	m_Tn[21] = 35221832;
	m_Tn[22] = 97055181;
	m_Tn[23] = 268282855;
	m_Tn[24] = 743724984;
	m_Tn[25] = integer("2067174645");
	m_Tn[26] = integer("5759636510");
	m_Tn[27] = integer("16083734329");
	m_Tn[28] = integer("45007066269");
	m_Tn[29] = integer("126186554308");
	m_Tn[30] = integer("354426847597");
}

void rand_free_ulab_trees::compute_T() {
	if (m_Tn.size() >= m_n + 1) {
		// values already computed
		return;
	}

	const int last = static_cast<int>(m_Tn.size());
	m_Tn.resize(m_n + 1);

	// using the algorithm in the book...

	uint64_t LAST = _to_uint64(last) - 1;
	//Tn[1] = 1;
	while (LAST < m_n) {
		integer s = 0;
		for (uint64_t d = 1; d <= LAST; ++d) {
			int64_t i = _to_int64(LAST) + 1;
			int64_t j = 1;
			const integer td = m_Tn[d]*d;
			while (j <= _to_int64(LAST) and i > 0) {
				i -= _to_int64(d);
				if (i > 0) {
					s += m_Tn[_to_uint64(i)]*td;
				}
				j += 1;
			}
		}
		LAST += 1;
		m_Tn[LAST] = s/(LAST - 1);
	}
}

void rand_free_ulab_trees::compute_Amq_rec(uint64_t m, uint64_t q) {
	if (m == 0) {
		m_Amq[q][m] = 1;
		return;
	}
	if (m <= q) {
		// read the paper...
		m_Amq[q][m] = m_Tn[m + 1];
		return;
	}
	if (m_Amq[q][m] != 0) {
		// already computed
		return;
	}

	integer s(0);
	for (uint64_t j = 1; j <= m; ++j) {
		const uint64_t sup = std::min( m/j, q );
		for (uint64_t d = 1; d <= sup; ++d) {
			compute_Amq_rec(m - j*d, q);
			compute_Amq_rec(d - 1, q);
			const integer& A1 = m_Amq[q][m - j*d];
			const integer& A2 = m_Amq[q][d - 1];
			s += A1*A2*d;
		}
	}
	m_Amq[q][m] = s/m;
}

void rand_free_ulab_trees::compute_Amq() {
	const uint64_t m = m_n - 1;
	const uint64_t q = (m_n - 1)/2;

	// if Amq is already computed, do nothing
	if (m_Amq.find(q) != m_Amq.end()) {
		return;
	}

	// allocate one more integer than the value of 'm'
	// if this variabe is even, for future odd values
	// that are just one unit away from 'm'. (same 'q',
	// but different 'm', basically).
	const uint64_t mm = m + (m%2 == 0);
	/*
	if (m%2 == 0) {
		++mm;
	}*/

	m_Amq[q] = vector<integer>(mm + 1, 0);
	compute_Amq_rec(mm, q);
}

void rand_free_ulab_trees::choose_jd_from_T(uint64_t k, uint64_t& j, uint64_t& d) {
	// Weight of the pair to choose. It will be decreased at
	// every iteration and when it reaches a value below 0 we
	// will have found our pair

	const double r = m_unif(m_gen);
	double z = (m_Tn[k]*(k - 1)).to_double()*r;

	// Generate all possible pairs. For each pair calculate
	// the weight and substract it from z. As soon as 'z'
	// reaches 0 or less, we found a pair with its probability.
	j = 1;
	d = 1;
	while (z > 0) {

		if (k <= j*d) {
			// we need to "start a next pair"
			++d;
			j = 1;
		}
		else {
			// substract weight of current pair
			z -= m_Tn[k - j*d].to_double()*m_Tn[d].to_double()*_to_double(d);
			// if 'z' has not reached 0 then generate next pair
			if (z > 0) {
				++j;
			}
		}
	}
}

void rand_free_ulab_trees::choose_jd_from_Amq(uint64_t m, uint64_t q, uint64_t& j, uint64_t& d) {
	// Weight of the pair to choose. It will be decreased at
	// every iteration and when it reaches a value below 0 we
	// will have found our pair

	const double r = m_unif(m_gen);
	double z = m_Amq[q][m].to_double()*_to_double(m)*_to_double(r);

	// Generate all possible pairs. For each pair calculate
	// the weight and substract it from z. As soon as 'z'
	// reaches 0 or less, we found a pair with its probability.
	j = 1;
	d = 1;
	while (z > 0) {
		if (m < j*d) {
			// we need to "start a next pair"
			++d;
			if (d > q) {
				d = 0;
				++j;
			}
			else {
				j = 1;
			}
		}
		else {
			// substract weight of current pair.
			// NOTE: Amq[q] is supposed to exist
			assert(m_Amq.find(q) != m_Amq.end());

			z -= (m_Tn[d].to_double())*
				 (m_Amq[q][m - j*d].to_double())*
				 _to_double(d);
			// if 'z' has not reached 0 then generate next pair
			if (z > 0) {
				++j;
			}
		}
	}
}

pair<uint64_t,uint64_t> rand_free_ulab_trees::ranrut(uint64_t k, uint64_t lr, uint64_t nt) {
	if (k == 0) {
		// do nothing
		return make_pair(lr,nt);
	}
	if (k == 1) {
		m_TREE[nt] = lr;
		return make_pair(nt, nt + 1);
	}
	if (k == 2) {
		m_TREE[nt] = lr;
		m_TREE[nt + 1] = nt;
		return make_pair(nt, nt + 2);
	}

	uint64_t j, d;
	choose_jd_from_T(k, j,d);

	// both 'j' and 'd' must be strictly positive.
	// use two assertions to spot which fails more easily
	assert(j > 0);
	assert(d > 0);

	// as in the book, m = j*d.

	// generate T' (a random rooted tree of k - m vertices)
	const pair<uint64_t,uint64_t> lr1_nt = ranrut(k - j*d, lr,nt);
	const uint64_t lr1 = lr1_nt.first;	// lr1: index of the root of T'
	nt = lr1_nt.second;					// nt: where to store T''

	// generate T'' (a random rooted tree of d vertices)
	// NOTE!! recall that we have to make j copies of T''
	// and that now we are making one copy which is already
	// connected to the root of T'
	const pair<uint64_t,uint64_t> lr2_nt = ranrut(d, lr1,nt);
	const uint64_t lr2 = lr2_nt.first;
	nt = lr2_nt.second;

	// make j-1 copies of T'' and connect them to T'
	// T'' is contained in TREE[lr2:(lr2 + d)]
	for (uint64_t c = 1; c < j; ++c) {
		// each new copy's root is a child of the root of T'
		m_TREE[nt] = lr1;
		// make a copy of T''
		for (uint64_t v = nt + 1; v < nt + d; ++v) {
			// 'v - c*d' is the position of 'v' relative to
			// the root of the 'c'-th copy, but translated
			// to make it relative to the first copy's root.

			// 'TREE[v - c*d] - lr2' is the increment with
			// respect to the new root ('nt') so that the
			// vertex in 'v' connects with 'nt'.
			m_TREE[v] = nt + m_TREE[v - c*d] - lr2;
		}
		nt += d;
	}

	// the root of this tree generated is placed at the
	// position at which the root of T' was stored: lr1
	return make_pair(lr1, nt);
}

void rand_free_ulab_trees::bicenter(uint64_t k) {
	if (k == 0) {
		return;
	}

	const uint64_t h = k/2;
	const double prob = 1.0/(m_Tn[h].to_double() + 1.0);
	const double r = m_unif(m_gen);

	if (r <= prob) {
		// step B1: make one tree and make a SINGLE copy
		const pair<uint64_t,uint64_t> lr_nt = ranrut(h, 0, 1);
		const uint64_t lr = lr_nt.first;
		const uint64_t nt = lr_nt.second;
		m_TREE[nt] = lr;
		for (uint64_t v = nt + 1; v < nt + h; ++v) {
			m_TREE[v] = nt + m_TREE[v - h] - lr;
		}
	}
	else {
		// step B2: generate two trees
		const pair<uint64_t,uint64_t> lr_nt = ranrut(h, 0, 1);
		ranrut(h, lr_nt.first, lr_nt.second);
	}
}

pair<uint64_t,uint64_t> rand_free_ulab_trees::forest
(uint64_t m, uint64_t q, uint64_t r_idx, uint64_t nt, vector<uint64_t>& ROOTS)
{
	if (m == 0) {
		// forest of 0 vertices
		return make_pair(r_idx, nt);
	}
	if (m == 1) {
		// forest of 1 vertex
		assert(q >= 1);

		// next root points to the same place
		// where to store the next tree
		ROOTS[r_idx] = nt;

		// no need to modify TREE (as we are adding a root,
		// and the places corresponding to roots are modified
		// at the end of the procedure)
		return make_pair(r_idx + 1, nt + 1);
	}

	uint64_t j, d;
	choose_jd_from_Amq(m, q, j,d);

	// make a forest of trees of 'm - j*d' vertices in
	// total, so that each tree has at most 'q' vertices
	const pair<uint64_t,uint64_t> nr_nt = forest(m - j*d,q, r_idx,nt, ROOTS);
	r_idx = nr_nt.first;
	nt = nr_nt.second;

	// The forest is now in TREE, and the roots in ROOTS.
	// The last root was stored in ROOTS[nr - 1]
	// The next root index has to be stored at 'nr' in ROOTS
	// The next tree has to be stored at 'nt' in TREE

	// we have to give to the "ranrut" procedure the label
	// of the last root generated. This is in ROOTS[nr - 1],
	// or it is as long as nr > 0. Recall that 'nr' is stored
	// in 'r_idx'

	uint64_t lr = (r_idx > 0 ? ROOTS[r_idx - 1] : 0);
	/*uint64_t lr = 0;
	if (r_idx > 0) {
		lr = ROOTS[r_idx - 1];
	}*/

	// generate a random rooted tree T', stored in TREE,
	// starting at position 'nt'. Recall that the last
	// root generated is stored in variable 'lr'.
	// "The last root generated" actually is
	// "the index in TREE of the last root generated"
	const pair<uint64_t,uint64_t> lr_nt = ranrut(d, lr, nt);
	lr = lr_nt.first;
	nt = lr_nt.second;

	// update ROOTS
	ROOTS[r_idx] = lr;
	++r_idx;

	// make 'j - 1' copies of the tree generated in the
	// previous call and exit
	for (uint64_t k = 1; k < j; ++k) {
		// the copy has a root which must
		// be stored appropriately
		ROOTS[r_idx] = nt;
		++r_idx;

		// Copy tree structure. We do not need to
		// specify to what vertex this new copy's
		// root is pointing to because it is done
		// at the end of function make_rand_tree()
		for (uint64_t v = nt + 1; v < nt + d; ++v) {
			// for details on why this assignment
			// see end of method ranrut()
			m_TREE[v] = nt + m_TREE[v - k*d] - lr;
		}
		nt += d;
	}

	return make_pair(r_idx, nt);
}

} // -- namespace generate
} // -- namespace lal
