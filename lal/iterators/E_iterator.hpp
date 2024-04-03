/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <utility>
#include <tuple>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace iterators {

/**
 * @brief Iterator over the set of edges of a graph.
 *
 * This class is used to easily iterate over the edges of a graph \f$E\f$.
 * For undirected graphs, the edge returned is an edge \f$(u,v)\f$ so that the
 * inequality \f$u < v\f$ always holds. For directed graphs, the edge returned
 * always has left-to-right orientation, therefore said inequality need not
 * always hold.
 *
 * This class has to be initialised with a constant reference to a graph.
 *
 * A possible usage of this class is the following:
 * @code
 *		E_iterator it(g); // g is a graph
 *		while (not it.end()) {
 *			const auto [u, v] = it.get_edge();
 *			// ...
 *			it.next();
 *		}
 * @endcode
 * or, in a more compact way
 * @code
 *		E_iterator it(g); // g is a graph
 *		while (not it.end()) {
 *			const auto [u, v] = it.get_edge();
 *			// ...
 *		}
 * @endcode
 * Alternatively, the @ref lal::iterators::E_iterator object can be used in a
 * for loop:
 * @code
 *		for (E_iterator it(g); not it.end(); it.next()) {
 *			const auto [u, v] = it.get_edge();
 *			// ...
 *		}
 * @endcode
 */
template <
	typename graph_t,
	bool is_directed = std::is_base_of_v<graphs::directed_graph, graph_t>,
	std::enable_if_t<std::is_base_of_v<graphs::graph, graph_t>, bool> = true
>
class E_iterator {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor
	 * @param g Constant reference to the graph over which we iterate.
	 */
	E_iterator(const graph_t& g) noexcept
		: m_G(g),
		  m_num_nodes(m_G.get_num_nodes())
	{
		reset();
	}
	/// Default destructor.
	~E_iterator() = default;

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	bool end() const noexcept { return m_reached_end; }

	/// Returns the current edge.
	const edge& get_edge() const noexcept { return m_cur_edge; }

	/// Returns the current edge.
	edge_t get_edge_t() const noexcept { return m_cur_edge; }

	/// Returns the current edge and advances the iterator
	edge yield_edge() noexcept {
		const auto e = get_edge();
		next();
		return e;
	}

	/// Returns the current edge and advances the iterator
	edge_t yield_edge_t() noexcept {
		const auto e = get_edge_t();
		next();
		return e;
	}

	/* MODIFIERS */

	/// Moves the iterator to the next edge.
	void next() noexcept {
		if (not m_exists_next) {
			m_reached_end = true;
			return;
		}

		m_cur_edge = make_current_edge();

		// find the next edge
		std::tie(m_exists_next, m_cur) = find_next_edge();
	}

	/// Sets the iterator at the beginning of the set of edges.
	void reset() noexcept {
		__reset();
		next();
	}

private:
	/// Useful typedef.
	typedef std::pair<node,std::size_t> E_pointer;

private:
	/// The graph whose edges have to be iterated on.
	const graph_t& m_G;
	/// Number of nodes of the graph.
	const uint64_t m_num_nodes;

	/// Pointer to the next edge.
	E_pointer m_cur;
	/// Is there a next edge to iterate over?
	bool m_exists_next = true;
	/// Has the end of the iteration been reached?
	bool m_reached_end = false;
	/// Copy of the current edge.
	edge m_cur_edge;

private:
	/// Sets the iterator at the beginning of the set of edges.
	void __reset() noexcept {
		m_exists_next = true;
		m_reached_end = false;

		m_cur.first = 0;
		m_cur.second = static_cast<std::size_t>(-1);

		auto [found, new_pointer] = find_next_edge();
		if (not found) {
			// if we can't find the next edge, then there is no next...
			m_exists_next = false;
			m_reached_end = true;
			return;
		}

		// since an edge was found, store it in current
		m_cur = new_pointer;
		m_cur_edge = make_current_edge();

		// how do we know there is a next edge?
		// well, find it!
		auto [f2, _] = find_next_edge();
		m_exists_next = f2;

#if defined DEBUG
		if (m_G.get_num_edges() == 1) {
			assert(not m_exists_next);
			assert(not m_reached_end);
		}
#endif

		// this is the case of the graph having only one edge
		if (not m_exists_next) {
			m_exists_next = true;
		}
	}

	/// Returns the edge pointed by @ref m_cur.
	edge make_current_edge() const noexcept {
		node s, t;
		if constexpr (is_directed) {
			s = m_cur.first;
			t = m_G.get_out_neighbours(s)[m_cur.second];
		}
		else {
			s = m_cur.first;
			t = m_G.get_neighbours(s)[m_cur.second];
		}
		return {s,t};
	}

	/**
	 * @brief Finds the next edge on a directed graph.
	 * @returns A pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	template <bool isdir = is_directed, std::enable_if_t<isdir, bool> = true>
	std::pair<bool, E_pointer> find_next_edge() const noexcept {
		node s = m_cur.first;
		std::size_t pt = m_cur.second;
		bool found = false;

		++pt;
		if (s < m_num_nodes and pt < m_G.get_out_degree(s)) {
			found = true;
		}
		else {
			pt = 0;
			++s;
			while (s < m_num_nodes and m_G.get_out_degree(s) == 0) { ++s; }
			found = s < m_num_nodes;
		}
		return {found, E_pointer(s, pt)};
	}
	/**
	 * @brief Finds the next edge on an undirected graph.
	 * @returns A pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	template <bool isdir = is_directed, std::enable_if_t<not isdir, bool> = true>
	std::pair<bool, E_pointer> find_next_edge() const noexcept {
		node s = m_cur.first;
		std::size_t pt = m_cur.second + 1;
		bool found = false;

		while (s < m_num_nodes and not found) {
			const auto& Ns = m_G.get_neighbours(s);
			while (pt < Ns.size() and s > Ns[pt]) { ++pt; }

			found = pt < Ns.size();
			if (not found) {
				++s;
				pt = 0;
			}
		}
		return {found, E_pointer(s, pt)};
	}
};

} // -- namespace iterators
} // -- namespace lal
