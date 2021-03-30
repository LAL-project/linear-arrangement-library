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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <array>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/syntactic_dependency_structure_type.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/sorting/bit_sort.hpp>
#include <lal/internal/data_array.hpp>
#include <lal/internal/make_array_with_value.hpp>

#define sort_by_index(a,b) (a < b ? make_pair(a,b) : make_pair(b,a))
#define sort_by_pos(a,b, P) (P[a] < P[b] ? make_pair(a,b) : make_pair(b,a))
#define enum_to_sizet(e) static_cast<size_t>(e)

namespace lal {
using namespace graphs;
using namespace iterators;

namespace linarr {

typedef syntactic_dependency_structure_type syndepstr_type;

inline bool __is_root_covered(const rooted_tree& T, const linear_arrangement& pi) {
	const node R = T.get_root();
	E_iterator it(T);
	while (it.has_next()) {
		it.next();
		const auto [s,t] = it.get_edge();
		const bool covered =
			(pi[s] < pi[R] and pi[R] < pi[t]) or
			(pi[t] < pi[R] and pi[R] < pi[s]);

		// the root is covered
		if (covered) {
			return true;
		}
	}
	return false;
}

inline void __get_yields(
	const rooted_tree& t, const linear_arrangement& pi,
	node u,
	vector<vector<position>>& yields
)
{
	// add this node to its own yield
	auto& yu = yields[u];
	yu.push_back(pi[u]);

	for (node v : t.get_out_neighbours(u)) {
		__get_yields(t,pi, v, yields);
		const auto& yv = yields[v];
		yu.insert(yu.end(), yv.begin(), yv.end());
	}

	internal::bit_sort<node, vector<node>::iterator>
	(yields[u].begin(), yields[u].end(), yields[u].size());
}

#define sort2(a,b) (a < b ? make_pair(a,b) : make_pair(b,a))
inline bool __are_yields_wellnested(
	const uint32_t n, const vector<vector<position>>& yields
)
{
	// for every pair of nodes
	for (node u = 0; u < n; ++u) {
	for (node v = u + 1; v < n; ++v) {
		const auto& yu = yields[u];
		const auto& yv = yields[v];

		// ensure the yields have empty intersection
		vector<position> inter;
		std::set_intersection(
			yu.begin(), yu.end(),
			yv.begin(), yv.end(),
			std::back_inserter(inter)
		);
		if (inter.size() > 0) {
			// ignore intersecting yeilds
			continue;
		}

		// ensure the yields do not 'cross'
		for (size_t iu_1 = 0;        iu_1 < yu.size(); ++iu_1) {
		const position u1 = yu[iu_1];
		for (size_t iu_2 = iu_1 + 1; iu_2 < yu.size(); ++iu_2) {
		const position u2 = yu[iu_2];
		// sorted values u1,u2
		const auto [su1,su2] = sort_by_index(u1, u2);

		for (size_t iv_1 = 0;        iv_1 < yv.size(); ++iv_1) {
		const position v1 = yv[iv_1];
		for (size_t iv_2 = iv_1 + 1; iv_2 < yv.size(); ++iv_2) {
		const position v2 = yv[iv_2];
		// sorted values v1,v2
		const auto [sv1,sv2] = sort_by_index(v1, v2);

			const bool edges_cross =
				(su1 < sv1 and sv1 < su2 and su2 < sv2) or
				(sv1 < su1 and su1 < sv2 and sv2 < su2);

			if (edges_cross) { return false; }

		}}}}
	}}
	return true;
}

inline
uint32_t __get_n_discont(const uint32_t n, const vector<vector<node>>& yields)
{
	uint32_t max_g = 0;
	for (node u = 0; u < n; ++u) {
		const auto& yu = yields[u];
		uint32_t g = 0;
		for (size_t i = 1; i < yu.size(); ++i) {
			if (yu[i] - yu[i - 1] > 1) {
				++g;
			}
		}
		max_g = std::max(max_g, g);
	}
	return max_g;
}

inline
bool __is_WG1(const rooted_tree& rT, const linear_arrangement& pi) {
	const uint32_t n = rT.get_num_nodes();

	// compute the yield of each node
	vector<vector<position>> yields(n);
	__get_yields(rT,pi, rT.get_root(), yields);

	// test whether the tree is well nested
	const bool is_well_nested = __are_yields_wellnested(n, yields);

	// calculate degree of discontinuities in the yields
	const uint32_t max_dis = (is_well_nested ? __get_n_discont(n, yields) : 0);

	return (is_well_nested and max_dis > 0 ? max_dis == 1 : false);
}

// The input tree has an "artificial" vertex pointing to the root of the
// actual (input) tree. This artificial vertex was added to the arrangement.
inline uint32_t __is_1EC(const rooted_tree& rT, const linear_arrangement& pi) {
	// use the paper in
	// https://compling.ucdavis.edu/iwpt2017/proceedings/pdf/IWPT12.pdf
	// as a reference for the definition of 1ec

	const uint32_t n = rT.get_num_nodes();
	internal::data_array<node> T(n);
	for (node u = 0; u < n; ++u) {
		T[ pi[u] ] = u;
	}

	bool classified = false;
	bool _1ec = false;

	E_iterator it(rT);
	while (it.has_next() and not classified) {
		it.next();
		// check other edges crossing the current edge
		const auto [s,t] = it.get_edge();
		const auto [ps, pt] = sort_by_index(pi[s], pi[t]);

		// the edges crossing the current edge
		vector<edge> crossing;

		// iterate over the nodes between the endpoints
		// of 'dep' in the linear arrangement
		for (position r = ps + 1; r <= pt - 1; ++r) {
			const node u = T[r];

			neighbourhood neighs_u = rT.get_out_neighbours(u);
			if (u != rT.get_root()) {
				neighs_u.push_back(rT.get_in_neighbours(u)[0]);
			}

			// check neighbours
			for (const node v : neighs_u) {
				if (pi[v] < ps or pt < pi[v]) {
					// the edge (u,v) crosses (s,t)
					crossing.push_back(sort_by_index(u,v));
				}
			}
		}

		if (crossing.size() == 1) {
			// only one edge
			_1ec = true;
		}
		else if (crossing.size() >= 2) {

			// compute the number of common nodes among
			// the edges that cross the current edge
			set<node> common_nodes;
			for (size_t i = 0; i < crossing.size() and not classified; ++i) {
				const auto [ss,tt] = crossing[i];
				for (size_t j = i + 1; j < crossing.size() and not classified; ++j) {
					const auto [uu,vv] = crossing[j];

					// if the intersection is empty then the tree is certainly
					// NOT 1-EC
					const int size_intersection =
						ss == uu or ss == vv or tt == uu or tt == vv;

					if (size_intersection == 0) {
						_1ec = false;
						classified = true;
					}
					else {
						if (ss == uu or ss == vv) { common_nodes.insert(ss); }
						if (tt == uu or tt == vv) { common_nodes.insert(tt); }
					}
				}
			}

			if (not classified) {
				// If this tree does not belong to 1-EC,finish.
				// Continue otherwise.
				if (common_nodes.size() == 1) {
					_1ec = true;
				}
				else {
					// There are 2 or more common nodes.
					// This class is not defined.
					_1ec = false;
					classified = true;
				}
			}
		}
	}

	return _1ec;
}

inline array<bool, __tree_structure_type_size>
__get_syn_dep_tree_type
(const rooted_tree& rT, const linear_arrangement& pi)
{
#define nullify(X) cl[enum_to_sizet(syndepstr_type::X)] = false;

	bool is_some_class = false;
	array<bool, __tree_structure_type_size> cl =
		internal::make_array_with_value
		<bool, __tree_structure_type_size, false>();

	cl[static_cast<size_t>(syndepstr_type::unknown)] = true;

	const auto __set_type = [&](const syndepstr_type& ts) {
		is_some_class = true;
		cl[enum_to_sizet(ts)] = true;

		if (ts == syndepstr_type::projective) {
			cl[enum_to_sizet(syndepstr_type::planar)] = true;
			cl[enum_to_sizet(syndepstr_type::EC1)] = true;
			cl[enum_to_sizet(syndepstr_type::WG1)] = true;
		}
		else if (ts == syndepstr_type::planar) {
			cl[enum_to_sizet(syndepstr_type::EC1)] = true;
			cl[enum_to_sizet(syndepstr_type::WG1)] = true;
		}
	};

	const uint32_t n = rT.get_num_nodes();
	if (n == 0) { return cl; }

	// -------------------------------------------------------------------------

	// add an artificial vertex first
	rooted_tree _rT(1);
	_rT.set_root(0);
	_rT.disjoint_union(rT);
#if defined DEBUG
	_rT.find_edge_orientation();
	assert(_rT.is_rooted_tree());
#endif
	// update the linear arrangement
	linear_arrangement _pi(pi.size() + 1);
	if (_pi.size() > 0) {
		_pi[0] = 0;
	}
	std::copy(pi.begin(), pi.end(), _pi.begin() + 1);
	std::transform(
		_pi.begin() + 1, _pi.end(),
		_pi.begin() + 1,
		[](const position p) -> position { return p + 1; }
	);

	// -------------------------------------------------------------------------
	// classify small trees
	if (n <= 2) {
		__set_type(syndepstr_type::projective);
		nullify(unknown);
		return cl;
	}

	const bool is_root_covered = __is_root_covered(rT, pi);

	// the case of n == 3, C is trivially 0, and we only need to test
	// whether the root is covered or not.
	if (n == 3) {
		const auto t =
			is_root_covered ?
			syndepstr_type::planar : syndepstr_type::projective;

		__set_type(t);
		nullify(unknown);
		return cl;
	}

	// -------------------------------------------------------------------------
	// n >= 4
	// from this point on we need an artificial vertex pointing to the
	// root of the input tree

	const uint32_t C = number_of_crossings(rT, pi);

	// If C=0 then the structure is either projective or planar
	if (C == 0) {
		__set_type(
			__is_root_covered(rT, pi) ?
			syndepstr_type::planar : syndepstr_type::projective
		);

		// remove 1-ec from the types when needed
		const uint32_t _C = number_of_crossings(_rT, _pi);
		if (_C > 0 and not __is_1EC(_rT, _pi)) {
			nullify(EC1);
		}

		nullify(unknown);
		return cl;
	}

	// +++++++++++++++++++++++++
	// non-projective structures

	// ---------------------------------------------------
	// is the structure Well-Nest of Gap degree at most 1?

	if (__is_WG1(rT, pi)) {
		__set_type(syndepstr_type::WG1);
	}

	// ---------------------------------------------------
	// is the structure 1-Endpoint Crossing?

	if (__is_1EC(_rT, _pi)) {
		__set_type(syndepstr_type::EC1);
	}

	if (is_some_class) {
		nullify(unknown);
	}
	return cl;
}

array<bool, __tree_structure_type_size>
classify_tree_structure(const rooted_tree& rT, const linear_arrangement& pi) {
#if defined DEBUG
	assert(rT.is_rooted_tree());
#endif
	return internal::call_with_empty_arrangement(__get_syn_dep_tree_type, rT, pi);
}

} // -- namespace linarr
} // -- namespace lal
