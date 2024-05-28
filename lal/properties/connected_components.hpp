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
	}

	/// Add a graph to the list of connected components.
	void add_graph(graph_t&& g) noexcept {
		m_connected_components.push_back(std::forward<graph_t>(g));
	}
	/// Add a graph to the list of connected components.
	void add_graph(const graph_t& g) noexcept {
		m_connected_components.push_back(g);
	}

	/// Sets the label @e lof the connected component to which node @e u belongs.
	void set_node_label(node u, std::size_t label) noexcept {
		m_node_to_cc[u] = label;
	}

	// GETTERS

	/// Returns the number of connected components.
	std::size_t size() const noexcept {
		return m_connected_components.size();
	}

	/// Returns the label of the connected component to which @e u belongs.
	std::size_t get_label(node u) const noexcept {
		return m_node_to_cc[u];
	}

private:
	/// The connected components of the graph.
	std::vector<graph_t> m_connected_components;
	/// The label of the connected component a vertex belongs to.
	lal::detail::array<std::size_t> m_node_to_cc;
};

} // -- namespace properties
} // -- namespace lal
