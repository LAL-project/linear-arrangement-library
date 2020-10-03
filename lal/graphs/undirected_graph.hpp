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
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Undirected graph class.
 *
 * Class implementing an undirected graph, using the adjacency list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool, bool) ).
 */
class undirected_graph : virtual public graph {
	public:
		/// Default constructor.
		undirected_graph() = default;
		/// Default move constructor.
		undirected_graph(undirected_graph&&) = default;
		/// Default copy constructor.
		undirected_graph(const undirected_graph&) = default;
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		undirected_graph(uint32_t n);
		/// Default destructor.
		virtual ~undirected_graph() = default;

		/* OPERATORS */

		/// Default move assignment operator.
		undirected_graph& operator= (undirected_graph&&) = default;
		/// Default copy assignment operator.
		undirected_graph& operator= (const undirected_graph&) = default;

		/* MODIFIERS */

		/**
		 * @brief Adds an edge to the graph.
		 * @param s Valid node index: \f$0 \le s < n\f$.
		 * @param t Valid node index: \f$0 \le t < n\f$.
		 * @param norm Should the graph be normalised?
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre \f$u \neq v\f$. The edge \f$\{s,t\}\f$ is not part of the graph.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual undirected_graph& add_edge
		(node s, node t, bool norm = false, bool check_norm = true);

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This operation is faster than calling @ref add_edge(node,node,bool,bool)
		 * since the edges are added in bulk.
		 * @param edges The edges to be added.
		 * @param norm Normalise the graph after the insertions.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge(node,node,bool,bool).
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual undirected_graph& add_edges
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Remove an edge from this graph.
		 * @param s Valid node index: \f$0 \le s < n\f$.
		 * @param t Valid node index: \f$0 \le t < n\f$.
		 * @param norm Normalise the graph after the deletion.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre The edge must exist.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual undirected_graph& remove_edge
		(node s, node t, bool norm = false, bool check_norm = true);

		/**
		 * @brief Remove an edge from this graph.
		 *
		 * This operation is faster than removing edges one by one with
		 * @ref remove_edge(node,node,bool,bool) since the edges are removed in
		 * bulk.
		 * @param edges The edges to be deleted.
		 * @param norm Normalise the graph after the deletion.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge(node,node,bool,bool).
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual undirected_graph& remove_edges
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Disjoint union of graphs.
		 *
		 * Given a graph, append it to the current graph.
		 *
		 * All the nodes in @e g are relabelled starting at @e n,
		 * the number of nodes of the current graph.
		 * @param g Input graph.
		 * @post The graph is normalised only if it was normalised before
		 * the call and @e g is also normalised.
		 */
		void disjoint_union(const undirected_graph& g);

		/* SETTERS */

		/* GETTERS */

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
		 * @param u Node.
		 * @return Returns the list of nodes adjacent to node @e u.
		 */
		const neighbourhood& get_neighbours(node u) const;

		/**
		 * @brief Returns the out-neighbours of node @e u.
		 *
		 * This is the same as @ref get_neighbours.
		 * @param u Node
		 * @return Returns the list of nodes leaving node @e u.
		 */
		const neighbourhood& get_out_neighbours(node u) const;
		/**
		 * @brief Returns the in-neighbours of node @e u.
		 *
		 * This is the same as @ref get_neighbours.
		 * @param u Node
		 * @return Returns the list of nodes entering at node @e u.
		 */
		const neighbourhood& get_in_neighbours(node u) const;

		/**
		 * @brief Returns the number of neighbours of @e u.
		 * @param u Node to be queried.
		 * @return Returns the number of adjacent nodes.
		 */
		uint32_t degree(node u) const;

		/// Same as @ref degree
		uint32_t out_degree(node u) const;
		/// Same as @ref degree
		uint32_t in_degree(node u) const;

		/// Returns true if the edge \f$\{u,v\}\f$ exists in the graph.
		bool has_edge(node u, node v) const;

		bool is_directed() const;
		bool is_undirected() const;

	protected:
		/// Initialises memory of @ref undirected_graph and @ref graph classes.
		virtual void _init(uint32_t n);
		/// Clears the memory of @ref undirected_graph and @ref graph classes.
		virtual void _clear();

	private:
		/**
		 * @brief Removes a single edge.
		 * @param u First node of edge.
		 * @param v Second node of edge.
		 * @param out_u Out-neighbourhood of node @e u.
		 * @param in_v In-neighbourhood of node @e v.
		 */
		void remove_single_edge
		(node u, node v, neighbourhood& out_u, neighbourhood& in_v);
};

} // -- namespace graphs
} // -- namespace lal
