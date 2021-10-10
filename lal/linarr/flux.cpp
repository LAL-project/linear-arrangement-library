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
#include <optional>
#include <vector>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/dependency_flux.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/sorted_vector.hpp>
#include <lal/internal/data_array.hpp>

#define max_pos(u,v) (std::max(pi[u], pi[v]))

namespace lal {
namespace linarr {

namespace flux {

inline
std::vector<std::pair<edge,uint64_t>> get_edges_with_max_pos_at
(const graphs::free_tree& t, const linear_arrangement& pi)
noexcept
{
	std::vector<std::pair<edge,uint64_t>> edge_ending_at
			(t.get_num_nodes(), make_pair(edge(), 0));

	for (iterators::E_iterator e_it(t); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		const position max = max_pos(u, v);
		edge_ending_at[max].first = edge(u,v);
		++edge_ending_at[max].second;
	}
	return edge_ending_at;
}

inline
void calculate_dependencies_span
(
	const graphs::free_tree& t,
	const linear_arrangement& pi,
	const internal::data_array<node>& inv_pi,
	const std::vector<std::pair<edge,uint64_t>>& edge_with_max_pos_at,
	position cur_pos,
	std::vector<dependency_flux>& flux,
	std::vector<edge>& cur_deps
)
noexcept
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

	// add the new dependencies
	for (const node v : t.get_neighbours(u)) {
		if (pi[v] > cur_pos) {
			cur_deps.push_back(edge(u,v));
		}
	}

	sorted_vector<node,true> set_endpoints;
	for (const auto& [v,w] : cur_deps) {
		set_endpoints.insert_sorted(v);
		set_endpoints.insert_sorted(w);
	}
	for (node v : set_endpoints) {
		flux[cur_pos].get_left_span() += (pi[v] <= cur_pos);
		flux[cur_pos].get_right_span() += (pi[v] > cur_pos);
	}
}

inline
uint64_t calculate_weight
(const std::vector<edge>& dependencies, graphs::undirected_graph& ug)
noexcept
{
	if (dependencies.size() <= 1) { return dependencies.size(); }

	// build graph
	ug.set_edges(dependencies);

	// ------------------------------------------
	// apply the "algorithm", see what happens...
	// step 1. while we can find a leaf
	// step 2. put incident edge in the set of disjoint dependencies
	// step 3. delete edges incident to the other vertex

	const auto find_leaf =
	[](const graphs::undirected_graph& g) -> std::optional<node> {
		for (node u = 0; u < g.get_num_nodes(); ++u) {
			if (g.get_degree(u) == 1) { return u; }
		}
		return {};
	};

	uint64_t weight = 0;
	// step 1
	while (const auto leaf = find_leaf(ug)) {
		const node u = *leaf;
		const node v = ug.get_neighbours(u)[0];
		// step 2
		++weight;
		// step 3 -- remove edges incident to the only neighbour of the leaf
		ug.remove_edges_incident_to(v);
	}

	return weight;
}

} // -- namespace flux

inline
std::vector<dependency_flux> __compute_flux
(const graphs::free_tree& t, const linear_arrangement& pi)
noexcept
{
	const uint64_t n = t.get_num_nodes();
	if (n == 1) { return  {}; }

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	internal::data_array<node> inv_pi(n, 0);
	for (node u = 0; u < n; ++u) {
		inv_pi[ pi[u] ] = u;
	}

	// one edge entering each position
	const auto edge_with_max_pos_at = flux::get_edges_with_max_pos_at(t, pi);

	// the graph (of n vertices) used to calculate the weight
	graphs::undirected_graph ug(n);

	// the reusable memory for the sorting algorithm
	internal::countingsort::memory_counting_sort<edge> mem(n, n);

	// declare the result to be returned
	std::vector<dependency_flux> flux(n - 1);

	for (position cur_pos = 0; cur_pos < n - 1; ++cur_pos) {
		// current dependencies
		auto& cur_deps = flux[cur_pos].get_dependencies();

		// ----------------------
		// calculate dependencies
		flux::calculate_dependencies_span
		(t, pi, inv_pi, edge_with_max_pos_at, cur_pos, flux, cur_deps);

		// -------------------------------------------------
		// calculate the weight of the flux at this position
		// (read the paper for an "algorithm")
		flux[cur_pos].set_weight(flux::calculate_weight(cur_deps, ug));

		// sort the dependencies by ending position so that edges
		// can be erased more efficiently in the next iteration
		internal::counting_sort
		<edge, std::vector<edge>::iterator, internal::countingsort::increasing_t, false>
		(
			// iterators to the container to be sorted
			cur_deps.begin(), cur_deps.end(),
			// largest key possible + 1
			n,
			// key
			[&](const edge& e) -> std::size_t
			{ return max_pos(e.first, e.second); },
			// reusable memory
			mem
		);
		mem.reset_count();
	}

	return flux;
}

std::vector<dependency_flux>
compute_flux(const graphs::free_tree& t, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	return internal::call_with_empty_arrangement
		<std::vector<dependency_flux>,graphs::free_tree>
		(__compute_flux, t, pi);
}

} // -- namespace linarr
} // -- namespace lal
