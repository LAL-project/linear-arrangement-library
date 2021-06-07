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

#include <lal/generate/all_planar_arrangements.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
using namespace std;

// lal includes
#include <lal/internal/graphs/make_arrangement.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/sorting/bit_sort.hpp>
using namespace lal;
using namespace graphs;

namespace lal {
namespace generate {

all_planar_arrangements::all_planar_arrangements(const free_tree& T) noexcept
	: m_T(T),
	  m_parent(m_T.get_num_nodes()),
	  m_memory_bit_sort(T.get_num_nodes(), 0)
{
#if defined DEBUG
	assert(m_T.is_tree());
#endif

	m_intervals = vector<vector<node>>(m_T.get_num_nodes());
	__reset();
}

all_planar_arrangements::all_planar_arrangements(const rooted_tree& T) noexcept
	: m_T_copy(T.to_free_tree()),
	  m_T(m_T_copy),
	  m_parent(m_T.get_num_nodes()),
	  m_memory_bit_sort(T.get_num_nodes())
{
#if defined DEBUG
	assert(m_T.is_tree());
#endif

	m_intervals = vector<vector<node>>(m_T.get_num_nodes());
	__reset();
}

void all_planar_arrangements::next() noexcept {
	if (not m_exists_next) {
		m_reached_end = true;
		return;
	}

	if (m_T.get_num_nodes() == 1) {
		m_reached_end = true;
		return;
	}

	// permute every level
	bool has_perm = false;
	node u = 0;
	while (u < m_T.get_num_nodes() and not has_perm) {
		vector<node>& inter_u = m_intervals[u];

		if (u == m_root) {
			// the root's level should be permuted carefully:
			// the root must stay at the leftmost position
			has_perm = next_permutation(inter_u.begin()+1, inter_u.end());
		}
		else {
			// permute all the vertices in the interval of the
			// non-root vertices
			has_perm = next_permutation(inter_u.begin(), inter_u.end());
		}

		++u;
	}

	// if all intervals have been permuted past their last permutation,
	// in other words, if we went back at beginning, then
	const bool all_past_last = (u == m_T.get_num_nodes() and not has_perm);

	if (all_past_last) {
		if (m_root == m_T.get_num_nodes() - 1) {
			m_reached_end = true;
		}
		else {
			++m_root;
			initialise_intervals_tree();
		}
	}
}

linear_arrangement all_planar_arrangements::get_arrangement() const noexcept {
	return (m_T.get_num_nodes() == 1 ?
		linear_arrangement(1) :
		internal::make_arrangement_intervals(m_T, m_root, m_intervals)
	);
}

/* PRIVATE */

void all_planar_arrangements::__reset() noexcept {
	m_root = 0;
	m_exists_next = true;
	m_reached_end = false;
	initialise_intervals_tree();
}

void all_planar_arrangements::initialise_intervals_tree() noexcept {
	m_parent[m_root] = m_root;
	m_intervals[m_root].resize(m_T.get_degree(m_root) + 1);
	initialise_interval_node(m_root);

	// we use a BFS to be able to keep track of the parent of
	// every vertex with respect to the root.
	internal::BFS<free_tree> bfs(m_T);
	bfs.set_process_neighbour(
	[&](const auto&, node u, node v, bool) -> void {
		m_intervals[v] = vector<node>(m_T.get_degree(v));
		m_parent[v] = u;
		initialise_interval_node(v);
	}
	);
	bfs.start_at(m_root);
}

void all_planar_arrangements::initialise_interval_node(node u) noexcept {
	const node parent = m_parent[u];
	const neighbourhood& neighs_u = m_T.get_neighbours(u);

	// the interval of this vertex
	vector<node>& inter_u = m_intervals[u];

	if (u == m_root) {
		// the root must be placed at the left most position
		inter_u[0] = u;

		std::copy(neighs_u.begin(), neighs_u.end(), inter_u.begin() + 1);

		// if the neighbours of node u are not normalised
		if (not m_T.is_normalised()) {
			internal::bit_sort<node>
			(inter_u.begin() + 1, inter_u.end(), inter_u.size());
		}
	}
	else {
		// this vertex is placed at the left most position
		inter_u[0] = u;

		std::copy_if(
			neighs_u.begin(), neighs_u.end(), inter_u.begin() + 1,
			[parent](const node v) -> bool { return v != parent; }
		);

		// in order to obtain a lexicographically sorted permutation,
		// we must sort it (vertex 'u' might not be placed properly).
		internal::bit_sort_mem<node>
		(inter_u.begin(), inter_u.end(), inter_u.size(), m_memory_bit_sort.data);
	}
}

} // -- namespace generate
} // -- namespace lal
