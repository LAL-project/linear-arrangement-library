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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/detail/array.hpp>
#include <lal/basic_types.hpp>

namespace lal {
namespace properties {

/**
 * @brief The connected components of a graph.
 *
 * This class is to be used paired with another graph.
 *
 * @tparam graph_t Type of graph.
 */
template <class graph_t>
class connected_components {
public:

	graph_t& operator[] (std::size_t i) noexcept {
#if defined DEBUG
		assert(i < m_connected_components.size());
#endif
		return m_connected_components[i];
	}

	const graph_t& operator[] (std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < m_connected_components.size());
#endif
		return m_connected_components[i];
	}

	// MODIFIERS

	/**
	 * @brief Initializes this object
	 * @param n Number of nodes.
	 * @post @ref m_node_to_cc is initialized.
	 */
	void init(std::size_t n) noexcept {
		m_node_to_cc.resize(n, n + 1);
		m__graph_node__to__cc_node.resize(n, n + 1);
	}

	/// Add a graph to the list of connected components.
	void add_graph(graph_t&& g) noexcept {
		const auto n = g.get_num_nodes();
		m_connected_components.push_back(std::forward<graph_t>(g));
		m__cc_node__to__graph_node.emplace_back(n, n + 1);
	}
	/// Add a graph to the list of connected components.
	void add_graph(const graph_t& g) noexcept {
		const auto n = g.get_num_nodes();
		m_connected_components.push_back(g);
		m__cc_node__to__graph_node.emplace_back(n, n + 1);
	}

	/**
	 * @brief Relates vertex @e u to the label of its connected component.
	 * @param u Input node (of the original graph).
	 * @param label The label of the connected component of @e u.
	 */
	void set_node_cc(node u, std::size_t label) noexcept {
		m_node_to_cc[u] = label;
	}

	/**
	 * @brief Relates vertex @e u to the corresponding vertex within its connected component.
	 * @param u Input node (of the original graph).
	 * @param label The label of the vertex @e u within its connected component.
	 */
	void set_label_graph_node_to_cc_node(node u, std::size_t label) noexcept {
		m__graph_node__to__cc_node[u] = label;
	}
	/**
	 * @brief Relates vertex @e u to the corresponding vertex within its connected component.
	 * @param cc_idx The label of the connected component of @e u.
	 * @param u Input node (within the connected component).
	 * @param label The label of @e u in the whole graph.
	 */
	void set_label_cc_node_to_graph_node(std::size_t cc_idx, node u, std::size_t label) noexcept {
		m__cc_node__to__graph_node[cc_idx][u] = label;
	}

	// GETTERS

	/// Returns the number of connected components.
	std::size_t size() const noexcept {
		return m_connected_components.size();
	}

	/**
	 * @brief Returns the label of the connected component @e u belongs to.
	 * @param u Node of the original graph.
	 * @returns A numeric value from 0 to the number of connected components (see
	 * @ref size())
	 */
	std::size_t get_cc_node(node u) const noexcept {
		return m_node_to_cc[u];
	}

	/**
	 * @brief The corresponding vertex within its connected component.
	 * @param u Input node (of the original graph).
	 * @returns The label of the vertex @e u within its connected component.
	 */
	std::size_t get_label_graph_node_to_cc_node(node u) const noexcept {
		return m__graph_node__to__cc_node[u];
	}
	/**
	 * @brief The corresponding vertex within its connected component.
	 * @param cc_idx The label of the connected component of @e u.
	 * @param u Input node (within the connected component).
	 * @returns The label of @e u in the whole graph.
	 */
	std::size_t get_label_cc_node_to_graph_node(std::size_t cc_idx, node u) const noexcept {
		return m__cc_node__to__graph_node[cc_idx][u];
	}

private:
	/// The connected components of the graph.
	std::vector<graph_t> m_connected_components;

	/// Relates the vertices in each connected components to their corresponding
	/// vertex in the original graph.
	std::vector<detail::array<std::size_t>> m__cc_node__to__graph_node;
	/// Relates the vertices in the original graph components to their corresponding
	/// vertex in the connected_component.
	detail::array<std::size_t> m__graph_node__to__cc_node;

	/// The label of the connected component a vertex belongs to.
	detail::array<std::size_t> m_node_to_cc;
};

} // -- namespace properties
} // -- namespace lal
