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

#include <lal/iterators/edge_iterator.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

// lal includes
#include <lal/graphs/dgraph.hpp>
#include <lal/utils/macros.hpp>

namespace lal {
using namespace graphs;

namespace iterators {

typedef edge_iterator::E_pointer E_pointer;

edge_iterator::edge_iterator(const graph& g) : m_G(g) {
	reset();
}
edge_iterator::~edge_iterator() { }

bool edge_iterator::has_next() const {
	return m_exists_next;
}

void edge_iterator::next() {
	m_cur_edge = make_current_edge();

	// find the next edge
	auto _p = find_next_edge();
	m_exists_next = _p.first;
	m_cur = _p.second;
}

edge edge_iterator::get_edge() const {
	return m_cur_edge;
}

void edge_iterator::reset() {
	m_exists_next = true;

	m_cur.first = 0;
	m_cur.second = static_cast<size_t>(-1);

	auto [found, new_pointer] = find_next_edge();
	if (not found) {
		// if we can't find the next edge, then there is no next...
		m_exists_next = false;
		return;
	}

	// since an edge was found, store it in current
	m_cur = new_pointer;
	m_cur_edge = make_current_edge();

	// how do we know there is a next edge?
	// well, find it!
	auto [f2, _] = find_next_edge();
	m_exists_next = f2;

#ifdef DEBUG
	if (m_G.n_edges() == 1) {
		assert(m_exists_next == false);
	}
#endif

	// this is the case of the graph having only one edge
	if (not m_exists_next) {
		m_exists_next = true;
	}
}

/* PRIVATE */

edge edge_iterator::make_current_edge() const {
	const node s = m_cur.first;
	const node t = m_G.get_neighbours(s)[m_cur.second];
	return edge(s,t);
}

pair<bool, E_pointer> edge_iterator::find_next_edge() const {
	return (
		m_G.is_directed() ?
			find_next_edge_directed() :
			find_next_edge_undirected()
	);
}

pair<bool, E_pointer> edge_iterator::find_next_edge_directed() const {
	const uint64_t n = m_G.n_nodes();

	node s = m_cur.first;
	size_t pt = m_cur.second;
	bool found = false;

	const dgraph& dG = dynamic_cast<const dgraph&>(m_G);

	++pt;
	if (s < n and pt < dG.out_degree(s)) {
		found = true;
	}
	else {
		pt = 0;
		++s;
		while (s < n and dG.out_degree(s) == 0) { ++s; }
		found = s < n;
	}
	return make_pair(found, E_pointer(s, pt));
}

pair<bool, E_pointer> edge_iterator::find_next_edge_undirected() const {
	const uint64_t n = m_G.n_nodes();

	node s = m_cur.first;
	size_t pt = m_cur.second + 1;
	bool found = false;

	while (s < n and not found) {
		auto Ns = m_G.get_neighbours(s);
		while (pt < Ns.size() and s > Ns[pt]) { ++pt; }

		found = pt < Ns.size();
		if (not found) {
			++s;
			pt = 0;
		}
	}
	return make_pair(found, E_pointer(s, pt));
}

} // -- namespace iterators
} // -- namespace lal
