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
#include <algorithm>
using namespace std;

// lal includes
#include <lal/generate/all_projective_arrangements.hpp>
#include <lal/internal/sorting/bit_sort.hpp>
#include <lal/internal/graphs/make_arrangement.hpp>

namespace lal {
using namespace graphs;
using namespace internal;

namespace generate {

/* PUBLIC */

/* CONSTRUCTORS */

all_projective_arrangements::all_projective_arrangements
(const rooted_tree& rT) noexcept : m_rT(rT)
{
#if defined DEBUG
	assert(m_rT.is_rooted_tree());
#endif

	m_intervals = vector<vector<node>>(m_rT.get_num_nodes());
	reset();
}

/* GETTERS */

linear_arrangement all_projective_arrangements::get_arrangement() const noexcept {
	return (m_rT.get_num_nodes() == 1 ?
		linear_arrangement(1,0) :
		make_arrangement_intervals(m_rT, m_intervals)
	);
}

/* MODIFIERS */

void all_projective_arrangements::next() noexcept {
	if (not m_exists_next) {
		m_reached_end = true;
		return;
	}

	if (m_rT.get_num_nodes() == 1) {
		m_exists_next = false;
		return;
	}

	bool has_perm = false;
	node u = 0;
	while (u < m_rT.get_num_nodes() and not has_perm) {
		vector<node>& inter_u = m_intervals[u];

		has_perm = std::next_permutation(inter_u.begin(), inter_u.end());
		if (not has_perm) {
			initialise_interval_node(u);
		}
		++u;
	}

	if (u == m_rT.get_num_nodes() and not has_perm) {
		m_reached_end = true;
	}
}

/* PRIVATE */

void all_projective_arrangements::reset() noexcept {
	m_exists_next = true;
	m_reached_end = false;
	initialise_intervals_tree();
}

void all_projective_arrangements::initialise_intervals_tree() noexcept {
	for (node u = 0; u < m_rT.get_num_nodes(); ++u) {
		const uint32_t d = m_rT.get_out_degree(u);
		m_intervals[u] = vector<node>(d + 1);
		initialise_interval_node(u);
	}
}

void all_projective_arrangements::initialise_interval_node(node u) noexcept {
	const neighbourhood& neighs_u = m_rT.get_out_neighbours(u);
	vector<node>& interval_u = m_intervals[u];

	if (m_rT.is_normalised()) {
		size_t neighs_it = 0;
		size_t interval_it = 0;

		while (
			neighs_it < neighs_u.size() and interval_it < interval_u.size() and
			neighs_u[neighs_it] < u
		)
		{
			interval_u[interval_it] = neighs_u[neighs_it];
			++neighs_it;
			++interval_it;
		}
		interval_u[interval_it] = u;
		++interval_it;
		while (neighs_it < neighs_u.size() and interval_it < interval_u.size()) {
			interval_u[interval_it] = neighs_u[neighs_it];
			++neighs_it;
			++interval_it;
		}
	}
	else {
		// fill the interval and then sort it
		for (size_t i = 0; i < neighs_u.size(); ++i) {
			interval_u[i] = neighs_u[i];
		}
		interval_u.back() = u;
		// sort using bitsort
		internal::bit_sort<node>(
			interval_u.begin(), interval_u.end(), interval_u.size()
		);
	}
}

} // -- namespace generate
} // -- namespace lal
