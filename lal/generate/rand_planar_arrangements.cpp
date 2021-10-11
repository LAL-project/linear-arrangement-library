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

#include <lal/generate/rand_planar_arrangements.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <vector>

// lal includes
#include <lal/internal/graphs/make_arrangement.hpp>

namespace lal {
namespace generate {

rand_planar_arrangements::rand_planar_arrangements
(const graphs::free_tree& T, uint64_t seed)
noexcept
	: m_T(T),
	  m_rdata(m_T.get_num_nodes()),
	  m_previous_root(m_T.get_num_nodes() + 1)
{
#if defined DEBUG
	assert(m_T.is_tree());
#endif
	if (seed == 0) {
		std::random_device rd;
		m_gen = std::mt19937(rd());
	}
	else {
		m_gen = std::mt19937(seed);
	}

	// initialise m_rdata with the degrees of the tree
	for (node u = 0; u < m_T.get_num_nodes(); ++u) {
		m_rdata[u].resize(m_T.get_degree(u));
	}
}

rand_planar_arrangements::rand_planar_arrangements
(const graphs::rooted_tree& T, uint64_t seed)
noexcept
	: m_T_copy(T.to_free_tree()),
	  m_T(m_T_copy),
	  m_rdata(m_T.get_num_nodes()),
	  m_previous_root(m_T.get_num_nodes() + 1)
{
#if defined DEBUG
	assert(m_T.is_tree());
#endif
	if (seed == 0) {
		std::random_device rd;
		m_gen = std::mt19937(rd());
	}
	else {
		m_gen = std::mt19937(seed);
	}

	// initialise m_rdata with the degrees of the tree
	for (node u = 0; u < m_T.get_num_nodes(); ++u) {
		m_rdata[u].resize(m_T.get_degree(u));
	}
}

template<bool assign_vertices, class GEN>
void make_random_projective(
	const graphs::free_tree& T,
	node parent_u, node u,
	// Its size must be equal to the number of vertices of the tree.
	internal::data_array<std::vector<node>>& data,
	// random number generator
	GEN& gen
)
{
	const neighbourhood& neighs_u = T.get_neighbours(u);

	// fill interval with the root vertex and its children
	auto& inter = data[u];

	if constexpr (assign_vertices) {
		inter[0] = u;
		std::copy_if(
			neighs_u.begin(), neighs_u.end(), inter.begin() + 1,
			[&](node v) -> bool { return v != parent_u; }
		);
	}

	// Choose random positions for the intervals corresponding
	// to the trees rooted at 'u's children, and to the interval
	// corresponding to vertex 'u'.
	std::shuffle(
		inter.begin(),
		inter.end(),
		gen
	);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.
	for (const node& v : neighs_u) {
		if (v != parent_u) {
			make_random_projective<assign_vertices>
				(T, u, v, data, gen);
		}
	}
}

linear_arrangement rand_planar_arrangements::get_arrangement() noexcept {
	std::uniform_int_distribution<node> U(0, m_T.get_num_nodes() - 1);
	const node rand_root = U(m_gen);

	// number of children of 'r' with respect to the tree's root
	const neighbourhood& neighs_root = m_T.get_neighbours(rand_root);

	// intervals corresponding to the root
	auto& root_interval = m_rdata[rand_root];

	// resize m_rdata only when the random root is different
	// from the one chosen (u.a.r.) before
	if (rand_root != m_previous_root) {
		if (m_previous_root >= m_T.get_num_nodes()) {
			// first call to the get_arrangement method
			m_rdata[rand_root].push_back(0);
		}
		else {
			// not the first call
			m_rdata[m_previous_root].pop_back();
			m_rdata[rand_root].push_back(0);
		}

		root_interval[0] = rand_root;
		std::copy(
			neighs_root.begin(),
			neighs_root.end(),
			root_interval.begin() + 1
		);
	}

	// Choose random positions for the intervals corresponding
	// to the trees rooted at 'r's children.
	std::shuffle(
		root_interval.begin() + 1,
		root_interval.end(),
		m_gen
	);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them recursively.
	if (m_previous_root == rand_root) {
		// do-not-assign-vertices branch, only shuffle
		for (const node& u : neighs_root) {
			make_random_projective<false>
				(m_T, rand_root, u, m_rdata, m_gen);
		}
	}
	else {
		// assign-vertices branch, only shuffle
		for (const node& u : neighs_root) {
			make_random_projective<true>
				(m_T, rand_root, u, m_rdata, m_gen);
		}
	}

	m_previous_root = rand_root;

	// construct arrangement
	return internal::make_arrangement_intervals(m_T, rand_root, m_rdata);
}

} // -- namespace generate
} // -- namespace lal
