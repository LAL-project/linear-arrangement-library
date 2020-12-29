/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <iostream>
#if defined DEBUG
#include <cassert>
#endif
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/syntactic_dependency_structure_type.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/sorting/bit_sort.hpp>

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
		const edge e = it.get_edge();
		const node s = e.first;
		const node t = e.second;

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
	vector<bool>& vis,
	vector<vector<position>>& yields
)
{
	// add this node to its own yield
	auto& yu = yields[u];
	yu.push_back(pi[u]);

	vis[u] = true;
	if (t.degree(u) == 1) {
		// it is either the root or a leaf
		if (vis[ t.get_neighbours(u)[0] ]) {
			// its only neighbour has been visited --> leaf
			// stop recursion
			return;
		}
	}

	for (node v : t.get_neighbours(u)) {
		if (not vis[v]) {
			__get_yields(t,pi, v, vis, yields);
			const auto& yv = yields[v];
			yu.insert(yu.end(), yv.begin(), yv.end());
		}
	}

	internal::bit_sort(
		yields[u].begin(), yields[u].end(), yields[u].size()
	);
}

#define sort2(a,b) (a < b ? make_pair(a,b) : make_pair(b,a))
inline bool __disjoint_yields(
	const uint32_t n, const vector<vector<position>>& yields
)
{
	bool disjoint_yields = true;
	// for every pair of nodes
	for (node u = 0; u < n; ++u) {
	for (node v = u + 1; v < n; ++v) {
		const auto& yu = yields[u];
		const auto& yv = yields[v];

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

			disjoint_yields =
				(su1 < sv1 and sv1 < su2 and su2 < sv2) or
				(sv1 < su1 and su1 < sv2 and sv2 < su2);

			if (disjoint_yields) { return true; }

		}}}}
	}}
	return false;
}

inline
uint32_t __get_n_discont(const uint32_t n, const vector<vector<node>>& yields)
{
	uint32_t max_dis = 0;
	for (node u = 0; u < n; ++u) {
		const auto& yu = yields[u];
		uint32_t dis = 0;
		for (size_t i = 1; i < yu.size(); ++i) {
			if (yu[i] - yu[i - 1] > 1) {
				++dis;
			}
		}
		max_dis = std::max(max_dis, dis);
	}
	return max_dis;
}

inline uint32_t __is_1EC(const rooted_tree& rT, const linear_arrangement& pi) {
	// use the paper in
	// https://compling.ucdavis.edu/iwpt2017/proceedings/pdf/IWPT12.pdf
	// as a reference for the definition of 1ec

	const uint32_t n = rT.n_nodes();
	node *T = new node[n];
	for (node u = 0; u < n; ++u) {
		T[ pi[u] ] = u;
	}

	bool classified = false;
	bool _1ec = false;

	E_iterator it1(rT);
	while (it1.has_next() and not classified) {
		it1.next();
		// check other edges crossing the current edge
		const node s = it1.get_edge().first;
		const node t = it1.get_edge().second;
		cout << "Current edge: " << s << "," << t << endl;
		const auto [ps, pt] = sort_by_index(pi[s], pi[t]);
		cout << "At position:  " << pi[s] << "," << pi[t] << endl;

		// the edges crossing the current edge
		vector<edge> crossing;

		// iterate over the nodes between the endpoints
		// of 'dep' in the linear arrangement
		for (position r = ps + 1; r <= pt - 1; ++r) {
			const node u = T[r];
			cout << "    inspecting node " << u << " at " << r << endl;

			// check out-neighbours
			for (const node v : rT.get_out_neighbours(u)) {
				cout << "        neighbour " << v << " at " << pi[v] << endl;
				if (pi[v] < ps or pt < pi[v]) {
					// the edge (u,v) crosses (s,t)
					crossing.push_back(sort_by_index(u,v));
					cout << "    edge " << u << "," << v << " crosses current." << endl;
					cout << "    at   " << pi[u] << "," << pi[v] << endl;
				}
			}
			// check in-neighbours
			for (const node v : rT.get_in_neighbours(u)) {
				cout << "        neighbour " << v << " at " << pi[v] << endl;
				if (pi[v] < ps or pt < pi[v]) {
					// the edge (u,v) crosses (s,t)
					crossing.push_back(sort_by_index(u,v));
					cout << "    edge " << u << "," << v << " crosses current." << endl;
					cout << "    at   " << pi[u] << "," << pi[v] << endl;
				}
			}
		}

		if (crossing.size() > 1) {
			cout << "    Edge {" << s << "," << t << "} is crossed by: "
				 << crossing.size() << " edges:"
				 << endl;
			for (const edge& e : crossing) {
			cout << "        " << "{" << e.first << "," << e.second << "}" << endl;
			}

			// compute the number of common nodes among
			// the edges that cross the current edge
			set<node> common_nodes;
			for (size_t i = 0; i < crossing.size(); ++i) {
				const auto [ss,tt] = crossing[i];
				for (size_t j = i + 1; j < crossing.size(); ++j) {
					const auto [uu,vv] = crossing[j];
					if (ss == uu or ss == vv) { common_nodes.insert(ss); }
					if (tt == uu or tt == vv) { common_nodes.insert(tt); }
				}
			}

			// If this tree does not belong to 1-EC,finish.
			// Continue otherwise.
			cout << "    common nodes found: " << common_nodes.size() << endl;
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

	delete[] T;
	return _1ec;
}

inline vector<bool> __get_syn_dep_tree_type(
	const rooted_tree& rT, const linear_arrangement& pi
)
{
#define nullify_none cl[enum_to_sizet(syndepstr_type::none)] = false;

	bool is_some_class = false;
	const auto __set_type =
	[&](vector<bool>& cls, const syndepstr_type& ts) {
		is_some_class = true;
		cls[enum_to_sizet(ts)] = true;

		if (ts == syndepstr_type::projective) {
			cls[enum_to_sizet(syndepstr_type::planar)] = true;
			cls[enum_to_sizet(syndepstr_type::EC1)] = true;
			cls[enum_to_sizet(syndepstr_type::WG1)] = true;
		}
		else if (ts == syndepstr_type::planar) {
			cls[enum_to_sizet(syndepstr_type::EC1)] = true;
			cls[enum_to_sizet(syndepstr_type::WG1)] = true;
		}
	};

	vector<bool> cl(__tree_structure_size, false);
	cl[static_cast<size_t>(syndepstr_type::none)] = true;
	if (rT.n_nodes() <= 2) {
		__set_type(cl, syndepstr_type::projective);
		nullify_none;
		return cl;
	}

	// for the case of n <= 3, C is trivially 0
	const bool is_root_covered = __is_root_covered(rT, pi);
	if (rT.n_nodes() == 3) {
		const auto t =
			is_root_covered ?
			syndepstr_type::planar : syndepstr_type::projective;
		__set_type(cl, t);
		nullify_none;
		return cl;
	}

	const uint32_t C = n_crossings(rT, pi);
	cout << "C= " << C << endl;

	// If C=0 then the structure is either projective or planar
	if (C == 0) {
		__set_type(cl,
		__is_root_covered(rT, pi) ?
			syndepstr_type::planar :
			syndepstr_type::projective
		);
		nullify_none;
		return cl;
	}

	// ++++++++++++++++++++++++
	// non-projective structure

	// if C=1 then the structure is 1-Endpoint Crossing
	if (C == 1) {
		__set_type(cl, syndepstr_type::EC1);
	}

	// ---------------------------------------------------
	// is the structure Well-Nest of Gap degree at most 1?

	// compute the yield of each node
	const uint32_t n = rT.n_nodes();
	vector<vector<position>> yields(n);
	vector<bool> vis(n, false);

	__get_yields(rT,pi, rT.get_root(), vis, yields);

	// are the yields non-interleaving?
	const bool disjoint_yields = __disjoint_yields(n, yields);

	// discontinuities in the yields
	const uint32_t max_dis = (disjoint_yields ? __get_n_discont(n, yields) : 0);

	// this structure is well-nested
	if (disjoint_yields and max_dis > 0) {
		// of gap degree at most 1?
		if (max_dis == 1) {
			__set_type(cl, syndepstr_type::WG1);
		}
	}

	// ---------------------------------------------------
	// is the structure 1-Endpoint Crossing?

	if (__is_1EC(rT, pi)) {
		__set_type(cl, syndepstr_type::EC1);
	}

	if (is_some_class) {
		nullify_none;
	}
	return cl;
}

vector<bool> classify_tree_structure(const rooted_tree& rT, const linear_arrangement& pi) {
#if defined DEBUG
	assert(rT.is_rooted_tree());
#endif
	return internal::call_with_empty_arrangement(__get_syn_dep_tree_type, rT, pi);
}

} // -- namespace linarr
} // -- namespace lal
