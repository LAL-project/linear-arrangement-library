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
#include <lal/internal/properties/tree_centroid.hpp>
#include <lal/internal/linarr/Dmin_Projective_rooted_adjacency_list.hpp>

namespace lal {
using namespace graphs;

namespace internal {

inline void make_directed(
	const free_tree& t, node pu, node u, vector<vector<pair<node,uint64_t>>>& M
)
noexcept
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

pair<uint64_t, linear_arrangement> Dmin_Planar(const free_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	// In short, Hochberg and Stallmann described their algorithm
	// as rooting a free tree at one of its centroidal vertices and
	// arranging it so that the root is not covered and the arrangement
	// yields minimum D.

	// Therefore, they proved (kind of) that any optimal projective arrangement
	// of a free tree (T) rooted at one of its centroidal vertices (T^c) yields
	// the same value of D as any of the optimal planar arrangements
	// of T. For this reason, any optimal projective arrangement of T^c
	// is an optimal planar arrangement of T.

	vector<vector<pair<node,uint64_t>>> M;
	vector<pair<edge, uint64_t>> sizes_edge;

	// Find a centroidal vertex of the tree.
	// With this method we can retrieve the sorted adjacency matrix;
	// such matrix is used to retrieve the centroid and arrange the tree.
	const node c = internal::retrieve_centroid(t, M, sizes_edge).first;

	// convert M into a rooted (also, directed) adjacency matrix
	make_directed(t, c, c, M);

	// construct the optimal interval by calculating the optimal
	// projective arrangement
	linear_arrangement arr(n);
	const uint64_t D = Dmin_Projective_rooted_adjacency_list(n, M, c, arr);

	return make_pair(D, std::move(arr));
}

} // -- namespace internal
} // -- namespace lal
