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
#include <lal/graphs/graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Undirected graph class.
 *
 * Simple class implementing an undirected graph, using the adjacency
 * list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool) ).
 */
class ugraph : virtual public graph {
	public:
		/// Default constructor.
		ugraph();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		ugraph(uint32_t n);
		/// Default destructor.
		virtual ~ugraph();

		/* OPERATORS */

		/* MODIFIERS */

		/**
		 * @brief Adds an edge to the graph.
		 * @param s Valid node index: \f$0 \le s < n\f$.
		 * @param t Valid node index: \f$0 \le t < n\f$.
		 * @param norm Should the graph be normalised?
		 * @pre \f$u \neq v\f$. The edge \f$\{s,t\}\f$ is not part of the graph.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		virtual ugraph& add_edge(node s, node t, bool norm = false);

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
		virtual ugraph& add_edges(const std::vector<edge>& edges, bool norm = true);

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
		 * @brief Returns the number of neighbours of @e u.
		 * @param u Node to be queried.
		 * @return Returns the number of adjacent nodes.
		 */
		uint32_t degree(node u) const;

		/// Returns true if the edge \f$\{u,v\}\f$ exists in the graph.
		bool has_edge(node u, node v) const;

		bool is_directed() const;
		bool is_undirected() const;
};

} // -- namespace graphs
} // -- namespace lal
