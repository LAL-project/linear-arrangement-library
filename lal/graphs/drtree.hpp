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

// lal includes
#include <lal/graphs/dtree.hpp>
#include <lal/graphs/rtree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Rooted directed tree class.
 *
 * This class represents a rooted directed tree. The edges can be oriented
 * either inwards (towards the root, an anti-arborescence) or outwards (away
 * from the roots, towards the leaves, an arborescence).
 *
 * This class can be built from an undirected tree and by orienting its edges
 * from a chosen node, the root, or by inserting edges one by one.
 */
class drtree : public dtree, virtual public rtree {
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
		 */
		drtree(const utree& t, node r, bool arb = true);
		/// Destructor
		~drtree();

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
		 */
		void init_rooted(const utree& t, node r, bool arb = true);
};

} // -- namespace graphs
} // -- namespace lal
