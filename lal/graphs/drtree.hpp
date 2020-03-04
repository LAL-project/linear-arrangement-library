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
 * This class represents a rooted directed tree. The edges can be oriented
 * either outwards or inwards (see @ref drtree_type for details).
 *
 * This class can be built in two different ways:
 * - from an undirected tree and by orienting its edges from a chosen node, i.e.,
 * the root,
 * - by inserting edges one by one.
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
		/// Destructor
		~drtree();

		/* MODIFIERS */

		// This method is re-implemented to change the value
		// of m_drtree_type_valid accordingly.
		void set_root(node r);

		/**
		 * @brief Initialiser with undirected tree and root node.
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
		void init_rooted
		(const utree& t, node r, drtree_type arb = drtree_type::arborescence);

		/**
		 * @brief Calculates the type of directed rooted tree.
		 *
		 * This method should be if the user is interested in knowing this
		 * tree's type (see @ref drtree_type) and method
		 * @ref is_tree_type_valid returns false.
		 * @pre This object is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 */
		void find_drtree_type();

		/* GETTERS */

		/**
		 * @brief Converts this directed rooted tree into an undirected rooted
		 * tree.
		 * @return Returns an object of type undirected tree.
		 */
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
		 * @return Returns whether @ref m_drtree_type is valid or not.
		 */
		bool is_tree_type_valid() const;

		bool is_rooted() const;

		void calculate_nodes_subtrees();

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

	private:
		/**
		 * @brief Calculates the number of vertices of the tree rooted rooted
		 * at @e r on undirected trees.
		 * @param r Root of the subtree.
		 * @post The attribute @ref m_num_verts_subtree[@e r] contains the
		 * number of vertices of the subtree rooted at @e r.
		 */
		void calc_nodes_subtree(node r);

};

} // -- namespace graphs
} // -- namespace lal
