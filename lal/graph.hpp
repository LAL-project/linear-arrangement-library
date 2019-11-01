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

// C++ includes
#include <fstream>
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {

/**
 * @brief Abstract class for graphs.
 *
 * Simple class implementing an undirected graph, using the adjacency
 * list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool) ).
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

		/**
		 * @brief Allocate memory for @e n nodes.
		 * @param n Number of nodes.
		 * @post The previous graph structure is cleared. See @ref clear.
		 */
		virtual void init(uint32_t n);

		/* OPERATORS */

		/// Standard output operator.
		inline friend
		std::ostream& operator<< (std::ostream& os, const graph& d) {
			for (uint32_t i = 0; i < d.m_adjacency_list.size(); ++i) {
				os << i << ":";
				lcit begin = d.m_adjacency_list[i].begin();
				lcit end = d.m_adjacency_list[i].end();
				for (lcit it = begin; it != end; ++it) {
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
		 * @brief Disjoint union of graphs.
		 *
		 * Given a graph, append it to the current graph.
		 *
		 * All the vertices in @e g are relabelled starting at @e n,
		 * the number of vertices of the current graph.
		 * @param g Input graph.
		 * @post The graph is normalised only if it was normalised before
		 * the call and @e g is also normalised.
		 */
		void disjoint_union(const graph& g);

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

		/**
		 * @brief Adds an edge to the graph.
		 * @param u Valid node index: \f$0 \le u < n\f$.
		 * @param v Valid node index: \f$0 \le v < n\f$.
		 * @param norm Should the graph be normalised?
		 * @pre \f$u \neq v\f$. The edge \f$\{u,v\}\f$ is not part of the graph.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual graph& add_edge(node u, node v, bool norm = false) = 0;

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This operation is faster than calling @ref add_edge(node,node,bool)
		 * since the edges are added in bulk.
		 * @param edges The edges to be added.
		 * @param norm Normalise the graph after the insertions.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge(node,node,bool).
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual graph& add_edges(const std::vector<edge>& edges, bool norm = true) = 0;

		/**
		 * @brief Deletes all edges and nodes from the graph.
		 * @post The graph is normalised.
		 */
		void clear();

		/* SETTERS */

		/* GETTERS */

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
		 * a directed graph, returns the out-degree of a node.
		 */
		uint32_t degree(node u) const;

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

	protected:
		/// Data structure that implements the graph.
		std::vector<neighbourhood> m_adjacency_list;
		/// Amount of edges of this graph.
		uint32_t m_num_edges = 0;
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
		 * @brief Find node in a neighbourhood list.
		 *
		 * Returns a constant iterator to node u in the neighbourhood n
		 * of some node in the graph.
		 */
		lcit cget_neighbour_position(const neighbourhood& n, node u) const;

		/**
		 * @brief Find node in a neighbourhood list.
		 *
		 * Returns an iterator to node u in the neighbourhood n
		 * of some node in the graph.
		 */
		lit get_neighbour_position(neighbourhood& n, node u);
};

} // -- namespace lal
