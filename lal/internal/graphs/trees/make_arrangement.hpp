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
namespace internal {

namespace __lal {
inline void __make_arrangement_intervals(
	const graphs::rooted_tree& T, node r,
	const std::vector<std::vector<lal::node>>& data,
	position& pos, linear_arrangement& arr
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.out_degree(r);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr[r] = pos++;
		return;
	}
	const std::vector<lal::node>& interval = data[r];
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

inline void __make_arrangement_intervals(
	const graphs::free_tree& T, node parent, node r,
	const std::vector<std::vector<lal::node>>& data,
	position& pos, linear_arrangement& arr
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.degree(r) - (r == parent ? 0 : 1);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr[r] = pos++;
		return;
	}
	const std::vector<lal::node>& interval = data[r];
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr[vi] = pos++;
		}
		else {
			__make_arrangement_intervals(T, r, vi, data, pos, arr);
		}
	}
}
} // -- namespace __lal

inline linear_arrangement make_arrangement_intervals(
	const graphs::rooted_tree& T,
	const std::vector<std::vector<lal::node>>& data
)
{
	linear_arrangement arr(T.num_nodes());
	position pos = 0;
	__lal::__make_arrangement_intervals(T, T.get_root(), data, pos, arr);
	return arr;
}

inline linear_arrangement make_arrangement_intervals(
	const graphs::free_tree& T, node root,
	const std::vector<std::vector<lal::node>>& data
)
{
	linear_arrangement arr(T.num_nodes());
	position pos = 0;
	__lal::__make_arrangement_intervals(T, root, root, data, pos, arr);
	return arr;
}

} // -- namespace internal
} // -- namespace lal
