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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/linarr/Dmin_Projective_rooted_adjacency_list.hpp>

namespace lal {
using namespace graphs;
using namespace iterators;

namespace internal {

pair<uint64_t, linear_arrangement> Dmin_Projective(const rooted_tree& t) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	const node r = t.get_root();

	// for every edge (u,v), store the tuple
	//    (n_v, (u,v))
	// at L[u]
	data_array<edge_size> L(n - 1);

	{
	const size_t k = t.are_size_subtrees_valid() ? 0 : t.get_num_nodes();
	data_array<uint64_t> size_subtrees(k, 0);

	countingsort::memory_counting_sort<edge_size> memcs(n, n);
	auto it = L.begin();
	E_iterator<rooted_tree> E_it(t);

	if (t.are_size_subtrees_valid()) {
		// use the sizes that are already calculated
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = t.get_num_nodes_subtree(v);
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			E_it.next();
		}
	}
	else {
		// fill in the size of the subtrees
		internal::get_size_subtrees(t, r, size_subtrees.begin());
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = size_subtrees[v];
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			E_it.next();
		}
	}

	// sort all tuples in L using the size of the subtree
	internal::counting_sort
	<edge_size, edge_size*, countingsort::decreasing_t, true>
	(
		L.begin(), L.end(), n,
		[](const edge_size& T) -> size_t { return T.second; },
		memcs
	);
	}

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	// This is used to find the optimal projective arrangement of the tree.
	vector<vector<node_size>> M(n);
	for (const auto& T : L) {
		const auto [u, v] = T.first;
		const uint64_t nv = T.second;
		M[u].push_back(make_pair(v,nv));
#if defined DEBUG
		assert(t.has_edge(u,v));
#endif
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(M[u].size() == t.get_out_degree(u));
	}
#endif

	// construct the optimal planar arrangement
	linear_arrangement arr(n);
	const uint64_t D = Dmin_Projective_rooted_adjacency_list(n, M, r, arr);

	return make_pair(D, std::move(arr));
}

} // -- namespace internal
} // -- namespace lal
