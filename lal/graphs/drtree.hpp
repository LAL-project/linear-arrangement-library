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
#include <lal/graphs/dtree.hpp>
#include <lal/graphs/utree.hpp>
#include <lal/graphs/rtree.hpp>
#include <lal/graphs/urtree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Directed rooted tree class.
 *
 * This class represents a rooted directed tree (see @ref rtree class for information
 * on rooted trees). The edges can be oriented either outwards or inwards (see
 * @ref drtree_type for details).
 *
 * This class can be built in two different ways:
 * - from an undirected tree and by orienting its edges from a chosen node, i.e.,
 * the root (see @ref init_rooted),
 * - by inserting edges one by one or in bulk (see @ref add_edge and @ref add_edges)..
 *
 * In the latter case, the user is recommended to use method @ref find_drtree_type()
 * in order to determine if the tree is an arborescence, an anti-arborescence,
 * or neither (see @ref drtree_type).
 */
class drtree : public dtree, virtual public rtree {
	public:
		/**
		 * @brief Types of rooted directed trees.
		 *
		 * In this library, we distinguish between three disjoint types of
		 * directed trees:
		 * - Arborescences (see @ref drtree_type::arborescence).
		 * - Anti-arborescences (see @ref drtree_type::anti_arborescence).
		 * - None of the above (see @ref drtree_type::none).
		 */
		enum class drtree_type {
			/**
			 * An arboresence is a rooted directed tree in which all the edges
			 * point away from the root, i.e., towards the leaves.
			 */
			arborescence,
			/**
			 * An anti-arboresence is a rooted directed tree in which all the
			 * edges point towards the root, i.e., away from the leaves.
			 */
			anti_arborescence,
			/**
			 * If the direction of the edges of this tree is not uniform
			 * (it is uniform when they always point towards or outwards the
			 * root), the tree has no particular type.
			 */
			none
		};

	public:
		/// Default constructor.
		drtree();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		drtree(uint32_t n);
		/**
		 * @brief Constructor with undirected tree and root node.
		 *
		 * Constructs a rooted directed tree from an undirected tree and one of
		 * its nodes as the root of the rooted tree.
		 * @param t Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 * @param arb If true then this graph is an arborescence, i.e., the edges
		 * point towards the leaves (away from the root). If false, then this tree
		 * is an anti-arborescence, i.e., the edges point towards the root.
		 * @post Sets the type of directed rooted tree (see @ref m_drtree_type).
		 */
		drtree(const utree& t, node r, drtree_type arb = drtree_type::arborescence);
		/// Default copy constructor.
		drtree(const drtree&) = default;
		/// Destructor
		virtual ~drtree();

		/* MODIFIERS */

		/**
		 * @brief Set the root of this tree.
		 *
		 * Changing the root of a rooted tree invalidates information dependant
		 * on the tree. See the postconditions for details.
		 *
		 * @param r Vertex that represents the root.
		 * @post Method @ref has_root returns true.
		 * @post Values in @ref m_size_subtrees are invalidated. Call method
		 * @ref recalc_size_subtrees to update them.
		 * @post Attribute @ref drtree::m_drtree_type) is invalidated. Call
		 * method @ref drtree::find_drtree_type() to update it.
		 */
		void set_root(node r);

		/**
		 * @brief Initialiser with undirected tree and root node.
		 *
		 * Constructs a rooted directed tree from an undirected tree and one of
		 * its nodes as the root of the rooted tree.
		 *
		 * Since the edges are oriented, method @ref is_tree must be true on
		 * parameter @e t (otherwise, some edges might not be reachable from
		 * the root and hence completely undirectable).
		 * @param t Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 * @param arb The type of directed rooted tree.
		 * @pre Parameter @e arb must be either @ref drtree_type::arborescence
		 * or @ref drtree_type::anti_arborescence.
		 * @post Method @ref has_root returns true.
		 * @post Method @ref is_tree returns true.
		 * @post Method @ref is_tree_type_valid returns true.
		 */
		void init_rooted
		(const utree& t, node r, drtree_type arb = drtree_type::arborescence);

		/**
		 * @brief Calculates the type of directed rooted tree.
		 *
		 * Examines the orientation of the tree with respect to the root and
		 * to the leaves. Then, determines the tree's type (see @ref drtree_type)
		 * according to this orientation.
		 *
		 * @pre This object is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 * @post Method @ref is_tree_type_valid evaluates to true.
		 */
		void find_drtree_type();

		/**
		 * @brief Calculates the number of vertices at every rooted subtree.
		 *
		 * The method can traverse the directed tree using reversed edges, i.e.,
		 * from a root vertex 's' the method can follow out-edges (of the form
		 * s->t) and in-edges (of the form t->s). If parameter @e rev is true
		 * then the method uses both.
		 * @param rev Should reversed edges be used?
		 * @pre The object must be a tree (see @ref is_tree()).
		 * @pre The tree must have a root (see @ref has_root()).
		 * @pre In case @e rev is true, method @ref is_tree_type_valid must
		 * return true and the tree must be an @ref drtree_type::arborescence.
		 * @post Method @ref need_recalc_size_subtrees returns false.
		 */
		void recalc_size_subtrees(bool rev = true);

		/* GETTERS */

		/// Converts this directed rooted tree into an undirected rooted tree.
		urtree to_undirected() const;

		/**
		 * @brief Returns the type of directed rooted tree.
		 *
		 * See @ref drtree_type for details.
		 * @return Returns the type of directed rooted tree.
		 * @pre Method @ref is_tree_type_valid returns true.
		 */
		drtree_type get_drtree_type() const;

		/**
		 * @brief Is the value that returns @ref get_drtree_type valid?
		 *
		 * If the value returned is false, then call @ref find_drtree_type.
		 * @return Returns whether @ref m_drtree_type should be recalculated
		 * or not.
		 */
		bool is_tree_type_valid() const;

		bool is_rooted() const;

		std::vector<edge> get_edges_subtree(node r, bool relab = false) const;

		/**
		 * @brief Retrieve the subtree rooted at node r.
		 * @param r Root of the subtree.
		 * @return Returns a tree containing the vertices of the subtree
		 * rooted at vertex @e r.
		 * @pre This graph is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 * @post The subtree keeps the orientation of the edges in the original
		 * tree.
		 */
		drtree get_subtree(node r) const;

	protected:
		/// Initialises memory of @ref drtree class.
		virtual void _init(uint32_t n);
		/// Clears the memory used by this directed rooted tree.
		virtual void _clear();

	private:
		/**
		 * @brief Type of rooted directed tree.
		 *
		 * This parameter is decided during the construction of the tree via
		 * constructor @ref drtree(const utree&,node,drtree_type), or via calling
		 * method @ref find_drtree_type().
		 */
		drtree_type m_drtree_type = drtree_type::none;
		/// Are the contents of @ref m_drtree_type valid?
		bool m_drtree_type_valid = false;
};

} // -- namespace graphs
} // -- namespace lal
