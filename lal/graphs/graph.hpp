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
#include <fstream>
#include <vector>

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Abstract class for graphs.
 *
 * Simple class implementing an undirected graph, using the adjacency
 * list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init method. Edges can then be added one by one
 * by calling its subclass' methods @e add_edge or @e add_edges methods.
 *
 * In most classes of graphs it is allowed to use the @ref graph::disjoint_union
 * operation, which will join two graphs into a single one.
 */
class graph {
	public:
		/// Default constructor.
		graph();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		graph(uint32_t n);
		/// Default destructor.
		virtual ~graph();

		/* OPERATORS */

		/// Standard output operator.
		inline friend
		std::ostream& operator<< (std::ostream& os, const graph& d) {
			for (uint32_t i = 0; i < d.m_adjacency_list.size(); ++i) {
				os << i << ":";
				auto begin = d.m_adjacency_list[i].begin();
				auto end = d.m_adjacency_list[i].end();
				for (auto it = begin; it != end; ++it) {
					os << " " << *it;
				}
				if (i < d.m_adjacency_list.size() - 1) {
					os << std::endl;
				}
			}
			return os;
		}

		/* MODIFIERS */

		/**
		 * @brief Allocate memory for @e n nodes.
		 *
		 * Calls @ref clear and @ref _init(uint32_t).
		 * @param n Number of nodes.
		 * @post The previous graph structure is cleared. See @ref clear.
		 */
		void init(uint32_t n);
		/**
		 * @brief Deletes all edges and nodes from the graph.
		 *
		 * Frees the memory occupied by this graph.
		 * @post The graph is normalised. The number of edges is 0.
		 */
		void clear();

		/**
		 * @brief Disjoint union of graphs.
		 *
		 * Given a graph, append it to the current graph.
		 *
		 * All the vertices in @e g are relabelled starting at @e n,
		 * the number of vertices of the current graph.
		 * @param g Input graph.
		 * @pre This graph and @e g must be of the same type (both must
		 * be either undirected, or both directed).
		 * @post The graph is normalised only if it was normalised before
		 * the call and @e g is also normalised.
		 */
		virtual void disjoint_union(const graph& g);

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
		void normalise();

		/**
		 * @brief Checks if the graph is normalised.
		 *
		 * Checks, in linear time, that the graph is normalised or not.
		 * In case it is, attribute @ref m_normalised is set to true, so
		 * method @ref is_normalised evaluates to true.
		 */
		bool check_normalised();

		/* SETTERS */

		/* GETTERS */

		/// Returns true if node @e u is in this graph.
		bool has_node(node u) const;

		/// Returns true if the undirected edge (@e u, @e v) exists in the graph.
		virtual bool has_edge(node u, node v) const = 0;

		/// Returns the number of ndoes.
		uint32_t n_nodes() const;

		/// Returns the number of edges.
		uint64_t n_edges() const;

		/// Returns all edges of this graph.
		std::vector<edge> edges() const;

		/**
		 * @brief Returns all independent pairs of edges of this graph.
		 *
		 * The set \f$Q(G)\f$ is defined as the pairs of edges of \f$G\f$,
		 * \f$E(G) \times E(G)\f$, that are independent, that is, that share
		 * no nodes.
		 */
		std::vector<edge_pair> Q() const;

		/**
		 * @brief Returns the neighbourhood of node @e u.
		 * @param u Node whose neighbourhood is to be returned.
		 * @return Returns the list of nodes adjacent to node @e u.
		 */
		const neighbourhood& get_neighbours(node u) const;

		/**
		 * @brief Returns the neighbourhood of node @e u.
		 * @param u Node whose neighbourhood is to be returned.
		 * @return Returns the list of nodes adjacent to node @e u as a list
		 * of Boolean values.
		 * @pre This vector must have size the number of nodes of this graph.
		 */
		neighbourhood_B get_bool_neighbours(node u) const;

		/**
		 * @brief Returns the number of neighbours of @e u.
		 * @param u Node to be queried.
		 * @return In undirected graphs, returns the number of neighbours. In
		 * a directed graph, returns the number of outgoing and ingoing edges.
		 */
		virtual uint32_t degree(node u) const;

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

		/**
		 * @brief Constructs the adjacency matrix of the graph.
		 * @param[out] mat The adjacency matrix of the graph. @e mat[i][j]
		 * is true if, and only if, there exists an edge between @e i and @e j.
		 */
		void get_adjacency_matrix(std::vector<std::vector<bool> >& mat) const;

	public:
		typedef node graph_node_type;

	protected:
		/// Data structure that implements the graph.
		std::vector<neighbourhood> m_adjacency_list;
		/// Amount of edges of this graph.
		uint64_t m_num_edges = 0;
		/**
		 * @brief Is this graph normalised?
		 *
		 * A graph is normalised iff every node's adjacency list is sorted in
		 * increasing order. It is set to 'true' in its initialisation and
		 * destruction (when @ref clear() method is called).
		 */
		bool m_normalised = true;

	protected:
		/**
		 * @brief Initialises memory for the @ref graph class only.
		 * @param n Number of nodes.
		 */
		virtual void _init(uint32_t n);
		/// Clears memory for the @ref graph class only.
		virtual void _clear();
};

} // -- namespace graphs
} // -- namespace lal
