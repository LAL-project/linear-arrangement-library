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

#include <lal/generate/all_projective_arrangements.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/make_projective_arr.hpp>

namespace lal {
using namespace graphs;
using namespace internal;

namespace generate {

all_projective_arrangements::all_projective_arrangements(const rooted_tree& rT) : m_rT(rT)
{
	assert(m_rT.is_rooted_tree());
	assert(m_rT.is_normalised());

	m_intervals = vector<vector<node>>(m_rT.n_nodes());
	for (node u = 0; u < m_rT.n_nodes(); ++u) {
		const uint32_t d = m_rT.out_degree(u);
		m_intervals[u] = vector<node>(d + 1);
	}

	initialise_intervals_tree(m_rT.get_root());
}

all_projective_arrangements::~all_projective_arrangements() {}

bool all_projective_arrangements::has_next() const {
	return m_has_next;
}

void all_projective_arrangements::next() {
	if (m_rT.n_nodes() == 1) {
		m_has_next = false;
		return;
	}

	bool has_perm = false;
	node u = 0;
	while (u < m_rT.n_nodes() and not has_perm) {
		vector<node>& inter_u = m_intervals[u];

		has_perm = next_permutation(inter_u.begin(), inter_u.end());
		if (not has_perm) {
			initialise_interval_node(u);
		}
		++u;
	}

	/*if (i == m_por_vertices.size() and not has_perm) {
		m_has_next = false;
	}*/
	m_has_next = u != m_rT.n_nodes() or has_perm;
}

linear_arrangement all_projective_arrangements::get_arrangement() const {
	return (m_rT.n_nodes() == 1 ?
			linear_arrangement(1) : put_in_arrangement(m_rT, m_intervals));
}

/* PRIVATE */

void all_projective_arrangements::initialise_intervals_tree(node r) {
	initialise_interval_node(r);
	const neighbourhood& neighs_r = m_rT.get_out_neighbours(r);
	for (node u : neighs_r) {
		initialise_intervals_tree(u);
	}
}

void all_projective_arrangements::initialise_interval_node(node u) {
	const neighbourhood& neighs_r = m_rT.get_out_neighbours(u);
	vector<node>& inter_u = m_intervals[u];

	size_t i = 0;
	while (i < neighs_r.size() and neighs_r[i] < u) {
		inter_u[i] = neighs_r[i];
		++i;
	}
	inter_u[i] = u;
	++i;
	for (; i < neighs_r.size(); ++i) {
		inter_u[i] = neighs_r[i - 1];
	}
}

} // -- namespace generate
} // -- namespace lal
