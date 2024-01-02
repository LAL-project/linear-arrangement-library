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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/chunking/chunk.hpp>

namespace lal {
namespace detail {

/**
 * @brief Implementation of Anderson's algorithm for chunking.
 *
 * Chunking applied to syntactic dependency trees alone (a.k.a., rooted trees).
 *
 * See \cite Anderson2021a and \cite Anderson2019a for a complete definition of
 * Anderson (et al.)'s chunks.
 *
 * @tparam arr_t Type of arrangement.
 */
template <class arrangement_t>
class chunks_Anderson {
public:
	/// Input rooted tree.
	const graphs::rooted_tree& m_rt;
	/// Linear arrangement.
	const arrangement_t m_arr;
	/// Number of vertices of the tree.
	const uint64_t m_n;

	/// The sequence of chunks obtained.
	linarr::chunk_sequence m_sequence;

public:
	/**
	 * @brief Constructor
	 * @param rt Input rooted tree.
	 * @param arr Input linear arrangement.
	 */
	chunks_Anderson(const graphs::rooted_tree& rt, const arrangement_t& arr)
	noexcept
		: m_rt(rt), m_arr(arr),
		  m_n(rt.get_num_nodes())
	{
	}

	/**
	 * @brief Main method of this class
	 *
	 * Calling this method will chunk the input rooted tree using Anderson
	 * (et al.)'s definition.
	 */
	void chunk_input_tree() noexcept {
		m_sequence.init(m_n);

		if (m_rt.get_num_nodes() == 1) {
			m_sequence.set_chunk_index(0, 0);
			m_sequence.push_chunk(0);
		}
		else {
			// assign chunk indices
			std::size_t chunk_idx = 0;
			assign_chunk_indices(m_rt.get_root(), chunk_idx);

			// relabel chunk indices from 0 to k so that chunk 0 is the
			// leftmost in the linear arrangement, and k is the rightmost
			relabel_chunks();

			// make chunks out of the map
			make_chunks();
		}
	}

private:

	/// Can node @e u be added to the same chunk as @e r.
	bool can_be_added(node r, node u) const noexcept {
		return m_rt.get_out_degree(u) == 0 and m_rt.has_edge(r, u);
	}
	/// Returns the chunk index of node @e u.
	std::size_t node_to_chunk(node u) const noexcept {
		return m_sequence.get_chunk_index(u);
	}
	/// Sets the chunk index of node @e u to index @e i.
	void set_chunk_index(node u, std::size_t i) noexcept {
		m_sequence.set_chunk_index(u, i);
	}
	/// Returns a reference to the last chunk in the sentence.
	linarr::chunk& last_chunk() noexcept {
		return m_sequence.get_chunk(m_sequence.size() - 1);
	}

	/**
	 * @brief Assign chunk indices to all vertices of the subtree rooted at @e r.
	 * @param r Root of the current subtree.
	 * @param[inout] chunk_idx Current chunk index.
	 */
	void assign_chunk_indices(node_t r, std::size_t& chunk_idx) noexcept {
		++chunk_idx;

#if defined DEBUG
		assert(m_rt.get_out_degree(*r) > 0);
#endif

		set_chunk_index(*r, chunk_idx);

		// march leftwards in the arrangement assigning to the root's children
		// the same chunk index as the root
		position_t p_root = m_arr[r];
		if (p_root > 0ull) {
			position_t pl = p_root - 1ull;
			while (pl > 0ull and can_be_added(*r, m_arr[pl])) {
				set_chunk_index(m_arr[pl], chunk_idx);
				--pl;
			}
			// pl == 0
			if (can_be_added(*r, m_arr[pl])) {
				set_chunk_index(m_arr[pl], chunk_idx);
			}
		}
		// march rightwards in the arrangement assigning to the root's children
		// the same chunk index as the root
		if (p_root < m_n - 1) {
			position_t pr = p_root + 1ull;
			while (pr < m_n and can_be_added(*r, m_arr[pr])) {
				set_chunk_index(m_arr[pr], chunk_idx);
				++pr;
			}
		}

		// assign new chunk indices to the terminal, unassigned children of 'r'
		for (node v : m_rt.get_out_neighbours(*r)) {
			if (node_to_chunk(v) > m_n and m_rt.get_out_degree(v) == 0) {
				set_chunk_index(v, chunk_idx);
				++chunk_idx;
			}
		}

		// traverse down the tree and recursively build new chunks
		for (node_t v : m_rt.get_out_neighbours(*r)) {
			if (m_rt.get_out_degree(*v) > 0) {
				assign_chunk_indices(v, chunk_idx);
			}
		}
	}

	/**
	 * @brief Relabels the chunks obtained in method @ref assign_chunk_indices.
	 *
	 * Reassigns labels to chunks so that the leftmost chunk is labelled 0.
	 */
	void relabel_chunks() noexcept {
		std::size_t chunk_idx = 0;
		position_t p = 0ull;

		while (p < m_n) {
			const std::size_t current_index = node_to_chunk(m_arr[p]);

			set_chunk_index(m_arr[p], chunk_idx);
			++p;

			while (p < m_n and current_index == node_to_chunk(m_arr[p])) {
				set_chunk_index(m_arr[p], chunk_idx);
				++p;
			}

			++chunk_idx;
		}
	}

	/**
	 * @brief Actually builds the sequence of chunks.
	 *
	 * Puts all equally-labelled nodes in the same chunk and computes the parent
	 * node for each chunk.
	 */
	void make_chunks() noexcept {
		m_sequence.push_chunk();

		position_t p = 0ull;
		while (p < m_n) {

			const std::size_t current_index = node_to_chunk(m_arr[p]);

			linarr::chunk& c = last_chunk();
			c.add_node(m_arr[p]);

			++p;
			while (p < m_n and current_index == node_to_chunk(m_arr[p])) {
				c.add_node(m_arr[p]);
				++p;
			}

			// set the parent of the chunk
			bool head_found = false;
			for (node v : c.get_nodes()) {
				if (m_rt.get_out_degree(v) > 0) {
					// found the head of the chunk!
					head_found = true;

					if (not m_rt.node_has_parent(v)) {
						// the chunk should not have a parent.
						// this is the "chunk's root"
					}
					else {
						const auto chunk_parent = m_rt.get_parent_node(v);
						c.set_parent_node( chunk_parent );
					}
				}
			}
			// if the chunk does not have a head, assign
			// the parent of any of the vertices
			if (not head_found) {
				const auto first_vertex = c.get_nodes()[0];
				c.set_parent_node( m_rt.get_parent_node(first_vertex) );
			}

			if (p < m_n) {
				m_sequence.push_chunk();
			}
		}
	}
};

} // -- namespace detail
} // -- namespace lal
