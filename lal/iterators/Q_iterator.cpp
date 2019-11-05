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

#include <lal/iterators/Q_iterator.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

namespace lal {
using namespace graphs;

namespace iterators {

/* LOCAL FUNCTIONS */

typedef Q_iterator::E_pointer E_pointer;

static inline
bool share_vertices(const edge_pair& st_uv) {
	const edge& st = st_uv.first;
	const edge& uv = st_uv.second;
	const node s = st.first;
	const node t = st.second;
	const node u = uv.first;
	const node v = uv.second;
	return s == u or s == v or t == u or t == v;
}

static inline
bool share_vertices(const graph& g, const E_pointer& p1, const E_pointer& p2) {
	const node s = p1.first;
	const node t = g.get_neighbours(s)[p1.second];
	const node u = p2.first;
	const node v = g.get_neighbours(u)[p2.second];
	return share_vertices(edge_pair(edge(s,t),edge(u,v)));
}

static tuple<bool, E_pointer, E_pointer>
find_next_pair_directed(
	const graph& g, node s, size_t pt, node u, size_t pv
)
{
	// FOR GOD'S SAKE! DO NOT USE 'STATIC'!!!
	const uint32_t n = g.n_nodes();

	// base case 1: consumed all pairs
	if (s == n) {
		return make_tuple(false, E_pointer(s,pt), E_pointer(u,pv));
	}
	// base case 2: consumed neighbours of 's'
	if (pt >= g.degree(s)) {
		return find_next_pair_directed(g, s+1, 0, s+2, 0);
	}
	// base case 3: consumed second pointer
	if (u == n) {
		// advance the first pointer
		return find_next_pair_directed(g, s, pt + 1, s + 1, 0);
	}
	// base case 4: consumed neighbours of 'u'
	if (pv >= g.degree(u)) {
		// advance second pointer
		return find_next_pair_directed(g, s, pt, u + 1, 0);
	}

	if (share_vertices(g, E_pointer(s,pt), E_pointer(u,pv))) {
		return find_next_pair_directed(g, s, pt, u, pv + 1);
	}

	return make_tuple(true, E_pointer(s,pt), E_pointer(u,pv));
}

static tuple<bool, E_pointer, E_pointer>
find_next_pair_undirected(
	const graph& g, node s, size_t pt, node u, size_t pv
)
{
	// FOR GOD'S SAKE! DO NOT USE 'STATIC'!!!
	const uint32_t n = g.n_nodes();

	// base case 1: consumed all pairs
	if (s == n) {
		return make_tuple(false, E_pointer(s,pt), E_pointer(u,pv));
	}
	// base case 2: consumed neighbours of 's'
	if (pt >= g.degree(s)) {
		return find_next_pair_undirected(g, s+1, 0, s+2, 0);
	}
	// base case 3: consumed second pointer
	if (u == n) {
		// advance the first pointer
		return find_next_pair_undirected(g, s, pt + 1, s + 1, 0);
	}
	// base case 4: consumed neighbours of 'u'
	if (pv >= g.degree(u)) {
		// advance second pointer
		return find_next_pair_undirected(g, s, pt, u + 1, 0);
	}

	auto Ns = g.get_neighbours(s);
	if (s > Ns[pt]) {
		return find_next_pair_undirected(g, s, pt+1, u, 0);
	}

	auto Nu = g.get_neighbours(u);
	if (u > Nu[pv] or share_vertices(g, E_pointer(s,pt), E_pointer(u,pv))) {
		return find_next_pair_undirected(g, s, pt, u, pv + 1);
	}

	return make_tuple(true, E_pointer(s,pt), E_pointer(u,pv));
}

static tuple<bool, E_pointer, E_pointer>
find_next_pair(const graph& g, node s, size_t pt, node u, size_t pv)
{
	return (
		g.is_directed() ?
			find_next_pair_directed(g, s,pt,u,pv) :
			find_next_pair_undirected(g, s,pt,u,pv)
	);
}

/* Q_ITERATOR */

Q_iterator::Q_iterator(const graph& g) : m_G(g) {
	reset();
}
Q_iterator::~Q_iterator() { }

bool Q_iterator::has_next() const {
	return m_exists_next;
}

void Q_iterator::next() {
	m_cur_pair = make_current_pair();
	assert(not share_vertices(m_cur_pair));

	// find the next edge
	auto [found, new_cur1, new_cur2] =
		find_next_pair(
			m_G,
			m_cur1.first, m_cur1.second,
			m_cur2.first, m_cur2.second + 1
		);
	m_exists_next = found;
	m_cur1 = new_cur1;
	m_cur2 = new_cur2;
}

edge_pair Q_iterator::get_pair() const {
	return m_cur_pair;
}

void Q_iterator::reset() {
	// there are not enough edges to have |Q| > 0
	if (m_G.n_edges() <= 1) {
		m_exists_next = false;
		return;
	}

	m_cur1 = E_pointer(0, 0);
	m_cur2 = E_pointer(1, static_cast<size_t>(-1));

	auto [found, new_cur1, new_cur2] =
		find_next_pair(
			m_G,
			m_cur1.first, m_cur1.second,
			m_cur2.first, m_cur2.second + 1
		);
	if (not found) {
		// if we can't find the next pair, then there is no next...
		m_exists_next = false;
		return;
	}

	assert(not share_vertices(m_G, new_cur1, new_cur2));

	// since a pair was found, store it in current
	m_cur1 = new_cur1;
	m_cur2 = new_cur2;
	m_cur_pair = make_current_pair();

	// how do we know there is a next edge?
	// well, find it!
	auto [f2, _, __] =
		find_next_pair(
			m_G,
			m_cur1.first, m_cur1.second,
			m_cur2.first, m_cur2.second + 1
		);

	m_exists_next = f2;

	// this is the case of the graph
	// having only one independent pair of edges
	if (not m_exists_next) {
		m_exists_next = true;
	}
}

/* PRIVATE */

edge_pair Q_iterator::make_current_pair() const {
	const node s = m_cur1.first;
	const node t = m_G.get_neighbours(s)[m_cur1.second];
	const node u = m_cur2.first;
	const node v = m_G.get_neighbours(u)[m_cur2.second];
	return edge_pair(edge(s,t), edge(u,v));
}

} // -- namespace iterators
} // -- namespace lal
