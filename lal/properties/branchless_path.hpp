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
#include <lal/detail/data_array.hpp>
#include <lal/graphs/tree.hpp>

namespace lal {
namespace properties {

/**
 * @brief Branchless paths in trees.
 *
 * A branchless path in a tree is a sequence of vertices of degree two, enclosed
 * by (at most) two vertices of degree different from two. The path graph is a
 * branchless path itself. The legs of a spider graph are all branchless paths,
 * which include the center of the spider.
 *
 * Before adding vertices to this path, it must be initialized via function
 * @ref init.
 *
 * The vertices in each branchless path are of two categories: internal and endpoints.
 * The endpoints of the path are the only two vertices of degree different from
 * two. The internal vertices are the (possibly 0) vertices of degree 2.
 */
class branchless_path {
public:

	/* MODIFIERS */

	/**
	 * @brief Initializes this path
	 * @param n Number of nodes of the tree.
	 * @post Nodes @ref m_lowest_lexicographic, @ref m_h1 and @ref m_h2 have
	 * been initialized.
	 * @post Memory for @ref m_vertex_set has been allocated.
	 */
	void init(std::size_t n) noexcept {
		m_h1 = m_h2 = m_lowest_lexicographic = n + 1;
		m_vertex_sequence.clear();
		m_vertex_sequence.reserve(n);
		m_vertex_set.resize(n, 0);
		m_position.resize(n, n + 1);
	}

	/**
	 * @brief Adds a node to this path.
	 * @param u Node to be added.
	 * @post @ref m_vertex_set and @ref m_vertex_sequence are updated.
	 */
	void add_node(node u) noexcept {
		m_vertex_set[u] = 1;
		m_vertex_sequence.push_back(u);
		m_position[u] = m_vertex_sequence.size() - 1;
	}

	/* SETTERS */

	/// Sets the first vertex of degree different from 2.
	void set_h1(node h) noexcept { m_h1 = h; }
	/// Sets the second vertex of degree different from 2.
	void set_h2(node h) noexcept { m_h2 = h; }
	/// Set lowest lexicographic vertex.
	void set_lowest_lexicographic(node l) noexcept { m_lowest_lexicographic = l; }

	/* GETTERS */

	/// Gets the first vertex of degree different from 2.
	node get_h1() const noexcept { return m_h1; }
	/// Gets the second vertex of degree different from 2.
	node get_h2() const noexcept { return m_h2; }
	/// Gets the lowest lexicographic vertex.
	node get_lowest_lexicographic() const noexcept { return m_lowest_lexicographic; }
	/**
	 * @brief Gets the vertex sequence of this path as a vector.
	 *
	 * This sequence includes vertices @ref m_h1 and @ref m_h2 (returned by
	 * @ref get_h1 and @ref get_h2 and are returned sequentially, that is, in
	 * the order they are found in the tree.
	 * @returns A vector with all nodes in the path.
	 */
	const std::vector<node>& get_vertex_sequence() const noexcept
	{ return m_vertex_sequence; }

	/**
	 * @brief Number of vertices in this path.
	 *
	 * The number of vertices includes the endpoints.
	 * @returns The number of vertices.
	 */
	std::size_t get_num_nodes() const noexcept { return m_vertex_sequence.size(); }
	/**
	 * @brief Number of edges in this path.
	 *
	 * The number of edges includes the edges incident to the endpoints.
	 * @returns The number of edges.
	 */
	std::size_t get_num_edges() const noexcept { return m_vertex_sequence.size() - 1; }
	/// Does this path include node @e u?
	bool has_node(node u) const noexcept { return m_vertex_set[u] == 1; }
	/// Returns the position of node @e u in @ref m_vetrex_sequence.
	std::size_t position(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_position[u];
	}

	/**
	 * @brief Is the given path an antenna?
	 *
	 * A branchless path is an antenna if any of its two endpoints is a degree-1
	 * vertex.
	 * @tparam tree_t The type of tree.
	 * @param t The tree of which @e bp is a path.
	 * @returns True if @e bp is an antenna. False if it is not.
	 */
	template <class graph_t>
	bool is_antenna(const graph_t& t) const noexcept {
		static_assert(std::is_base_of_v<graphs::graph, graph_t>);
		return t.get_degree(m_h1) == 1 or t.get_degree(m_h2) == 1;
	}

private:
	/// A 0-1 array to indicate if a vertex belongs to this path or not.
	detail::data_array<char> m_vertex_set;
	/// The position in @ref m_vertex_sequence of each vertex.
	detail::data_array<std::size_t> m_position;
	/// The vertex sequence of this branchless path (includes h1 and h2).
	std::vector<node> m_vertex_sequence;

	/// The first endpoint of this path.
	node m_h1;
	/// The second endpoint of this path.
	node m_h2;
	/// The @e internal vertex with lowest index lexicographically.
	node m_lowest_lexicographic;
};

} // -- namespace properties
} // -- namespace lal
