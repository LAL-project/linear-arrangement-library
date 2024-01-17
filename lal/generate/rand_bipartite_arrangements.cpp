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

#include <lal/generate/rand_bipartite_arrangements.hpp>

// C++ includes
#include <algorithm>

// lal includes
#include <lal/basic_types.hpp>

namespace lal {
namespace generate {

const linear_arrangement& rand_bipartite_arrangements::get_arrangement() noexcept {

	const bool use_red = m_red_or_blue(m_gen);
	init_arrangement(use_red);

	if (use_red) {
		std::shuffle(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_red, m_gen);
		std::shuffle(m_arr.begin_inverse() + m_n_red, m_arr.end_inverse(), m_gen);
	}
	else {
		std::shuffle(m_arr.begin_inverse(), m_arr.begin_inverse() + m_n_blue, m_gen);
		std::shuffle(m_arr.begin_inverse() + m_n_blue, m_arr.end_inverse(), m_gen);
	}

	m_arr.update_direct();
	return m_arr;
}

void rand_bipartite_arrangements::init(uint64_t seed) noexcept
{
	if (seed == 0) {
		std::random_device rd;
		m_gen = std::mt19937(rd());
	}
	else {
		m_gen = std::mt19937(seed);
	}

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

	m_what_in_left = properties::bipartite_graph_coloring::invalid_color;
	m_red_or_blue = std::bernoulli_distribution(0.5);
}

void rand_bipartite_arrangements::init_arrangement(bool red_first) noexcept {
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
	}
}

} // -- namespace generate
} // -- namespace lal
