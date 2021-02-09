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

#include <lal/generate/all_lab_free_trees.hpp>

// C++ includes
#include <limits>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>

namespace lal {
using namespace graphs;

namespace generate {

// PUBLIC

all_lab_free_trees::all_lab_free_trees(uint32_t _n) noexcept {
	init(_n);
}
all_lab_free_trees::~all_lab_free_trees() noexcept {
	delete[] m_sm;
	delete[] m_seq;
}

bool all_lab_free_trees::has_next() const {
	if (m_n <= 2) {
		return m_sm[0] != 1;
	}
	return m_sm[m_n - 3] != 1;
}

void all_lab_free_trees::next() {
	if (m_n <= 2) {
		// there is only one tree we can make
		m_sm[0] = true;
		return;
	}

	while (m_it > 0 and m_seq[m_it] == m_n - 1) {
		--m_it;
	}
	++m_seq[m_it];

	/*
	if (m_seq[m_it] == m_n - 1) {
		m_sm[m_it] =
		(m_it == 0) or (m_sm[m_it - 1] and m_seq[m_it - 1] == m_n - 1);
	}
	*/
	m_sm[m_it] =
	(m_seq[m_it] == m_n - 1 ?
		(m_it == 0) or (m_sm[m_it - 1] and m_seq[m_it - 1] == m_n - 1)
		:
		m_sm[m_it]
	);

	++m_it;
	if (m_it < m_n - 2) {
		auto _it = &m_seq[0];
		advance(_it, m_it);
		fill(_it, &m_seq[m_n - 2], 0);
	}
	m_it = m_n - 3;
}

void all_lab_free_trees::reset() {
	if (m_n <= 2) {
		m_sm[0] = 0;
		// there is only one tree we can make
		return;
	}

	m_it = 0;
	std::fill(&m_sm[0], &m_sm[m_n - 2], 0);
	std::fill(&m_seq[0], &m_seq[m_n - 2], 0);
	// place 'it' at the end of the sequence
	m_it = m_n - 3;
	// make sure that the first call to next()
	// produces the sequence 0 0 ... 0
	m_seq[m_it] = numeric_limits<uint32_t>::max();
	m_L = m_n - 2;
}

/* PROTECTED */

free_tree all_lab_free_trees::__get_tree() {
	if (m_n <= 1) { return free_tree(m_n); }
	if (m_n == 2) {
		free_tree t(2);
		t.set_edges(vector<edge>{edge(0,1)});
		return t;
	}
	return internal::Prufer_sequence_to_ftree(m_seq, m_n, false, false);
}

void all_lab_free_trees::init(uint32_t _n) {
	m_n = _n;
	if (m_n <= 2) {
		m_sm = new char[1]{0};
		// there is only one tree we can make
		return;
	}

	m_sm = new char[m_n - 2]{0};
	m_seq = new uint32_t[m_n - 2]{0};
	reset();
}

} // -- namespace generate
} // -- namespace lal

