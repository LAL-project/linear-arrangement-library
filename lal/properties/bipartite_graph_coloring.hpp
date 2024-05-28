/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/basic_types.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace properties {

/**
 * @brief A class to represent a coloring of the vertices of a bipartite graph.
 *
 * Coloring of general graphs is known to be NP-complete \cite Karp1972a. This
 * class is used to represent the vertex coloring of bipartite graphs of @e n vertices,
 * for which two colors are sufficient. This class uses static members @ref red
 * and @ref blue to designate these colors.
 * Function @ref lal::properties::bipartite_coloring calculates such coloring.
 */
class bipartite_graph_coloring {
public:
	/**
	 * @brief A useful type for colors
	 *
	 * A 64-bit integer is used so that these values can be used to index vectors
	 * and arrays through operator [].
	 */
	typedef uint64_t color_t;
	/// An invalid color, used to initialize colors to an invalid value.
	static constexpr color_t invalid_color = 2;
	/// A color, called red, of value 0.
	static constexpr color_t red = 0;
	/// A color, called blue, of value 1.
	static constexpr color_t blue = 1;

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

	/// Default destrutor.
	~bipartite_graph_coloring() = default;

	/**
	 * @brief Initializes this object with @e n vertices.
	 * @param n The number of vertices of the graph.
	 */
	void init(std::size_t n) noexcept {
		m_coloring.resize(n, invalid_color);
	}

	/// Returns whether or not the color of vertex @e u is valid.
	bool is_color_valid(node u) const noexcept {
		return get_color_of(u) != invalid_color;
	}

	/// Returns the size of this m_coloring (the number of vertices)
	std::size_t size() const noexcept { return m_coloring.size(); }

	/// Returns the color of node @e u
	color_t get_color_of(node u) const noexcept { return m_coloring[u]; }
	/// Returns the color of node @e u
	color_t& get_color_of(node u) noexcept { return m_coloring[u]; }

	/// Returns the color of node @e u
	color_t operator[] (node u) const noexcept { return m_coloring[u]; }
	/// Returns the color of node @e u
	color_t& operator[] (node u) noexcept { return m_coloring[u]; }

	/// Returns the color of node @e u
	color_t operator[] (lal::node_t u) const noexcept { return m_coloring[*u]; }
	/// Returns the color of node @e u
	color_t& operator[] (lal::node_t u) noexcept { return m_coloring[*u]; }

private:
	/**
	 * @brief The array that contains the m_coloring.
	 *
	 * For any vertex @e u, m_coloring[u] is the color of vertex @e u.
	 */
	detail::array<color_t> m_coloring;
};

} // -- namespace properties
} // -- namespace lal
