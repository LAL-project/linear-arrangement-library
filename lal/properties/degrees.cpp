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
#include <lal/properties/degrees.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/numeric/rational_output.hpp>

namespace lal {
namespace properties {

template <graphs::Tree tree_t>
numeric::rational hubiness_general_rational(const tree_t& t) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Ferrer2018a);
#endif

	const uint64_t n = t.get_num_nodes();

// for n <= 3, <k^2>_star = <k^2>_linear
// which means that hubiness is not defined:
// division by 0.
#if defined DEBUG
	assert(t.is_tree());
	assert(n > 3);
#endif
	[[assume(n >= 4)]];

	const numeric::rational k2_tree = moment_degree_rational(t, 2);
	const numeric::rational k2_linear = numeric::rational(4 * n - 6, n);
	const numeric::rational k2_star = numeric::rational(n - 1);
	return (k2_tree - k2_linear) / (k2_star - k2_linear);
}

numeric::rational hubiness_rational(const graphs::free_tree& t) noexcept
{
	return hubiness_general_rational(t);
}

numeric::rational hubiness_rational(const graphs::rooted_tree& t) noexcept
{
	return hubiness_general_rational(t);
}

template <graphs::Tree tree_t>
double hubiness_general(const tree_t& t) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Ferrer2018a);
#endif

	const uint64_t n = t.get_num_nodes();

// for n <= 3, <k^2>_star = <k^2>_linear
// which means that hubiness is not defined:
// division by 0.
#if defined DEBUG
	assert(t.is_tree());
	assert(n > 3);
#endif
	[[assume(n >= 4)]];

	const double k2_tree = moment_degree(t, 2);
	const double k2_linear =
		static_cast<double>(4 * n - 6) / static_cast<double>(n);
	const double k2_star = static_cast<double>(n - 1);
	return (k2_tree - k2_linear) / (k2_star - k2_linear);
}

double hubiness(const graphs::free_tree& t) noexcept
{
	return hubiness_general(t);
}

double hubiness(const graphs::rooted_tree& t) noexcept
{
	return hubiness_general(t);
}

} // namespace properties
} // namespace lal
