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

#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/graphs/ugraph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Directed graph class.
 *
 * Simple class implementing a directed graph, using the adjacency
 * list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool) ).
 */
class dgraph : public graph {
	public:
		/// Default constructor.
		dgraph();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		dgraph(uint32_t n);
		/// Default destructor.
		virtual ~dgraph();

		/* OPERATORS */

		/* MODIFIERS */

		/**
		 * @brief Disjoint union of graphs.
		 *
		 * Given a graph, append it to the current graph.
		 *
		 * All the vertices in @e g are relabelled starting at @e n,
		 * the number of vertices of the current graph.
		 * @param g Input graph.
		 * @pre Graph @e g must be directed.
		 * @post The graph is normalised only if it was normalised before
		 * the call and @e g is also normalised.
		 */
		void disjoint_union(const graph& g);

		/**
		 * @brief Adds a directed edge.
		 *
		 * For more details see @ref graph::add_edge(node,node,bool)
		 */
		dgraph& add_edge(node u, node v, bool norm = false);

		/**
		 * @brief Adds a list of directed edges.
		 *
		 * For more details see @ref graph::add_edges(const std::vector<edge>&,bool)
		 */
		dgraph& add_edges(const std::vector<edge>& edges, bool norm = true);

		/* SETTERS */

		/* GETTERS */

		/// Returns true if the undirected edge (@e u, @e v) exists in the graph.
		bool has_edge(node u, node v) const;

		bool is_directed() const;
		bool is_undirected() const;

		/// Returns the in-degree of a node.
		uint32_t in_degree(node u) const;

		/**
		 * @brief Converts this directed graph into an undirected graph.
		 * @return Returns an object of typ undirected graph.
		 */
		ugraph to_undirected() const;

		/**
		 * @brief Deletes all edges and nodes from the graph.
		 *
		 * Frees the memory occupied by this graph. For more details, see
		 * @ref graph::clear().
		 */
		void clear();

	protected:
		/// In-degree per vertex.
		std::vector<uint32_t> m_in_degree;

	protected:
		/**
		 * @brief Initialises the memory of the @ref dgraph class.
		 * @param n Number of nodes.
		 */
		void _init(uint32_t n);
		/// Clears the memory of the @ref dgraph class.
		void _clear();
};

} // -- namespace graphs
} // -- namespace lal
