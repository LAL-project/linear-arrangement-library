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
 
#include <lal/internal/graphs/algorithms_crossings.hpp>

// C++ includes
#include <cassert>
#include <map>
using namespace std;

// lal includes
#include <lal/internal/macros.hpp>
#include <lal/internal/avl.hpp>

typedef pair<uint32_t,lal::edge> indexed_edge;

namespace lal {
using namespace graphs;

namespace internal {

#define sorted_edge(u,v) (u < v ? edge(u,v) : edge(v,u) )
#define my_abs_diff(a,b) (a < b ? b - a : a - b)

inline uint32_t __compute_C_stack_based(
	const graph& g, const linear_arrangement& pi,
	node * __restrict__ T
)
{
	const uint32_t n = g.n_nodes();

	for (node u = 0; u < n; ++u) {
		T[ pi[u] ] = u;
	}

	// make adjacency lists, sorted by edge length
	vector<neighbourhood> adjP(n);
	vector<vector<indexed_edge> > adjN(n);

	for (node u = 0; u < n; ++u) {
		// 1. fill vectors ...

		// if the graph is undirected this is the same as g.get_neighbours
		const neighbourhood& Nu_out = g.get_out_neighbours(u);
		for (const node& v : Nu_out) {
			if (pi[v] < pi[u]) {
				// oriented edge (v,u), "enters" node u
				adjP[u].push_back(v);
			}
			else {
				// Oriented edge (u,v), "leaves" node u,.
				// Indices are assigned in step 3. It has to be done
				// in this order since the indices are dependent on
				// the ordering of the edges w.r.t. their edge length.
				adjN[u].push_back( indexed_edge(0, sorted_edge(u,v)) );
			}
		}
		if (g.is_directed()) {
			const neighbourhood& Nu_in = g.get_in_neighbours(u);
			for (const node& v : Nu_in) {
				if (pi[v] < pi[u]) {
					// oriented edge (v,u), "enters" node u
					adjP[u].push_back(v);
				}
				else {
					// Oriented edge (u,v), "leaves" node u,.
					// Indices are assigned in step 3. It has to be done
					// in this order since the indices are dependent on
					// the ordering of the edges w.r.t. their edge length.
					adjN[u].push_back( indexed_edge(0, sorted_edge(u,v)) );
				}
			}
		}

		// 2. ... sort contents ...

		// 2.1. sort increasingly
		auto sort_edge_length1 =
		[&u,pi](const node& v, const node& w) {
			return my_abs_diff(pi[u],pi[v]) < my_abs_diff(pi[u],pi[w]);
			//return std::abs(int(pi[u]) - int(pi[v])) < std::abs(int(pi[u]) - int(pi[w]));
		};
		sort(adjP[u].begin(), adjP[u].end(), sort_edge_length1);

		// 2.2. sort decreasingly
		auto sort_edge_length2 =
		[&u,pi](const pair<uint32_t,edge>& ie1, const pair<uint32_t,edge>& ie2) {
			const edge& e1 = ie1.second;
			const edge& e2 = ie2.second;
			const node& v = (e1.first == u ? e1.second : e1.first);
			const node& w = (e2.first == u ? e2.second : e2.first);
			return my_abs_diff(pi[u],pi[v]) > my_abs_diff(pi[u],pi[w]);
			//return std::abs(int(pi[u]) - int(pi[v])) > std::abs(int(pi[u]) - int(pi[w]));
		};
		sort(adjN[u].begin(), adjN[u].end(), sort_edge_length2);
	}

	// relation between edges and their insertion index
	map<edge, uint32_t> edge_to_idx;
	uint32_t idx = 0;

	// 3. ... assign indices now
	for (position pu = 0; pu < n; ++pu) {
		const node u = T[pu];
		for (auto& v : adjN[u]) {
			v.first = idx;

			edge_to_idx.insert( make_pair(v.second, idx) );
			++idx;
		}
	}

	// stack of the algorithm
	internal::AVL<pair<uint32_t, edge> > S;

	uint32_t C = 0;

	for (position pu = 0; pu < n; ++pu) {
		const node u = T[pu];

		for (node v : adjP[u]) {
			const edge uv = sorted_edge(u,v);
			const uint32_t on_top =
				static_cast<uint32_t>(
					S.remove(make_pair(edge_to_idx[uv], uv))
				);
			C += on_top;

			/*
			size c = S.size_on_top( make_pair(idx, uv) );
			C += c;
			S.remove( make_pair(idx, uv) );
			*/
		}
		S.join_sorted_all_greater(adjN[u]);
	}

	return C;
}

inline uint32_t __call_C_stack_based(const graph& g, const linear_arrangement& pi) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ T = new node[n];

	const uint32_t C = __compute_C_stack_based(g, pi, T);

	/* free memory */
	delete[] T;
	return C;
}

uint32_t n_C_stack_based(const graph& g, const linear_arrangement& pi) {
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
	return internal::call_with_empty_arrangement(__call_C_stack_based, g, pi);
}

vector<uint32_t> n_C_stack_based_list
(const graph& g, const vector<linear_arrangement>& pis)
{
	const uint32_t n = g.n_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ T = new node[n];

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);

		// compute C
		cs[i] = __compute_C_stack_based(g, pis[i], T);
	}

	/* free memory */
	delete[] T;
	return cs;
}

} // -- namespace internal
} // -- namespace lal
