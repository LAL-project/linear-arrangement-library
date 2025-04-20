/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <optional>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/free_tree.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/arrangement_wrapper.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/sorting/sorted_vector.hpp>
#include <lal/detail/array.hpp>

#define max_pos(u, v) (std::max(arr[u], arr[v]))

namespace lal {
namespace detail {

/**
 * @brief Calculate the dependencies and their span.
 * @tparam arrangement_t Type of arrangement.
 * @param t Input tree.
 * @param arr Input linear arrangement.
 * @param edge_with_max_pos_at
 * @param cur_pos Current position in the arrangement for which we calculate
 * the dependencies.
 * @param[out] flux The flux at this position.
 * @param[out] cur_deps The dependencies crossing this position.
 */
template <class depflux, Arrangement arrangement_t>
void calculate_dependencies_and_span(
	const graphs::free_tree& t,
	const arrangement_t& arr,
	const std::vector<std::pair<edge_t, uint64_t>>& edge_with_max_pos_at,
	const position cur_pos,
	std::vector<depflux>& flux,
	std::vector<edge>& cur_deps
) noexcept
{
	const node u = arr[position_t{cur_pos}];

	if (cur_pos > 0) {
		// copy previous dependencies
		cur_deps = flux[cur_pos - 1].get_dependencies();
	}

	// find those edges ending at position 'p' ...
	if (edge_with_max_pos_at[cur_pos].second > 0) {
		const auto [first, last] = std::equal_range(
			cur_deps.begin(),
			cur_deps.end(),
			edge_with_max_pos_at[cur_pos].first, // this ends at position p-1
			[&](const edge_t& e1, const edge_t& e2) -> bool
			{
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
	for (const node_t v : t.get_neighbors(u)) {
		if (arr[v] > cur_pos) {
			cur_deps.push_back({u, *v});
		}
	}

	sorting::sorted_vector<node, true> set_endpoints;
	for (const auto& [v, w] : cur_deps) {
		set_endpoints.insert_sorted(v);
		set_endpoints.insert_sorted(w);
	}
	for (node_t v : set_endpoints) {
		flux[cur_pos].get_left_span() += (arr[v] <= cur_pos);
		flux[cur_pos].get_right_span() += (arr[v] > cur_pos);
	}
}

/**
 * @brief Calculates the weight of a set of dependencies in a flux.
 * @param dependencies Input dependencies.
 * @param ug Input undirected graph.
 * @returns The size of the largest subset of independent dependencies.
 */
[[nodiscard]] inline uint64_t calculate_weight(
	const std::vector<edge>& dependencies, graphs::undirected_graph& ug
) noexcept
{
	if (dependencies.size() <= 1) {
		return dependencies.size();
	}

	// build graph
	ug.set_edges(dependencies);

	// ------------------------------------------
	// apply the "algorithm", see what happens...
	// step 1. while we can find a leaf
	// step 2. put incident edge in the set of disjoint dependencies
	// step 3. delete edges incident to the other vertex

	const auto find_leaf = [](const graphs::undirected_graph& g
						   ) -> std::optional<node>
	{
		for (node u = 0; u < g.get_num_nodes(); ++u) {
			if (g.get_degree(u) == 1) {
				return u;
			}
		}
		return {};
	};

	uint64_t weight = 0;
	// step 1
	while (const auto leaf = find_leaf(ug)) {
		const node u = *leaf;
		const node v = ug.get_neighbors(u)[0];
		// step 2
		++weight;
		// step 3 -- remove edges incident to the only neighbour of the leaf
		ug.remove_edges_incident_to(v);
	}

	return weight;
}

/*
 * @brief Calculate fluxes in a linear arrangement.
 * @tparam arrangement_t Type of arrangement.
 * @param t Input free tree.
 * @param arr Input linear arrangement.
 * @returns The set of dependency fluxes in the arrangement.
 */
template <class depflux, Arrangement arrangement_t>
[[nodiscard]] std::vector<depflux> dependency_flux_compute(
	const graphs::free_tree& t, const arrangement_t& arr
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Kahane2017a);
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		return {};
	}

	// one edge entering each position
	std::vector<std::pair<edge_t, uint64_t>> edge_with_max_pos_at(n, {{}, 0});
	for (iterators::E_iterator e_it(t); not e_it.end(); e_it.next()) {
		const auto [u, v] = e_it.get_edge_t();
		const position max = max_pos(u, v);
		edge_with_max_pos_at[max].first = {u, v};
		++edge_with_max_pos_at[max].second;
	}

	// the graph (of n vertices) used to calculate the weight
	graphs::undirected_graph ug(n);

	// the reusable memory for the sorting algorithm
	sorting::countingsort::memory<edge> mem(n, n);

	// declare the result to be returned
	std::vector<depflux> flux(n - 1);

	for (position cur_pos = 0; cur_pos < n - 1; ++cur_pos) {
		// current dependencies
		std::vector<edge> cur_deps;

		// ----------------------
		// calculate dependencies
		calculate_dependencies_and_span<depflux>(
			t, arr, edge_with_max_pos_at, cur_pos, flux, cur_deps
		);

		// -------------------------------------------------
		// calculate the weight of the flux at this position
		// (read the paper for an "algorithm")
		flux[cur_pos].set_weight(calculate_weight(cur_deps, ug));

		// sort the dependencies by ending position so that edges
		// can be erased more efficiently in the next iteration
		sorting::counting_sort<edge, sorting::sort_type::non_decreasing, false>(
			// iterators to the container to be sorted
			cur_deps.begin(),
			cur_deps.end(),
			// key
			[&](const edge_t& e) -> std::size_t
			{
				return max_pos(e.first, e.second);
			},
			// reusable memory
			mem
		);
		mem.reset_count();

		flux[cur_pos].set_dependencies(std::move(cur_deps));
	}

	return flux;
}

} // namespace detail
} // namespace lal
