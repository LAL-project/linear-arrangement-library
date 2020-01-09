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

#include <lal/generation/rand_ulab_free_trees.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
#include <numeric>
using namespace std;

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
#include <lal/utils/conversions.hpp>

#define _to_int32(x) static_cast<int32_t>(x)
#define _to_uint32(x) static_cast<uint32_t>(x)
#define _to_double(x) static_cast<double>(x)

namespace std {
template<typename T>
ostream& operator<< (ostream& os, const vector<T>& v) {
	if (v.size() > 0) {
		os << v[0];
		for (size_t i = 1; i < v.size(); ++i) {
			os << ", " << v[i];
		}
	}
	return os;
}

template<typename T>
ostream& operator<< (ostream& os, const stack<T>& v) {
	stack<T> copy = v;
	if (not copy.empty()) {
		os << copy.top();
		copy.pop();
		while (not copy.empty()) {
			os << ", " << copy.top();
			copy.pop();
		}
	}
	return os;
}
}

#define get_alpha(m,q) (m_alpha[make_pair(m,q)])
#define alpha_exists(m,q) (m_alpha.find(make_pair(m,q)) != m_alpha.end())

namespace lal {
using namespace graphs;
using namespace numeric;

namespace generate {

static inline
utree make_tree(uint32_t m_n, const vector<uint32_t>& m_tree) {
	utree T(m_n);
	vector<edge> edges(m_n - 1);
	for (node u = 1; u < m_n; ++u) {
		edges[u - 1] = edge(u, m_tree[u]);
	}
	T.add_edges(edges);
	return T;
}

/* PUBLIC */

rand_ulab_free_trees::rand_ulab_free_trees() : rand_ulab_rooted_trees() {
}
rand_ulab_free_trees::rand_ulab_free_trees(uint32_t _n, uint32_t seed) {
	init(_n, seed);
}
rand_ulab_free_trees::~rand_ulab_free_trees() { }

void rand_ulab_free_trees::init(uint32_t _n, uint32_t seed) {
	rand_ulab_rooted_trees::init(_n, seed);
	init_fn();
}

utree rand_ulab_free_trees::make_rand_tree() {
	//__kout << "make_rand_tree::" << endl;

	if (m_n <= 1) { return utree(m_n); }
	if (m_n == 2) {
		utree t(2);
		t.add_edge(0,1);
		return t;
	}
	if (m_n == 3) {
		utree t(3);
		t.add_edges({edge(0,1),edge(1,2)});
		return t;
	}

	std::fill(m_tree.begin(), m_tree.end(), 0);

	//__kout << "compute probability to make a bicentroidal tree" << endl;

	// calculate the probability of generating a bicentroidal tree
	rational bicent_prob = 0;
	if (m_n%2 == 0) {
		// if n is even...
		const integer k = get_rn(m_n/2) + 1;
		//__kout << "m_n/2= " << m_n/2 << endl;
		//__kout << "m_rn[" << m_n/2 << "]+1= " << k << endl;
		//__kout << "m_rn[" << m_n << "]+1= " << m_rn[m_n] << endl;

		const integer k_choose_2 = (k*(k - 1))/2;
		bicent_prob = rational(k_choose_2, get_fn(m_n));
		//__kout << bicent_prob << endl;

		//bicent_prob = 0.5;
	}
	assert(bicent_prob.to_double() <= 1.0);

	//__kout << "probability: " << bicent_prob << " ~ " << bicent_prob.to_double() << endl;

	// with probability 'bicent_prob' the tree will be bicentroidal
	if (m_unif(m_gen) <= bicent_prob.to_double()) {
		//__kout << "    Make a bicentroidal tree!" << endl;
		bicenter(m_n);
		const utree T = make_tree(m_n, m_tree);
		assert(T.is_tree());
		return T;
	}

	//__kout << "Make a random forest and then join the trees into a single root" << endl;

	// -----------------------------------
	// make a forest on (n - 1) vertices
	const uint32_t m = m_n - 1;
	const uint32_t q = (m_n - 1)/2;

	// parameters:
	//     m: make a forest of m vertices
	//     q: needed to choose pairs (j,d)
	//     1: where to start storing vertices in m_tree
	forest(m,q, 1);
	// -----------------------------------

	const utree T = make_tree(m_n, m_tree);
	assert(T.is_tree());
	return T;
}

void rand_ulab_free_trees::clear() {
	rand_ulab_rooted_trees::clear();
	m_fn.clear();
	m_alpha.clear();
	init_fn();
}

/* PRIVATE */


/* PLEASE, NOTE!
 *
 *	-- T is the random free tree that this method's calle (make_rand_tree)
 *  is supposed to generate.
 *
 *	-- F' refers to a random forest generated within the method.
 *  -- T' refers to a random rooted tree generated within the method.
 *	This can be easiliy identified because this has to be copied a certain
 *	number of times.
 *
 */
uint32_t rand_ulab_free_trees::forest
(uint32_t m, uint32_t q, uint32_t nt, const string& tab)
{
	//__kout << tab << "Make a forest F of " << m << " vertices..." << endl;

	if (m == 0) {
		// Forest of 0 vertices
		return nt;
	}
	if (m == 1) {
		// forest of 1 vertex, i.e., a single vertex
		assert(q >= 1);

		// this vertex should be connected to the root of T
		m_tree[nt] = 0;

		// No need to modify m_tree since we are adding a root, and the
		// positions corresponding to roots in m_tree are modified at the
		// end of the procedure
		return nt + 1;
	}

	auto [j, d] = choose_jd_from_alpha(m, q);

	//__kout << tab << "Choose forest F' of " << m-j*d << " vertices..." << endl;
	//__kout << tab << "    m= " << m << endl;
	//__kout << tab << "    j= " << j << endl;
	//__kout << tab << "    d= " << d << endl;
	//__kout << tab << "Contents before: " << m_tree << endl;

	// Make a forest F' of trees of m - j*d vertices in
	// total, so that each tree has at most q vertices
	nt = forest(m - j*d,q, nt, tab + "    ");

	//__kout << tab << "Contents after:  " << m_tree << endl;

	// The forest is now in m_tree, and the roots in m_roots.
	// The root of the last tree generated is stored in nr in m_roots.
	// The root of the next tree generated has to be stored in nr in m_roots
	// The next tree has to be stored at nt in m_tree

	//__kout << tab << "Making rooted T' of " << d << " vertices..." << endl;
	//__kout << tab << "Contents before: " << m_tree << endl;

	// Generate a random rooted tree T' in m_tree starting at position nt.
	// Join this tree to T's root (vertex 0)
	uint32_t root_Tp;
	std::tie(root_Tp, nt) = ranrut(d, 0, nt, tab);

	//__kout << tab << "Contents after:  " << m_tree << endl;
	//__kout << tab << "Making " << j-1 << " copies..." << endl;

	for (uint32_t c = 1; c < j; ++c) {

		//__kout << tab << "    Contents before " << c << "-th copy: " <<  m_tree << endl;

		// Each of the copies of T' has to be adjoined to F', i.e.,
		// do not connect them to the forest's root. Instead,
		// leave them orphan until the end of the procedure connects
		// them to the parent vertex.
		m_tree[nt] = 0;

		// Copy the tree structure.
		for (uint32_t v = nt + 1; v < nt + d; ++v) {
			// for details on why this assignment
			// see end of method ranrut()
			m_tree[v] = nt + m_tree[v - c*d] - root_Tp;
		}
		nt += d;

		//__kout << tab << "    Contents after " << c << "-th copy:  " << m_tree << endl;
	}

	return nt;
}

void rand_ulab_free_trees::bicenter(uint32_t n) {
	// make sure that the number of vertices is even
	assert(n%2 == 0);
	if (n == 0) {
		return;
	}
	const uint32_t h = n/2;

	//__kout << "Make first tree..." << endl;

	// for both steps, make one tree ...
	auto [lr, nt] = ranrut(h, 0, 0);

	//__kout << "Contents: " << m_tree << endl;

	const rational prob(1, get_rn(h) + 1);
	//__kout << "probability of making a copy= " << prob << " ~ "
	//	 << prob.to_double() << endl;
	//__kout << "    h= " << h << endl;
	//__kout << "    m_rn[" << h << "]= " << m_rn[h] << endl;

	if (m_unif(m_gen) <= prob.to_double()) {
		// step B1: ... and make a SINGLE copy

		//__kout << "Make a copy..." << endl;

		m_tree[nt] = lr;
		for (uint32_t v = nt + 1; v < nt + h; ++v) {
			m_tree[v] = nt + m_tree[v - h] - lr;
		}
		lr = nt;
		nt += h;

		//__kout << "Contents: " << m_tree << endl;
	}
	else {

		//__kout << "Make another tree..." << endl;
		// step B2: generate another tree
		tie(lr,nt) = ranrut(h, lr, nt);

		//__kout << "Contents: " << m_tree << endl;
	}

	// for the sake of debugging
	assert(nt == m_n);
}

const integer& rand_ulab_free_trees::get_alpha_mq(const uint32_t m, const uint32_t q) {

	if (alpha_exists(m,q)) {
		// already computed
		return get_alpha(m,q);
	}

	// base cases, read the paper
	if (m == 0) {
		m_alpha[make_pair(m,q)] = 1;
		return get_alpha(m,q);
	}
	if (m <= q) {
		m_alpha[make_pair(m,q)] = get_rn(m + 1);
		return get_alpha(m,q);
	}

	integer alpha_mq(0);
	for (uint32_t j = 1; j <= m; ++j) {
		// The variable 'sup' is used to avoid obtaining
		// negative values in the operation 'm - j*d'.
		const uint32_t sup = std::min( m/j, q );

		for (uint32_t d = 1; d <= sup; ++d) {
			const integer& A1 = get_alpha_mq(m - j*d, q);
			const integer& A2 = get_alpha_mq(d - 1, q);
			alpha_mq += A1*A2*d;
		}
	}
	m_alpha[make_pair(m,q)] = alpha_mq/m;
	return get_alpha(m,q);
}

void rand_ulab_free_trees::init_fn() {
	// from the OEIS: https://oeis.org/A000055

	m_fn = vector<integer>(31);
	m_fn[0] = 1;
	m_fn[1] = 1;
	m_fn[2] = 1;
	m_fn[3] = 1;
	m_fn[4] = 2;
	m_fn[5] = 3;
	m_fn[6] = 6;
	m_fn[7] = 11;
	m_fn[8] = 23;
	m_fn[9] = 47;
	m_fn[10] = 106;
	m_fn[11] = 235;
	m_fn[12] = 551;
	m_fn[13] = 1301;
	m_fn[14] = 3159;
	m_fn[15] = 7741;
	m_fn[16] = 19320;
	m_fn[17] = 48629;
	m_fn[18] = 123867;
	m_fn[19] = 317955;
	m_fn[20] = 823065;
	m_fn[21] = 2144505;
	m_fn[22] = 5623756;
	m_fn[23] = 14828074;
	m_fn[24] = 39299897;
	m_fn[25] = integer("104636890");
	m_fn[26] = integer("279793450");
	m_fn[27] = integer("751065460");
	m_fn[28] = integer("2023443032");
	m_fn[29] = integer("5469566585");
	m_fn[30] = integer("14830871802");
}

const integer& rand_ulab_free_trees::get_fn(const uint32_t n) {
	if (m_fn.size() >= n + 1) {
		// value already computed
		return m_fn[n];
	}

	//__kout << "* tn for n= " << n << endl;

	integer tk(0);
	uint32_t k = static_cast<uint32_t>(m_fn.size());
	while (k <= n) {

		//__kout << "*     computing for k= " << k << endl;

		// if k=0 then tk=1
		// else tk=0
		tk = (k == 0);

		if (k%2 != 0) {
			tk += get_rn(k);
		}
		else {
			tk += get_rn(k) + get_rn(k/2)/2;
		}

		integer s(0);
		for (uint32_t j = 0; j <= k; ++j) {
			s += get_rn(j)*get_rn(k - j);
		}

		tk -= (s/2);
		m_fn.push_back(integer());
		m_fn[k] = tk;

		++k;
	}

	return m_fn[n];
}

pair<uint32_t, uint32_t>
rand_ulab_free_trees::choose_jd_from_alpha(const uint32_t m, const uint32_t q)
{
	// Weight of the pair to choose. It will be decreased at
	// every iteration and when it reaches a value below 0 we
	// will have found our pair

	const double r = m_unif(m_gen);
	double z = (get_alpha_mq(m,q)*m).to_double()*r;

	// Generate all possible pairs. For each pair calculate
	// the weight and substract it from z. As soon as 'z'
	// reaches 0 or less, we found a pair with its probability.
	uint32_t j = 1;
	uint32_t d = 1;
	while (z > 0) {
		if (m < j*d) {
			// we need to "start a next pair"
			++d;
			if (d > q) {
				d = 1;
				++j;
			}
			else {
				j = 1;
			}
		}
		else {
			z -= (get_rn(d)*get_alpha_mq(m-j*d, q)*d).to_double();

			// if 'z' has not reached 0 then generate next pair
			if (z > 0) {
				++j;
			}
		}
	}

	return make_pair(j, d);
}

} // -- namespace generate
} // -- namespace lal
