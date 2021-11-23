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

// lal includes
#include <lal/detail/graphs/union_find.hpp>
#include <lal/detail/graphs/tree_type.hpp>

namespace lal {
namespace graphs {

/* GETTERS */

bool tree::can_add_edge(node u, node v) const noexcept {
#if defined DEBUG
	assert(has_node(u));
	assert(has_node(v));
#endif

	// in a tree we must have m <= n - 1
	if (get_num_edges() + 1 > get_num_nodes() - 1) {
		return false;
	}

	// * if m_root_of[s] == m_root_of[t] then either
	//		- the edge already exists, or
	//		- there exists a path from 's' to 't'
	return m_root_of[u] != m_root_of[v];
}

bool tree::can_add_edges(const std::vector<edge>& edges) const noexcept {
	const uint64_t n = get_num_nodes();
	const uint64_t m = get_num_edges();
	const uint64_t more_m = edges.size();

	// in a tree we must have m <= n - 1
	if (m + more_m > n - 1) {
		return false;
	}

	std::vector<uint64_t> _root_of = m_root_of;
	std::vector<uint64_t> _root_size = m_root_size;

	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(has_node(u));
		assert(has_node(v));
		assert(not has_edge(u,v));
#endif

		// if m_labels[s] == m_labels[t] then
		// either the edge exists or there
		// exists a path from 's' to 't'
		if (_root_of[u] == _root_of[v]) { return false; }

		call_union_find_after_add
			(u, v, &_root_of[0], &_root_size[0]);
	}
	return true;
}

std::vector<std::string> tree::get_tree_type_list() const noexcept {
	std::vector<std::string> l;
	l.reserve(__tree_type_size);
	for (std::size_t i = 0; i < __tree_type_size; ++i) {
		if (m_tree_type[i]) {
			l.emplace_back
			(detail::tree_type_string(static_cast<tree_type>(i)));
		}
	}
	return l;
}

/* PROTECTED */

void tree::extra_work_per_edge_add(node u, node v) noexcept {
	m_is_tree_type_valid = false;
	graph::extra_work_per_edge_add(u, v);
	call_union_find_after_add
		(u, v, &m_root_of[0], &m_root_size[0]);
}

void tree::tree_only_extra_work_edges_set() noexcept {
	m_is_tree_type_valid = false;
	fill_union_find();
}

void tree::extra_work_per_edge_remove(node u, node v) noexcept {
	m_is_tree_type_valid = false;
	graph::extra_work_per_edge_remove(u, v);
	call_union_find_after_remove
		(u, v, &m_root_of[0], &m_root_size[0]);
}

} // -- namespace graphs
} // -- namespace lal
