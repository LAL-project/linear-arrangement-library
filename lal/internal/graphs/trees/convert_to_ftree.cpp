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
#include <optional>
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/internal/data_array.hpp>

#define to_uint32(x) static_cast<uint32_t>(x)

namespace lal {
using namespace graphs;

namespace internal {

free_tree level_sequence_to_ftree
(const uint32_t * const L, uint32_t n, bool normalise, bool check)
noexcept
{
#if defined DEBUG
	// a little sanity check
	assert(L[0] == 0);
	assert(L[1] == 1);
#endif

	// output tree
	free_tree t(n);

	// 'stack' of root candidates: node at every level in {1,...,N}.
	// at position j, lev[j] contains the last node added at level j.
	data_array<node> lev(n+1, 0);
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
		const auto [u,v] = (r == 0 ? edge(r, i - 1) : edge(r - 1, i - 1));
		t.add_edge_bulk(u, v);

		// the last node added at level L[i] is i.
		++stack_it;
#if defined DEBUG
		assert(stack_it == L[i]);
#endif
		lev[stack_it] = i;
	}

	t.finish_bulk_add(normalise, check);
	return t;
}

free_tree level_sequence_to_ftree
(const vector<uint32_t>& L, uint32_t n, bool normalise, bool check)
noexcept
{ return level_sequence_to_ftree(&L[0], n, normalise, check); }

free_tree Prufer_sequence_to_ftree
(const uint32_t * const seq, uint32_t n, bool normalise, bool check)
noexcept
{
	// initialisation
	const uint32_t L = n - 2;
	data_array<uint32_t> degree(n, 1);
	for (uint32_t i = 0; i < L; ++i) {
		degree[ seq[i] ] += 1;
	}

	// the output tree
	free_tree t(n);

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
				t.add_edge_bulk(value, w);

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
	t.add_edge_bulk(u, v);
	t.finish_bulk_add(normalise, check);
	return t;
}

free_tree Prufer_sequence_to_ftree
(const vector<uint32_t>& seq, uint32_t n, bool normalise, bool check)
noexcept
{ return Prufer_sequence_to_ftree(&seq[0], n, normalise, check); }

} // -- namespace internal
} // -- namespace lal
