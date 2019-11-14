/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#include <lal/generation/rand_free_lab_trees.hpp>

// C++ includes
using namespace std;

// lal includes
#include <lal/utils/conversions.hpp>

namespace lal {
using namespace graphs;

namespace generate {

rand_free_lab_trees::rand_free_lab_trees() { }
rand_free_lab_trees::rand_free_lab_trees(uint64_t _n, uint64_t seed) {
	init(_n, seed);
}
rand_free_lab_trees::~rand_free_lab_trees() { }

void rand_free_lab_trees::init(uint64_t _n, uint64_t seed) {
	m_n = _n;
	if (m_n <= 2) { return; }

	m_seq = vector<uint64_t>(m_n - 2);

	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}
	m_unif = uniform_int_distribution<uint64_t>(0, m_n - 1);
}

utree rand_free_lab_trees::make_rand_tree() {
	if (m_n <= 1) { return utree(m_n); }
	if (m_n == 2) {
		utree t(2);
		t.add_edge(0,1);
		return t;
	}

	for (uint64_t i = 0; i < m_n - 2; ++i) {
		m_seq[i] = m_unif(m_gen);
	}
	return utils::Prufer_sequence_to_tree(m_seq, m_n);
}

} // -- namespace generate
} // -- namespace lal
