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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/dependency_flux.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/sorted_vector.hpp>

#define max_pos(u,v) (std::max(pi[u], pi[v]))
#define to_uint32(x) static_cast<uint32_t>(x)

namespace lal {
using namespace graphs;
using namespace iterators;

namespace linarr {

namespace flux {

inline vector<pair<edge,uint32_t>> get_edges_with_max_pos_at(
	const free_tree& t, const linear_arrangement& pi
)
{
	vector<pair<edge,uint32_t>> edge_ending_at(t.n_nodes(), make_pair(edge(), 0));

	E_iterator it(t);
	while (it.has_next()) {
		it.next();
		const auto [u,v] = it.get_edge();
		const position max = max_pos(u, v);
		edge_ending_at[max].first = edge(u,v);
		++edge_ending_at[max].second;
	}
	return edge_ending_at;
}

inline
pair<uint32_t, uint32_t>
calculate_dependencies_span(
	const free_tree& t,
	const linear_arrangement& pi, const node * __restrict__ inv_pi,
	const vector<pair<edge,uint32_t>>& edge_with_max_pos_at,
	position cur_pos,
	vector<dependency_flux>& flux,
	vector<edge>& cur_deps
)
{
	const node u = inv_pi[cur_pos];
	if (cur_pos > 0) {
		// copy previous dependencies
		cur_deps = flux[cur_pos - 1].get_dependencies();
	}

	// find those edges ending at position 'p' ...
	if (edge_with_max_pos_at[cur_pos].second > 0) {
		const auto [first, last] =
		std::equal_range(
			cur_deps.begin(), cur_deps.end(),
			edge_with_max_pos_at[cur_pos].first, // this ends at position p-1
			[&](const edge& e1, const edge& e2) -> bool {
				const auto pos_e1 = max_pos(e1.first, e1.second);
				const auto pos_e2 = max_pos(e2.first, e2.second);
				return pos_e1 < pos_e2;
			}
		);
		if (first != cur_deps.end()) {
			cur_deps.erase(first, last);
		}
	}

	const uint32_t size_after_del = to_uint32(cur_deps.size());

	// add the new dependencies
	for (const node v : t.get_neighbours(u)) {
		if (pi[v] > cur_pos) {
			cur_deps.push_back(edge(u,v));
		}
	}

	sorted_vector<node> set_endpoints;
	for (const auto& [v,w] : cur_deps) {
		set_endpoints.insert_sorted_unique(v);
		set_endpoints.insert_sorted_unique(w);
	}
	for (node v : set_endpoints) {
		flux[cur_pos].get_left_span() += (pi[v] <= cur_pos);
		flux[cur_pos].get_right_span() += (pi[v] > cur_pos);
	}

	const uint32_t size_after_add = to_uint32(cur_deps.size());

	return make_pair(size_after_del, size_after_add);
}

inline uint32_t calculate_weight(
	const vector<edge>& dependencies,
	undirected_graph& ug
)
{
	if (dependencies.size() <= 1) {
		return to_uint32(dependencies.size());
	}
	// build graph
	ug.set_edges(dependencies);

	// ------------------------------------------
	// apply the "algorithm", see what happens...
	// step 1. while we can find a leaf
	// step 2. put incident edge in the set of disjoint dependencies
	// step 3. delete edges incident to the other vertex

	const auto find_leaf =
	[](const undirected_graph& g) -> std::optional<node> {
		for (node u = 0; u < g.n_nodes(); ++u) {
			if (g.degree(u) == 1) { return u; }
		}
		return {};
	};

	uint32_t weight = 0;
	// step 1
	while (const auto leaf = find_leaf(ug)) {
		const node u = *leaf;
		const node v = ug.get_neighbours(u)[0];
		// step 2
		++weight;
		// step 3
		vector<edge> edges_to_be_removed(ug.degree(v));
		auto it = edges_to_be_removed.begin();
		for (const node w : ug.get_neighbours(v)) {
			*it++ = edge(v,w);
		}
		ug.remove_edges(edges_to_be_removed);
	}

	return weight;
}

inline void calculate_bouquet_type(
	uint32_t n,
	uint32_t size_after_del, uint32_t size_after_add,
	position cur_pos,
	vector<dependency_flux>& flux
)
{
	const bool can_be_bouquet = flux[cur_pos].get_size() > 1 and
								flux[cur_pos].get_weight() == 1;

	if (not can_be_bouquet) { return; }

	if (cur_pos == 0) {
		// the leftmost position is always a right-branching bouquet
		flux[cur_pos].set_right_bouquet(true);
	}
	else if (cur_pos == n - 2) {
		// the rightmost position is always a left-branching bouquet
		flux[cur_pos].set_left_bouquet(true);
	}
	else if (cur_pos > 0) {
		const size_t new_deps_added = size_after_add - size_after_del;

		// this flux is a right-branching bouquet if new
		// dependencies have been added
		const bool is_right_bouquet = new_deps_added > 1;
		flux[cur_pos].set_right_bouquet(is_right_bouquet);

		// this flux is a left-branching bouquet if no new
		// dependencies have been added
		const bool is_left_bouquet =
			size_after_del > 0 and flux[cur_pos].get_size() > 1;

		flux[cur_pos].set_left_bouquet(is_left_bouquet);
	}
}

} // -- namespace flux

inline vector<dependency_flux>
__compute_flux(const free_tree& t, const linear_arrangement& pi) {
	const uint32_t n = t.n_nodes();

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ inv_pi = new node[n]{0};
	for (node u = 0; u < n; ++u) {
		inv_pi[ pi[u] ] = u;
	}

	// one edge entering each position
	const auto edge_with_max_pos_at = flux::get_edges_with_max_pos_at(t, pi);

	// the graph (of n vertices) used to calculate the weight
	undirected_graph ug(n);

	// the reusable memory for the sorting algorithm
	internal::memory_counting_sort<edge> mem(n, n);

	// initialise 'result' with 'n-1' positions
	vector<dependency_flux> flux(n - 1);

	for (position cur_pos = 0; cur_pos < n - 1; ++cur_pos) {
		// current dependencies
		auto& cur_deps = flux[cur_pos].get_dependencies();

		// ----------------------
		// calculate dependencies
		const auto [size_after_del, size_after_add] =
		flux::calculate_dependencies_span(
			t, pi, inv_pi, edge_with_max_pos_at, cur_pos, flux, cur_deps
		);

		flux[cur_pos].set_size(size_after_add);

		// -------------------------------------------------
		// calculate the weight of the flux at this position
		// (read the paper for an "algorithm")
		flux[cur_pos].set_weight(flux::calculate_weight(cur_deps, ug));

		// -------------------------
		// calculate type of bouquet
		flux::calculate_bouquet_type(
			n, size_after_del, size_after_add, cur_pos, flux
		);

		// sort the dependencies by ending position so that edges
		// can be erased more efficiently in the next iteration
		internal::counting_sort<vector<edge>::iterator, edge, true>
		(
			// iterators to the container to be sorted
			cur_deps.begin(), cur_deps.end(),
			// largest key possible + 1
			n,
			// key
			[&](const edge& e) -> size_t { return max_pos(e.first, e.second); },
			// reusable memory
			mem
		);
		mem.reset_count();

#if defined DEBUG
		if (0 < cur_pos and cur_pos < n - 2) {
			assert(
				flux[cur_pos].is_left_bouquet() +
				flux[cur_pos].is_right_bouquet() < 2
			);
		}
		if (flux[cur_pos].is_left_bouquet() or flux[cur_pos].is_right_bouquet()) {
			assert(flux[cur_pos].get_weight() == 1);
		}
#endif
	}

	delete[] inv_pi;
	return flux;
}

vector<dependency_flux>
compute_flux(const free_tree& t, const linear_arrangement& pi) {
#if defined DEBUG
	assert(t.is_tree());
#endif
	return
	internal::call_with_empty_arrangement<vector<dependency_flux>,free_tree>
	(__compute_flux, t, pi);
}

} // -- namespace linarr
} // -- namespace lal
