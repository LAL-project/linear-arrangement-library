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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <optional>
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/detail/data_array.hpp>

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
	/* MODIFIERS */

	/**
	 * @brief Adds a new node to this chunk.
	 * @param u Node to be added.
	 */
	void add_node(node u) noexcept {
		m_nodes.push_back(u);
	}

	/* SETTERS */

	/**
	 * @brief Unsets the parent node of this chunk.
	 * @post After calling this, method @ref has_parent_node returns false.
	 */
	void unset_parent_node() noexcept {
		m_parent = {};
	}

	/**
	 * @brief Sets the parent node of this chunk.
	 * @param u Node parent of this chunk.
	 * @post After calling this, method @ref has_parent_node returns true.
	 */
	void set_parent_node(node u) noexcept {
		m_parent = u;
	}

	/* GETTERS */

	/**
	 * @brief Collection of nodes of this chunk.
	 *
	 * See member @ref m_nodes.
	 * @returns A constant reference to the collection of nodes.
	 */
	const std::vector<node>& get_nodes() const noexcept { return m_nodes; }

	/// Does this chunk have a parent vertex?
	bool has_parent_node() const noexcept { return m_parent.has_value(); }

	/**
	 * @brief Retrieve the parent node of this chunk.
	 * @returns The parent node if it exists.
	 * @pre Method @ref has_parent_node must evaluate to true.
	 */
	node get_parent_node() const noexcept {
#if defined DEBUG
		assert(has_parent_node());
#endif
		return *m_parent;
	}

private:
	/// Collection of nodes in this chunk
	std::vector<node> m_nodes;
	/// The parent vertex of this chunk
	std::optional<node> m_parent;
};

/**
 * @brief Chunk sequence of a syntactic dependency tree.
 * 
 * This can be seen as the ordered sequence of chunks obtained from applying a
 * chunking algorithm. The sequence is ordered because the first chunk (at index
 * 0) is the leftmost chunk in the ordering of the nodes. For instance, we may
 * have the following tree (in the head vector format --
 * see @ref LAL_concepts__head_vector),
 *
 @verbatim
 2 5 2 5 0 9 9 9 10 5
 @endverbatim
 *
 * The chunks obtained could be (there are other ways to obtain chunks) the
 * follownig
 *
 @verbatim
 |-------|-----|----------|---|
 | 2 5 2 | 5 0 | 9 9 9 10 | 5 |
 |-------|-----|----------|---|
     0      1       2       3
 @endverbatim
 * 
 * and so the first chunk has index 0, the second chunk index 1, and so on. Use
 * @code
 * 		lal::linarr::chunk_sequence s;
 * 		// call a chunking algorithm
 * 		const std::vector<chunk> chunks = s.get_chunks();
 * 		const chunk& c0 = chunks[0];
 * 		const chunk& c1 = chunks[1];
 * 		// ...
 * @endcode
 * 
 * Nodes can be queried for their chunk index with method @ref get_chunk_index.
 * Since in the tree example the nodes are distributed from left to right, the
 * chunk indices are the following (left column: nodes, right column: chunk index).
 * 
 @verbatim
 0: 0
 1: 0
 2: 0
 3: 1
 4: 1
 5: 2
 6: 2
 7: 2
 8: 2
 9: 3
 @endverbatim
 */
class chunk_sequence {
public:

	/**
	 * @brief Initializes this chunk sequence.
	 * @param n Size to initialize the sequence with, the number of nodes of the
	 * tree to be chunked.
	 */
	void init(std::size_t n) noexcept {
		m_from_node_to_chunk.resize(n, n + 1);
	}

	/* MODIFIERS */

	/**
	 * @brief Adds a new chunk to the collection.
	 * @post The new chunk does not have a parent.
	 */
	void push_chunk() noexcept {
		m_chunks.push_back({});
	}

	/**
	 * @brief Adds a new chunk to the collection.
	 * @param u Node to be added into the new chunk.
	 * @post The new chunk does not have a parent.
	 */
	void push_chunk(node u) noexcept {
		push_chunk();
		m_chunks.back().add_node(u);
	}

	/* SETTERS */

	void set_chunk_index(node u, std::size_t i) noexcept {
#if defined DEBUG
		assert(u < m_from_node_to_chunk.size());
#endif
		m_from_node_to_chunk[u] = i;
	}

	/* GETTERS */

	/// Returns the number of chunks.
	std::size_t size() const noexcept {
		return m_chunks.size();
	}

	/// Returns the chunk index of node @e u.
	std::size_t get_chunk_index(node u) const noexcept {
#if defined DEBUG
		assert(u < m_from_node_to_chunk.size());
#endif
		return m_from_node_to_chunk[u];
	}

	/**
	 * @brief The @e i-th chunk.
	 * @param i Chunk index.
	 * @returns A constant reference to the @e i-th chunk.
	 */
	const chunk& get_chunk(std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_chunks[i];
	}

	/**
	 * @brief The @e i-th chunk.
	 * @param i Chunk index.
	 * @returns A non-constant reference to the @e i-th chunk.
	 */
	chunk& get_chunk(std::size_t i) noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_chunks[i];
	}

	/**
	 * @brief The sequence of chunks.
	 * 
	 * See member @ref m_chunks.
	 * @returns A constant reference to the sequence of chunks.
	 */
	const std::vector<chunk>& get_chunks() const noexcept {
		return m_chunks;
	}

private:
	/// The sequence of chunks
	std::vector<chunk> m_chunks;
	/// Index of every 
	detail::data_array<std::size_t> m_from_node_to_chunk;
};

} // -- namespace linarr
} // -- namespace lal
