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

#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/detail/arrangement_wrapper.hpp>
#include <lal/detail/linarr/headedness.hpp>

namespace lal {
namespace linarr {

#define lal_ident_arr(arr) detail::identity_arr(arr)
#define lal_nonident_arr(arr) detail::nonidentity_arr(arr)

numeric::rational head_initial_rational(
	const graphs::directed_graph& g, const linear_arrangement& arr
) noexcept
{
#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	return (
		arr.size() == 0
			? detail::head_initial<numeric::rational>(g, lal_ident_arr(arr))
			: detail::head_initial<numeric::rational>(g, lal_nonident_arr(arr))
	);
}

double head_initial(
	const graphs::directed_graph& g, const linear_arrangement& arr
) noexcept
{
#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	return (
		arr.size() == 0 ? detail::head_initial<double>(g, lal_ident_arr(arr))
						: detail::head_initial<double>(g, lal_nonident_arr(arr))
	);
}

} // namespace linarr
} // namespace lal
