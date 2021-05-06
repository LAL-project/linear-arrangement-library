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

#include <lal/generate/all_lab_free_trees.hpp>

// C++ includes
#include <limits>
using namespace std;

// lal includes
#include <lal/internal/graphs/convert_to_ftree.hpp>

namespace lal {
using namespace graphs;

namespace generate {

/* PUBLIC */

/* CONSTRUCTORS */

all_lab_free_trees::all_lab_free_trees(uint32_t _n) noexcept
	: _tree_generator<free_tree>(_n),
	  m_Prufer_seq(m_n <= 2 ? 1 : m_n - 2, 0),
	  m_sm(m_n <= 2 ? 1 : m_n - 2, 0)
{
	reset();
}

/* MODIFIERS */

void all_lab_free_trees::next() noexcept {
	if (not has_next() or m_reached_end) {
		m_reached_end = true;
		return;
	}

	if (m_n <= 2) {
		// there is only one tree we can make
		m_sm[0] = true;
		return;
	}

	while (m_it > 0 and m_Prufer_seq[m_it] == m_n - 1) {
		--m_it;
	}
	++m_Prufer_seq[m_it];

	m_sm[m_it] =
	(m_Prufer_seq[m_it] == m_n - 1 ?
		(m_it == 0) or (m_sm[m_it - 1] and m_Prufer_seq[m_it - 1] == m_n - 1)
		:
		m_sm[m_it]
	);

	++m_it;
	if (m_it < m_Prufer_seq.size()) {
		std::fill(m_Prufer_seq.begin() + m_it, m_Prufer_seq.end(), 0);
	}
	m_it = m_n - 3;
}

/* PROTECTED */

free_tree all_lab_free_trees::__get_tree() noexcept {
	if (m_n <= 1) { return free_tree(m_n); }
	if (m_n == 2) {
		free_tree t(2);
		t.set_edges(vector<edge>{edge(0,1)});
		return t;
	}
	return internal::Prufer_sequence_to_ftree(m_Prufer_seq.begin(), m_n, false, false);
}

void all_lab_free_trees::__reset() noexcept {
	m_reached_end = false;

	if (m_n <= 2) {
		m_sm[0] = false;
		// there is only one tree we can make
		return;
	}

	m_it = 0;
	m_sm.fill(false);
	m_Prufer_seq.fill(0);
	// place 'it' at the end of the sequence
	m_it = m_n - 3;
	// make sure that the first call to next()
	// produces the sequence 0 0 ... 0
	m_Prufer_seq[m_it] = numeric_limits<uint32_t>::max();
	m_L = m_n - 2;
}

} // -- namespace generate
} // -- namespace lal

