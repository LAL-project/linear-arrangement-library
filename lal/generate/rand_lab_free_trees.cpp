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
 
#include <lal/generate/rand_lab_free_trees.hpp>

// C++ includes
using namespace std;

// lal includes
#include <lal/internal/graphs/conversions.hpp>

namespace lal {
using namespace graphs;

namespace generate {

// -----------------------------------------------------------------------------
// ACTUAL GENERATOR

/* PUBLIC */

_rand_lab_free_trees::_rand_lab_free_trees
(uint64_t _n, uint64_t seed) noexcept
	: m_n(_n),
	  m_Prufer_seq(m_n <= 2 ? 2 : m_n - 2)
{
	init(seed);
}

free_tree _rand_lab_free_trees::get_tree() noexcept {
	if (m_n <= 1) { return free_tree(m_n); }
	if (m_n == 2) {
		free_tree t(2);
		t.add_edge(0,1);
		return t;
	}

	for (uint64_t i = 0; i < m_n - 2; ++i) {
		m_Prufer_seq[i] = m_unif(m_gen);
	}
	return internal::Prufer_sequence_to_ftree(m_Prufer_seq.begin(), m_n, false, false);
}

/* PROTECTED */

void _rand_lab_free_trees::init(uint64_t seed) noexcept {
	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}
	m_unif = uniform_int_distribution<uint64_t>(0, m_n - 1);
}

} // -- namespace generate
} // -- namespace lal
