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
#include <lal/graphs/utree.hpp>
#include <lal/graphs/rtree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Undirected rooted tree class.
 *
 * This class represents a rooted undirected tree.
 *
 * This class can be built from an undirected tree and a chosen node, the
 * root, or by inserting edges one by one.
 */
class urtree : public utree, virtual public rtree {
	public:
		/// Default constructor
		urtree();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		urtree(uint32_t n);
		/**
		 * @brief Constructor with undirected tree and root node.
		 *
		 * Constructs a rooted undirected tree from an undirected tree
		 * and one of its nodes as the root of the rooted tree.
		 *
		 * It simply copies the tree into its own structure and keeps the
		 * root node.
		 * @param t Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 */
		urtree(const utree& t, node r);
		/// Default copy constructor.
		urtree(const urtree&) = default;
		/// Default destructor
		virtual ~urtree();

		/**
		 * @brief Initialiser with undirected tree and root node.
		 *
		 * Constructs a rooted undirected tree from an undirected tree
		 * and one of its nodes as the root of the rooted tree.
		 *
		 * It simply copies the tree into its own structure and keeps the
		 * root node. For this reason, @e t need not be a tree (see @ref is_tree).
		 * Namely, this method admits trees with fewer edges than the number
		 * of vertices minus 1.
		 * @param t Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 */
		void init_rooted(const utree& t, node r);

		bool is_rooted() const;

		std::vector<edge> get_edges_subtree(node r, bool relab) const;

		/**
		 * @brief Retrieve the subtree rooted at node r.
		 * @param r Root of the subtree.
		 * @return Returns a tree containing the vertices of the subtree
		 * rooted at vertex @e r.
		 * @pre This graph is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 */
		urtree get_subtree(node r) const;

		/**
		 * @brief Calculates the number of vertices at every rooted subtree.
		 * @pre The object must be a tree (see @ref is_tree()).
		 * @pre The tree must have a root (see @ref has_root()).
		 * @post Method @ref need_recalc_size_subtrees returns false.
		 */
		void recalc_size_subtrees();

	protected:
		/// Initialises memory of @ref urtree class.
		virtual void _init(uint32_t n);
		/// Clears the memory used by this undirected rooted tree.
		virtual void _clear();
};

} // -- namespace graphs
} // -- namespace lal
