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

// C++ includes
#include <vector>

// lal includes
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace detail {

// -- for rooted trees

namespace __lal {

template<
	template<typename... Args> class oContainer,
	template<typename... Args> class iContainer, typename... iTypes
>
inline
void __make_arrangement_intervals(
	const graphs::rooted_tree& T, node r,
	const oContainer<iContainer<iTypes...>>& data,
	position& pos, linear_arrangement& arr
)
noexcept
{
	// number of children of 'r' with respect to the tree's root
	const uint64_t d_out = T.get_out_degree(r);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr[r] = pos++;
		return;
	}
	const auto& interval = data[r];
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr[vi] = pos++;
		}
		else {
			__make_arrangement_intervals(T, vi, data, pos, arr);
		}
	}
}

} // namespace __lal

template<
	template<typename... Args> class oContainer,
	template<typename... Args> class iContainer, typename... iTypes
>
inline
linear_arrangement make_arrangement_intervals(
	const graphs::rooted_tree& T,
	const oContainer<iContainer<iTypes...>>& data
)
noexcept
{
	linear_arrangement arr(T.get_num_nodes());
	position pos = 0;
	__lal::__make_arrangement_intervals(T, T.get_root(), data, pos, arr);
	return arr;
}

// -----------------------------------------------------------------------------
// -- for free trees

namespace __lal {

template<
	template<typename... Args> class oContainer,
	template<typename... Args> class iContainer, typename... iTypes
>
inline
void __make_arrangement_intervals(
	const graphs::free_tree& T, node parent, node u,
	const oContainer<iContainer<iTypes...>>& data,
	position& pos, linear_arrangement& arr
)
noexcept
{
	// number of children of 'u' with respect to the tree's root
	const uint64_t d_out = T.get_degree(u) - (u == parent ? 0 : 1);

	// vertex 'u' is a leaf in the rooted version of T
	if (d_out == 0) {
		arr[u] = pos++;
		return;
	}
	const auto& interval = data[u];
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == u) {
			arr[vi] = pos++;
		}
		else {
			__make_arrangement_intervals(T, u, vi, data, pos, arr);
		}
	}
}
} // -- namespace __lal

template<
	template<typename... Args> class oContainer,
	template<typename... Args> class iContainer, typename... iTypes
>
inline
linear_arrangement make_arrangement_intervals(
	const graphs::free_tree& T, node root,
	const oContainer<iContainer<iTypes...>>& data
)
noexcept
{
	linear_arrangement arr(T.get_num_nodes());
	position pos = 0;
	__lal::__make_arrangement_intervals(T, root, root, data, pos, arr);
	return arr;
}

} // -- namespace detail
} // -- namespace lal
