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
 
#include <lal/linarr/C.hpp>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>
#include <lal/iterators/Q_iterator.hpp>

#define to_int64(x) static_cast<int64_t>(x)

namespace lal {
using namespace numeric;

namespace linarr {

inline constexpr int64_t alpha(int64_t n, int64_t d1, int64_t d2) {
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
		// sum(d1 - 1, i, 1 + d2, n - d1)
		f += (d1 - 1)*(n - d2 - d1);
	}
	if (d1 <= d2) {
		if (1 + d2 <= n - d1) {
			// sum(i + d1 - d2 - 1, i, 1 + d2 - d1, d2)
			f += (d1*(d1 - 1))/2;
		}
		else {
			// sum(i + d1 - d2 - 1, i, 1 + d2 - d1, n - d1)
			f += ((d2 - n)*(d2 - n + 1))/2;
		}
	}
	else {
		if (1 + d2 <= n - d1) {
			// sum(i + d1 - d2 - 1, i, 1, d2)
			f += (d2*(2*d1 - d2 - 1))/2;
		}
		else {
			// sum(i + d1 - d2 - 1, i, 1, n - d1)
			f += ((d1 - n)*(2*d2 - d1 - n + 1))/2;
		}
	}
	return f;
}

inline constexpr int64_t beta(int64_t n, int64_t d1, int64_t d2) {
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

	return f/2;
}

rational __get_approximate_C_2_rational(const ugraph& g, const vector<node>& pi) {
	rational Ec2(0);
	const uint32_t n = g.n_nodes();

	iterators::Q_iterator q(g);
	while (q.has_next()) {
		q.next();
		const edge_pair st_uv = q.get_pair();
		const edge st = st_uv.first;
		const edge uv = st_uv.second;
		const node s = st.first;
		const node t = st.second;
		const node u = uv.first;
		const node v = uv.second;

		int64_t al;
		uint64_t be;

		int64_t len_st = std::abs(to_int64(pi[s]) - to_int64(pi[t]));
		int64_t len_uv = std::abs(to_int64(pi[u]) - to_int64(pi[v]));
		if (len_st <= len_uv) {
			al = alpha(n, len_st, len_uv);
			be = static_cast<uint64_t>(beta(n, len_st, len_uv));
		}
		else {
			al = alpha(n, len_uv, len_st);
			be = static_cast<uint64_t>(beta(n, len_uv, len_st));
		}

		Ec2 += rational(al, be);
	}

	return Ec2;
}

rational approximate_C_2_rational(const ugraph& g, const vector<node>& pi) {
	return utils::call_with_empty_arrangement(__get_approximate_C_2_rational, g, pi);
}

double approximate_C_2(const ugraph& g, const vector<node>& pi) {
	return approximate_C_2_rational(g, pi).to_double();
}

} // -- namespace linarr
} // -- namespace lal
