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
#include <lal/properties/Q.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/detail/pairs_utils.hpp>

namespace lal {
namespace properties {

/* ---------------------------- */
/*  EXPECTATION OF D: E_rla[D]  */
/* (unconstrained arrangements) */

namespace E_pr_D {

template <bool size_subtrees_valid>
inline
numeric::rational exp_sum_edge_lengths(const graphs::rooted_tree& t)
noexcept
{
	detail::array<uint64_t> size_subtrees;
	if constexpr (not size_subtrees_valid) {
		size_subtrees.resize(t.get_num_nodes());
		detail::get_size_subtrees(t, t.get_root(), size_subtrees.begin());
	}

	numeric::rational E_pr_D = 0;

	for (node u = 0; u < t.get_num_nodes(); ++u) {
		const uint64_t nu = (
			size_subtrees_valid ?
				t.get_num_nodes_subtree(u) :
				size_subtrees[u]
		);

		const uint64_t du = t.get_out_degree(u);
		E_pr_D += nu*(2*du + 1);
	}
	E_pr_D -= 1;
	E_pr_D /= 6;

	return E_pr_D;
}

} // -- namespace E_pr_D

numeric::rational exp_sum_edge_lengths_projective_rational
(const graphs::rooted_tree& t)
noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	if (t.are_size_subtrees_valid()) {
		return E_pr_D::exp_sum_edge_lengths<true>(t);
	}
	return E_pr_D::exp_sum_edge_lengths<false>(t);
}

/* ------------------------- */
/* EXPECTATION OF D: E_pl[D] */
/*   (planar arrangements)   */

numeric::rational exp_sum_edge_lengths_planar_rational(const graphs::free_tree& T)
noexcept
{
	const uint64_t n = T.get_num_nodes();

	std::vector<detail::edge_size> edge_size(2*(n - 1));
	detail::calculate_bidirectional_sizes(T, n, 0, edge_size.begin());

	uint64_t V = 0;

	{
	detail::array<uint64_t> L(n, 0);
	for (const auto& p : edge_size) {
		const edge& e = p.e;
		const uint64_t s = p.size;
		L[e.first] += s*s;
	}
	for (node v = 0; v < n; ++v) {
		V += L[v]*(2*T.get_degree(v) - 1);
	}
	}

	return numeric::rational((n - 1)*(3*n*n + 2*n - 2) - V, 6*n);
}

/* ---------------------------- */
/*    VARIANCE OF D: V_rla[D]   */
/* (unconstrained arrangements) */

numeric::rational var_sum_edge_lengths_rational(const graphs::undirected_graph& g)
noexcept
{
	// E_rla[D]
	const numeric::rational Ed = exp_sum_edge_lengths_rational(g);
	// E_rla[D^2]
	const numeric::integer n = g.get_num_nodes();
	const numeric::integer m = g.get_num_edges();

	// calculate expectations of types
	const numeric::rational E0((n + 1)*(n*5 + 4), 45);
	const numeric::rational E1((n + 1)*(n*7 + 4), 60);
	const numeric::rational E2(n*(n + 1), 6);

	// calculate frequencies
	const numeric::integer f0 = num_pairs_independent_edges_integer(g)*2;
	const numeric::integer f2(m);
	const numeric::integer f1(m*(m - 1) - f0);

	// calculate second moment
	numeric::rational Ed2 = 0;
	Ed2 += E0*f0;
	Ed2 += E1*f1;
	Ed2 += E2*f2;

	// return variance
	return Ed2 - Ed*Ed;
}

} // -- namespace properties
} // -- namespace lal
