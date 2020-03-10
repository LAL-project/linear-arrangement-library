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

// C++ includes
#include <vector>

// lal includes
#include <lal/graphs/tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Rooted tree class.
 *
 * This class provides its users with an abstraction of rooted trees. This
 * class of trees are similar to regular trees but with an specially labelled
 * vertex that represents the root. However, since it cannot be instantiated,
 * users of the library should use the classes @ref urtree and/or @ref drtree.
 *
 * Moreover, the root allows defining further properties on these graphs. For
 * example, the user can query information regarding subtrees of a particular
 * rooted tree (see methods @ref n_nodes_subtree and
 * @ref urtree::recalc_size_subtrees or @ref drtree::recalc_size_subtrees).
 *
 * This class allows flexibility of use of rooted trees regarding root's
 * choice. Method @ref set_root allows changing the root of rooted trees
 * multiple times and at any time. However, any information dependent
 * on the root becomes invalid upon any change of the root. For example, the
 * size of each subtree of this tree is not valid (see attribute
 * @ref m_recalc_size_subtrees and method @ref need_recalc_size_subtrees)
 * until the tree has a root (see @ref has_root).
 */
class rtree : virtual public tree {
	public:
		/// Default constructor
		rtree();
		/// Default destructor
		virtual ~rtree();

		/**
		 * @brief Set the root of this tree.
		 *
		 * Changing the root of a rooted tree invalidates information dependant
		 * on the tree. See the postconditions for details.
		 * @param r Vertex that represents the root.
		 * @post Method @ref has_root returns true.
		 * @post Values in @ref m_size_subtrees are invalidated, i.e., method
		 * @ref need_recalc_size_subtrees returns true.
		 */
		virtual void set_root(node r);

		/* MODIFIERS */

		/* GETTERS */

		/// Return the root of this tree.
		node get_root() const;

		virtual bool is_rooted() const;

		/// Returns whether the root has been set or not.
		bool has_root() const;

		/**
		 * @brief Returns the number of vertices of the subtree rooted at @e u.
		 * @param u Vertex of the tree.
		 * @return Returns @ref m_size_subtrees[u].
		 * @pre Method @ref need_recalc_size_subtrees returns false.
		 */
		uint32_t n_nodes_subtree(node u) const;

		/**
		 * @brief Is a recalculation of the subtree's sizes needed?
		 *
		 * If the method returns true, then call @ref drtree::recalc_size_subtrees
		 * or @ref urtree::recalc_size_subtrees accordingly.
		 * @return Returns whether @ref m_size_subtrees should be recalculated
		 * or not.
		 */
		bool need_recalc_size_subtrees() const;

		/**
		 * @brief Retrieve the edges of the subtree rooted at @e r.
		 *
		 * The list of edges returned contains labels that depend on the parameter
		 * @e relab. If @e relab is true then the vertices are relabelled to
		 * numbers in \f$[0, n_r)\f$, where \f$n_r\f$ is the number of vertices
		 * of the subtree rooted at @e r, rather than keeping the original
		 * labelling of numbers in \f$[0,n)\f$, where @e n is the number of
		 * vertices of the tree.
		 *
		 * In case of directed trees, the subtree is extracted regardless of the
		 * orientation of the edges. For example, consider an anti-arborescence
		 * of a complete binary tree of 7 vertices, whose edges are
		 * <pre>
		 * 0 <- 1
		 *		1 <- 3
		 *		1 <- 4
		 * 0 <- 2
		 *		2 <- 5
		 *		2 <- 6
		 * </pre>
		 * The edges of the subtree rooted at 1 are "3 -> 1" and "4 -> 1".
		 * Moreover, the orientation of the edges is guaranteed to be
		 * first-vertex-to-second-vertex.
		 *
		 * Regardless of the directedness of the graph, this method can be seen
		 * as a way of relabelling vertices when @e r is the root of the tree
		 * and @e relab is true.
		 * @param r Root vertex of the subtree.
		 * @param relab Should the vertices be relabelled?
		 * @return Returns a list of edges.
		 * @pre This graph is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 * @post Whenever @e relab is true, the label of the first vertex of
		 * the first edge is guaranteed to be vertex '0'.
		 */
		virtual std::vector<edge> get_edges_subtree(node r, bool relab = false) const = 0;

	protected:
		/// Root of the tree.
		node m_r;
		/// Is the root set?
		bool m_root_set = false;

		/**
		 * @brief Number of vertices of the subtrees rooted at a certain vertex.
		 *
		 * Given a vertex @e u, @ref m_size_subtrees[u] gives the number
		 * of vertices of the subtree rooted at @e u.
		 */
		std::vector<uint32_t> m_size_subtrees;
		/// Are the contents of @ref m_size_subtrees valid?
		bool m_recalc_size_subtrees = true;

	protected:
		/// Initialises memory of @ref rtree class.
		virtual void tree_init(uint32_t n);
		/// Clears the memory used by this rooted tree.
		virtual void tree_clear();
};

} // -- namespace graphs
} // -- namespace lal
