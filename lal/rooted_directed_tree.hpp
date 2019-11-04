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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/dgraph.hpp>
#include <lal/ugraph.hpp>

namespace lal {

/**
 * @brief Rooted directed tree class.
 *
 * This class represents a rooted directed tree. The edges should be oriented
 * from the root to the leaves.
 *
 * This class can be built from an undirected tree by orienting its edges from
 * a chosen node. This node represents the root of the directed rooted tree.
 */
class rooted_directed_tree : public dgraph {
	public:
		/// Default constructor.
		rooted_directed_tree();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		rooted_directed_tree(uint32_t n);
		/**
		 * @brief Constructor with undirected tree and root node.
		 *
		 * Constructs a rooted directed tree from an undirected tree and one of
		 * its nodes as the root of the rooted tree.
		 * @param g Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 * @pre The graph @e g must be a tree.
		 */
		rooted_directed_tree(const ugraph& g, node r);
		/// Destructor
		~rooted_directed_tree();

		/**
		 * @brief Constructor with undirected tree and root node.
		 *
		 * Constructs a rooted directed tree from an undirected tree and one of
		 * its nodes as the root of the rooted tree.
		 * @param g Undirected tree.
		 * @param r Root of the directed tree. A node of @e g.
		 * @pre The graph @e g must be a tree.
		 */
		void init_rooted(const ugraph& g, node r);

		/* MODIFIERS */

		/// Does nothing. Do not use.
		void disjoint_union(const graph&);

		/* SETTERS */

		/**
		 * @brief Sets the root of this tree.
		 *
		 * This value is simply stored for a later retrieval.
		 * @param r Root of the tree.
		 * @pre @e r is a node of this graph.
		 */
		void set_root(node r);

		/* GETTERS */

		/// Returns the root of this tree.
		node get_root() const;

		/**
		 * @brief Is node @e r the root of this tree?
		 * @param r Node of the tree.
		 * @return Returns whether the node is the root or not. A node is the
		 * root of a tree if its in-degree equals 0.
		 */
		bool is_root(node r) const;

	private:
		/**
		 * @brief Root of the tree.
		 *
		 * This variable provides constant-time access to the node with null in-degree.
		 */
		node m_r = 0;
};

} // -- namespace lal
