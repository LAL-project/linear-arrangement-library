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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#include <lal/properties/C_rla.hpp>

// C includes
#include <assert.h>
#include <omp.h>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

typedef uint64_t bigint;

// lal includes
#include <lal/properties/Q.hpp>

enum class frequency_type : int8_t {
	invalid,
	f00, f01, f021, f022,
	f03, f04, f12, f13, f24
};

namespace lal {
using namespace numeric;

/* UTILITIES */

inline
int tau(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	return
		static_cast<int>(st == wx or st == yz) +
		static_cast<int>(uv == wx or uv == yz);
}

inline
int share(const edge& e1, const edge& e2) {
	return
		static_cast<int>(e1.first == e2.first or e1.first == e2.second) +
		static_cast<int>(e1.second == e2.first or e1.second == e2.second);
}

inline
int phi(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	return share(st, wx) + share(st, yz) + share(uv, wx) + share(uv, yz);
}

inline
int subtype(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	int e1e3 = share(st, wx);
	int e1e4 = share(st, yz);
	int e2e3 = share(uv, wx);
	int e2e4 = share(uv, yz);

	if (e1e3 == 1 and e1e4 == 0 and e2e3 == 0 and e2e4 == 1) {
		return 2;
	}
	if (e1e3 == 0 and e1e4 == 1 and e2e3 == 1 and e2e4 == 0) {
		return 2;
	}
	return 1;
}

inline
frequency_type edge_pair_type(const edge_pair& ep1, const edge_pair& ep2)
{
	const edge& st = ep1.first;
	const edge& uv = ep1.second;
	const edge& wx = ep2.first;
	const edge& yz = ep2.second;

	int t = tau(st, uv, wx, yz);
	int p = phi(st, uv, wx, yz);

	if (t == 2) {
		// the only frequency type with tau=2
		return frequency_type::f24;
	}

	if (t == 1) {
		if (p == 2) {
			return frequency_type::f12;
		}
		if (p == 3) {
			return frequency_type::f13;
		}
		return frequency_type::invalid;
	}

	// tau=0
	if (p == 4) {
		return frequency_type::f04;
	}
	if (p == 3) {
		return frequency_type::f03;
	}
	if (p == 1) {
		return frequency_type::f01;
	}
	if (p == 0) {
		return frequency_type::f00;
	}

	// tau=0, phi=2
	int s = subtype(st, uv, wx, yz);
	if (s == 1) {
		return frequency_type::f021;
	}
	if (s == 2) {
		// the remaining true frequency
		return frequency_type::f022;
	}

	return frequency_type::invalid;
}

#define classify(ft, F021, F022, F03, F04, F12, F13, F24)	\
	switch (ft) {											\
	case frequency_type::f021: F021 += 1; break;			\
	case frequency_type::f022: F022 += 1; break;			\
	case frequency_type::f03: F03 += 1; break;				\
	case frequency_type::f04: F04 += 1; break;				\
	case frequency_type::f12: F12 += 1; break;				\
	case frequency_type::f13: F13 += 1; break;				\
	case frequency_type::f24: F24 += 1; break;				\
	case frequency_type::invalid:							\
		/* this should never happen */						\
		assert(false);										\
		/* 'break' is added so the compiler doesn't cry */	\
		break;												\
	default:												\
		/* do nothing */									\
		;													\
	}

namespace properties {

// ---------------------
// Number of crossings C

rational variance_C_freqs_rational(const graph& g, uint32_t nthreads) {
	// compute set Q(g)
	vector<edge_pair> Q;
	enumerate_Q(g, Q);
	return variance_C_freqs_Q_rational(Q, nthreads);
}

double variance_C_freqs(const graph& g, uint32_t nthreads) {
	// compute set Q(g)
	vector<edge_pair> Q;
	enumerate_Q(g, Q);
	return variance_C_freqs_Q(Q, nthreads);
}

rational variance_C_freqs_Q_rational(const vector<edge_pair>& Q, uint32_t nthreads) {
	assert(nthreads > 0);
	// frequencies f00 and f01 are not measured
	// because they have expectation 0

	static const rational exps[9] =
	{
		rational(0),		// 0, f00:   0
		rational(2, 9),		// 1, f24:   2/9
		rational(1, 18),	// 2, f13:   1/18
		rational(1, 45),	// 3, f12:   1/45
		rational(-1, 9),	// 4, f04:  -1/9
		rational(-1, 36),	// 5, f03:  -1/36
		rational(-1, 90),	// 6, f021: -1/90
		rational(1, 180),	// 7, f022:  1/180
		rational(0)			// 8, f01:   0
	};

	// value of V_rla[C]
	rational V(0);

	// values of the frequencies
	bigint f021(0), f022(0),	f03(0);
	bigint f04(0),	f12(0),		f13(0);
	bigint f24(0);

	const size_t frac = Q.size()/nthreads;

	if (Q.size() <= 128*nthreads) {
		// for a small enough set Q
		for (const edge_pair& q1 : Q) {
			for (const edge_pair& q2 : Q) {
				frequency_type ft = edge_pair_type(q1, q2);
				classify(ft, f021, f022, f03, f04, f12, f13, f24)
			}
		}
	}
	else {
		// values of each frequency per thread
		vector<bigint> _f021(nthreads, 0), _f022(nthreads, 0), _f03(nthreads, 0);
		vector<bigint> _f04(nthreads, 0), _f12(nthreads, 0), _f13(nthreads, 0);
		vector<bigint> _f24(nthreads, 0);

		#pragma omp parallel num_threads(nthreads)
		{
		uint32_t tid = static_cast<uint32_t>(omp_get_thread_num());

		size_t begin = tid*frac;
		size_t end = (tid + 1)*frac;
		if (tid == nthreads - 1) {
			end = Q.size();
		}

		for (size_t p_q1 = begin; p_q1 < end; ++p_q1) {
			for (size_t p_q2 = 0; p_q2 < Q.size(); ++p_q2) {
				const edge_pair& q1 = Q[p_q1];
				const edge_pair& q2 = Q[p_q2];

				frequency_type ft = edge_pair_type(q1, q2);
				classify(ft,
					_f021[tid], _f022[tid], _f03[tid],
					_f04[tid], _f12[tid], _f13[tid],
					_f24[tid]
				)
			}
		}
		}

		// reduce
		for (uint32_t i = 0; i < nthreads; ++i) {
			f021 += _f021[i];	f022 += _f022[i];	f03 += _f03[i];
			f04 += _f04[i];		f12 += _f12[i];		f13 += _f13[i];
			f24 += _f24[i];
		}
	}

	integer J(0);

	//J.init_ui(f00);  V += exps[0]*f00;
	J.init_ui(f24);  V += exps[1]*J;
	J.init_ui(f13);  V += exps[2]*J;
	J.init_ui(f12);  V += exps[3]*J;
	J.init_ui(f04);  V += exps[4]*J;
	J.init_ui(f03);  V += exps[5]*J;
	J.init_ui(f021); V += exps[6]*J;
	J.init_ui(f022); V += exps[7]*J;
	//J.init_ui(f01);  V += exps[8]*f01;
	return V;
}

double variance_C_freqs_Q(const vector<edge_pair>& Q, uint32_t nthreads) {
	assert(nthreads > 0);
	rational V = variance_C_freqs_Q_rational(Q, nthreads);
	return V.to_double();
}

} // -- namespace properties
} // -- namespace lal
