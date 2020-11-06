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
#include <cassert>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

#include <lal/internal/graphs/trees/Dmin.hpp>
#include <lal/internal/graphs/trees/make_arrangement.hpp>
#include <lal/internal/graphs/trees/tree_centroid.hpp>

namespace lal {
using namespace graphs;

namespace internal {

inline void make_directed(
	const rooted_tree& rt, vector<vector<pair<node,uint32_t>>>& M
)
{
	for (node u = 0; u < rt.n_nodes(); ++u) {
		for (node v : rt.get_out_neighbours(u)) {
			// find 'u' in M[v] and remove it

			auto& Mv = M[v];
			auto it = Mv.begin();

			bool found = false;
			while (not found and it != Mv.end()) {
				if (it->first == u) {
					Mv.erase(it);
					found = true;
				}
				else {
					++it;
				}
			}
		}
	}
}

pair<uint32_t, linear_arrangement> Dmin_Planar(const free_tree& t) {
	assert(t.is_tree());

	const uint32_t n = t.n_nodes();
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

	// find the centroid of the tree
	const node c = internal::retrieve_centroid(t, M,sizes_edge).first;

	// root the free tree
	const rooted_tree rt(t, c);

	// convert M into a directed adjacency matrix
	make_directed(rt, M);

	// construct the optimal intervals using Gildea and Temperley's
	// algorithm to calculate an optimal projective arrangement.
	linear_arrangement arr(t.n_nodes());
	const uint32_t D =
	Dmin_Pr__optimal_interval_of(rt, M, rt.get_root(), arr);

	return make_pair(D, arr);
}

} // -- namespace internal
} // -- namespace lal
