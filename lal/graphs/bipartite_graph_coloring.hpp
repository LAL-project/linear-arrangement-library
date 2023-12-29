/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/basic_types.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {
namespace graphs {

/**
 * @brief A class to represent a coloring of the vertices of a bipartite graph.
 *
 * Coloring of general graphs is known to be NP-complete \cite Karp1972a. This
 * class is used to represent the vertex coloring of bipartite graphs of @e n vertices,
 * for which two colors are sufficient. This class uses static members @ref red
 * and @ref blue to designate these colors. Function @ref lal::graphs::coloring
 * calculates such coloring.
 */
class bipartite_graph_coloring {
public:
	/// A color, called red, of value 0.
	static const char red = 0;
	/// A color, called blue, of value 1.
	static const char blue = 1;

public:
	/// Default constructor.
	bipartite_graph_coloring() noexcept = default;
	/// Default copy constructor.
	bipartite_graph_coloring(const bipartite_graph_coloring&) noexcept = default;
	/// Default move constructor.
	bipartite_graph_coloring(bipartite_graph_coloring&&) noexcept = default;
	/// Default copy assignment operator.
	bipartite_graph_coloring& operator= (const bipartite_graph_coloring&) noexcept = default;
	/// Default move assignment operator.
	bipartite_graph_coloring& operator= (bipartite_graph_coloring&&) noexcept = default;

	/**
	 * @brief Constructor with number of vertices.
	 * @param n The number of vertices of the graph.
	 */
	bipartite_graph_coloring(std::size_t n) noexcept {
		init(n);
	}

	/**
	 * @brief Initializes this object with @e n vertices.
	 * @param n The number of vertices of the graph.
	 */
	void init(std::size_t n) noexcept {
		coloring.resize(n, -1);
	}

	/// Returns the color of node @e u
	char get_color_of(node u) const noexcept { return coloring[u]; }
	/// Returns the color of node @e u
	char& get_color_of(node u) noexcept { return coloring[u]; }

	/// Returns the color of node @e u
	char operator[] (std::size_t u) const noexcept { return coloring[u]; }
	/// Returns the color of node @e u
	char& operator[] (std::size_t u) noexcept { return coloring[u]; }

	/// Returns the color of node @e u
	char operator[] (lal::node_t u) const noexcept { return coloring[*u]; }
	/// Returns the color of node @e u
	char& operator[] (lal::node_t u) noexcept { return coloring[*u]; }

private:
	/**
	 * @brief The array that contains the coloring.
	 *
	 * For any vertex @e u, coloring[u] is the color of vertex @e u.
	 */
	detail::data_array<char> coloring;
};

} // -- namespace graphs
} // -- namespace lal
