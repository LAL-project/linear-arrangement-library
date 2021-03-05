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

#include <lal/generate/all_ulab_rooted_trees.hpp>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>

// lal includes

namespace lal {
using namespace graphs;

namespace generate {

/* PUBLIC */

all_ulab_rooted_trees::all_ulab_rooted_trees(uint32_t _n) noexcept
	: tree_gen<rooted_tree>(_n),
	  m_save(m_n + 1),
	  m_prev(m_n + 1),
	  m_L(m_n + 1)
{
	init();
}

/* MODIFIERS */

void all_ulab_rooted_trees::next() noexcept {
	if (m_n <= 2) {
		m_is_last = true;
		return;
	}

	if (m_is_first) {
		m_is_first = false;
		return;
	}

	//m_L[m_p] = m_L[m_p] - 1;
	--m_L[m_p];

	if (m_p < m_n and (m_L[m_p] != 2 or m_L[m_p - 1] != 2)) {
		const uint32_t diff = m_p - m_prev[m_L[m_p]];
		while (m_p < m_n) {
			m_save[m_p] = m_prev[m_L[m_p]];
			m_prev[m_L[m_p]] = m_p;
			++m_p;
			m_L[m_p] = m_L[m_p - diff];
		}
	}
	while (m_L[m_p] == 2) {
		--m_p;
		m_prev[m_L[m_p]] = m_save[m_p];
	}

	m_is_last = (m_p <= 1);
}

void all_ulab_rooted_trees::reset() noexcept {
	m_is_first = true;

	// simplest cases
	if (m_n == 0) {
		m_is_last = true;
		return;
	}

	m_is_last = false;

	m_save.fill(0);
	m_prev.fill(0);
	m_L.fill(0);

	// -------------------
	// generate first tree

	std::iota(m_L.begin(), m_L.end(), 0);
	m_p = m_n;
	if (m_p > 1) {
		for (uint32_t i = 1; i <= m_p - 1; ++i) {
			m_prev[i] = i;
			m_save[i] = 0;
		}
	}
}

/* PROTECTED */

rooted_tree all_ulab_rooted_trees::__get_tree() noexcept {
	if (m_n == 0) {
		return rooted_tree(free_tree(0), 0);
	}
	if (m_n == 1) {
		return rooted_tree(free_tree(1), 0);
	}
	if (m_n == 2) {
		rooted_tree rT(2);
		rT.add_edge(0,1);
		rT.set_root(0);
		rT.set_valid_orientation(true);
		return rT;
	}

	const free_tree t =
		internal::level_sequence_to_ftree(m_L.begin(), m_n, false, false);
	return rooted_tree(t, 0);
}

void all_ulab_rooted_trees::init() noexcept {
	//m_save = new node[m_n + 1];
	//m_prev = new node[m_n + 1];
	//m_L = new node[m_n + 1];

	reset();
}


} // -- namespace generate
} // -- namespace lal
