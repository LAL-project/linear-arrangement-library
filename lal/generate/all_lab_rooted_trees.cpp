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

#include <lal/generate/all_lab_rooted_trees.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

namespace lal {
using namespace graphs;

namespace generate {

all_lab_rooted_trees::all_lab_rooted_trees(uint32_t n) noexcept
	: tree_gen<rooted_tree>(n),
	  m_gen_lab_free_tree(m_n)
{
	init();
}

void all_lab_rooted_trees::next() noexcept {
	if (m_cur_root == m_n - 1) {
		m_cur_root = 0;
		m_gen_lab_free_tree.next();
		m_cur_ftree = m_gen_lab_free_tree.get_tree();
	}
	else {
		++m_cur_root;
	}
}

void all_lab_rooted_trees::reset() noexcept {
	m_cur_root = m_n - 1;
	m_gen_lab_free_tree.reset();
}

/* PROTECTED */

rooted_tree all_lab_rooted_trees::__get_tree() noexcept {
#if defined DEBUG
	assert(m_cur_root < m_n);
#endif
	return rooted_tree(m_cur_ftree, m_cur_root);
}

void all_lab_rooted_trees::init() noexcept {
	reset();

	// deactivate all processing of the free tree
	m_gen_lab_free_tree.calculate_size_subtrees = false;
	m_gen_lab_free_tree.calculate_tree_type = false;
	m_gen_lab_free_tree.normalise_tree = false;
}

} // -- namespace generate
} // -- namespace lal
