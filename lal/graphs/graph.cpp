/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

#include <lal/graphs/graph.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

void graph::init(uint64_t n) noexcept {
	_clear();
	_init(n);
}

void graph::clear() noexcept {
	_clear();
}

void graph::normalize() noexcept {
	detail::array<char> mem(get_num_nodes(), 0);
	for (node u = 0; u < get_num_nodes(); ++u) {
		neighbourhood& nu = m_adjacency_list[u];

		if (not std::is_sorted(nu.begin(), nu.end())) {
			detail::sorting::bit_sort_mem<node>
			(nu.begin(), nu.end(), nu.size(), mem.begin());
		}
	}
	m_is_normalized = true;
}

bool graph::check_normalized() noexcept {
	// check that every adjacency list is sorted
	for (node u = 0; u < get_num_nodes(); ++u) {
		const neighbourhood& nu = m_adjacency_list[u];
		if (not std::is_sorted(nu.begin(), nu.end())) {
			// if some is not then the graph is not normalized
			m_is_normalized = false;
			return false;
		}
	}
	// all adjacency lists are sorted so
	// the graph is normalized
	m_is_normalized = true;
	return true;
}

/* SETTERS */

/* GETTERS */

/* PROTECTED */

void graph::__disjoint_union(const graph& g) noexcept {
#if defined DEBUG
	// If I'm directed, g must be directed.
	// If I'm undirected, g must be undirected.
	assert(is_directed() ? g.is_directed() : g.is_undirected());
#endif

	// update number of edges
	m_num_edges += g.m_num_edges;

	// If one or none of the two graphs involved are normalized,
	// the result is not normalized.
	// If both graphs are normalized, the result is normalized.
	m_is_normalized = m_is_normalized and g.is_normalized();
}

void graph::actions_after_add_edge(const node, const node) noexcept {
	++m_num_edges;
}

void graph::actions_after_add_edges(const edge_list& e) noexcept {
	m_num_edges += e.size();
}

void graph::actions_after_add_edges_bulk() noexcept { }

void graph::actions_after_remove_edge(const node, const node) noexcept {
	--m_num_edges;
}

void graph::actions_after_remove_edges(const edge_list& e) noexcept {
	m_num_edges -= e.size();
}

void graph::actions_after_remove_edges_bulk() noexcept { }

void graph::actions_before_remove_edges_incident_to(const node) noexcept { }

void graph::actions_after_remove_node(const node) noexcept { }

void graph::normalize_after_edge_addition(const bool to_norm, const bool check_norm)
noexcept
{
	if (to_norm) {
		// the graph needs to be normalized from a non-normalized state
		normalize();
	}
	else if (check_norm) {
		if (not is_normalized()) {
			// the graph is certainly not normalized --
			// no need to check anything
		}
		else {
			// the graph structure has been modified
			// so we have to check whether it is normalized or not
			check_normalized();
		}
	}
	else {
		// not 'to_norm' and not 'check_norm' --
		m_is_normalized = false;
	}
}

void graph::normalize_after_edge_removal(const bool to_norm, const bool check_norm)
noexcept
{
	// if (is_normalized()) {
	//		Removing an edge does not change normalisation
	// }
	// if (not is_normalized()) {
	//		Since the graph was not normalized, we need to do something about it.
	//      if (norm)     ... NORMALISE THE GRAPH
	//      if (not norm) ... the result of deleting edges is certainly
	//                        not normalized since the deletion of edges
	//                        keeps the normalisation invariant. No need
	//                        to check.
	// }

	if (not is_normalized()) {
		if (to_norm) {
			normalize();
		}
		else if (check_norm) {
			// we might have been lucky...
			check_normalized();
		}
		else {
			// not 'to_norm' and not 'check_norm' --
			m_is_normalized = false;
		}
	}
}

} // -- namespace graphs
} // -- namespace lal
