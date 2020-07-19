/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
 
#include <lal/utils/graphs/trees/convert_to_ftree.hpp>

// C++ includes
#include <cassert>
using namespace std;

#define to_uint32(x) static_cast<uint32_t>(x)

namespace lal {
using namespace graphs;

namespace utils {

ftree level_sequence_to_tree(const vector<uint32_t>& L, uint32_t n) {
	// a little sanity check
	assert(L[0] == 0);
	assert(L[1] == 1);
    assert(to_uint32(L.size()) == n + 1);

	// edges of the tree
	vector<edge> edges(n - 1);
	auto eit = edges.begin();

	// 'stack' of root candidates: node at every level in {1,...,N}.
	// at position j, lev[j], store the last node added
	// at level j.
	vector<node> lev(n+1, 0);
	uint32_t stack_it = 0;

	// evidently,
	lev[0] = 1;

	for (node i = 2; i <= n; ++i) {

		// find in the stack the node which
		// has to be connected to node 'i'.
		if (lev[stack_it] + 2 > L[i]) {
			stack_it = L[i] - 2 + 1;
		}

		// the top of the stack is the parent of this node
		const node r = lev[stack_it];

		// add the edge...
		*eit++ = (r == 0 ? edge(r, i - 1) : edge(r - 1, i - 1));

		// the last node added at level L[i] is i.
		++stack_it;
		assert(stack_it == L[i]);
		lev[stack_it] = i;
	}

	ftree t(n);
	t.add_edges(edges);
	return t;
}

ftree Prufer_sequence_to_tree(const vector<uint32_t>& seq, uint32_t n) {
	// initialisation
	const uint32_t L = n - 2;
	vector<uint32_t> degree(n, 1);
	for (uint32_t i = 0; i < L; ++i) {
		degree[ seq[i] ] += 1;
	}

	// edges of the tree
	vector<edge> edges(n - 1);
	auto eit = edges.begin();

	// for each number in the sequence seq[i], find the first
	// lowest-numbered node, j, with degree equal to 1, add
	// the edge (j, seq[i]) to the tree, and decrement the degrees
	// of j and seq[i].
	for (uint32_t v = 0; v < L; ++v) {
		const auto value = seq[v];
		bool node_found = false;
		node w = 0;
		while (w < n and not node_found) {
			if (degree[w] == 1) {
				*eit++ = edge(value, w);

				degree[value] -= 1;
				degree[w] -= 1;
				node_found = true;
			}
			++w;
		}
	}

	// two nodes u,v with degree 1 remain. Find them
	node u, v;
	u = v = n;
	for (node w = 0; w < n; ++w) {
		if (degree[w] == 1) {
			if (u == n) {
				u = w;
			}
			else {
				v = w;
			}
		}
	}

	// add edge (u,v) to the tree
	*eit++ = edge(u, v);

	ftree t(n);
	t.add_edges(edges);
	return t;
}

} // -- namespace convert
} // -- namespace lal
