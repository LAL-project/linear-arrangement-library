/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <vector>

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Abstract class for graphs.
 *
 * Class used as an interface for all types of graphs. This means that this
 * class cannot be instantiated. The classes that can be instantiated are
 * - @ref ugraph : undirected graphs,
 * - @ref dgraph : directed graphs,
 * - @ref ftree : free trees,
 * - @ref rtree : rooted trees.
 *
 * A usual way of initialising classes inheriting from this one is to use one
 * of the @ref init methods available. Depending on the subclass, this method
 * admits either the number of nodes of the graph or a whole other graph and
 * further information (see @ref rtree::init_rooted(const ftree&, node, rtree_type).
 * While these classes' internal memory can be initialised, it can also be cleared
 * using method @ref clear. Each class reimplements this method to carry this
 * task appropriately.
 */
class graph {
	public:
		/// Default constructor.
		graph() = default;
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		graph(uint32_t n);
		/// Default destructor.
		virtual ~graph() = default;

		/* OPERATORS */

		/* MODIFIERS */

		/**
		 * @brief Allocates the necessary memory for this class.
		 *
		 * See @ref _init for details.
		 * @param n Number of nodes.
		 */
		virtual void init(uint32_t n);
		/**
		 * @brief Frees the memory occupied by this graph.
		 *
		 * See @ref _clear for details.
		 * @post The graph is normalised. The number of edges is 0.
		 */
		virtual void clear();

		/**
		 * @brief Normalises the graph.
		 *
		 * Sorts every node's adjacency list in increasing order.
		 *
		 * Besides expensive, this method may be unnecessary. Method
		 * @ref check_normalised() checks in linear time if the graph is
		 * normalised or not.
		 * @post Method @ref is_normalised evaluates to true.
		 */
		virtual void normalise();

		/**
		 * @brief Checks if the graph is normalised.
		 *
		 * Checks, in linear time, that the graph is normalised or not.
		 * In case it is, attribute @ref m_normalised is set to true, so
		 * method @ref is_normalised evaluates to true.
		 */
		virtual bool check_normalised();

		/* SETTERS */

		/* GETTERS */

		/**
		 * @brief Returns the neighbourhood of node @e u.
		 * @param u Node.
		 * @return In undirected graphs, returns the list of nodes adjacent to
		 * node @e u. In a directed graph, returns the outgoing nodes.
		 */
		virtual const neighbourhood& get_neighbours(node u) const = 0;

		/**
		 * @brief Returns the number of neighbours of @e u.
		 * @param u Node to be queried.
		 * @return In undirected graphs, returns the number of neighbours. In
		 * a directed graph, returns the number of outgoing edges.
		 */
		virtual uint32_t degree(node u) const = 0;

		/// Returns true if node @e u is in this graph.
		bool has_node(node u) const;

		/// Returns true if the undirected edge (@e u, @e v) exists in the graph.
		virtual bool has_edge(node u, node v) const = 0;

		/// Returns the number of ndoes.
		uint32_t n_nodes() const;

		/// Returns the number of edges.
		uint32_t n_edges() const;

		/// Returns all edges of this graph.
		std::vector<edge> edges() const;

		/**
		 * @brief Returns whether this graph is normalised or not.
		 *
		 * A graph is normalised if every node's adjacency list is sorted
		 * increasingly. For this, use method @ref normalise().
		 * @return Returns the value of @ref m_normalised.
		 */
		bool is_normalised() const;

		/// Returns whether this graph is directed or not.
		virtual bool is_directed() const = 0;
		/// Returns whether this graph is undirected or not.
		virtual bool is_undirected() const = 0;

	protected:
		/// Data structure that implements the graph.
		std::vector<neighbourhood> m_adjacency_list;
		/// Amount of edges of this graph.
		uint32_t m_num_edges = 0;
		/**
		 * @brief Is this graph normalised?
		 *
		 * An undirected graph is normalised iff every node's adjacency list is
		 * sorted in increasing order.
		 *
		 * In directed graphs, however, it is necessary that the adjacency lists
		 * of the out-neighbours and in-neighbours of nodes be sorted.
		 *
		 * This attribute is set to 'true' in all graph's initialisation
		 * and destruction (when @ref clear() method is called).
		 */
		bool m_normalised = true;

	protected:
		/// Initialises memory of @ref graph class.
		virtual void _init(uint32_t n);
		/// Clears memory for the @ref graph class.
		virtual void _clear();

		/**
		 * @brief Disjoint union of graphs.
		 *
		 * Given a graph, append it to the current graph.
		 *
		 * All the nodes in @e g are relabelled starting at @e n,
		 * the number of nodes of the current graph.
		 * @param g Input graph.
		 * @pre This graph and @e g must be of the same type (both must
		 * be either undirected, or both directed).
		 * @post The graph is normalised only if it was normalised before
		 * the call and @e g is also normalised.
		 */
		void __disjoint_union(const graph& g);

		/**
		 * @brief Returns all independent pairs of edges of this graph.
		 *
		 * The set \f$Q(G)\f$ is defined as the pairs of edges of \f$G\f$,
		 * \f$E(G) \times E(G)\f$, that are independent, that is, that share
		 * no nodes.
		 * @param q The size of \f$Q\f$.
		 */
		std::vector<edge_pair> Q(uint32_t q) const;
};

} // -- namespace graphs
} // -- namespace lal
