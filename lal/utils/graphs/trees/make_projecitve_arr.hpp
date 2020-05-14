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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/graphs/rtree.hpp>

namespace lal {
namespace utils {

namespace __lal {
inline void __put_in_arrangement(
	const graphs::rtree& T, node r,
	const std::vector<std::vector<lal::node>>& data,
	uint32_t& pos, linearrgmnt& arr
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.degree(r);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr[r] = pos;
		pos += 1;
		return;
	}
	const std::vector<lal::node>& interval = data[r];
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr[vi] = pos;
			pos += 1;
		}
		else {
			__put_in_arrangement(T, vi, data, pos, arr);
		}
	}
}
} // -- namespace __lal

inline linearrgmnt put_in_arrangement(
	const graphs::rtree& T,
	const std::vector<std::vector<lal::node>>& data
)
{
	linearrgmnt arr(T.n_nodes());
	uint32_t pos = 0;
	__lal::__put_in_arrangement(T, T.get_root(), data, pos, arr);
	return arr;
}

} // -- namespace utils
} // -- namespace lal
