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

#include <lal/graphs/urtree.hpp>

// C++ includes
//#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

namespace lal {
namespace graphs {

urtree::urtree() : utree() { }
urtree::urtree(uint32_t n) {
	utree::_init(n);
	rtree::tree_init(n);
}
urtree::urtree(const utree& t, node r) : utree() {
	init_rooted(t, r);
}
urtree::~urtree() { }

void urtree::init_rooted(const utree& t, node r) {
	_clear();

	rtree::tree_init(t.n_nodes());
	*static_cast<ugraph *>(this) = t;
	set_root(r);
}

bool urtree::is_rooted() const { return true; }

void urtree::calculate_nodes_subtrees() {
	assert(is_tree());
	assert(has_root());

	m_num_verts_subtree_valid = true;
	m_num_verts_subtree = vector<uint32_t>(n_nodes());
	vector<bool> vis(n_nodes(), false);
	calc_nodes_subtree(get_root(), vis);
}

/* PROTECTED */

void urtree::_init(uint32_t n) {
	//cout << "urtree::_init(uint32_t)" << endl;
	rtree::tree_init(n);
	utree::_init(n);
}

void urtree::_clear() {
	//cout << "urtree::_clear()" << endl;
	rtree::_clear();
	utree::_clear();
}

/* PRIVATE */

void urtree::calc_nodes_subtree(node r, vector<bool>& vis) {
	m_num_verts_subtree[r] = 1;
	vis[r] = true;
	for (const node u : get_neighbours(r)) {
		if (not vis[u]) {
			calc_nodes_subtree(u, vis);
			m_num_verts_subtree[r] += m_num_verts_subtree[u];
		}
	}
}

} // -- namespace graphs
} // -- namespace lal
