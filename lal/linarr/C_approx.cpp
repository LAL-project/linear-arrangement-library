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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
using namespace std;

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/internal/macros.hpp>

#define to_int32(x) static_cast<int32_t>(x)
#define to_uint32(x) static_cast<uint32_t>(x)

namespace lal {
using namespace graphs;
using namespace numeric;

namespace linarr {

inline constexpr
uint32_t alpha(const int32_t n, const int32_t d1, const int32_t d2) {
	int32_t f = 0;
	// positions s1 < s2
	if (1 <= n - (d1 + d2)) {
		// sum(d1 - 1, i, 1, n - d2 - d1)
		f += (d1 - 1)*(n - d2 - d1);
		// sum(n - d2 - i, i, n - (d1 + d2) + 1, n - d2 - 1)
		f += (d1*(d1 - 1))/2;
	}
	else {
		// sum(n - i - d2, i, 1, n - d2 - 1)
		f += ((d2 - n)*(d2 - n + 1))/2;
	}

	// positions s2 < s1
	if (d1 + d2 <= n) {
		f += (d1 - 1)*(n - d2 - d1);
	}
	if (1 + d2 - d1 >= 1) {
		if (1 + d2 <= n - d1) {
			f += (d1*(d1 - 1))/2;
		}
		else {
			f += ((n - d2)*(n - d2 - 1))/2;
		}
	}

#if defined DEBUG
	assert(f >= 0);
#endif
	return to_uint32(f);
}

inline constexpr
uint32_t beta(const int32_t n, const int32_t d1, const int32_t d2) {
	int32_t f = 0;

	// positions s1 < s2
	if (1 <= n - (d1 + d2)) {
		// sum(n - i - d2 - 1, i, 1, n - d1 - d2)
		f += (n - d2)*(n - d2) + 3*(d1 + d2 - n) - d1*d1;
		// sum(n - i - d2, i, n - (d1 + d2) + 1, n - d2 - 1)
		f += d1*(d1 - 1);
	}
	else {
		// sum(n - i - d2, i, 1, n - d2 - 1)
		f += (d2 - n)*(d2 - n + 1);
	}

	// positions s2 < s1
	if (d1 < d2) {
		if (1 + d2 <= n - d1) {
			// sum(i - 3, i, 1 + d2, n - d1)
			f += (n - d1)*(n - d1) - 5*(n - d1 - d2) - d2*d2;
		}

		if (d2 <= n - d1) {
			// sum(i - 2, i, 1 + d2 - d1, d2)
			f += d1*(2*d2 - d1 - 3);
		}
		else {
			// sum(i - 2, i, 1 + d2 - d1, n - d1)
			f += (d2 - n)*(2*d1 - d2 - n + 3);
		}
	}
	else {
		// these sums are the same as in the positive
		// case above, but simplified assuming d1 = d2.

		if (1 + 2*d1 <= n) {
			f += n*(n - 3) + d1*(6 - 2*n);
		}

		if (2*d1 <= n) {
			f += d1*(d1 - 1);
		}
		else {
			f += (d1 - n)*(d1 - n + 1);
		}
	}

#if defined DEBUG
	assert(f >= 0);
#endif
	return to_uint32(f/2);
}

template<typename GRAPH>
rational __get_approximate_C_2_rational(const GRAPH& g, const linear_arrangement& pi) {
	rational Ec2(0);
	const uint32_t n = g.n_nodes();

	iterators::Q_iterator<GRAPH> q(g);
	while (q.has_next()) {
		q.next();
		const auto [st, uv] = q.get_pair();
		const auto [s,t] = st;
		const auto [u,v] = uv;

		const uint32_t len_st = (pi[s] < pi[t] ? pi[t] - pi[s] : pi[s] - pi[t]);
		const uint32_t len_uv = (pi[u] < pi[v] ? pi[v] - pi[u] : pi[u] - pi[v]);

		const auto [al, be] =
		(len_st <= len_uv ?
			make_pair(
				alpha(to_int32(n), to_int32(len_st), to_int32(len_uv)),
				beta(to_int32(n), to_int32(len_st), to_int32(len_uv))
			) :
			make_pair(
				alpha(to_int32(n), to_int32(len_uv), to_int32(len_st)),
				beta(to_int32(n), to_int32(len_uv), to_int32(len_st))
			)
		);
		Ec2 += rational(to_int32(al), be);
	}

	return Ec2;
}

rational approximate_C_rational
(const undirected_graph& g, const linear_arrangement& pi)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<undirected_graph>, g, pi);
}

rational approximate_C_rational
(const directed_graph& g, const linear_arrangement& pi)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<directed_graph>, g, pi);
}

double approximate_C(const undirected_graph& g, const linear_arrangement& pi) {
	return approximate_C_rational(g, pi).to_double();
}
double approximate_C(const directed_graph& g, const linear_arrangement& pi) {
	return approximate_C_rational(g, pi).to_double();
}

} // -- namespace linarr
} // -- namespace lal
