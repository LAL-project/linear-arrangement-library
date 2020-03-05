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
#include <lal/graphs/graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Tree graph class.
 *
 * This class is just a class that inherits from @ref graph so as to allow
 * passing classes that inherit from this class as parameters to functions
 * that only accept attributes of type @ref graph.
 *
 * Classes that implement different abstractions of trees and that inherit from
 * this class are:
 * - @ref utree
 * - @ref dtree
 * - @ref urtree
 * - @ref drtree
 *
 * In these classes the addition of edges is constrained so as to ensure that
 * the edges added actually yield trees, i.e., that cycles are never produced.
 * For the sake of efficiency, only debug compilations of the library
 * (compilations where the DEBUG symbol is defined) check that such additions
 * do not produce cycles. In case of doubt, one can query the class using methods
 * @ref can_add_edge or @ref can_add_edges prior to adding one or more edges.
 */
class tree : virtual public graph {
	public:
		tree();
		virtual ~tree();

		/**
		 * @brief Returns whether this graph is an actual tree or not.
		 *
		 * Returns true if the number of edges is one less than the
		 * number of vertices. Note that this would not really be true if the
		 * addition of edges was not constrained. Since it is constrained in a
		 * way that no cycles can be produced (see @ref utree::add_edge,
		 * @ref utree::add_edges, @ref dtree::add_edge, @ref dtree::add_edges),
		 * then we only need to check for the number of edges.
		 *
		 * For further characterisations of a tree, see \cite Tree_Wikipedia.
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
		virtual bool can_add_edge(node s, node t) const = 0;
		/**
		 * @brief Can these edges be added?
		 *
		 * In a tree, these edges can only be added if their addition to the
		 * tree do not produce cycles.
		 * @param edges List of edges.
		 * @return Returns whether the addition of these new edges can be added
		 * to the tree without producing cycles.
		 */
		virtual bool can_add_edges(const std::vector<edge>& edges) const = 0;

	protected:
		/// Initialises memory of @ref rtree class.
		virtual void tree_init(uint32_t n);
		/// Clears the memory used by this rooted tree.
		virtual void tree_clear();
};

} // -- namespace graphs
} // -- namespace lal
