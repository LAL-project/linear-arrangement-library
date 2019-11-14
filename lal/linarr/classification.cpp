/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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

#include <lal/linarr/classification.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>
#include <lal/utils/sort_integers.hpp>
#include <lal/linarr/C.hpp>
#include <lal/iterators/edge_iterator.hpp>

#define sort2(a,b) (a < b ? make_pair(a,b) : make_pair(b,a))

namespace lal {
using namespace graphs;
using namespace iterators;

namespace linarr {

inline bool __is_root_covered(const urtree& T, const vector<position>& pi) {
	const node R = T.get_root();
	iterators::edge_iterator it(T);
	while (it.has_next()) {
		it.next();
		edge e = it.get_edge();
		const node s = e.first;
		const node t = e.second;
		if (pi[s] < pi[R] and pi[R] < pi[t]) {
			// the root is covered
			return true;
		}
		else if (pi[t] < pi[R] and pi[R] < pi[s]) {
			// the root is covered
			return true;
		}
	}
	return false;
}

void __get_yields(
	const urtree& t, node u,
	vector<bool>& vis,
	vector<vector<node> >& yields
)
{
	vis[u] = true;
	if (t.degree(u) == 1) {
		// it is either the root or a leaf
		if (vis[ t.get_neighbours(u)[0] ]) {
			// its only neighbour has been visited --> leaf
			// nothing else to do, stop recursion
			return;
		}
	}

	for (node v : t.get_neighbours(u)) {
		if (not vis[v]) {
			__get_yields(t, v, vis, yields);
			auto& yu = yields[u];
			auto& yv = yields[v];
			yu.push_back(v);
			yu.insert(yu.end(), yv.begin(), yv.end());
		}
	}

	utils::sort_1_n_inc(yields[u].begin(), yields[u].end());
}

inline bool __disjoint_yields(
	const uint64_t n,
	const vector<vector<node> >& yields,
	const vector<position>& pi
)
{
	bool disjoint_yields = true;
	for (node u = 0; u < n; ++u) {
	for (node v = u + 1; v < n; ++v) {
		if (yields[u].size() == 0 or yields[v].size() == 0) { continue; }

		for (size_t iu_1 = 0; iu_1 < yields[u].size(); ++iu_1) {
		const node u1 = yields[u][iu_1];
		for (size_t iu_2 = iu_1 + 1; iu_2 < yields[u].size(); ++iu_2) {
		const node u2 = yields[u][iu_2];
		for (size_t iv_1 = 0; iv_1 < yields[v].size(); ++iv_1) {
		const node v1 = yields[v][iv_1];
		for (size_t iv_2 = iv_1 + 1; iv_2 < yields[v].size(); ++iv_2) {
		const node v2 = yields[v][iv_2];

			disjoint_yields =
				disjoint_yields and not
				((pi[u1] < pi[v1] and pi[v1] < pi[u2] and pi[u2] < pi[v2]) or
				 (pi[v1] < pi[u1] and pi[u1] < pi[v2] and pi[v2] < pi[u2]));
		}}}}
	}}
	return disjoint_yields;
}

inline uint64_t __get_discont(const uint64_t n, const vector<vector<node> >& yields) {
	uint64_t max_dis = 0;
	for (node u = 0; u < n; ++u) {
		uint64_t dis = 0;
		for (size_t i = 1; i < yields[u].size(); ++i) {
			if (yields[u][i] - yields[u][i - 1] > 1) {
				++dis;
			}
		}
		max_dis = std::max(max_dis, dis);
	}
	return max_dis;
}

inline uint64_t __is_1EC(const urtree& Tree, const vector<position>& pi) {
	const uint64_t n = Tree.n_nodes();
	vector<node> T(n);
	for (node u = 0; u < n; ++u) {
		T[ pi[u] ] = u;
	}

	bool _1ec = false;

	edge_iterator it1(Tree);
	while (it1.has_next()) {
		it1.next();
		// check other edges crossing the current edge
		const node s = it1.get_edge().first;
		const node t = it1.get_edge().second;
		cout << "Current edge: " << s << "," << t << endl;
		const auto [p, q] = sort2(pi[s], pi[t]);
		cout << "At position:  " << pi[s] << "," << pi[t] << endl;

		// the edges crossing the current edge
		set<edge> crossing;

		// iterate over the nodes between the endpoints
		// of 'dep' in the linear arrangement
		for (auto r = p + 1; r < q; ++r) {
			const node u = T[r];
			cout << "    inspecting node " << u << " at " << r << endl;
			for (const node v : Tree.get_neighbours(u)) {
				cout << "        neighbour " << v << " at " << pi[v] << endl;
				if (pi[v] < p or q < pi[v]) {
					// the edge (u,v) crosses (s,t)
					crossing.insert(sort2(u,v));
					cout << "    edge " << u << "," << v << " crosses current." << endl;
					cout << "    at   " << pi[u] << "," << pi[v] << endl;
				}
			}
		}

		// compute the number of common nodes among
		// the edges that cross the current edge
		uint32_t common = 0;
		if (crossing.size() > 1) {
			for (auto cit1 = crossing.begin(); cit1 != crossing.end(); ++cit1) {
				auto cit2 = cit1;
				++cit2;
				for (; cit2 != crossing.end(); ++cit2) {
					common += (
						cit1->first == cit2->first or cit1->first == cit2->second or
						cit1->second == cit2->first or cit1->second == cit2->second
					);
				}
			}
		}
		cout << "common vertices found: " << common << endl;
		if (common >= 1) {
			_1ec = true;
		}
	}

	return _1ec;
}

tree_structure_type __get_syn_dep_tree_type(
	const urtree& Tree, const vector<position>& pi
)
{
	uint64_t C = __n_crossings_stack_based(Tree, pi);
	cout << "C= " << C << endl;
	if (C == 0) {
		// projective or planar?
		return (__is_root_covered(Tree, pi) ?
			tree_structure_type::planar :
			tree_structure_type::projective);
	}

	// compute the yield of each node
	const uint64_t n = Tree.n_nodes();
	vector<vector<node> > yields(n);
	vector<bool> vis(n, false);

	__get_yields(Tree, Tree.get_root(), vis, yields);

	// are the yields non-interleaving?
	bool disjoint_yields = __disjoint_yields(n, yields, pi);

	// discontinuities in the yields
	uint64_t max_dis = (disjoint_yields ? __get_discont(n, yields) : 0);

	if (disjoint_yields and max_dis > 0) {
		// this structure is well-nested

		// is WG_k
		return
		[max_dis]() -> tree_structure_type {
			switch (max_dis) {
			case 1: return tree_structure_type::WG_1;
			case 2: return tree_structure_type::WG_2;
			case 3: return tree_structure_type::WG_3;
			case 4: return tree_structure_type::WG_4;
			case 5: return tree_structure_type::WG_5;
			case 6: return tree_structure_type::WG_6;
			case 7: return tree_structure_type::WG_7;
			case 8: return tree_structure_type::WG_8;
			case 9: return tree_structure_type::WG_9;
			case 10: return tree_structure_type::WG_10;
			}
			return tree_structure_type::WG_k;
		}();
	}

	if (C == 1) {
		// we need C > 1 for 1-EC structures
		return tree_structure_type::none;
	}

	bool is_1EC = __is_1EC(Tree, pi);
	return (is_1EC ? tree_structure_type::EC_1 : tree_structure_type::none);
}

tree_structure_type get_tree_structure_type(const urtree& t, const vector<position>& pi) {
	return utils::call_with_empty_arrangement(__get_syn_dep_tree_type, t, pi);
}

} // -- namespace linarr
} // -- namespace lal
