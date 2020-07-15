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

#include <lal/graphs/ftree.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/utils/graphs/cycles.hpp>
#include <lal/utils/graphs/reachability.hpp>
#include <lal/utils/graphs/trees/is_tree.hpp>

namespace lal {
namespace graphs {

//ftree::ftree() { }
ftree::ftree(uint32_t n) : ugraph(n) { }
ftree::ftree(const ugraph& t) : ugraph(t.n_nodes()) {
	// check that the input graph is a ftree
	assert(utils::is_graph_a_tree(t));
	add_edges(t.edges());
}
//ftree::~ftree() { }

ftree& ftree::add_edge(node s, node t, bool norm, bool check_norm) {
#if defined DEBUG
	assert(can_add_edge(s,t));
#endif

	ugraph::add_edge(s,t, norm, check_norm);
	return *this;
}

ftree& ftree::add_edges(const vector<edge>& edges, bool norm, bool check_norm) {
	ugraph::add_edges(edges, norm, check_norm);
	// NOTE: we can't do
	//     assert(utils::is_ftree(*this));
	// because the ftree might not be complete and lack some
	// edges. If we asserted "is_ftree", we would require the
	// user to insert ALL edges at once, but the edges might
	// not be available.
	assert(not utils::has_undirected_cycles(*this));
	return *this;
}

void ftree::disjoint_union(const ftree& t) {
	// tree 't' and tree 'this' do not have cycles, so the disjoint
	// union of both trees does not have cycles.
	// Nothing to check.

	ugraph::disjoint_union(t);
}

/* GETTERS */

bool ftree::is_rooted() const { return false; }

bool ftree::can_add_edge(node s, node t) const {
	// if the ftree already has n-1 edges then
	// adding another edge will produce a cycle
	if (n_edges() + 1 > n_nodes() - 1) {
		return false;
	}

	// the edge cannot be in the graph twice:
	// this is not a multigraph
	if (has_edge(s,t)) {
		return false;
	}

	// Check that adding this edge does not produce cycles.
	// Adding edge (u,v) produces cycles if 'u' is already
	// reachable from 'v' or viceversa.
	return not utils::is_node_reachable_from(*this, s, t);
}

bool ftree::can_add_edges(const vector<edge>& edges) const {
	// in a ftree we must have m <= n - 1
	const auto more_m = edges.size();
	if (n_edges() + more_m > n_nodes() - 1) {
		return false;
	}

	// check that none of the edges exist
	for (auto e : edges) {
		if (has_edge(e.first, e.second)) {
			return false;
		}
	}

	// 1. copy the current tree into an undirected graph
	ugraph copy = *this;
	// 2. add the edges to the copy
	copy.add_edges(edges, false);
	// 3. check that there are no cycles in the copy
	return not utils::has_undirected_cycles(copy);
}

/* PROTECTED */

void ftree::_init(uint32_t n) {
	tree::tree_init(n);
	ugraph::_init(n);
}

void ftree::_clear() {
	tree::tree_clear();
	ugraph::_clear();
}

} // -- namespace graphs
} // -- namespace lal
