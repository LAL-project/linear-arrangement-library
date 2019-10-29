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

#pragma once

// lal includes
#include <lal/graph.hpp>

namespace lal {

/**
 * @brief Edge iterator class.
 *
 * This class is used to easily iterate over the edges of a graph.
 *
 * This class iterates over the edges of a graph in increasing lexicographic
 * order. For undirected graphs, the edge returned is an edge (u,v) so that
 * the inequality \f$u < v\f$ always holds. For directed graphs, this is not
 * always true. However, the edge returned always has left-to-right direction.
 *
 * Bear in mind, however, that the graph whose edges are being iterated on
 * cannot possibly be modified in any way through this class.
 */
class edge_iterator {
	public:
		/**
		 * @brief Constructor
		 * @param g Constant reference to the graph over which we iterate.
		 */
		edge_iterator(const graph& g);
		/// Destructor.
		~edge_iterator();

		/// Returns true if there are edges left to be iterated over.
		bool has_next() const;

		/// Returns the next edge.
		edge next();

	private:
		/// The graph whose edges have to be iterated on.
		const graph& m_G;

		/// Current vertex
		node m_u = 0;
		/// Position within node @ref m_u's neighbourhood
		size_t m_p = 0;
		/// Is there a next edge to iterate over?
		bool m_exists_next = false;

	private:
		/**
		 * @brief Finds the first edge of the graph
		 *
		 * Places the pointers @ref m_u and @ref m_p so that @ref next()
		 * returns the first edge.
		 */
		void go_to_first_edge();

		/**
		 * @brief Finds the next node to iterate on.
		 *
		 * Places the pointers @ref m_u and @ref m_p so that @ref next()
		 * returns the next edge in line, of a directed graph.
		 */
		void find_next_node_directed();
		/**
		 * @brief Finds the next node to iterate on.
		 *
		 * Places the pointers @ref m_u and @ref m_p so that @ref next()
		 * returns the next edge in line, of a directed graph.
		 */
		void find_next_node_undirected();
};

} // -- namespace lal
