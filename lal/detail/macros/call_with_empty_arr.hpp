/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <numeric>

// lal includes
#include <lal/linear_arrangement.hpp>

namespace lal {
namespace detail {

/**
 * @brief Call a function @e F that does not admit empty arrangements.
 *
 * In case the arrangement @e arr is empty, function @e F is passed the
 * identity arrangement.
 *
 * @tparam result_t The type of the function's reuslt.
 * @tparam graph_t The type of input graph of the function @e F.
 * @tparam Params The remaining parameters of the function.
 * @param F Function to call.
 * @param g Input graph.
 * @param arr Arrangement.
 * @param P Remaining parameters of the function F.
 * @returns The value function @e F returns.
 */
template <typename result_t, typename graph_t, typename... Params>
result_t call_with_empty_arrangement(
	result_t (*F)(const graph_t&, const linear_arrangement&, Params...),
	const graph_t& g, const linear_arrangement& arr, Params... P
)
noexcept
{
	if (arr.size() != 0) {
		return F(g,arr,P...);
	}
	const auto n = g.get_num_nodes();
	return F(g, linear_arrangement::identity(n), P...);
}

} // -- namespace detail
} // -- namespace lal
