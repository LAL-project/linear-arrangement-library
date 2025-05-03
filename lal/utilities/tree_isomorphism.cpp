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
 ********************************************************************/

// lal includes
#include <lal/detail/utilities/tree_isomorphism.hpp>

namespace lal {
namespace utilities {

bool are_trees_isomorphic(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{

#if defined DEBUG
	assert(t1.is_rooted_tree());
	assert(t2.is_rooted_tree());
#endif

	if (t1.get_num_nodes() < 40) {
		return detail::are_trees_isomorphic<
			detail::isomorphism::algorithm::string,
			true>(t1, t2);
	}
	return detail::are_trees_isomorphic<
		detail::isomorphism::algorithm::tuple,
		true>(t1, t2);
}

bool are_trees_isomorphic(
	const graphs::free_tree& t1, const graphs::free_tree& t2
) noexcept
{
#if defined DEBUG
	assert(t1.is_tree());
	assert(t2.is_tree());
#endif

	if (t1.get_num_nodes() < 40) {
		return detail::are_trees_isomorphic<
			detail::isomorphism::algorithm::string,
			true>(t1, t2);
	}
	return detail::are_trees_isomorphic<
		detail::isomorphism::algorithm::tuple,
		true>(t1, t2);
}

bool are_trees_isomorphic(
	const graphs::free_tree& t1,
	const node r1,
	const graphs::free_tree& t2,
	const node r2
) noexcept
{
#if defined DEBUG
	assert(t1.is_tree());
	assert(t2.is_tree());
#endif

	if (t1.get_num_nodes() < 40) {
		return detail::are_trees_isomorphic<
			detail::isomorphism::algorithm::string,
			true>(t1, r1, t2, r2);
	}
	return detail::are_trees_isomorphic<
		detail::isomorphism::algorithm::tuple,
		true>(t1, r1, t2, r2);
}

} // namespace utilities
} // namespace lal
