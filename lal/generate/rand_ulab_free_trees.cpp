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

#include <lal/generate/rand_ulab_free_trees.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/detail/graphs/conversions.hpp>

#define get_alpha(m,q) (m_alpha.find({m,q})->second)
#define alpha_exists(m,q) (m_alpha.find({m,q}) != m_alpha.end())

namespace lal {
namespace generate {

#define make_tree(T, N, TREE)			\
	graphs::free_tree T(N);				\
	for (node u = 1; u < N; ++u) {		\
		T.add_edge_bulk(u, TREE[u]);	\
	}									\
	T.finish_bulk_add(false, false);

// -----------------------------------------------------------------------------
// ACTUAL GENERATOR

/* PUBLIC */

graphs::free_tree _rand_ulab_free_trees::get_tree() noexcept {
	if (m_n <= 1) { return graphs::free_tree(m_n); }
	if (m_n == 2) {
		graphs::free_tree t(2);
		t.set_edges({edge(0,1)});
		return t;
	}
	if (m_n == 3) {
		graphs::free_tree t(3);
		t.set_edges({edge(0,1),edge(1,2)});
		return t;
	}

	m_head_vector.fill(0);

	// calculate the probability of generating a bicentroidal tree
	numeric::rational bicent_prob = 0;
	if (m_n%2 == 0) {
		/* The following is a correction of Wilf's algorithm. Instead of
		 * calculating
		 *			bicent_prob = {1 + r_(n/2)}{2}/r_n
		 * we calculate
		 *			bicent_prob = {1 + r_(n/2)}{2}/f_n
		 *
		 * where
		 *		r_n: the number of unlabelled rooted trees of n nodes
		 *		f_n: the number of unlabelled free trees of n nodes
		 *		{n}{k}: is read as "n choose k"
		 *
		 * We followed the correction pointed out in the reference to
		 * Giac/Xcas's manual (read the documentation of this class
		 * for a reference).
		 */
		const numeric::integer k = get_rn(m_n/2) + 1;
		const numeric::integer k_choose_2 = k*(k - 1);
		bicent_prob = numeric::rational(k_choose_2, get_fn(m_n)*2);
	}
#if defined DEBUG
	assert(bicent_prob.to_double() <= 1.0);
#endif

	// -----------------------------------
	// with probability 'bicent_prob' the tree has two centroids
	if (m_unif(m_gen) <= bicent_prob.to_double()) {
		bicenter(m_n);
		make_tree(T, m_n, m_head_vector);
#if defined DEBUG
		assert(T.is_tree());
#endif
		return T;
	}

	// -----------------------------------
	// the tree has one centroid

	// -----------------------------------
	// make a forest on (n - 1) nodes
	const uint64_t m = m_n - 1;
	const uint64_t q = (m_n - 1)/2;

	// parameters:
	//     m: make a forest of m nodes
	//     q: needed to choose pairs (j,d)
	//     1: where to start storing nodes in m_tree
	forest(m,q, 1);
	// -----------------------------------

	make_tree(T, m_n, m_head_vector);
#if defined DEBUG
	assert(T.is_tree());
#endif
	return T;
}

/* PRIVATE */

/* PLEASE, NOTE!
 *
 *	-- T is the random free tree that this method's callee (make_rand_tree)
 *  is supposed to generate.
 *
 *	-- F' refers to a random forest generated within the method.
 *  -- T' refers to a random rooted tree generated within the method.
 *	This can be easiliy identified because this has to be copied a certain
 *	number of times.
 *
 */
uint64_t _rand_ulab_free_trees::forest(uint64_t m, uint64_t q, uint64_t nt) noexcept {
	if (m == 0) {
		// Forest of 0 nodes
		return nt;
	}
	if (m == 1) {
		// forest of 1 node, i.e., a single node
#if defined DEBUG
		assert(q >= 1);
#endif

		// this node should be connected to the root of T
		m_head_vector[nt] = 0;

		// No need to modify m_tree since we are adding a root, and the
		// positions corresponding to roots in m_tree are modified at the
		// end of the procedure
		return nt + 1;
	}

	auto [j, d] = choose_jd_from_alpha(m, q);

	// Make a forest F' of trees of m - j*d nodes in
	// total, so that each tree has at most q nodes
	nt = forest(m - j*d,q, nt);

	// The forest is now in m_tree, and the roots in m_roots.
	// The root of the last tree generated is stored in nr in m_roots.
	// The root of the next tree generated has to be stored in nr in m_roots
	// The next tree has to be stored at nt in m_tree

	// Generate a random rooted tree T' in m_tree starting at position nt.
	// Join this tree to T's root (node 0)
	uint64_t root_Tp;
	std::tie(root_Tp, nt) = ranrut(d, 0, nt);

	for (uint64_t c = 1; c < j; ++c) {

		// Each of the copies of T' has to be adjoined to F', i.e.,
		// do not connect them to the forest's root. Instead,
		// leave them orphan until the end of the procedure connects
		// them to the parent node.
		m_head_vector[nt] = 0;

		// Copy the tree structure.
		for (uint64_t v = nt + 1; v < nt + d; ++v) {
			// for details on why this assignment
			// see end of method ranrut()
			m_head_vector[v] = nt + m_head_vector[v - c*d] - root_Tp;
		}
		nt += d;

	}

	return nt;
}

void _rand_ulab_free_trees::bicenter(uint64_t n) noexcept {
	// make sure that the number of nodes is even
#if defined DEBUG
	assert(n%2 == 0);
#endif

	if (n == 0) { return; }
	const uint64_t h = n/2;

	// for both steps, make one tree ...
	auto [lr, nt] = ranrut(h, 0, 0);

	const numeric::rational prob(1, get_rn(h) + 1);
	if (m_unif(m_gen) <= prob.to_double()) {
		// step B1: ... and make a SINGLE copy

		m_head_vector[nt] = lr;
		for (uint64_t v = nt + 1; v < nt + h; ++v) {
			m_head_vector[v] = nt + m_head_vector[v - h] - lr;
		}
		lr = nt;
		nt += h;
	}
	else {
		// step B2: generate another tree
		std::tie(lr,nt) = ranrut(h, lr, nt);
	}

	// for the sake of debugging
#if defined DEBUG
	assert(nt == m_n);
#endif
}

const numeric::integer& _rand_ulab_free_trees::get_alpha_mq
(const uint64_t m, const uint64_t q)
noexcept
{

	/* This algorithm can be compared to the algorithm in
	 *		https://github.com/marohnicluka/giac/blob/master/graphe.cc#L7149
	 * (implementation of several Giac's functions)
	 *
	 * The output of this algorithm and the one following the link
	 * coincide up to n=400.
	 */

	if (alpha_exists(m,q)) {
		// already computed
		return get_alpha(m,q);
	}

	// base cases, read the paper
	if (m == 0) {
		m_alpha.insert({{m,q}, 1});
		return get_alpha(m,q);
	}
	if (m <= q) {
		m_alpha.insert({{m,q}, get_rn(m + 1)});
		return get_alpha(m,q);
	}

	numeric::integer alpha_mq(0);
	for (uint64_t j = 1; j <= m; ++j) {
		// The variable 'sup' is used to avoid obtaining
		// negative values in the operation 'm - j*d'.
		const uint64_t sup = std::min( m/j, q );

		for (uint64_t d = 1; d <= sup; ++d) {
			const numeric::integer& A1 = get_alpha_mq(m - j*d, q);
			const numeric::integer& A2 = get_alpha_mq(d - 1, q);
			alpha_mq += A1*A2*d;
		}
	}
	alpha_mq /= m;
	m_alpha.insert({{m,q}, std::move(alpha_mq)});
	return get_alpha(m,q);
}

const numeric::integer& _rand_ulab_free_trees::get_fn(const uint64_t n) noexcept {
	if (m_fn.size() >= n + 1) {
		// value already computed
		return m_fn[n];
	}

	// Compute f_k using Otter's formula (see reference in documentation)
	numeric::rational f_k(0);
	numeric::integer s(0);
	uint64_t k = m_fn.size();
	while (k <= n) {

		// for k=0, f_k=1.
		f_k = (k == 0);
		f_k += get_rn(k);
		f_k += (k%2 == 0 ? numeric::rational(get_rn(k/2), 2) : 0);

		s = 0;
		for (uint64_t j = 0; j <= k; ++j) {
			s += get_rn(j)*get_rn(k - j);
		}
		f_k -= numeric::rational(s,2);

		m_fn.emplace_back(f_k.to_integer());
		//m_fn[k] = std::move(f_i__int);

		++k;
	}

	return m_fn[n];
}

std::pair<uint64_t, uint64_t>
_rand_ulab_free_trees::choose_jd_from_alpha(const uint64_t m, const uint64_t q)
noexcept
{
	// Weight of the pair to choose. It will be decreased at
	// every iteration and when it reaches a value below 0 we
	// will have found our pair

	const double r = m_unif(m_gen);
	double z = (get_alpha_mq(m,q)*m).to_double()*r;

	// Generate all possible pairs. For each pair calculate
	// the weight and substract it from z. As soon as 'z'
	// reaches 0 or less, we found a pair with its probability.
	uint64_t j = 1;
	uint64_t d = 1;
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
			//if (z > 0) { ++j; }
			j += (z > 0 ? 1 : 0);
		}
	}

	return {j, d};
}

} // -- namespace generate
} // -- namespace lal
