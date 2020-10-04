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
#include <lal/graphs/graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Tree graph class.
 *
 * This is an abstract class for those tree-like graphs. Classes that implement
 * different abstractions of trees and that inherit from this class are:
 * @ref free_tree, @ref rooted_tree.
 *
 * In these classes the addition of edges is constrained so as to ensure that
 * the edges added actually yield trees, i.e., that cycles are never produced.
 * For the sake of efficiency, only debug compilations of the library
 * (compilations where the DEBUG symbol is defined) check that such additions
 * do not produce cycles. In case of doubt, one can query the class using methods
 * @ref can_add_edge or @ref can_add_edges prior to adding one or several edges.
 */
class tree : virtual public graph {
	public:
		tree() = default;
		virtual ~tree() = default;

		/* OPERATORS */

		/* GETTERS */

		/**
		 * @brief Returns whether this graph is an actual tree or not.
		 *
		 * Returns true if the number of edges is one less than the
		 * number of nodes. Note that this would not really be true if the
		 * addition of edges was not constrained. Since it is constrained in a
		 * way that no cycles can be produced (for example, see
		 * @ref free_tree::add_edge, or @ref free_tree::add_edges), then we
		 * only need to check for the number of edges.
		 *
		 * For further characterisations of a tree see \cite Harary1969a
		 * (chapter 4, page 33).
		 */
		bool is_tree() const;

		/// Returns whether this tree is a rooted tree.
		virtual bool is_rooted() const = 0;

		/**
		 * @brief Can this edge be added?
		 *
		 * In a tree, this edge can only be added if it does not produce cycles.
		 * @param s First node of the edge.
		 * @param t Second node of the edge.
		 * @return Returns whether the addition of this new edge can be added
		 * to the tree without producing cycles.
		 */
		bool can_add_edge(node s, node t) const;

		/**
		 * @brief Can these edges be added?
		 *
		 * In a tree, these edges can only be added if their addition to the
		 * tree do not produce cycles.
		 * @param edges List of edges.
		 * @return Returns whether the addition of these new edges can be added
		 * to the tree without producing cycles.
		 */
		bool can_add_edges(const std::vector<edge>& edges) const;

		/**
		 * @brief Amount of nodes in a connected component of the tree.
		 *
		 * When tree has had an edge removed, or when it is not completely
		 * built, i.e., it lack some edges, the resulting graph is clearly
		 * a forest. This function returns the size of the forest node @e u
		 * belongs to.
		 *
		 * In directed trees one has to see this amount as the number of nodes
		 * of the component in the undirected version of the forest.
		 * @param u Input node.
		 * @return Returns the size of the connected component of @e u.
		 */
		uint32_t n_nodes_component(node u) const;

	protected:
		/// The root of every vertex in the union-find data structure
		std::vector<node> m_root_of;
		/**
		 * @brief The size of the connected component that a @e root belongs to.
		 *
		 * Formally, @e m_size_of[v] is the size of the connected component of
		 * a @e root vertex @e v. A vertex @e u is a root vertex if there
		 * exists a vertex @e w such that @ref m_root_of[w] = u.
		 *
		 * In this context, root is within the union-find data structure.
		 */
		std::vector<uint32_t> m_root_size;

	protected:
		/// Initialises memory of @ref tree classes.
		virtual void tree_init(uint32_t n);
		/// Clears the memory used by this rooted tree.
		virtual void tree_clear();

		void extra_work_per_edge_add(node u, node v);
		void extra_work_per_edge_remove(node u, node v);
};

} // -- namespace graphs
} // -- namespace lal
