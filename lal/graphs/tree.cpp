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

#include <lal/graphs/tree.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/union_find.hpp>

namespace lal {
namespace graphs {

/* CONSTRUCTORS */

tree::tree() noexcept { }
tree::tree(const tree& t) noexcept : graph() {
	tree_only_copy(t);
}
tree::tree(tree&& t) noexcept {
	tree_only_move(std::move(t));
}
tree::~tree() noexcept { }

/* OPERATORS */

tree& tree::operator= (const tree& t) noexcept {
	tree_only_copy(t);
	return *this;
}

tree& tree::operator= (tree&& t) noexcept {
	tree_only_move(std::move(t));
	return *this;
}

/* GETTERS */

bool tree::is_tree() const noexcept {
	// NOTE: this would not really be true if the addition of edges
	// was not constrained. Since it is, in a way that no cycles can
	// be produced, then we only need to check for the number of edges.
	return (num_nodes() == 0 ? true : num_edges() == num_nodes() - 1);

	// NOTE 2: this is only true in a debug compilation of the library
	// since a release compilation does not actually constrain the addition
}

bool tree::can_add_edge(node u, node v) const noexcept {
#if defined DEBUG
	assert(has_node(u));
	assert(has_node(v));
#endif

	// in a tree we must have m <= n - 1
	if (num_edges() + 1 > num_nodes() - 1) {
		return false;
	}

	// if m_root_of[s] == m_root_of[t] then
	// either the edge already exists or there
	// exists a path from 's' to 't'
	return m_root_of[u] != m_root_of[v];
}

bool tree::can_add_edges(const vector<edge>& edges) const noexcept {
	const uint32_t n = num_nodes();
	const uint32_t m = num_edges();
	const uint32_t more_m = static_cast<uint32_t>(edges.size());

	// in a tree we must have m <= n - 1
	if (m + more_m > n - 1) {
		return false;
	}

	vector<uint32_t> _root_of = m_root_of;
	vector<uint32_t> _root_size = m_root_size;

	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(has_node(u));
		assert(has_node(v));
#endif

		// if m_labels[s] == m_labels[t] then
		// either the edge exists or there
		// exists a path from 's' to 't'
		if (_root_of[u] == _root_of[v]) { return false; }

		call_union_find_add(
			u, v, &_root_of[0], &_root_size[0]
		);
	}
	return true;
}

vector<string> tree::get_tree_type_list() const noexcept {
	vector<string> l;
	l.reserve(__tree_type_size);
	for (size_t i = 0; i < __tree_type_size; ++i) {
		if (m_tree_type[i]) {
			l.push_back(
				string(tree_type_to_string(static_cast<tree_type>(i)))
			);
		}
	}
	return l;
}

/* PROTECTED */

void tree::tree_only_init(uint32_t n) noexcept {
	m_root_of = vector<uint32_t>(n);
	m_root_size = vector<uint32_t>(n);
	for (node u = 0; u < n; ++u) {
		m_root_of[u] = u;
		m_root_size[u] = 1;
	}
	fill(m_tree_type.begin(), m_tree_type.end() - 1, false);
	m_tree_type[static_cast<size_t>(tree_type::none)] = true;
}

void tree::tree_only_clear() noexcept {
	m_root_of.clear();
	m_root_size.clear();
	fill(m_tree_type.begin(), m_tree_type.end() - 1, false);
	m_tree_type[static_cast<size_t>(tree_type::none)] = true;
}

void tree::tree_only_copy(const tree& t) noexcept {
	// copy this class' members
	m_root_of = t.m_root_of;
	m_root_size = t.m_root_size;
	m_tree_type = t.m_tree_type;
}

void tree::tree_only_move(tree&& t) noexcept {
	// move this class' members
	m_root_of = std::move(t.m_root_of);
	m_root_size = std::move(t.m_root_size);
	m_tree_type = std::move(t.m_tree_type);
}

void tree::extra_work_per_edge_add(node u, node v) noexcept {
	graph::extra_work_per_edge_add(u, v);
	call_union_find_add(
		u, v, &m_root_of[0], &m_root_size[0]
	);
}
void tree::tree_only_extra_work_edges_set() noexcept {
	fill_union_find();
}
void tree::extra_work_per_edge_remove(node u, node v) noexcept {
	graph::extra_work_per_edge_remove(u, v);
	call_union_find_remove(
		u, v, &m_root_of[0], &m_root_size[0]
	);
}

void tree::fill_union_find() noexcept {
	for (node u = 0; u < num_nodes(); ++u) {
		// all vertices point to root zero
		m_root_of[u] = 0;
	}
	// the size of the connected component of the root 0 is n
	m_root_size[0] = num_nodes();
}

} // -- namespace graphs
} // -- namespace lal
