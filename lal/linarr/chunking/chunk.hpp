/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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
#include <optional>
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Definition of a chunk.
 * 
 * A chunk is a collection of vertices with a parent node. This parent node is
 * relative to the syntactic dependency tree this chunks is part of.
 */
class chunk {
public:

	/// Useful typedef for constant iterators.
	typedef typename std::vector<node>::const_iterator const_iterator;
	/// Useful typedef for non-constant iterators.
	typedef typename std::vector<node>::iterator iterator;

public:

	/* MODIFIERS */

	/**
	 * @brief Adds a new node to this chunk.
	 * @param u Node to be added.
	 */
	void add_node(const node u) noexcept
	{
		m_nodes.push_back(u);
	}

	/* SETTERS */

	/**
	 * @brief Unsets the parent node of this chunk.
	 * @post After calling this, method @ref has_parent_node returns false.
	 */
	void unset_parent_node() noexcept
	{
		m_parent = {};
	}

	/**
	 * @brief Sets the parent node of this chunk.
	 * @param u Node parent of this chunk.
	 * @post After calling this, method @ref has_parent_node returns true.
	 */
	void set_parent_node(const node u) noexcept
	{
		m_parent = u;
	}

	/**
	 * @brief Unsets the root node of this chunk.
	 * @post After calling this, method @ref has_root_node returns false.
	 */
	void unset_root_node() noexcept
	{
		m_root = {};
	}

	/**
	 * @brief Sets the root node of this chunk.
	 * @param u Root parent of this chunk.
	 * @post After calling this, method @ref has_root_node returns true.
	 */
	void set_root_node(const node u) noexcept
	{
		m_root = u;
	}

	/* GETTERS */

	/// Does this chunk have a parent node?
	[[nodiscard]] bool has_parent_node() const noexcept
	{
		return m_parent.has_value();
	}

	/**
	 * @brief Retrieve the parent node of this chunk.
	 * @returns The parent node if it exists.
	 * @pre Method @ref has_parent_node must evaluate to true.
	 */
	[[nodiscard]] node get_parent_node() const noexcept
	{
#if defined DEBUG
		assert(has_parent_node());
#endif
		return *m_parent;
	}

	/// Does this chunk have a parent node?
	[[nodiscard]] bool has_root_node() const noexcept
	{
		return m_root.has_value();
	}

	/**
	 * @brief Retrieve the root node of this chunk.
	 * @returns The root node if it exists.
	 * @pre Method @ref has_root_node must evaluate to true.
	 */
	[[nodiscard]] node get_root_node() const noexcept
	{
#if defined DEBUG
		assert(has_root_node());
#endif
		return *m_root;
	}

	/// A pointer to the beginning of the node sequence.
	[[nodiscard]] const_iterator begin() const noexcept
	{
		return m_nodes.begin();
	}
	/// A pointer to the beginning of the node sequence.
	[[nodiscard]] iterator begin() noexcept
	{
		return m_nodes.begin();
	}

	/// A pointer to the ending of the node sequence.
	[[nodiscard]] const_iterator end() const noexcept
	{
		return m_nodes.end();
	}
	/// A pointer to the ending of the node sequence.
	[[nodiscard]] iterator end() noexcept
	{
		return m_nodes.end();
	}

	/**
	 * @brief Collection of nodes of this chunk.
	 *
	 * See member @ref m_nodes.
	 * @returns A constant reference to the collection of nodes.
	 */
	[[nodiscard]] const std::vector<node>& get_nodes() const noexcept
	{
		return m_nodes;
	}

private:

	/// Collection of nodes in this chunk
	std::vector<node> m_nodes;
	/// The parent vertex of this chunk
	std::optional<node> m_parent;
	/// The root vertex of this chunk
	std::optional<node> m_root;
};

} // namespace linarr
} // namespace lal
