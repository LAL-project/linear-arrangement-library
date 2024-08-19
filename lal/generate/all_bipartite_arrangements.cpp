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

#include <lal/generate/all_bipartite_arrangements.hpp>

// C++ includes
#include <algorithm>
#if defined DEBUG
#include <cassert>
#endif

namespace lal {
namespace generate {

void all_bipartite_arrangements::next() noexcept {
	if (m_arr.size() == 1) {
		m_do_mirror = true;
		m_reached_end_red = true;
	}

	if (not m_do_mirror) {
		// in this part of the enumeration, the left half contains red vertices
		m_reached_end_red =
			not
			std::next_permutation(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_red);
		m_arr.update_direct();

		if (m_reached_end_red) {
			m_reached_end_blue =
				not
				std::next_permutation(m_arr.begin_inverse() + m_n_red, m_arr.end_inverse());
			m_arr.update_direct();

			if (m_reached_end_blue) {
				init_arrangement(false);
				m_do_mirror = true;
			}
		}
	}
	else {
		// in this part of the enumeration, the left half contains blue vertices
		m_reached_end_blue =
			not
			std::next_permutation(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_blue);
		m_arr.update_direct();

		if (m_reached_end_blue) {
			m_reached_end_red =
				not
				std::next_permutation(m_arr.begin_inverse() + m_n_blue, m_arr.end_inverse());
			m_arr.update_direct();
		}
	}
}

void all_bipartite_arrangements::init() noexcept {
	const auto n = m_coloring.size();
	m_arr.resize(n);

	m_n_blue = m_n_red = 0;
	for (node u = 0; u < n; ++u) {
		if (m_coloring[u] == blue) {
			++m_n_blue;
		}
		else if (m_coloring[u] == red) {
			++m_n_red;
		}
	}

#if defined DEBUG
	assert(m_n_red + m_n_blue == n);
#endif

	init_arrangement(true);
}

void all_bipartite_arrangements::init_arrangement(const bool red_first) noexcept {
	const auto n = m_coloring.size();

	position left = 0ull;
	position right = n - 1ull;
	if (red_first) {
		for (node u = 0; u < n; ++u) {
			if (m_coloring[u] == red) {
				m_arr.assign(u, left++);
			}
			else if (m_coloring[u] == blue) {
				m_arr.assign(u, right--);
			}
		}

		// the vertices in the blue half have been placed in reversed order
		std::reverse(m_arr.begin_inverse() + m_n_red, m_arr.end_inverse());
	}
	else {
		for (node u = 0; u < n; ++u) {
			if (m_coloring[u] == blue) {
				m_arr.assign(u, left++);
			}
			else if (m_coloring[u] == red) {
				m_arr.assign(u, right--);
			}
		}

		// the vertices in the red half have been placed in reversed order
		std::reverse(m_arr.begin_inverse() + m_n_blue, m_arr.end_inverse());
	}

	m_arr.update_direct();

	m_do_mirror = false;
	m_reached_end_blue = false;
	m_reached_end_red = false;

#if defined DEBUG
	if (red_first) {
		assert(std::is_sorted(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_red));
		assert(std::is_sorted(m_arr.begin_inverse() + m_n_red, m_arr.end_inverse()));
	}
	else {
		assert(std::is_sorted(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_blue));
		assert(std::is_sorted(m_arr.begin_inverse() + m_n_blue, m_arr.end_inverse()));
	}
#endif
}

} // -- namespace generate
} // -- namespace lal
