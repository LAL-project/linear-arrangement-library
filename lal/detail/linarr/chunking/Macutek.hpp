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
#include <lal/detail/linarr/chunking/generic.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

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
template <Arrangement arrangement_t>
class chunks_Macutek : public chunks_generic<arrangement_t> {
public:

	/**
	 * @brief Constructor
	 * @param rt Input rooted tree.
	 * @param arr Input linear arrangement.
	 */
	chunks_Macutek(const graphs::rooted_tree& rt, const arrangement_t& arr)
		noexcept
		: generic(rt, arr)
	{ }

	/**
	 * @brief Main method of this class
	 *
	 * Calling this method will chunk the input rooted tree using Mačutek
	 * (et al.)'s definition.
	 */
	void chunk_input_tree() noexcept
	{
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
				last_chunk().add_node(current);
			}
			else {
				m_sequence.push_chunk(m_arr[p]);
			}

			m_sequence.set_chunk_index(m_arr[p], m_sequence.size() - 1);
		}

		set_parent_chunks();
	}

private:

	/// Set the parent node of all chunks.
	void set_parent_chunks() noexcept
	{
		for (std::size_t i = 0; i < m_sequence.size(); ++i) {
			set_parent_chunk(m_sequence[i]);
		}
	}

private:

	/// Useful typedef
	typedef chunks_generic<arrangement_t> generic;

	// member variables
	using generic::m_arr;
	using generic::m_n;
	using generic::m_rt;
	using generic::m_sequence;

	// member functions
	using generic::last_chunk;
	using generic::set_parent_chunk;
};

} // namespace detail
} // namespace lal
