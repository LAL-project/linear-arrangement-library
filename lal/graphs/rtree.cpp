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

#include <lal/graphs/rtree.hpp>

// C++ includes
#include <cassert>
using namespace std;

namespace lal {
namespace graphs {

rtree::rtree() { }
rtree::~rtree() { }

void rtree::set_root(node r) {
	// if the tree is empty simply consider it has a root...
	// although it really doesn't

	if (n_nodes() > 0) {
		assert(has_node(r));
		m_r = r;
	}
	m_root_set = true;
	m_recalc_size_subtrees = true;
}

node rtree::get_root() const { return m_r; }

bool rtree::is_rooted() const { return true; }

bool rtree::has_root() const { return m_root_set; }

uint32_t rtree::n_nodes_subtree(node u) const {
	assert(has_node(u));
	return m_size_subtrees[u];
}

bool rtree::need_recalc_size_subtrees() const {
	return m_recalc_size_subtrees;
}

/* PROTECTED */

void rtree::tree_init(uint32_t n) {
	tree::tree_init(n);
	m_size_subtrees = vector<uint32_t>(n);
	m_recalc_size_subtrees = true;
	m_root_set = false;
}

void rtree::tree_clear() {
	tree::tree_clear();
	m_size_subtrees.clear();
	m_recalc_size_subtrees = true;
	m_root_set = false;
}

} // -- namespace graphs
} // -- namespace lal
