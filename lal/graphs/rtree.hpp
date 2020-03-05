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
 * example, the user can query information regarding subtrees of these trees
 * (see methods @ref n_nodes_subtree and @ref calculate_nodes_subtrees).
 *
 * This class allows flexibility of use of rooted trees regarding root's
 * choice. Method @ref set_root allows setting changing the root of rooted
 * trees multiple times and at any time. However, any information dependant
 * on the root becomes invalid upon any change of the root. For example, the
 * size of each subtree of this tree is not valid (see @ref n_nodes_subtree_valid)
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
		 *
		 * @param r Vertex that represents the root.
		 * @post Method @ref has_root returns true.
		 * @post Values in @ref m_num_verts_subtree are invalidated. Call method
		 * @ref calculate_nodes_subtrees to update them.
		 */
		virtual void set_root(node r);

		/* MODIFIERS */

		/**
		 * @brief Calculates the number of vertices at every rooted subtree.
		 * @pre The object must be a tree (see @ref is_tree()).
		 * @pre The tree must have a root (see @ref has_root()).
		 * @post Method @ref n_nodes_subtree_valid returns true.
		 */
		virtual void calculate_nodes_subtrees() = 0;

		/* GETTERS */

		/// Return the root of this tree.
		node get_root() const;

		virtual bool is_rooted() const;

		/// Returns whether the root has been set or not.
		bool has_root() const;

		/**
		 * @brief Returns the number of vertices of the subtree rooted at @e u.
		 * @param u Vertex of the tree.
		 * @return Returns @ref m_num_verts_subtree[u]
		 * @pre Method @ref n_nodes_subtree_valid returns true.
		 */
		uint32_t n_nodes_subtree(node u) const;

		/**
		 * @brief Are the values that returns @ref n_nodes_subtree valid?
		 *
		 * If the value returned is false, then call @ref calculate_nodes_subtrees.
		 * @return Returns whether @ref m_num_verts_subtree is valid or not.
		 */
		bool n_nodes_subtree_valid() const;

	protected:
		/// Root of the tree.
		node m_r;
		/// Is the root set?
		bool m_root_set = false;

		/**
		 * @brief Number of vertices of the subtrees rooted at a certain vertex.
		 *
		 * Given a vertex @e u, @ref m_num_verts_subtree[u] gives the number
		 * of vertices of the subtree rooted at @e u.
		 *
		 * This attribute is only valid for @ref urtree and for @ref drtree
		 * that are arborescences (see @ref drtree::drtree_type::arborescence).
		 */
		std::vector<uint32_t> m_num_verts_subtree;
		/// Are the contents of @ref m_num_verts_subtree valid?
		bool m_num_verts_subtree_valid = false;

	protected:
		/// Initialises memory of @ref rtree class.
		virtual void tree_init(uint32_t n);
		/// Clears the memory used by this rooted tree.
		virtual void tree_clear();
};

} // -- namespace graphs
} // -- namespace lal
