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
#include <lal/internal/graphs/trees/is_tree.hpp>

namespace lal {
namespace graphs {

/* CONSTRUCTORS */

free_tree::free_tree() : tree(), undirected_graph() { }
free_tree::free_tree(uint32_t n) : undirected_graph(n) {
	free_tree::tree_only_init(n);
}
free_tree::free_tree(const free_tree& f) : graph(), tree(), undirected_graph() {
	copy_full_free_tree(f);
}
free_tree::free_tree(free_tree&& f) {
	move_full_free_tree(std::move(f));
}
free_tree::free_tree(const undirected_graph& t) : undirected_graph(t.n_nodes()) {
	// check that the input graph is a tree
	assert(internal::is_graph_a_tree(t));

	free_tree::tree_only_init(t.n_nodes());
	set_edges(t.edges());
}
free_tree::free_tree(undirected_graph&& t) : undirected_graph(t) {
	// check that the input graph is a tree
	assert(internal::is_graph_a_tree(t));

	free_tree::tree_only_init(t.n_nodes());
	// no need to call set_edges
	fill_union_find();
}
free_tree::~free_tree() { }

/* OPERATORS */

free_tree& free_tree::operator= (const free_tree& f) {
	copy_full_free_tree(f);
	return *this;
}

free_tree& free_tree::operator= (free_tree&& f) {
	move_full_free_tree(std::move(f));
	return *this;
}

/* MODIFIERS */

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

free_tree& free_tree::set_edges(
	const vector<edge>& edges, bool to_norm, bool check_norm
)
{
	{
	const uint32_t n = n_nodes();
	clear(); init(n);
	}

	assert(can_add_edges(edges));
	undirected_graph::set_edges(edges, to_norm, check_norm);
	fill_union_find();
	return *this;
}

void free_tree::disjoint_union(const free_tree& t) {
	const node prev_n = n_nodes();
	if (prev_n == 0) {
		*this = t;
		return;
	}

	// tree 't' and tree 'this' do not have cycles, so the disjoint
	// union of both trees does not have cycles. However, the resulting
	// tree will lack an edge.
	undirected_graph::disjoint_union(t);

	// join union-find
#define vec_join(A, B) A.insert(A.end(), B.begin(), B.end())
	vec_join(m_root_of, t.m_root_of);
	vec_join(m_root_size, t.m_root_size);

	// update the labels of the vertices' root of the union find
	for (node u = prev_n; u < n_nodes(); ++u) {
		m_root_of[u] += prev_n;
	}
}

/* GETTERS */

/* PROTECTED */

void free_tree::_init(uint32_t n) {
	undirected_graph::_init(n);
	tree::tree_only_init(n);
}

void free_tree::_clear() {
	undirected_graph::_clear();
	tree::tree_only_clear();
}

void free_tree::copy_full_free_tree(const free_tree& f) {
	// copy undirected_graph class
	copy_full_undirected_graph(f);

	// copy only tree's members
	tree_only_copy(f);

	// copy this class' members
}

void free_tree::move_full_free_tree(free_tree&& f) {
	// move-assign undirected_graph class
	move_full_undirected_graph(std::move(f));

	// move-assign only tree's members
	tree_only_move(std::move(f));

	// move this class' members
}

} // -- namespace graphs
} // -- namespace lal
