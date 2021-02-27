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
#include <utility>
#include <tuple>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace iterators {

/**
 * @brief Iterator over the set of edges.
 *
 * This class is used to easily iterate over the edges of a graph.
 *
 * This class iterates over the edges of a graph. For undirected graphs, the
 * edge returned is an edge \f$(u,v)\f$ so that the inequality \f$u < v\f$
 * always holds. For directed graphs, this is not always true since the edge
 * returned always has left-to-right orientation.
 *
 * Bear in mind, however, that this class does not modify in any way the
 * graph it is initialised with.
 *
 * The correct usage of this class is
 * @code
 *		E_iterator it(g); // g is a graph
 *		while (it.has_next()) {
 *			it.next();
 *			const edge e = it.get_edge();
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
class E_iterator {
public:
	/**
	 * @brief Constructor
	 * @param g Constant reference to the graph over which we iterate.
	 */
	E_iterator(const GRAPH& g) noexcept : m_G(g) {
		reset();
	}
	/// Default destructor.
	~E_iterator() = default;

	/// Returns true if there are edges left to be iterated over.
	inline bool has_next() const noexcept { return m_exists_next; }

	/// Moves the iterator to the next edge.
	void next() noexcept {
		m_cur_edge = make_current_edge();

		// find the next edge
		std::tie(m_exists_next, m_cur) = find_next_edge();
	}

	/// Returns the current edge.
	inline edge get_edge() const noexcept { return m_cur_edge; }

	/**
	 * @brief Sets the iterator at the beginning of the set of edges.
	 * @post The next call to method @ref next() returns the first edge
	 * of the graph.
	 */
	void reset() {
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

#if defined DEBUG
		if (m_G.num_edges() == 1) {
			assert(m_exists_next == false);
		}
#endif

		// this is the case of the graph having only one edge
		if (not m_exists_next) {
			m_exists_next = true;
		}
	}

private:
	typedef std::pair<node,std::size_t> E_pointer;

private:
	/// The graph whose edges have to be iterated on.
	const GRAPH& m_G;
	/// Pointer to the next edge.
	E_pointer m_cur;
	/// Is there a next edge to iterate over?
	bool m_exists_next = true;
	/// Copy of the current edge.
	edge m_cur_edge;

private:
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
		return edge(s,t);
	}

	/**
	 * @brief Finds the next edge on a directed graph.
	 * @return Returns a pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	template<bool isdir = is_directed, std::enable_if_t<isdir, bool> = true>
	std::pair<bool, E_pointer> find_next_edge() const {
		const uint32_t n = m_G.num_nodes();

		node s = m_cur.first;
		std::size_t pt = m_cur.second;
		bool found = false;

		++pt;
		if (s < n and pt < m_G.out_degree(s)) {
			found = true;
		}
		else {
			pt = 0;
			++s;
			while (s < n and m_G.out_degree(s) == 0) { ++s; }
			found = s < n;
		}
		return make_pair(found, E_pointer(s, pt));
	}
	/**
	 * @brief Finds the next edge on an undirected graph.
	 * @return Returns a pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	template<bool isdir = is_directed, std::enable_if_t<not isdir, bool> = true>
	std::pair<bool, E_pointer> find_next_edge() const {
		const uint32_t n = m_G.num_nodes();

		node s = m_cur.first;
		std::size_t pt = m_cur.second + 1;
		bool found = false;

		while (s < n and not found) {
			const auto& Ns = m_G.get_neighbours(s);
			while (pt < Ns.size() and s > Ns[pt]) { ++pt; }

			found = pt < Ns.size();
			if (not found) {
				++s;
				pt = 0;
			}
		}
		return make_pair(found, E_pointer(s, pt));
	}
};

} // -- namespace iterators
} // -- namespace lal
