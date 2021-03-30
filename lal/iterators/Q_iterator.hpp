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

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <type_traits>
#include <tuple>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace iterators {

/**
 * @brief Iterator over the set \f$Q\f$ of a graph.
 *
 * This class is used to easily iterate over the elements of the set \f$Q\f$
 * of a graph.
 *
 * This class iterates over the independent pairs of edges of a graph. For
 * undirected graphs, the edges of the pair returned are edges \f$(u,v)\f$
 * so that the inequality \f$u < v\f$ always holds. For directed graphs, this
 * is not always true, since the edges returned always has left-to-right direction.
 *
 * Bear in mind, however, that this class does not modify in any way the
 * graph it is initialised with.
 *
 * An example of usage of this class is the following:
 * @code
 *		Q_iterator it(g); // g is a graph
 *		while (it.has_next()) {
 *			it.next();
 *			const lal::edge_pair q = it.get_pair();
 *			// ...
 *		}
 * @endcode
 */
template<
	typename GRAPH,
	bool is_directed = std::is_base_of_v<graphs::directed_graph, GRAPH>,
	std::enable_if_t<
		std::is_base_of_v<graphs::directed_graph, GRAPH> ||
		std::is_base_of_v<graphs::undirected_graph, GRAPH>
		, bool
	> = true
>
class Q_iterator {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor
	 * @param g Constant reference to the graph over which we iterate.
	 */
	Q_iterator(const GRAPH& g) noexcept : m_G(g) {
		reset();
	}

	/// Default destructor.
	~Q_iterator() = default;

	/* GETTERS */

	/// Returns true if there are pairs of independent edges left to be iterated over.
	inline bool has_next() const noexcept { return m_exists_next; }

	/// Returns the current edge pair.
	inline edge_pair get_pair() const noexcept { return m_cur_pair; }

	/* MODIFIERS */

	/// Moves the iterator to the next pair, if there is any.
	void next() noexcept {
		m_cur_pair = make_current_pair();
#if defined DEBUG
		assert(not share_nodes(m_cur_pair));
#endif

		// find the next edge
		auto [found, new_cur1, new_cur2] =
			find_next_pair(
				m_cur1.first, m_cur1.second,
				m_cur2.first, m_cur2.second + 1
			);
		m_exists_next = found;
		m_cur1 = new_cur1;
		m_cur2 = new_cur2;
	}

	/**
	 * @brief Sets the iterator at the beginning of the set of edges.
	 * @post The next call to method @ref next() returns the first edge
	 * of the graph.
	 */
	void reset() noexcept {
		// there are not enough edges to have |Q| > 0
		if (m_G.get_num_edges() <= 1) {
			m_exists_next = false;
			return;
		}

		m_cur1 = E_pointer(0, 0);
		m_cur2 = E_pointer(1, static_cast<size_t>(-1));

		const auto [found, new_cur1, new_cur2] =
			find_next_pair(
				m_cur1.first, m_cur1.second,
				m_cur2.first, m_cur2.second + 1
			);
		if (not found) {
			// if we can't find the next pair, then there is no next...
			m_exists_next = false;
			return;
		}

#if defined DEBUG
		assert(not share_nodes(new_cur1, new_cur2));
#endif

		// since a pair was found, store it in current
		m_cur1 = new_cur1;
		m_cur2 = new_cur2;
		m_cur_pair = make_current_pair();

		// how do we know there is a next edge?
		// well, find it!
		const auto [f2, _, __] =
			find_next_pair(
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

private:
	typedef std::pair<node,std::size_t> E_pointer;

private:

	/// Graph we are iterating on
	const GRAPH& m_G;

	/// Current pointers to the first edge.
	E_pointer m_cur1 = E_pointer(0,0);
	/// Current pointers to the second edge.
	E_pointer m_cur2 = E_pointer(0,0);

	/// Is there a next pair of independent edges?
	bool m_exists_next = true;
	/// Current pair of independent edges.
	edge_pair m_cur_pair = edge_pair(edge(0,0),edge(0,0));

private:
	/**
	 * @brief Returns the current pair of independent edges.
	 *
	 * These are the edges pointed by attribute @ref m_cur1 and by attribute
	 * @ref m_cur2.
	 */
	edge_pair make_current_pair() const noexcept {
		node s,t,u,v;
		if constexpr (is_directed) {
			s = m_cur1.first;
			t = m_G.get_out_neighbours(s)[m_cur1.second];
			u = m_cur2.first;
			v = m_G.get_out_neighbours(u)[m_cur2.second];
		}
		else {
			s = m_cur1.first;
			t = m_G.get_neighbours(s)[m_cur1.second];
			u = m_cur2.first;
			v = m_G.get_neighbours(u)[m_cur2.second];
		}
		return edge_pair(edge(s,t), edge(u,v));
	}

	/// Returns whether the edges share vertices or not.
	static inline bool share_nodes(const edge_pair& st_uv) {
		const auto [s,t] = st_uv.first;
		const auto [u,v] = st_uv.second;
		return s == u or s == v or t == u or t == v;
	}

	/// Returns whether the edges share vertices or not.
	inline
	bool share_nodes(const E_pointer& p1, const E_pointer& p2)
	const noexcept
	{
		node s, t, u, v;
		if constexpr (is_directed) {
			s = p1.first;
			t = m_G.get_out_neighbours(s)[p1.second];
			u = p2.first;
			v = m_G.get_out_neighbours(u)[p2.second];
		}
		else {
			s = p1.first;
			t = m_G.get_neighbours(s)[p1.second];
			u = p2.first;
			v = m_G.get_neighbours(u)[p2.second];
		}
		return share_nodes(edge_pair(edge(s,t),edge(u,v)));
	}

	/// Find the next pair in a directed graph.
	template<bool isdir = is_directed, std::enable_if_t<isdir, bool> = true>
	std::tuple<bool, E_pointer, E_pointer>
	find_next_pair(node s, std::size_t pt, node u, std::size_t pv)
	noexcept
	{
		const uint32_t n = m_G.get_num_nodes();

		// base case 1: consumed all pairs
		if (s == n) {
			return make_tuple(false, E_pointer(s,pt), E_pointer(u,pv));
		}
		// base case 2: consumed neighbours of 's'
		if (pt >= m_G.get_out_degree(s)) {
			return find_next_pair(s+1, 0, s+2, 0);
		}
		// base case 3: consumed second pointer
		if (u == n) {
			// advance the first pointer
			return find_next_pair(s, pt + 1, s + 1, 0);
		}
		// base case 4: consumed neighbours of 'u'
		if (pv >= m_G.get_out_degree(u)) {
			// advance second pointer
			return find_next_pair(s, pt, u + 1, 0);
		}

		if (share_nodes(E_pointer(s,pt), E_pointer(u,pv))) {
			return find_next_pair(s, pt, u, pv + 1);
		}

		return make_tuple(true, E_pointer(s,pt), E_pointer(u,pv));
	}

	/// Find the next pair in an undirected graph.
	template<bool isdir = is_directed, std::enable_if_t<not isdir, bool> = true>
	std::tuple<bool, E_pointer, E_pointer>
	find_next_pair(node s, std::size_t pt, node u, std::size_t pv)
	noexcept
	{
		// FOR GOD'S SAKE! DO NOT USE 'STATIC'!!!
		const uint32_t n = m_G.get_num_nodes();

		// base case 1: consumed all pairs
		if (s == n) {
			return std::make_tuple(false, E_pointer(s,pt), E_pointer(u,pv));
		}
		// base case 2: consumed neighbours of 's'
		if (pt >= m_G.get_degree(s)) {
			return find_next_pair(s+1, 0, s+2, 0);
		}
		// base case 3: consumed second pointer
		if (u == n) {
			// advance the first pointer
			return find_next_pair(s, pt + 1, s + 1, 0);
		}
		// base case 4: consumed neighbours of 'u'
		if (pv >= m_G.get_degree(u)) {
			// advance second pointer
			return find_next_pair(s, pt, u + 1, 0);
		}

		auto Ns = m_G.get_neighbours(s);
		if (s > Ns[pt]) {
			return find_next_pair(s, pt+1, u, 0);
		}

		auto Nu = m_G.get_neighbours(u);
		if (u > Nu[pv] or share_nodes(E_pointer(s,pt), E_pointer(u,pv))) {
			return find_next_pair(s, pt, u, pv + 1);
		}

		return make_tuple(true, E_pointer(s,pt), E_pointer(u,pv));
	}

};

} // -- namespace iterators
} // -- namespace lal
