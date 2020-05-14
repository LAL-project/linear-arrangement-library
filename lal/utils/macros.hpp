/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
#include <functional>
#include <numeric>

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace utils {

// Conversion functions
template<typename In, typename Out = uint32_t>
inline constexpr Out to_uint32(const In& x) { return static_cast<Out>(x); }
template<typename In, typename Out = int32_t>
inline constexpr Out to_int32(const In& x) { return static_cast<Out>(x); }

template<typename In, typename Out = uint64_t>
inline constexpr Out to_uint64(const In& x) { return static_cast<Out>(x); }
template<typename In, typename Out = int64_t>
inline constexpr Out to_int64(const In& x) { return static_cast<Out>(x); }

template<typename In, typename Out = double>
inline constexpr Out to_double(const In& x) { return static_cast<Out>(x); }

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
template<typename T, class G>
T call_with_empty_arrangement(
	T (*F)(const G&, const linearrgmnt&),
	const G& g, const linearrgmnt& pi
)
{
	if (pi.size() != 0) {
		return F(g,pi);
	}
	linearrgmnt __pi(g.n_nodes());
	std::iota(__pi.begin(), __pi.end(), 0);
	return F(g,__pi);
}

/* @brief Retrieves the neighbours of a node in a graph as a list of
 * boolean values.
 *
 * Sets to true the positions in @e neighs that correspond to the nodes
 * neighours of @e u.
 * @param g Input graph.
 * @param u Input node.
 * @param neighs Boolean list of neighbours of @e u in @e g.
 * @pre The contents of @e neighs must be all 0 (or false).
 */
template<class G>
void get_bool_neighbours(const G& g, node u, std::vector<bool>& neighs) {
	for (const node v : g.get_neighbours(u)) {
		neighs[v] = true;
	}
}

} // -- namespace macros
} // -- namespace lal
