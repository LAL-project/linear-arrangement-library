/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
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

bool tree::can_add_edge(const node u, const node v) const noexcept
{
#if defined DEBUG
	assert(has_node(u));
	assert(has_node(v));
#endif

	// in a tree we must have m <= n - 1
	if (get_num_edges() + 1 > get_num_nodes() - 1) {
		return false;
	}

	// if m_root_of[u] == m_root_of[v] then either
	//     - the edge already exists, or
	//     - there exists a path from 'u' to 'v' in the undirected
	//       underlying structure, hence creating a cycle
	return m_union_find_root_of[u] != m_union_find_root_of[v];
}

bool tree::can_add_edges(const std::vector<edge>& edges) const noexcept
{
	const uint64_t n = get_num_nodes();
	const uint64_t m = get_num_edges();
	const uint64_t more_m = edges.size();

	// in a tree we must have m <= n - 1
	if (m + more_m > n - 1) {
		return false;
	}

	// make a copy
	std::vector<uint64_t> _root_of = m_union_find_root_of;
	std::vector<uint64_t> _root_size = m_union_find_root_size;

	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(has_node(u));
		assert(has_node(v));
		assert(not has_edge(u, v));
#endif

		// if m_labels[s] == m_labels[t] then
		// either the edge exists or there
		// exists a path from 's' to 't'
		if (_root_of[u] == _root_of[v]) {
			return false;
		}

		update_union_find_after_add_edge(u, v, &_root_of[0], &_root_size[0]);
	}
	return true;
}

std::vector<std::string> tree::get_tree_type_list() const noexcept
{
	std::vector<std::string> l;
	l.reserve(_tree_type_size);
	for (std::size_t i = 0; i < _tree_type_size; ++i) {
		if (m_tree_type[i]) {
			l.emplace_back(detail::tree_type_string(static_cast<tree_type>(i)));
		}
	}
	return l;
}

/* PROTECTED */

void tree::tree_only_actions_after_add_edge(const node u, const node v) noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_add_edge(
		u, v, &m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_add_edges(const edge_list& e) noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_add_edges(
		e, &m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_add_edges_bulk() noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_add_edges_bulk(
		&m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_add_edges_bulk_complete() noexcept
{
	m_is_tree_type_valid = false;
	fill_union_find();
}

void tree::tree_only_actions_after_remove_edges_bulk() noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_remove_edges_bulk(
		&m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_remove_edges_bulk_complete() noexcept
{
	m_is_tree_type_valid = false;
	empty_union_find();
}

void tree::tree_only_actions_after_remove_edge(const node u, const node v)
	noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_remove_edge(
		u, v, &m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_remove_edges(const edge_list& e) noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_after_remove_edges(
		e, &m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_actions_after_remove_node(const node u) noexcept
{
	m_is_tree_type_valid = false;

#if defined DEBUG
	assert(m_union_find_root_of[u] == u);
	assert(m_union_find_root_size[u] == 1);
#endif

	// update union-find data structure
	{
		auto e = m_union_find_root_of.begin();
		std::advance(e, u);
		m_union_find_root_of.erase(e);
	}
	{
		auto e = m_union_find_root_size.begin();
		std::advance(e, u);
		m_union_find_root_size.erase(e);
	}

	// relabel the roots when necessary
	for (node v = 0; v < m_union_find_root_of.size(); ++v) {
		m_union_find_root_of[v] -= (m_union_find_root_of[v] > u);
	}
}

void tree::tree_only_actions_before_remove_edges_incident_to(const node u
) noexcept
{
	m_is_tree_type_valid = false;
	update_union_find_before_remove_incident_edges_to(
		u, &m_union_find_root_of[0], &m_union_find_root_size[0]
	);
}

void tree::tree_only_set_edges() noexcept
{
	m_is_tree_type_valid = false;
	fill_union_find();
}

} // namespace graphs
} // namespace lal
