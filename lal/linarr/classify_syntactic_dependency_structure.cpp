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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <array>
#include <set>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/linarr/syntactic_dependency_structure.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/macros/call_with_empty_arr.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/make_array.hpp>

#define sort_by_index(a,b) (a < b ? std::make_pair(a,b) : std::make_pair(b,a))
#define sort_by_pos(a,b, P) (P[a] < P[b] ? std::make_pair(a,b) : std::make_pair(b,a))
#define enum_to_sizet(e) static_cast<std::size_t>(e)

namespace lal {
namespace linarr {

typedef syntactic_dependency_structure syndepstr_type;

void __get_yields(
	const graphs::rooted_tree& t, const linear_arrangement& pi,
	node u,
	std::vector<std::vector<position>>& yields
)
noexcept
{
	// add this node to its own yield
	auto& yu = yields[u];
	yu.push_back(pi[node_t{u}]);

	for (node v : t.get_out_neighbours(u)) {
		__get_yields(t,pi, v, yields);
		const auto& yv = yields[v];
		yu.insert(yu.end(), yv.begin(), yv.end());
	}

	detail::sorting::bit_sort<node, std::vector<node>::iterator>
	(yields[u].begin(), yields[u].end(), yields[u].size());
}

#define sort2(a,b) (a < b ? make_pair(a,b) : make_pair(b,a))

bool __are_yields_wellnested
(const uint64_t n, const std::vector<std::vector<position>>& yields)
noexcept
{
	// for every pair of nodes
	for (node u = 0; u < n; ++u) {
	for (node v = u + 1; v < n; ++v) {
		const auto& yu = yields[u];
		const auto& yv = yields[v];

		// ensure the yields have empty intersection
		std::vector<position> inter;
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
		for (std::size_t iu_1 = 0;        iu_1 < yu.size(); ++iu_1) {
		const position u1 = yu[iu_1];
		for (std::size_t iu_2 = iu_1 + 1; iu_2 < yu.size(); ++iu_2) {
		const position u2 = yu[iu_2];
		// sorted values u1,u2
		const auto [su1,su2] = sort_by_index(u1, u2);

		for (std::size_t iv_1 = 0;        iv_1 < yv.size(); ++iv_1) {
		const position v1 = yv[iv_1];
		for (std::size_t iv_2 = iv_1 + 1; iv_2 < yv.size(); ++iv_2) {
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

uint64_t __get_n_discont
(const uint64_t n, const std::vector<std::vector<node>>& yields)
noexcept
{
	uint64_t max_g = 0;
	for (node u = 0; u < n; ++u) {
		const auto& yu = yields[u];
		uint64_t g = 0;
		for (std::size_t i = 1; i < yu.size(); ++i) {
			if (yu[i] - yu[i - 1] > 1) {
				++g;
			}
		}
		max_g = std::max(max_g, g);
	}
	return max_g;
}

bool __is_WG1(const graphs::rooted_tree& rT, const linear_arrangement& pi)
noexcept
{
	const uint64_t n = rT.get_num_nodes();

	// compute the yield of each node
	std::vector<std::vector<position>> yields(n);
	__get_yields(rT,pi, rT.get_root(), yields);

	// test whether the tree is well nested
	const bool is_well_nested = __are_yields_wellnested(n, yields);

	// calculate degree of discontinuities in the yields
	const uint64_t max_dis = (is_well_nested ? __get_n_discont(n, yields) : 0);

	return (is_well_nested and max_dis > 0 ? max_dis == 1 : false);
}

// The input tree has an "artificial" vertex pointing to the root of the
// actual (input) tree. This artificial vertex was added to the arrangement.
uint64_t __is_1EC(const graphs::rooted_tree& rT, const linear_arrangement& pi)
noexcept
{
	// use the paper in
	// https://compling.ucdavis.edu/iwpt2017/proceedings/pdf/IWPT12.pdf
	// as a reference for the definition of 1ec

	bool classified = false;
	bool _1ec = false;

	iterators::E_iterator e_it(rT);
	while (not e_it.end() and not classified) {
		// check other edges crossing the current edge
		const auto [s,t] = e_it.get_edge_t();
		e_it.next();

		const auto [ps, pt] = sort_by_index(pi[s], pi[t]);

		// the edges crossing the current edge
		std::vector<edge> crossing;

		// iterate over the nodes between the endpoints
		// of 'dep' in the linear arrangement
		for (position_t pu = ps + 1; pu <= pt - 1; ++pu) {
			const node u = pi[pu];

			neighbourhood neighs_u = rT.get_out_neighbours(u);
			if (u != rT.get_root()) {
				neighs_u.push_back(rT.get_in_neighbours(u)[0]);
			}

			// check neighbours
			for (const node_t v : neighs_u) {
				if (pi[v] < ps or pt < pi[v]) {
					// the edge (u,v) crosses (s,t)
					crossing.push_back(sort_by_index(u,*v));
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
			std::set<node> common_nodes;
			for (std::size_t i = 0; i < crossing.size() and not classified; ++i) {
				const auto [ss,tt] = crossing[i];
				for (std::size_t j = i + 1; j < crossing.size() and not classified; ++j) {
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

std::array<bool, __syntactic_dependency_structure_size>
__get_syn_dep_tree_type
(
	const graphs::rooted_tree& rT,
	const linear_arrangement& pi,
	const uint64_t C
)
noexcept
{
#define nullify(X) cl[enum_to_sizet(syndepstr_type::X)] = false;

	bool is_some_class = false;
	std::array<bool, __syntactic_dependency_structure_size> cl =
		detail::make_array_with_value
		<bool, __syntactic_dependency_structure_size, false>();

	cl[static_cast<std::size_t>(syndepstr_type::unknown)] = true;

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

	const uint64_t n = rT.get_num_nodes();
	if (n == 0) { return cl; }

	// -------------------------------------------------------------------------
	// classify small trees
	if (n <= 2) {
		__set_type(syndepstr_type::projective);
		nullify(unknown);
		return cl;
	}

	// the case of n == 3, C is trivially 0, and we only need to test
	// whether the root is covered or not.
	if (n == 3) {
		const auto t =
			is_root_covered(rT, pi) ?
			syndepstr_type::planar : syndepstr_type::projective;

		__set_type(t);
		nullify(unknown);
		return cl;
	}

	// -------------------------------------------------------------------------
	// n >= 4
	// from this point on we need an artificial vertex pointing to the
	// root of the input tree

	// add an artificial vertex first
	graphs::rooted_tree _rT(1);
	_rT.set_root(0);
	_rT.disjoint_union(rT);
#if defined DEBUG
	assert(_rT.is_rooted_tree());
#endif

	// update the linear arrangement
	linear_arrangement _pi(pi.size() + 1);
	if (_pi.size() > 0) {
		_pi.assign(0ull, 0ull);
	}
	for (node u = 0; u < n; ++u) {
		_pi.assign(u + 1, pi[node_t{u}] + 1);
	}

	// +++++++++++++++++++++++++
	// projective structures

	// If C=0 then the structure is either projective or planar
	if (C == 0) {
		__set_type(
			is_root_covered(rT, pi) ?
			syndepstr_type::planar : syndepstr_type::projective
		);

		// calculate the number of crossings taking into account
		// the edge from the artificial root to the root of the
		// original tree
		uint64_t _C = C;
		{
		const node only_child = _rT.get_out_neighbours(0)[0];
		const position poc = _pi[node_t{only_child}];

		iterators::E_iterator eit(_rT);
		while (not eit.end()) {
			const auto [u,v] = eit.yield_edge_t();
			const position pu = _pi[u];
			const position pv = _pi[v];
			if (pu < pv) {
				_C += 0 < pu and pu < poc and poc < pv;
			}
			else {
				_C += 0 < pv and pv < poc and poc < pu;
			}
		}
		}

		// remove 1-ec from the types when needed
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

std::array<bool, __syntactic_dependency_structure_size>
syntactic_dependency_structure_class
(
	const graphs::rooted_tree& rT,
	const uint64_t C,
	const linear_arrangement& pi
)
noexcept
{
#if defined DEBUG
	assert(rT.is_rooted_tree());
#endif
	return detail::call_with_empty_arrangement(__get_syn_dep_tree_type, rT, pi, C);
}

std::array<bool, __syntactic_dependency_structure_size>
syntactic_dependency_structure_class
(const graphs::rooted_tree& rT, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(rT.is_rooted_tree());
#endif
	const uint64_t C = rT.get_num_nodes() >= 4 ? num_crossings(rT, pi) : 0;
	return detail::call_with_empty_arrangement(__get_syn_dep_tree_type, rT, pi, C);
}

} // -- namespace linarr
} // -- namespace lal
