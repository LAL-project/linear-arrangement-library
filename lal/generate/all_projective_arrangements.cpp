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

#include <lal/generate/all_projective_arrangements.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
using namespace std;

// lal includes
#include <lal/utils/graphs/trees/make_projective_arr.hpp>

#define degree_tree(T, u)										\
	(T.get_rtree_type() == rtree::rtree_type::arborescence ?	\
		T.out_degree(u) : T.in_degree(u) )

#define neighs_tree(T, u)										\
	(T.get_rtree_type() == rtree::rtree_type::arborescence ?	\
		T.get_out_neighbours(u) : T.get_in_neighbours(u) )

namespace lal {
using namespace graphs;
using namespace utils;

namespace generate {

all_proj_arr::all_proj_arr(const rtree& rT) : m_rT(rT)
{
	assert(m_rT.is_rooted_tree());

	m_intervals = vector<vector<node>>(m_rT.n_nodes());
	m_por_vertices = vector<node>();

	post_order_vertex_ordering(m_rT.get_root());
	canonical_interval_tree(m_rT.get_root());
}

all_proj_arr::~all_proj_arr() {}

bool all_proj_arr::has_next() const {
	return m_has_next;
}

void all_proj_arr::next() {
	if (m_rT.n_nodes() == 1) {
		m_has_next = false;
		return;
	}

	bool has_perm = false;
	size_t i = 0;
	while (i < m_por_vertices.size() and not has_perm) {
		const node u = m_por_vertices[i];
		vector<node>& inter_u = m_intervals[u];

		has_perm = next_permutation(inter_u.begin(), inter_u.end());
		if (not has_perm) {
			canonical_interval_single(u);
		}
		++i;
	}

	/*if (i == m_por_vertices.size() and not has_perm) {
		m_has_next = false;
	}*/
	m_has_next = i != m_por_vertices.size() or has_perm;
}

linearrgmnt all_proj_arr::get_arrangement() const {
	return (m_rT.n_nodes() == 1 ?
			linearrgmnt(1) : put_in_arrangement(m_rT, m_intervals));
}

/* PRIVATE */

void all_proj_arr::post_order_vertex_ordering(node r) {
	const uint32_t d_out = degree_tree(m_rT, r);
	const neighbourhood& neighs_r = neighs_tree(m_rT, r);
	// leaf
	if (d_out == 0) {
		m_por_vertices.push_back(r);
		return;
	}
	// internal node
	for (node vi : neighs_r) {
		post_order_vertex_ordering(vi);
	}
	m_por_vertices.push_back(r);
}

void all_proj_arr::canonical_interval_tree(node r) {
	canonical_interval_single(r);
	const neighbourhood& neighs_r = neighs_tree(m_rT, r);
	for (node u : neighs_r) {
		canonical_interval_tree(u);
	}
}

void all_proj_arr::canonical_interval_single(node u) {
	const uint32_t d = degree_tree(m_rT, u);
	const neighbourhood& neighs_r = neighs_tree(m_rT, u);

	vector<node>& inter_u = m_intervals[u];

	inter_u = vector<node>(d + 1);
	inter_u[0] = u;
	for (uint32_t i = 0; i < d; ++i) {
		const node ui = neighs_r[i];
		inter_u[i + 1] = ui;
	}
}

} // -- namespace generate
} // -- namespace lal
