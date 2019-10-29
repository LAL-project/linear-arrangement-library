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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#include <lal/edge_iterator.hpp>

// C++ includes
#include <iostream>
using namespace std;

inline void advance_base_node(const lal::graph& g, lal::node& u) {
	while (u < g.n_nodes() and g.degree(u) == 0) { ++u; }
}

inline void advance_pointer(const lal::graph& g, lal::node u, size_t& p) {
	auto nb = g.get_neighbours(u);
	while (p < nb.size() and nb[p] < u) { ++p; }
}

namespace lal {

edge_iterator::edge_iterator(const graph& g) : m_G(g) {
	go_to_first_edge();
}
edge_iterator::~edge_iterator() { }

bool edge_iterator::has_next() const {
	return m_exists_next;
}

edge edge_iterator::next() {
	// build the edge that is next in line
	edge edge_to_return(m_u, m_G.get_neighbours(m_u)[m_p]);
	if (m_G.is_directed()) {
		find_next_node_directed();
	}
	else {
		find_next_node_undirected();
	}
	return edge_to_return;
}

/* PRIVATE */

void edge_iterator::go_to_first_edge() {
	m_p = 0;
	// find the first vertex with at least one neighbour
	m_u = 0;
	advance_base_node(m_G, m_u);
	if (m_u < m_G.n_nodes()) {
		m_exists_next = true;
	}
}

void edge_iterator::find_next_node_directed() {
	if (m_p < m_G.degree(m_u) - 1) {
		++m_p;
	}
	else {
		m_p = 0;
		++m_u;
		advance_base_node(m_G, m_u);
	}
	m_exists_next = (m_u != m_G.n_nodes());
}

void edge_iterator::find_next_node_undirected() {
	++m_p;

	bool found_next = false;
	while (not found_next and m_u < m_G.n_nodes()) {

		// place m_p at the first position such that "u < g[u][p]"
		advance_pointer(m_G, m_u, m_p);

		if (m_p < m_G.degree(m_u)) {
			found_next = true;
		}
		else {
			m_p = 0;
			++m_u;
			advance_base_node(m_G, m_u);
		}
	}
	m_exists_next = (m_u != m_G.n_nodes());
}

} // -- namespace lal
