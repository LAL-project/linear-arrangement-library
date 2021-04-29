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

	const uint32_t n = t.get_num_nodes();

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

template<
	class tree_type,
	bool is_rooted = std::is_base_of_v<rooted_tree, tree_type>
>
std::conditional_t<
	is_rooted,
	rooted_tree,
	pair<free_tree,node>
>
from_head_vector_to_tree
(const head_vector& hv, bool normalise, bool check)
noexcept
{
	if (hv.size() == 0) {
		if constexpr (is_rooted) {
			return rooted_tree(0);
		}
		else {
			return make_pair(free_tree(0), 0);
		}
	}
	if (hv.size() == 1) {
#if defined DEBUG
		// the only vertex can only be the root
		assert(hv[0] == 0);
#endif
		if constexpr (is_rooted) {
			return rooted_tree(1);
		}
		else {
			return make_pair(free_tree(1), 0);
		}
	}

	const uint32_t n = static_cast<uint32_t>(hv.size());

	// output tree
	tree_type t(n);

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
			// note:
			// * i ranges in [0,n-1]
			// * L[i] ranges in [1,n] (hence the '-1')

			// In the head vector the edge (i, hv[i] - 1) is an edge of an
			// anti-arborescence. Since for our rooted trees we need the edge
			// of an arborescence, then we add the edge as (hv[i] - 1, i).
			// For free trees the order of vertices does not matter.
			t.add_edge_bulk(hv[i] - 1, i);

#if defined DEBUG
			++num_edges_added;
#endif
		}
	}

#if defined DEBUG
	// root must have been set
	assert(r);
	// amount of edges added must be 'n-1'
	assert(num_edges_added == n - 1);
#endif

	t.finish_bulk_add(normalise, check);

	if constexpr (is_rooted) {
		t.set_root(*r);
		t.set_valid_orientation(true);
#if defined DEBUG
		assert(t.is_rooted_tree());
#endif

		return t;
	}
	else {
#if defined DEBUG
		assert(t.is_tree());
#endif

		return make_pair(t, *r);
	}
}

pair<free_tree,node> from_head_vector_to_free_tree
(const head_vector& hv, bool normalise, bool check)
noexcept
{
	return from_head_vector_to_tree<free_tree>(hv, normalise, check);
}

rooted_tree from_head_vector_to_rooted_tree
(const head_vector& hv, bool normalise, bool check)
noexcept
{
	return from_head_vector_to_tree<rooted_tree>(hv, normalise, check);
}

} // -- namespace graphs
} // -- namespace lal
