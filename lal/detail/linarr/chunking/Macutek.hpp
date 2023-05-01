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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/chunking/chunk.hpp>

namespace lal {
namespace detail {

/**
 * @brief Implementation of Mačutek's algorithm for chunking.
 *
 * Chunking applied to syntactic dependency trees alone (a.k.a., rooted trees).
 *
 * See \cite Macutek2021a for a complete definition of Mačutek (et al.)'s chunks.
 *
 * @tparam arr_t Type of arrangement.
 */
template <class arrangement_t>
class chunks_Macutek {
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
	chunks_Macutek(const graphs::rooted_tree& rt, const arrangement_t& arr)
	noexcept
		: m_rt(rt), m_arr(arr),
		  m_n(rt.get_num_nodes())
	{
	}

	/**
	 * @brief Main method of this class
	 *
	 * Calling this method will chunk the input rooted tree using Mačutek
	 * (et al.)'s definition.
	 */
	void chunk_input_tree() noexcept {
		m_sequence.init(m_n);

		position_t p{0ull};

		m_sequence.push_chunk(m_arr[p]);
		m_sequence.set_chunk_index(m_arr[p], 0);

		++p;
		for (; p < m_n; ++p) {
			const node prev = m_arr[p - 1ull];
			const node current = m_arr[p];

			const bool are_syntactically_linked =
				m_rt.has_edge(prev, current) or m_rt.has_edge(current, prev);

			if (are_syntactically_linked) {
				last_chunk().add_node( current );
			}
			else {
				m_sequence.push_chunk(m_arr[p]);
			}

			m_sequence.set_chunk_index(m_arr[p], m_sequence.size() - 1);
		}

		set_parent_chunks();
	}

private:

	/// Returns a reference to the last chunk in the sentence.
	linarr::chunk& last_chunk() noexcept {
		return m_sequence.get_chunk(m_sequence.size() - 1);
	}

	/// Returns the chunk index of node @e u.
	std::size_t node_to_chunk(node u) const noexcept {
		return m_sequence.get_chunk_index(u);
	}
	/// Sets the chunk index of node @e u to index @e i.
	void set_chunk_index(node u, std::size_t i) noexcept {
		m_sequence.set_chunk_index(u, i);
	}

	/// Set the parent node of all chunks.
	void set_parent_chunks() noexcept {
		for (std::size_t i = 0; i < m_sequence.size(); ++i) {
			linarr::chunk& c = m_sequence.get_chunk(i);

			for (node v : c.get_nodes()) {
				if (m_rt.get_in_degree(v) == 0) {
					// this chunk contains the root of the tree
					break;
				}
				else {
					const node pv = m_rt.get_parent_node(v);
					if (node_to_chunk(pv) != i) {
						c.set_parent_node(pv);
					}
				}
			}
		}
	}
};

} // -- namespace detail
} // -- namespace lal
