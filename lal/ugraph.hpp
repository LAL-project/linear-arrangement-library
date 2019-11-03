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

// C++ includes
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/graph.hpp>

namespace lal {

/**
 * @brief Undirected graph class.
 *
 * Simple class implementing an undirected graph, using the adjacency
 * list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool) ).
 */
class ugraph : public graph {
	public:
		/// Default constructor.
		ugraph();
		/**
		 * @brief Constructor with number of nodes.
		 * @param n Number of nodes.
		 */
		ugraph(uint32_t n);
		/// Default destructor.
		~ugraph();

		/* OPERATORS */

		/* MODIFIERS */

		/**
		 * @brief Adds an undirected edge.
		 *
		 * For more details see @ref graph::add_edge(node,node,bool)
		 */
		ugraph& add_edge(node u, node v, bool norm = false);

		/**
		 * @brief Adds a list of undirected edges.
		 *
		 * For more details see @ref graph::add_edges(const std::vector<edge>&,bool)
		 */
		ugraph& add_edges(const std::vector<edge>& edges, bool norm = true);

		/* SETTERS */

		/* GETTERS */

		/// Returns true if the undirected edge (@e u, @e v) exists in the graph.
		bool has_edge(node u, node v) const;

		bool is_directed() const;
		bool is_undirected() const;
};

} // -- namespace lal
