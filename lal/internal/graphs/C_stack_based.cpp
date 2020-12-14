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
#include <cassert>
#include <map>
using namespace std;

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/avl.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

typedef pair<uint32_t,lal::edge> indexed_edge;

namespace lal {
using namespace graphs;

namespace internal {

#define edge_sorted_by_vertex(u,v) (u < v ? edge(u,v) : edge(v,u) )
#define edge_sorted_by_pos(u,v) (pi[u] < pi[v] ? edge(u,v) : edge(v,u) )
#define my_abs_diff(a,b) (a < b ? b - a : a - b)

inline uint32_t __compute_C_stack_based(
	const graph& g, const linear_arrangement& pi,
	node * __restrict__ T, size_t * __restrict__ size_adjN_u
)
{
	const uint32_t n = g.n_nodes();

	// construct inverse arrangement
	for (node u = 0; u < n; ++u) {
		T[ pi[u] ] = u;
	}

	// Adjacency lists, sorted by edge length:
	// - adjP is sorted by increasing edge length
	// - adjN is sorted by decreasing edge length
	vector<neighbourhood> adjP(n);
	vector<vector<indexed_edge>> adjN(n);

	{
		// Retrieve all edges of the graph to sort
		vector<edge> edges = g.edges();

		// sort edges of the graph by increasing edge length
		internal::counting_sort<vector<edge>::iterator, edge, true>
		(
		edges.begin(), edges.end(),
		n-1, // length of the longest edge
		edges.size(),
		[&](const edge& e) -> size_t {
			const edge se = edge_sorted_by_pos(e.first, e.second);
			++size_adjN_u[se.first];
			return my_abs_diff(pi[e.first],pi[e.second]);
		}
		);

		// initialise adjN
		for (node u = 0; u < n; ++u) {
			// divide by two because the 'key' function in the call to
			// the sorting function is called twice for every edge
			size_adjN_u[u] /= 2;
			adjN[u].resize(size_adjN_u[u]);
		}

		// fill adjP and adjN at the same time
		for (const edge& e : edges) {
			const edge se = edge_sorted_by_pos(e.first, e.second);
			const node u = se.first;	// pi[u] < pi[v]
			const node v = se.second;
			// oriented edge (u,v) "enters" node v
			adjP[v].push_back(u);

			// Oriented edge (u,v) "leaves" node u
			--size_adjN_u[u];
			adjN[u][size_adjN_u[u]] = make_pair(0, edge_sorted_by_vertex(u,v));
		}
	}

	// relate each edge to an index
	map<edge, uint32_t> edge_to_idx;

	uint32_t idx = 0;
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

	// calculate the number of crossings
	uint32_t C = 0;
	for (position pu = 0; pu < n; ++pu) {
		const node u = T[pu];
		for (node v : adjP[u]) {
			const edge uv = edge_sorted_by_vertex(u,v);
			const uint32_t on_top =
				static_cast<uint32_t>(
					S.remove(make_pair(edge_to_idx[uv], uv))
				);
			C += on_top;
		}
		S.join_sorted_all_greater(adjN[u]);
	}

	return C;
}

inline uint32_t __call_C_stack_based
(const graph& g, const linear_arrangement& pi)
{
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ T = new node[n];

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	size_t *size_adjN_u = new size_t[n]{0};

	/* compute number of crossings */

	const uint32_t C = __compute_C_stack_based(g, pi, T, size_adjN_u);

	/* free memory */
	delete[] T;
	delete[] size_adjN_u;
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

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	size_t *size_adjN_u = new size_t[n]{0};

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);

		// compute C
		cs[i] = __compute_C_stack_based(g, pis[i], T, size_adjN_u);
	}

	/* free memory */
	delete[] T;
	delete[] size_adjN_u;
	return cs;
}

} // -- namespace internal
} // -- namespace lal