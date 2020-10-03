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
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#include <lal/graphs/free_tree.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
//#include <lal/graphs/output.hpp>
#include <lal/internal/graphs/cycles.hpp>
#include <lal/internal/graphs/reachability.hpp>
#include <lal/internal/graphs/trees/is_tree.hpp>

namespace lal {
namespace graphs {

//ftree::ftree() { }
free_tree::free_tree(uint32_t n) : undirected_graph(n) {
	free_tree::_init(n);
}
free_tree::free_tree(const undirected_graph& t) : undirected_graph(t.n_nodes()) {
	// check that the input graph is a ftree
	assert(internal::is_graph_a_tree(t));

	free_tree::_init(t.n_nodes());
	add_edges(t.edges());
}
//ftree::~ftree() { }

free_tree& free_tree::add_edge(node u, node v, bool norm, bool check_norm) {
	assert(can_add_edge(u,v));
	undirected_graph::add_edge(u,v, norm, check_norm);
	return *this;
}

free_tree& free_tree::add_edges(
	const vector<edge>& edges, bool norm, bool check_norm
)
{
	assert(can_add_edges(edges));
	undirected_graph::add_edges(edges, norm, check_norm);
	return *this;
}

void free_tree::disjoint_union(const free_tree& t) {
	const node pre_join = n_nodes();

	// tree 't' and tree 'this' do not have cycles, so the disjoint
	// union of both trees does not have cycles.
	// Nothing to check.
	undirected_graph::disjoint_union(t);

	// join union-find
#define vec_join(A, B) A.insert(A.end(), B.begin(), B.end())
	vec_join(m_root_of, t.m_root_of);
	vec_join(m_root_size, t.m_root_size);

	// update the labels of the vertices' root of the union find
	for (node u = pre_join; u < n_nodes(); ++u) {
		m_root_of[u] += pre_join;
	}
}

/* GETTERS */

bool free_tree::is_rooted() const { return false; }

/* PROTECTED */

void free_tree::_init(uint32_t n) {
	tree::tree_init(n);
	undirected_graph::_init(n);
}

void free_tree::_clear() {
	tree::tree_clear();
	undirected_graph::_clear();
}

} // -- namespace graphs
} // -- namespace lal
