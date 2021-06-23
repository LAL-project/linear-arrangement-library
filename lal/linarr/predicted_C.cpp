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

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/internal/macros.hpp>

#define to_int64(x) static_cast<int64_t>(x)
#define to_uint64(x) static_cast<uint64_t>(x)
#define to_double(x) static_cast<double>(x)

namespace lal {
using namespace graphs;
using namespace numeric;

namespace linarr {

inline constexpr
uint64_t alpha(const int64_t n, const int64_t d1, const int64_t d2) noexcept {
	int64_t f = 0;
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
	return to_uint64(f);
}

inline constexpr
uint64_t beta(const int64_t n, const int64_t d1, const int64_t d2) noexcept {
	int64_t f = 0;

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
	assert(f%2 == 0);
#endif
	return to_uint64(f/2);
}

template<class G, typename result>
result __get_approximate_C_2_rational
(const G& g, const linear_arrangement& pi) noexcept
{
	result Ec2(0);
	const uint64_t n = g.get_num_nodes();

	for (iterators::Q_iterator<G> q(g); not q.end(); q.next()) {
		const auto [st, uv] = q.get_edge_pair();
		const auto [s,t] = st;
		const auto [u,v] = uv;

		const uint64_t len_st = (pi[s] < pi[t] ? pi[t] - pi[s] : pi[s] - pi[t]);
		const uint64_t len_uv = (pi[u] < pi[v] ? pi[v] - pi[u] : pi[u] - pi[v]);

		const auto [al, be] =
		(len_st <= len_uv ?
			std::make_pair(
				alpha(to_int64(n), to_int64(len_st), to_int64(len_uv)),
				beta(to_int64(n), to_int64(len_st), to_int64(len_uv))
			) :
			std::make_pair(
				alpha(to_int64(n), to_int64(len_uv), to_int64(len_st)),
				beta(to_int64(n), to_int64(len_uv), to_int64(len_st))
			)
		);

		if constexpr (std::is_same_v<result, rational>) {
			Ec2 += rational(to_int64(al), be);
		}
		else {
			Ec2 += to_double(al)/to_double(be);
		}
	}

	return Ec2;
}

rational predicted_num_crossings_rational
(const undirected_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<undirected_graph, rational>, g, pi);
}

rational predicted_num_crossings_rational
(const directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<directed_graph, rational>, g, pi);
}

double predicted_num_crossings
(const undirected_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<undirected_graph, double>, g, pi);
}
double predicted_num_crossings
(const directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__get_approximate_C_2_rational<directed_graph, double>, g, pi);
}

} // -- namespace linarr
} // -- namespace lal
