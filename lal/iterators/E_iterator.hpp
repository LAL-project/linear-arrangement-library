/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

// lal includes
#include <lal/graphs/graph.hpp>

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
 *			edge e = it.get_edge();
 *			// ...
 *		}
 * @endcode
 */
class E_iterator {
public:
	/**
	 * @brief Constructor
	 * @param g Constant reference to the graph over which we iterate.
	 */
	E_iterator(const graphs::graph& g);
	/// Destructor.
	~E_iterator();

	/// Returns true if there are edges left to be iterated over.
	bool has_next() const;

	/// Moves the iterator to the next edge.
	void next();

	/// Returns the current edge.
	edge get_edge() const;

	/**
	 * @brief Sets the iterator at the beginning of the set of edges.
	 * @post The next call to method @ref next() returns the first edge
	 * of the graph.
	 */
	void reset();

private:
	typedef std::pair<node,std::size_t> E_pointer;

private:
	/// The graph whose edges have to be iterated on.
	const graphs::graph& m_G;
	/// Pointer to the next edge.
	E_pointer m_cur;
	/// Is there a next edge to iterate over?
	bool m_exists_next = true;
	/// Copy of the current edge.
	edge m_cur_edge;

private:
	/// Returns the edge pointed by @ref m_cur.
	edge make_current_edge() const;

	/**
	 * @brief Finds the next edge.
	 *
	 * Calls @ref find_next_edge_directed() or @ref find_next_edge_undirected().
	 */
	std::pair<bool, E_pointer> find_next_edge() const;
	/**
	 * @brief Finds the next edge on a directed graph.
	 * @return Returns a pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	std::pair<bool, E_pointer> find_next_edge_directed() const;
	/**
	 * @brief Finds the next edge on an undirected graph.
	 * @return Returns a pair of a Boolean indicating if the next edge is
	 * valid, and pointers to the next edge.
	 * @pre Starts at the values in @ref m_cur.
	 */
	std::pair<bool, E_pointer> find_next_edge_undirected() const;
};

} // -- namespace iterators
} // -- namespace lal
