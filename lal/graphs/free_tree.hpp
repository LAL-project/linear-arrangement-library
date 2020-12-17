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

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Free tree graph class.
 *
 * This class constrains the addition of edges so that the resulting graphs
 * does not contain cycles. Furthermore, the edges added are undirected.
 *
 * For another type of tree-like graphs, see @ref rooted_tree.
 */
class free_tree : public undirected_graph, virtual public tree {
	public:
		/* CONSTRUCTORS */

		/// Empty constructor.
		free_tree();
		/// Constructor with number of vertices.
		free_tree(uint32_t n);
		/// Copy constructor.
		free_tree(const free_tree&);
		/// Move constructor.
		free_tree(free_tree&&);
		/**
		 * @brief Copy constructor with undirected graph.
		 * @param t An undirected graph.
		 * @pre Graph @e t is a tree.
		 */
		free_tree(const undirected_graph& t);
		/**
		 * @brief Move constructor with undirected graph.
		 * @param t An undirected graph.
		 * @pre Graph @e t is a tree.
		 */
		free_tree(undirected_graph&& t);
		/// Destructor.
		virtual ~free_tree();

		/* OPERATORS */

		/// Copy assignment operator.
		free_tree& operator= (const free_tree&);
		/// Move assignment operator.
		free_tree& operator= (free_tree&&);

		/* MODIFIERS */

		/**
		 * @brief Adds an edge to the tree.
		 *
		 * This operation checks that the edge added does not produce cycles
		 * only in a @e debug compilation of the library. For a more controlled
		 * addition of the edges, see @ref can_add_edge.
		 *
		 * For developers: method @ref graph::extra_work_per_edge_add is
		 * called after the edge has been added.
		 * @param s Valid node index: \f$0 \le s < n\f$.
		 * @param t Valid node index: \f$0 \le t < n\f$.
		 * @param norm Should the graph be normalised?
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre \f$s \neq t\f$
		 * @pre Edge \f$\{s,t\}\f$ is not part of the graph.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		free_tree& add_edge
		(node s, node t, bool norm = false, bool check_norm = true);

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This function checks that edges will not produce cycles only in a
		 * @e debug compilation of the library. Moreover, this operation is
		 * faster than calling @ref add_edge since the edges are added in bulk.
		 * For a more controlled addition of the edges, see @ref can_add_edges.
		 *
		 * For developers: method @ref graph::extra_work_per_edge_add is
		 * called after each edge has been added.
		 * @param edges The edges to be added.
		 * @param norm Normalise the graph after the insertions.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge.
		 * @pre None of the subsets of the list of edges can produce cycles
		 * when added.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edges.
		 */
		free_tree& add_edges
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This list of tedges is assumed to be all the edges that are going
		 * to be added to this graph. This means that the internal data structures
		 * are constructed more efficiently than when adding edges one by one
		 * (see @ref add_edge) or in several chunks (see @ref add_edges).
		 * For a more controlled addition of the edges, see @ref can_add_edges.
		 * @param edges The edges to be added.
		 * @param norm Normalise the graph after the insertions.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised. If @e norm is true then @e check_norm
		 * is ignored.
		 * @pre There are no repeated edges in the list.
		 * @pre The graph is empty prior to the addition.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 */
		free_tree& set_edges
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Disjoint union of trees.
		 *
		 * Given a free tree, append it to the current tree.
		 *
		 * All the nodes in @e t are relabelled starting at @e n,
		 * the number of nodes of the current tree.
		 * @param t Input tree.
		 * @post The current tree is not an actual tree, i.e., method
		 * @ref is_tree() returns false since the resulting tree lacks an edge.
		 */
		void disjoint_union(const free_tree& t);

		/* GETTERS */

		inline bool is_rooted() const { return false; }

	protected:
		/// Initialises memory of @ref free_tree, @ref undirected_graph and
		/// @ref graph classes.
		virtual void _init(uint32_t n);
		/// Clears the memory of @ref free_tree, @ref undirected_graph and
		/// @ref graph classes.
		virtual void _clear();

		/// Copies all members of this class and the parent class.
		void copy_full_free_tree(const free_tree& f);
		/// Moves all members of this class and the parent class.
		void move_full_free_tree(free_tree&& f);

	private:
		using undirected_graph::disjoint_union;
};

} // -- namespace graphs
} // -- namespace lal
