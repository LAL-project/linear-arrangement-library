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
 
#include <lal/conversions/conversions.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

namespace lal {
namespace convert {

ugraph level_sequence_to_tree(const vector<uint32_t>& L, uint32_t n) {
	// a little sanity check
	assert(L[0] == 1);

	ugraph t(n);

	// 'stack' of root candidates
	vector<node> s(n, 0);
	uint32_t it = 0;
	for (node i = 1; i < n; ++i) {
		// decrease the pointer to the 'stack'
		// to reach the desired level
		if (it + 2 > L[i]) {
			it = L[i] - 2;
		}

		// the top of the stack is the root
		// for this vertex
		node r = s[it];

		// add the vertex...
		t.add_edge(r, i);

		// this is the next potential root
		++it;
		s[it] = i;
	}
	return t;
}

ugraph linear_sequence_to_tree(const vector<uint32_t>& L, uint32_t n) {
	assert(L.size() == n + 1);

	ugraph t(n);
	for (uint32_t i = 1; i <= n; ++i) {
		if (L[i] == 0) {
			// root, do nothing
		}
		else {
			t.add_edge(i - 1, L[i] - 1);
		}
	}
	return t;
}

ugraph Prufer_sequence_to_tree(const vector<uint32_t>& seq, uint32_t n) {
	// initialisation
	const uint32_t L = n - 2;
	vector<uint32_t> degree(n, 1);
	for (uint32_t i = 0; i < L; ++i) {
		degree[ seq[i] ] += 1;
	}
	ugraph t(n);

	// for each number in the sequence seq[i], find the first
	// lowest-numbered node, j, with degree equal to 1, add
	// the edge (j, seq[i]) to the tree, and decrement the degrees
	// of j and seq[i].
	for (uint32_t v = 0; v < L; ++v) {
		const uint32_t value = seq[v];
		bool node_found = false;
		node w = 0;
		while (w < n and not node_found) {
			if (degree[w] == 1) {
				t.add_edge(value, w);
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
	t.add_edge(u, v);
	return t;
}

} // -- namespace convert
} // -- namespace lal
