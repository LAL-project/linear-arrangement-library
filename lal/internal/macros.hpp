/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace internal {

// It type 'It' a pointer to type or an iterator whose value type is T?
template<typename T, typename It>
struct is_pointer_iterator {
	static constexpr bool value =
	std::is_same_v<typename std::iterator_traits<It>::value_type, T> ||
	std::is_same_v<It, T*>;
};
// value of 'is_pointer_iterator'
template<typename T, typename It>
inline constexpr bool is_pointer_iterator_v = is_pointer_iterator<T, It>::value;

// Function used to tell the compiler what is not used.
template<class T>
inline void UNUSED(const T& x) { (void)x; }

/*
 * @brief Call a function @e F that does not admit empty arrangements.
 *
 * In case the arrangement @e pi is empty, function @e F is passed the
 * identity arrangement.
 * @param F Function to call.
 * @param g Input graph.
 * @param pi Arrangement.
 * @return Returns the value function @e F returns.
 */
template<typename result_t, typename graph_t>
result_t call_with_empty_arrangement(
	result_t (*F)(const graph_t&, const linear_arrangement&),
	const graph_t& g, const linear_arrangement& pi
)
{
	if (pi.size() != 0) {
		return F(g,pi);
	}
	linear_arrangement __pi(g.n_nodes());
	{
	position p = 0;
	auto it = __pi.begin();
	while (it != __pi.end()) { *(it++) = p++; }
	}
	return F(g,__pi);
}

} // -- namespace internal
} // -- namespace lal
