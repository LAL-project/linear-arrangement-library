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
#include <cmath>
#include <map>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/iterators/E_iterator.hpp>

#define sorted_edge(a,b) (a < b ? edge(a,b) : edge(b,a))
inline int64_t sum(uint32_t n, uint32_t t) {
	return 2*(t*(t - 1)) + (n*(n - 1))/2 + 2*t*n;
}

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace properties {

integer size_Q_integer(const undirected_graph& g) {
	const uint64_t m = g.n_edges();
	integer q2 = integer_from_ui(m*(m + 1));

	// substract sum of squared degrees
	for (node u = 0; u < g.n_nodes(); ++u) {
		const uint64_t ku = g.degree(u);
		q2 -= ku*ku;
	}

	return q2/2;
}

uint64_t size_Q(const undirected_graph& g) {
	return size_Q_integer(g).to_uint();
}

integer size_Q_integer(const directed_graph& g) {
	const uint32_t n = g.n_nodes();
	map<edge, uint64_t> collapsed_edges;

	// count the amount of edges that collapse into a single one
	E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const auto [u,v] = it.get_edge();

		const edge es = sorted_edge(u,v);
		const auto it_es = collapsed_edges.find(es);
		if (it_es == collapsed_edges.end()) {
			collapsed_edges.insert(make_pair(es, 1));
		}
		else {
			it_es->second = 2;
		}
	}

	// the size of Q
	integer q = 0;

	// for each vertex, count how many pairs of
	// non-independent we can make
	for (node u = 0; u < n; ++u) {
		uint32_t no_u = 0;
		uint32_t t_u = 0;
		for (node v : g.get_out_neighbours(u)) {
			// u -> v
			const bool edge_vu = g.has_edge(v,u);
			no_u += not edge_vu;
			t_u += edge_vu;
		}
		for (node v : g.get_in_neighbours(u)) {
			// v -> u
			const bool edge_uv = g.has_edge(u,v);
			no_u += not edge_uv;
		}
		q -= sum(no_u, t_u);
	}

	// count how many total pairs of edges we can make
	uint32_t no = 0;
	uint32_t t = 0;
	for (const auto& p : collapsed_edges) {
		no += (p.second == 1);
		t += (p.second == 2);
	}
	q += sum(no, t);

	return q;
}

uint64_t size_Q(const directed_graph& g) {
	return size_Q_integer(g).to_uint();
}

} // -- namespace properties
} // -- namespace lal
