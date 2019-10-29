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
 
#include <lal/linarr/C.hpp>

// C includes
#include <string.h>

// C++ includes
#include <numeric>
using namespace std;

namespace lal {
namespace linarr {

#define max2(a,b) std::max(a,b)
#define max4(a,b,c,d) max2(max2(a,b),max2(c,d))

#define sort2(a,b, sa,sb)			\
	(a < b) && ((sa=a) & (sb=b));	\
	(a > b) && ((sa=b) & (sb=a));

uint32_t __n_crossings_Q(const vector<edge_pair>& Q, const vector<node>& T) {
	const uint32_t n = static_cast<uint32_t>(T.size());
	if (n < 4) {
		return 0;
	}

	// actual linear arrangement (following notation used in the thesis):
	// pi[u] = p <-> node u is at position p
	uint32_t *pi = static_cast<uint32_t *>( malloc(n*sizeof(uint32_t)) );
	for (uint32_t i = 0; i < n; ++i) {
		pi[ T[i] ] = i;
	}

	// number of crossings
	uint32_t C = 0;

	uint32_t ps,pt,pu,pv;
	ps = pt = pu = pv = 0; // so compiler doesn't cry

	for (const edge_pair& ep : Q) {
		const node s = ep.first.first;
		const node t = ep.first.second;
		const node u = ep.second.first;
		const node v = ep.second.second;

		sort2(pi[s],pi[t], ps,pt)
		sort2(pi[u],pi[v], pu,pv)

		C += (ps < pu and pu < pt and pt < pv) or
			 (pu < ps and ps < pv and pv < pt);
	}

	free(pi);
	return C;
}

uint32_t n_crossings_Q(const vector<edge_pair>& Q, const vector<node>& arr) {
	if (arr.size() != 0) {
		return __n_crossings_Q(Q, arr);
	}
	// note: the number of vertices of
	// the graph is unknown... work it out!
	uint32_t n = 0;
	for (const edge_pair& p : Q) {
		n = max4(p.first.first, p.first.second, p.second.first, p.second.second);
	}
	vector<node> T(n);
	iota(T.begin(), T.end(), 0);
	return __n_crossings_Q(Q, T);
}

} // -- namespace linarr
} // -- namespace lal
