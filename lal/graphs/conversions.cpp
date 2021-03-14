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
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

// -----------------------------------------------------------------------------
// tree -> head vector

head_vector from_tree_to_head_vector(const rooted_tree& t) noexcept {
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint32_t n = t.num_nodes();

	vector<uint32_t> head_vector(n);
	for (node u = 0; u < n; ++u) {
		if (u == t.get_root()) {
			head_vector[u] = 0;
		}
		else {
			// this is guaranteed to be a legal access (within bounds).
			head_vector[u] = t.get_in_neighbours(u)[0] + 1;
		}
	}

	return head_vector;
}

vector<uint32_t> from_tree_to_head_vector(const free_tree& t, node r) noexcept {
	return from_tree_to_head_vector(rooted_tree(t,r));
}

// -----------------------------------------------------------------------------
// head vector -> graph

pair<free_tree,node> from_head_vector_to_free_tree
(const head_vector& hv, bool normalise, bool check)
noexcept
{
	if (hv.size() == 0) { return make_pair(free_tree(0), 0); }

	const uint32_t n = static_cast<uint32_t>(hv.size());

	// output tree
	free_tree t(n);

	// root node of the tree
	std::optional<node> r;

#if defined DEBUG
	uint32_t num_edges_added = 0;
#endif

	for (uint32_t i = 0; i < n; ++i) {
		if (hv[i] == 0) {
			// root, do nothing
			r = i;
		}
		else {
			// add the edge:
			// * i ranges in [0,n-1]
			// * L[i] ranges in [1,n]
			t.add_edge_bulk(i, hv[i] - 1);
#if defined DEBUG
			++num_edges_added;
#endif
		}
	}

#if defined DEBUG
	// root must have been set.
	assert(r);
	// amount of edges added must be 'n-1'
	assert(num_edges_added == n - 1);
#endif

	t.finish_bulk_add(normalise, check);
	return make_pair(t, *r);
}

rooted_tree from_head_vector_to_rooted_tree
(const head_vector& hv, bool normalise, bool check)
noexcept
{
	const auto [tree, root] =
		from_head_vector_to_free_tree(hv, normalise, check);
#if defined DEBUG
	assert(tree.is_tree());
#endif
	return rooted_tree(tree, root);
}

} // -- namespace graphs
} // -- namespace lal
