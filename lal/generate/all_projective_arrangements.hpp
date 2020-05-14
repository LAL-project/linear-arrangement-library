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
#include <lal/definitions.hpp>
#include <lal/graphs/rtree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive projective arrangement generator.
 *
 * Generates all projective arrangements of a given rooted tree.
 *
 * The arrangements generated do not take into account the symmetrical
 * arrangements produced by swapping leaves of the tree connected to the
 * same parent. That is, the arrangements produced can be seen as labelled
 * arrangements. Therefore, this class will generate \f$n!\f$ arrangements
 * for all star trees of \f$n\f$ vertices.
 *
 * The correct usage of this class is
 * @code
 *		all_proj_arr ArrGen(n);
 *		while (ArrGen.has_next()) {
 *			ArrGen.next();
 *			lal::linearrgmnt arr = ArrGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_proj_arr {
	public:
		/// Constructor with constant reference to a rooted tree.
		all_proj_arr(const graphs::rtree& rT);
		/// Default destructor.
		~all_proj_arr();

		/**
		 * @brief Returns whether there are more arrangements to generate.
		 * @return Returns true if there are still more arrangements
		 * to generate. Returns false if all arrangements have been
		 * generated.
		 */
		bool has_next() const;

		/**
		 * @brief Generates the next arrangement.
		 *
		 * Modifies the internal state so that the next arrangement
		 * can be retrieved using method @ref get_arrangement.
		 */
		void next();

		/**
		 * @brief Constructs the current arrangement.
		 * @pre Method @ref next must have been called at least once.
		 * @return Returns the arrangement generated with method @ref next().
		 */
		linearrgmnt get_arrangement() const;

	private:
		/// Constant reference to rooted tree.
		const graphs::rtree& m_rT;

		/// The interval of every node of the tree
		std::vector<std::vector<node>> m_intervals;

		/// Does exist a next arrangement?
		bool m_has_next = true;

		/// The vertices of the tree ordered in post order.
		std::vector<node> m_por_vertices;

	private:
		/// Retrieve the vertices in @ref m_por_vertices, starting at @e r.
		void post_order_vertex_ordering(node r);
		/// Make the canonical interval of all vertices of the tree, starting at @e r..
		void canonical_interval_tree(node r);
		/// Make the canonical interval of the node @e u.
		void canonical_interval_single(node u);
};

} // -- namespace generate
} // -- namespace lal