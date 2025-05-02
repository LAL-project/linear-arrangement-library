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

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/detail/properties/tree_centre.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/utilities/tree_isomorphism.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace utilities {

bool are_trees_isomorphic(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

#if defined DEBUG
	assert(t1.is_rooted_tree());
	assert(t2.is_rooted_tree());
#endif

	{
		const auto discard = detail::fast_non_iso(t1, t2);
		if (discard == 0) {
			return true;
		}
		if (discard == 1) {
			return false;
		}
	}

	return detail::are_rooted_trees_isomorphic(t1, t2);
}

bool are_trees_isomorphic(
	const graphs::free_tree& t1, const graphs::free_tree& t2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

#if defined DEBUG
	assert(t1.is_tree());
	assert(t2.is_tree());
#endif

	{
		const auto discard = detail::fast_non_iso(t1, t2);
		if (discard == 0) {
			return true;
		}
		if (discard == 1) {
			return false;
		}
	}

	const uint64_t n = t1.get_num_nodes();

	// find centres of the trees
	const auto c1 = detail::retrieve_centre(t1, 0);
	const auto c2 = detail::retrieve_centre(t2, 0);

	// check centre sizes
	const uint64_t size1 = (c1.second < n ? 2 : 1);
	const uint64_t size2 = (c2.second < n ? 2 : 1);
	if (size1 != size2) {
		return false;
	}

	const graphs::rooted_tree rt1(t1, c1.first, false, false);
	graphs::rooted_tree rt2(t2, c2.first, false, false);

	// the centres have only one vertex
	if (size1 == 1) {
		return detail::are_rooted_trees_isomorphic(rt1, rt2);
	}

	// the centres have two vertices

	// try with the first centre of the second tree
	const bool iso1 = detail::are_rooted_trees_isomorphic(rt1, rt2);
	if (iso1) {
		return true;
	}

	// try with the second centre of the second tree
	rt2.init_rooted(t2, c2.second, false, false);
	return detail::are_rooted_trees_isomorphic(rt1, rt2);
}

} // namespace utilities
} // namespace lal
