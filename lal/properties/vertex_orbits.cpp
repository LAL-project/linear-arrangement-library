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

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

#include <lal/detail/array.hpp>
#include <lal/detail/utilities/tree_isomorphism.hpp>

namespace lal {
namespace properties {

std::vector<std::vector<node>> vertex_orbits_compute(const graphs::free_tree& t)
noexcept
{
	const uint64_t n = t.get_num_nodes();

	detail::array<graphs::rooted_tree> rts(n);
	for (node u = 0; u < n; ++u) {
		rts[u] = graphs::rooted_tree(t, u);
	}

	detail::array<std::string> names(n);
	detail::array<char> has_orbit(n, 0);
	std::vector<std::vector<node>> orbits;

	for (node u = 0; u < n; ++u) {
		if (has_orbit[u]) { continue; }

		// open a new orbit
		orbits.push_back({u});
		has_orbit[u] = 1;

		for (node v = u + 1; v < n; ++v) {
			if (has_orbit[v]) { continue; }

			const std::string name_r1 = detail::assign_name(rts[u], u, names, 0);
			const std::string name_r2 = detail::assign_name(rts[v], v, names, 0);
			if (name_r1 == name_r2) {
				orbits.back().push_back(v);
				has_orbit[v] = 1;
			}
		}
	}

	return orbits;
}

} // -- namespace properties
} // -- namespace lal
