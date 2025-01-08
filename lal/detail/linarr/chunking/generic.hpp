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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/chunking/chunk.hpp>
#include <lal/linarr/chunking/chunk_sequence.hpp>

namespace lal {
namespace detail {


/**
 * @brief Basic algorithms existent in every definition of chunking.
 *
 * Chunking applied to syntactic dependency trees alone (a.k.a., rooted trees).
 *
 * @tparam arr_t Type of arrangement.
 */
template <class arrangement_t>
class chunks_generic {
public:
	/**
	 * @brief Constructor
	 * @param rt Input rooted tree.
	 * @param arr Input linear arrangement.
	 */
	chunks_generic(const graphs::rooted_tree& rt, const arrangement_t& arr)
	noexcept
		: m_rt(rt), m_arr(arr),
		  m_n(rt.get_num_nodes())
	{
	}

	/// Destructor
	virtual ~chunks_generic() noexcept { }

	/// Returns a constant reference to the chunk sequence @ref m_sequence.
	const linarr::chunk_sequence& get_chunk_sequence() const noexcept {
		return m_sequence;
	}
	/*
	/// Returns a reference to the chunk sequence @ref m_sequence.
	linarr::chunk_sequence& get_chunk_sequence() noexcept {
		return m_sequence;
	}
	*/
	/// Moves the chunk sequence @ref m_sequence.
	[[nodiscard]] linarr::chunk_sequence&& retrieve_chunk_sequence() noexcept {
		return std::move(m_sequence);
	}

protected:
	/// Returns a reference to the last chunk in the sentence.
	[[nodiscard]] linarr::chunk& last_chunk() noexcept {
		return m_sequence[m_sequence.size() - 1];
	}

	/// Returns the chunk index of node @e u.
	[[nodiscard]] std::size_t node_to_chunk(const node u) const noexcept {
		return m_sequence.get_chunk_index(u);
	}
	/// Sets the chunk index of node @e u to index @e i.
	void set_chunk_index(const node u, const std::size_t i) noexcept {
		m_sequence.set_chunk_index(u, i);
	}

	/// Set the parent node of a chunks.
	void set_parent_chunk(linarr::chunk& c) noexcept {
#if defined DEBUG
		bool head_found = false;
#endif
		for (node u : c.get_nodes()) {

			// if the parent of 'u' (if it exists) is outside the chunk
			// of 'u', then that parent is the parent of this chunk and 'u'
			// is the root node of the chunk.
			if (m_rt.get_in_degree(u) == 1) {
				const node v = m_rt.get_parent_node(u);
				if (node_to_chunk(v) != node_to_chunk(u)) {
					c.set_root_node(u);
					c.set_parent_node(v);
#if defined DEBUG
					head_found = true;
#endif
				}
			}
			else {
				c.set_root_node(u);
#if defined DEBUG
				head_found = true;
#endif
			}
		}

#if defined DEBUG
		assert(head_found);
#endif
	}

protected:
	/// Input rooted tree.
	const graphs::rooted_tree& m_rt;
	/// Linear arrangement.
	const arrangement_t m_arr;
	/// Number of vertices of the tree.
	const uint64_t m_n;

	/// The sequence of chunks obtained.
	linarr::chunk_sequence m_sequence;
};

} // -- namespace detail
} // -- namespace lal
