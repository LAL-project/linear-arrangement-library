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

#include <lal/linarr/Dmin.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/linarr/D.hpp>
#include <lal/utils/sorting/counting_sort.hpp>
#include <lal/utils/graphs/trees/make_projecitve_arr.hpp>

typedef pair<lal::node,uint32_t> nodesize;

namespace lal {
using namespace graphs;
using namespace utils;

namespace linarr {

void place_subtrees_of(
	const rtree& t, node r, vector<projective_interval>& data
)
{
	const uint32_t d_out = t.out_degree(r);
	projective_interval& interval = data[r];
	interval = projective_interval(d_out + 1);

	if (d_out == 0) {
		// base case -- vertex 'r' is a leaf
		data[r][0] = 0;
		return;
	}

	// get the sizes of the subtrees
	uint32_t max_size = 0;
	vector<nodesize> children(d_out);
	for (size_t i = 0; i < t.out_degree(r); ++i) {
		const node vi = t.get_out_neighbours(r)[i];
		children[i].first = vi;
		children[i].second = t.n_nodes_subtree(vi);
		if (max_size < children[i].second) {
			max_size = children[i].second;
		}
	}

	// sort the sizes
	typedef vector<nodesize>::iterator it;
	auto key = [](const nodesize& v) -> size_t {
		return static_cast<size_t>(v.second);
	};
	utils::counting_sort<it, nodesize>
	(children.begin(), children.end(), max_size, key);

	// place the children
	size_t leftpos = 0;
	size_t rightpos = data[r].size() - 1;
	bool left = true;
	for (int32_t i = static_cast<int32_t>(d_out); i >= 0; --i) {
		if (left) {
			left = false;
			interval[leftpos] = children[i].first;
			++leftpos;
		}
		else {
			left = true;
			interval[rightpos] = children[i].first;
			--rightpos;
		}
	}

	// Place 'r'.
	// NOTE: 'leftpos' and 'rightpos' must be equal
	assert(leftpos == rightpos);
	interval[leftpos] = r;
}

pair<uint32_t, linearrgmnt> compute_Dmin_Projective(const rtree& t) {
	assert(t.is_tree());
	assert(t.has_root());
	assert(t.rtree_type_valid() and
		   t.get_rtree_type() == rtree::rtree_type::arborescence);
	assert(not t.need_recalc_size_subtrees());

	linearrgmnt arr(t.n_nodes());

	vector<projective_interval> data(t.n_nodes());
	for (node u = 0; u < t.n_nodes(); ++u) {
		place_subtrees_of(t, u, data);
	}

	utils::put_in_arrangement(t, data, arr);

	const uint32_t D = sum_length_edges(t, arr);
	return make_pair(D, arr);
}

} // -- namespace linarr
} // -- namespace lal
