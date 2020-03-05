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

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Undirected tree class.
 *
 * This class implements an undirected tree. It can be initialised just like
 * class @ref ugraph is.
 *
 * This class offers almost the same features as the @ref ugraph class. There
 * is one exception, however. One of the methods is not allowed to be used,
 * method @ref ugraph::disjoint_union, which has been made private.
 */
class utree : public ugraph, virtual public tree {
	public:
		/// Default constructor.
		utree();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		utree(uint32_t n);
		/**
		 * @brief Constructor from graph
		 * @param t Input graph.
		 * @pre @e t must be a tree.
		 */
		utree(const ugraph& t);

		/**
		 * @brief Adds an edge to the tree.
		 *
		 * This operation checks that the edge added does not produce cycles,
		 * but only in a @e debug compilation of the library.
		 * @param s Valid node index: \f$0 \le s < n\f$.
		 * @param t Valid node index: \f$0 \le t < n\f$.
		 * @param norm Should the graph be normalised?
		 * @pre \f$s \neq t\f$
		 * @pre Edge \f$\{s,t\}\f$ is not part of the graph.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		utree& add_edge(node s, node t, bool norm = true);

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This operation is faster than calling @ref add_edge since the
		 * edges are added in bulk.
		 *
		 * However, unlike function @ref add_edge, this only checks that
		 * the addition of the edges have not produced cycles only after they
		 * have been added.
		 * @param edges The edges to be added.
		 * @param norm Normalise the graph after the insertions.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge.
		 * @pre None of the subsets of the list of edges can produce cycles
		 * when added.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edges.
		 */
		utree& add_edges(const std::vector<edge>& edges, bool norm = true);

		bool can_add_edge(node s, node t) const;
		bool can_add_edges(const std::vector<edge>& edges) const;

		virtual bool is_rooted() const;

	protected:
		/// Initialises memory of @ref utree class.
		virtual void _init(uint32_t n);
		/// Clears the memory used by this undirected tree.
		virtual void _clear();

	private:
		// trees should not have this method
		using ugraph::disjoint_union;
};

} // -- namespace graphs
} // -- namespace lal
