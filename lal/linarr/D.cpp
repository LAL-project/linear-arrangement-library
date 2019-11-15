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
 
#include <lal/linarr/D.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>
#include <lal/iterators/edge_iterator.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace linarr {

/* D */

uint64_t __sum_length_edges(const ugraph& g, const LINARR& pi) {
	// sum of lengths
	uint64_t l = 0;

	edge_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		const node s = e.first;
		const node t = e.second;

		// accumulate length of edge
		l += (pi[s] >= pi[t] ? pi[s] - pi[t] : pi[t] - pi[s]);
	}
	return l;
}

uint64_t sum_length_edges(const ugraph& g, const LINARR& pi) {
	return utils::call_with_empty_arrangement(__sum_length_edges, g, pi);
}

/* MDD */

rational __MDD_rational(const ugraph& g, const LINARR& pi) {
	uint64_t D = sum_length_edges(g, pi);
	rational MDD = rational_from_ui(D, g.n_edges());
	return MDD;
}

rational MDD_rational(const ugraph& g, const LINARR& pi) {
	return utils::call_with_empty_arrangement(__MDD_rational, g, pi);
}

double MDD(const ugraph& g, const LINARR& pi) {
	return MDD_rational(g, pi).to_double();
}

/* 1-level METRICS */

rational MDD_1level_rational(
	const vector<ugraph>& Gs, const vector<LINARR>& pis
)
{
	// the number of graphs and number of linear arrangements
	// must coincide unless no arrangement was given.
	assert(pis.size() == 0 or Gs.size() == pis.size());

	uint64_t sumD = 0;
	uint64_t sumM = 0;
	if (pis.size() == 0) {
		const vector<position> empty_arr;
		for (size_t i = 0; i < Gs.size(); ++i) {
			sumD +=
			utils::call_with_empty_arrangement(__sum_length_edges, Gs[i], empty_arr);
			sumM += Gs[i].n_edges();
		}
	}
	else {
		for (size_t i = 0; i < Gs.size(); ++i) {
			assert(Gs[i].n_nodes() == pis[i].size());

			sumD += __sum_length_edges(Gs[i], pis[i]);
			sumM += Gs[i].n_edges();
		}
	}
	return rational_from_ui(sumD, sumM);
}

double MDD_1level(
	const vector<graphs::ugraph>& Gs, const vector<LINARR>& pis
)
{
	return MDD_1level_rational(Gs, pis).to_double();
}

/* 2-level METRICS */

rational MDD_2level_rational(
	const vector<ugraph>& Gs, const vector<LINARR>& pis
)
{
	assert(pis.size() == 0 or Gs.size() == pis.size());

	rational sum_MDD(0);
	if (pis.size() == 0) {
		const vector<position> empty_arr;
		for (size_t i = 0; i < Gs.size(); ++i) {
			sum_MDD += MDD_rational(Gs[i], empty_arr);
		}
	}
	else {
		for (size_t i = 0; i < Gs.size(); ++i) {
			assert(Gs[i].n_nodes() == pis[i].size());

			sum_MDD += MDD_rational(Gs[i], pis[i]);
		}
	}
	return sum_MDD/static_cast<int64_t>(Gs.size());
}

double MDD_2level(
	const vector<graphs::ugraph>& Gs, const vector<LINARR>& pis
)
{
	return MDD_2level_rational(Gs, pis).to_double();
}

} // -- namespace linarr
} // -- namespace lal
