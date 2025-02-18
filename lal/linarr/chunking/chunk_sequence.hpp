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

// lal includes
#include <lal/basic_types.hpp>
#include <lal/linarr/chunking/chunk.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace linarr {

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

	/// Useful typedef for constant iterators.
	typedef typename std::vector<chunk>::const_iterator const_iterator;
	/// Useful typedef for non-constant iterators.
	typedef typename std::vector<chunk>::iterator iterator;

public:

	/**
	 * @brief Initializes this chunk sequence.
	 * @param n Size to initialize the sequence with, the number of nodes of the
	 * tree to be chunked.
	 */
	void init(const std::size_t n) noexcept
	{
		m_from_node_to_chunk.resize(n, n + 1);
	}

	/**
	 * @brief The @e i-th chunk.
	 * @param i Chunk index.
	 * @returns A constant reference to the @e i-th chunk.
	 */
	[[nodiscard]] const chunk& operator[] (const std::size_t i) const noexcept
	{
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
	[[nodiscard]] chunk& operator[] (const std::size_t i) noexcept
	{
#if defined DEBUG
		assert(i < size());
#endif
		return m_chunks[i];
	}

	/* MODIFIERS */

	/**
	 * @brief Adds a new chunk to the collection.
	 * @post The new chunk does not have a parent.
	 */
	void push_chunk() noexcept
	{
		m_chunks.push_back({});
	}

	/**
	 * @brief Adds a new chunk to the collection.
	 * @param u Node to be added into the new chunk.
	 * @post The new chunk does not have a parent.
	 */
	void push_chunk(const node u) noexcept
	{
		push_chunk();
		m_chunks.back().add_node(u);
	}

	/* SETTERS */

	void set_chunk_index(const node u, const std::size_t i) noexcept
	{
#if defined DEBUG
		assert(u < m_from_node_to_chunk.size());
#endif
		m_from_node_to_chunk[u] = i;
	}

	/* GETTERS */

	/// Returns the number of chunks.
	[[nodiscard]] std::size_t size() const noexcept
	{
		return m_chunks.size();
	}

	/// Returns the chunk index of node @e u.
	[[nodiscard]] std::size_t get_chunk_index(const node u) const noexcept
	{
#if defined DEBUG
		assert(u < m_from_node_to_chunk.size());
#endif
		return m_from_node_to_chunk[u];
	}

	/// A pointer to the beginning of the chunk sequence.
	[[nodiscard]] const_iterator begin() const noexcept
	{
		return m_chunks.begin();
	}
	/// A pointer to the beginning of the chunk sequence.
	[[nodiscard]] iterator begin() noexcept
	{
		return m_chunks.begin();
	}

	/// A pointer to the ending of the chunk sequence.
	[[nodiscard]] const_iterator end() const noexcept
	{
		return m_chunks.end();
	}
	/// A pointer to the ending of the chunk sequence.
	[[nodiscard]] iterator end() noexcept
	{
		return m_chunks.end();
	}

	/**
	 * @brief The sequence of chunks.
	 *
	 * See member @ref m_chunks.
	 * @returns A constant reference to the sequence of chunks.
	 */
	[[nodiscard]] const std::vector<chunk>& get_chunks() const noexcept
	{
		return m_chunks;
	}

private:

	/// The sequence of chunks
	std::vector<chunk> m_chunks;
	/// Index of every
	detail::array<std::size_t> m_from_node_to_chunk;
};

} // namespace linarr
} // namespace lal
