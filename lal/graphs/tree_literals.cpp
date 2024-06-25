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

#include <lal/graphs/tree_literals.hpp>

// C++ includes
#include <sstream>

// lal includes
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/conversions.hpp>

template <
	typename tree_t,
	bool ensure_root_is_returned,
	bool free_tree_plus_root =
		ensure_root_is_returned and
		std::is_same_v<tree_t, lal::graphs::free_tree>
>
std::conditional_t<
	free_tree_plus_root,
	std::pair<tree_t, lal::node>,
	tree_t
>
parse_literal_string(const char *str, std::size_t) noexcept
{
	std::stringstream ss(str);

	const bool is_first_nonspace_character_a_curly_bracket =
	[&]() {
		char first_char;
		ss >> first_char;
		return first_char == '{';
	}();
	ss.seekg(0);

	if (is_first_nonspace_character_a_curly_bracket) {
		// The input is an edge list.
		return lal::detail::from_edge_list_to_tree
				<tree_t, ensure_root_is_returned>(ss);
	}

	// The input must be a head vector by
	// the requirements of this function.
	return lal::detail::from_head_vector_to_tree
			<tree_t, ensure_root_is_returned>(ss);
}

std::pair<lal::graphs::free_tree, lal::node>
operator""_root_free_tree (const char *str, std::size_t length)
noexcept
{
	return parse_literal_string<lal::graphs::free_tree, true>(str, length);
}

lal::graphs::free_tree
operator""_free_tree (const char *str, std::size_t length)
noexcept
{
	return parse_literal_string<lal::graphs::free_tree, false>(str, length);
}

lal::graphs::rooted_tree
operator""_rooted_tree (const char *str, std::size_t length)
noexcept
{
	return parse_literal_string<lal::graphs::rooted_tree, false>(str, length);
}
