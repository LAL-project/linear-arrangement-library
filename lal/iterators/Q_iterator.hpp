/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <type_traits>
#include <limits>
#include <tuple>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace iterators {

/**
 * @brief Iterator over the set of pairs of independent edges of a graph.
 *
 * This class is used to easily iterate over the elements of the set \f$Q\f$
 * of a graph.
 *
 * This class iterates over the independent pairs of edges of a graph. For
 * undirected graphs, the edges of the pair returned are edges \f$(u,v)\f$
 * so that the inequality \f$u < v\f$ always holds. For directed graphs, this
 * is not always true, since the edges returned always has left-to-right direction.
 *
 * This class has to be initialized with a constant reference to a graph.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::iterators::Q_iterator it(g); // g is a graph
 *		while (not it.end()) {
 *			const auto [e1,e2] = it.get_edge_pair();
 *			// ...
 * 			it.next();
 *		}
 * @endcode
 * or, in a more compact way:
 * @code
 *		lal::iterators::Q_iterator it(g); // g is a graph
 *		while (not it.end()) {
 *			const auto [e1,e2] = it.yield_edge_pair();
 *			// ...
 *		}
 * @endcode
 * Alternatively, the @ref lal::iterators::Q_iterator object can be used in a
 * for loop:
 * @code
 *		for (lal::iterators::Q_iterator it(g); not it.end(); it.next()) {
 *			const auto [e1,e2] = it.get_edge_pair();
 *			// ...
 *		}
 * @endcode
 */
template <
	typename GRAPH,
	bool is_directed = std::is_base_of_v<graphs::directed_graph, GRAPH>,
	std::enable_if_t<
		std::is_base_of_v<graphs::directed_graph, GRAPH> ||
			std::is_base_of_v<graphs::undirected_graph, GRAPH>,
		bool> = true>
class Q_iterator {
public:

	/* CONSTRUCTORS */

	/**
	 * @brief Constructor
	 * @param g Constant reference to the graph over which we iterate.
	 */
	Q_iterator(const GRAPH& g) noexcept
		: m_G(g),
		  m_n(m_G.get_num_nodes())
	{
		reset();
	}

	/// Default destructor.
	~Q_iterator() = default;

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	[[nodiscard]] bool end() const noexcept
	{
		return m_reached_end;
	}

	/// Returns the current edge pair.
	[[nodiscard]] const edge_pair& get_edge_pair() const noexcept
	{
		return m_cur_pair;
	}

	/// Returns the current edge pair.
	[[nodiscard]] edge_pair_t get_edge_pair_t() const noexcept
	{
		return m_cur_pair;
	}

	/// Returns the current edge pair and advances the iterator
	[[nodiscard]] edge_pair yield_edge_pair() noexcept
	{
		const auto e = get_edge_pair();
		next();
		return e;
	}

	/* MODIFIERS */

	/// Moves the iterator to the next pair, if there is any.
	void next() noexcept
	{
		if (not m_exists_next) {
			m_reached_end = true;
			return;
		}

		m_cur_pair = make_current_pair();
#if defined DEBUG
		assert(not share_nodes(
			m_cur_pair.first.first,
			m_cur_pair.first.second,
			m_cur_pair.second.first,
			m_cur_pair.second.second
		));
#endif

		// find the next edge
		auto [found, new_cur1, new_cur2] = find_next_pair(
			m_cur1.first, m_cur1.second, m_cur2.first, m_cur2.second + 1
		);
		m_exists_next = found;
		m_cur1 = new_cur1;
		m_cur2 = new_cur2;
	}

	/// Sets the iterator at the beginning of the set of edges.
	void reset() noexcept
	{
		__reset();
		next();
	}

private:

	typedef std::pair<node, std::size_t> E_pointer;

private:

	/// Graph we are iterating on
	const GRAPH& m_G;
	/// Number of vertices
	const uint64_t m_n;

	/// Current pointers to the first edge.
	E_pointer m_cur1;
	/// Current pointers to the second edge.
	E_pointer m_cur2;

	/// Is there a next pair of independent edges?
	bool m_exists_next;
	/// Has the end of the iteration been reached?
	bool m_reached_end;
	/// Current pair of independent edges.
	edge_pair m_cur_pair;

private:

	/// Sets the iterator at the beginning of the set of edges.
	void __reset() noexcept
	{
		m_exists_next = true;
		m_reached_end = false;

		// there are not enough edges to have |Q| > 0
		if (m_G.get_num_edges() <= 1) {
			m_exists_next = false;
			m_reached_end = true;
			return;
		}

		m_cur1.first = m_cur1.second = 0;
		m_cur2.first = 1;
		m_cur2.second = std::numeric_limits<std::size_t>::max();

		const auto [found, new_cur1, new_cur2] = find_next_pair(
			m_cur1.first, m_cur1.second, m_cur2.first, m_cur2.second + 1
		);

		if (not found) {
			// if we can't find the next pair, then there is no next...
			m_exists_next = false;
			m_reached_end = true;
			return;
		}

#if defined DEBUG
		assert(not share_nodes_pointer(
			new_cur1.first, new_cur1.second, new_cur2.first, new_cur2.second
		));
#endif

		// since a pair was found, store it in current
		m_cur1 = new_cur1;
		m_cur2 = new_cur2;
		m_cur_pair = make_current_pair();

		// how do we know there is a next edge?
		// well, look for it!
		const auto [f2, _, __] = find_next_pair(
			m_cur1.first, m_cur1.second, m_cur2.first, m_cur2.second + 1
		);

		m_exists_next = f2;

		// this is the case of the graph
		// having only one independent pair of edges
		if (not m_exists_next) {
			m_exists_next = true;
			m_reached_end = false;
		}
	}

	/**
	 * @brief Returns the current pair of independent edges.
	 *
	 * These are the edges pointed by attribute @ref m_cur1 and by attribute
	 * @ref m_cur2.
	 */
	[[nodiscard]] edge_pair make_current_pair() const noexcept
	{
		node s, t, u, v;
		s = m_cur1.first;
		u = m_cur2.first;
		if constexpr (is_directed) {
			t = m_G.get_out_neighbors(s)[m_cur1.second];
			v = m_G.get_out_neighbors(u)[m_cur2.second];
		}
		else {
			t = m_G.get_neighbors(s)[m_cur1.second];
			v = m_G.get_neighbors(u)[m_cur2.second];
		}
		return {{s, t}, {u, v}};
	}

	/**
	 * @brief Returns whether two edges share vertices or not.
	 *
	 * The edges are determined in the following manner. The first edge is
	 * determined by node @e s, and the node pointed by @e pt. The second edge
	 * is determined by node @e u, and the node pointed by @e pv.
	 * @param s First node of the first edge.
	 * @param pt Pointer to the second node of the first edge.
	 * @param u First node of the second edge.
	 * @param pv Pointer to the second node of the second edge.
	 * @returns Whether or not the edges share nodes.
	 */
	[[nodiscard]] bool share_nodes_pointer(
		const node s, const std::size_t pt, const node u, const std::size_t pv
	) const noexcept
	{
		node t, v;
		if constexpr (is_directed) {
			t = m_G.get_out_neighbors(s)[pt];
			v = m_G.get_out_neighbors(u)[pv];
		}
		else {
			t = m_G.get_neighbors(s)[pt];
			v = m_G.get_neighbors(u)[pv];
		}
		return share_nodes(s, t, u, v);
	}

	/**
	 * @brief Returns whether two edges share vertices or not.
	 * @param s First node of the first edge.
	 * @param t Second node of the first edge.
	 * @param u First node of the second edge.
	 * @param v Second node of the second edge.
	 * @returns Whether or not the edges share nodes.
	 */
	[[nodiscard]] bool share_nodes(
		const node s, const node t, const node u, const node v
	) const noexcept
	{
		return s == u or s == v or t == u or t == v;
	}

	/// Find the next pair in a directed graph.
	template <bool isdir = is_directed, std::enable_if_t<isdir, bool> = true>
	[[nodiscard]] std::tuple<bool, E_pointer, E_pointer> find_next_pair(
		const node s, const std::size_t pt, const node u, const std::size_t pv
	) noexcept
	{
		// base case 1: consumed all pairs
		if (s == m_n) {
			return {false, {s, pt}, {u, pv}};
		}
		// base case 2: consumed neighbors of 's'
		if (pt >= m_G.get_out_degree(s)) {
			return find_next_pair(s + 1, 0, s + 2, 0);
		}
		// base case 3: consumed second pointer
		if (u == m_n) {
			// advance the first^ pointer
			return find_next_pair(s, pt + 1, s + 1, 0);
		}
		// base case 4: consumed neighbors of 'u'
		if (pv >= m_G.get_out_degree(u)) {
			// advance second pointer
			return find_next_pair(s, pt, u + 1, 0);
		}

		if (share_nodes_pointer(s, pt, u, pv)) {
			return find_next_pair(s, pt, u, pv + 1);
		}

		return {true, {s, pt}, {u, pv}};
	}

	/// Find the next pair in an undirected graph.
	template <
		bool isdir = is_directed,
		std::enable_if_t<not isdir, bool> = true>
	[[nodiscard]] std::tuple<bool, E_pointer, E_pointer> find_next_pair(
		const node s, const std::size_t pt, const node u, const std::size_t pv
	) noexcept
	{
		// base case 1: consumed all pairs
		if (s == m_n) {
			return {false, {s, pt}, {u, pv}};
		}
		// base case 2: consumed neighbors of 's'
		if (pt >= m_G.get_degree(s)) {
			return find_next_pair(s + 1, 0, s + 2, 0);
		}
		// base case 3: consumed second pointer
		if (u == m_n) {
			// advance the first pointer
			return find_next_pair(s, pt + 1, s + 1, 0);
		}
		// base case 4: consumed neighbors of 'u'
		if (pv >= m_G.get_degree(u)) {
			// advance second pointer
			return find_next_pair(s, pt, u + 1, 0);
		}

		const auto& Ns = m_G.get_neighbors(s);
		if (s > Ns[pt]) {
			return find_next_pair(s, pt + 1, u, 0);
		}

		const auto& Nu = m_G.get_neighbors(u);
		if (u > Nu[pv] or share_nodes_pointer(s, pt, u, pv)) {
			return find_next_pair(s, pt, u, pv + 1);
		}

		return {true, {s, pt}, {u, pv}};
	}
};

} // namespace iterators
} // namespace lal
