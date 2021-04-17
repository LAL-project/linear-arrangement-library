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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
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
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>

#include <lal/internal/linarr/Dmin.hpp>
#include <lal/internal/properties/tree_centroid.hpp>

namespace lal {
using namespace graphs;

namespace internal {

inline void make_directed(
	const free_tree& t, node pu, node u, vector<vector<pair<node,uint32_t>>>& M
)
{
	if (pu == u) {
		for (node v : t.get_neighbours(u)) {
			make_directed(t, u, v, M);
		}
		return;
	}

	// find the only instance of 'pu' in the
	// neighbourhood of 'u' and erase it.
	auto& Mu = M[u];

	auto it = Mu.begin();
	bool found = false;
	while (not found and it != Mu.end()) {
		if (it->first == pu) {
			Mu.erase(it);
			found = true;
		}
		else {
			++it;
		}
	}

	for (node v : t.get_neighbours(u)) {
		if (v != pu) {
			make_directed(t, u, v, M);
		}
	}
}

pair<uint32_t, linear_arrangement> Dmin_Planar(const free_tree& t) {
#if defined DEBUG
	assert(t.is_tree());
#endif

	const uint32_t n = t.get_num_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	// In short, Hochberg and Stallmann described their algorithm
	// as rooting a free tree at one of its centroidal vertices and
	// arranging it so that the root is not covered and the arrangement
	// yields minimum D.

	// Therefore, they proved that any optimal projective arrangement of a
	// free tree (T) rooted at one of its centroidal vertices (T_c) yields
	// the same value of D as any of the optimal planar arrangements
	// of T. For this reason, any optimal projective arrangement of T_c
	// is an optimal planar arrangement of T.

	vector<vector<pair<node,uint32_t>>> M;
	vector<pair<edge, uint32_t>> sizes_edge;

	// find a centroidal vertex of the tree
	const node c = internal::retrieve_centroid(t, M, sizes_edge).first;

	// convert M into a directed adjacency matrix
	make_directed(t, c, c, M);

	// construct the optimal intervals using Gildea and Temperley's
	// algorithm to calculate an optimal projective arrangement.
	linear_arrangement arr(n);
	const uint32_t D =
	Dmin_Pr__optimal_interval_of(n, M, c, arr);

	return make_pair(D, arr);
}

} // -- namespace internal
} // -- namespace lal
