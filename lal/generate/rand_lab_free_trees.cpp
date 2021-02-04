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
 
#include <lal/generate/rand_lab_free_trees.hpp>

// C++ includes
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>

namespace lal {
using namespace graphs;

namespace generate {

// -----------------------------------------------------------------------------
// ACTUAL GENERATOR

/* PUBLIC */

__rand_lab_free_trees::__rand_lab_free_trees
(uint32_t _n, uint32_t seed) noexcept
{
	init(_n, seed);
}
__rand_lab_free_trees::~__rand_lab_free_trees() noexcept {
	delete[] m_seq;
}

free_tree __rand_lab_free_trees::get_tree() {
	if (m_n <= 1) { return free_tree(m_n); }
	if (m_n == 2) {
		free_tree t(2);
		t.add_edge(0,1);
		return t;
	}

	for (uint32_t i = 0; i < m_n - 2; ++i) {
		m_seq[i] = m_unif(m_gen);
	}
	return internal::Prufer_sequence_to_ftree(m_seq, m_n, false, false);
}

/* PROTECTED */

void __rand_lab_free_trees::init(uint32_t _n, uint32_t seed) {
	m_n = _n;

	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}
	m_unif = uniform_int_distribution<uint32_t>(0, m_n - 1);

	if (m_n <= 2) { return; }

	m_seq = new uint32_t[m_n - 2];
}

// -----------------------------------------------------------------------------
// WRAPPER CLASS

/* PUBLIC */

rand_lab_free_trees::rand_lab_free_trees
(uint32_t n, uint32_t seed) noexcept
: m_Gen(n, seed)
{
}

/* PROTECTED */

graphs::free_tree rand_lab_free_trees::__get_tree() {
	return m_Gen.get_tree();
}

} // -- namespace generate
} // -- namespace lal
