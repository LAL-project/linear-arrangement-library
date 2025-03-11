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

// C++ includes
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#include <cassert>
#endif
#include <vector>
#include <omp.h>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/properties/vertex_orbits.hpp>
#include <lal/linarr/D/DMax.hpp>

#include <lal/detail/array.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/properties/branchless_paths_compute.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

#include <lal/detail/linarr/D/DMax/Bipartite_AEF.hpp>
#include <lal/detail/linarr/D/DMax/1_eq_thistle_AEF.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/set_maximum_arrangements.hpp>

namespace lal {
namespace linarr {

void split_vertices_by_color(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& vertex_colors,
	std::vector<node>& blue_vertices_sorted_by_degree,
	std::vector<node>& red_vertices_sorted_by_degree
) noexcept
{
	const uint64_t n = t.get_num_nodes();

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "-----------------\n";
	for (node u = 0; u < n; ++u) {
		std::cout << "Node '" << u << "' has color '" << int(vertex_colors[u])
				  << "'.\n";
	}
	std::cout << "-----------------\n";
#endif

	blue_vertices_sorted_by_degree.reserve(n - 1);
	red_vertices_sorted_by_degree.reserve(n - 1);
	for (node u = 0; u < n; ++u) {
		if (vertex_colors[u] == properties::bipartite_graph_coloring::blue) {
			blue_vertices_sorted_by_degree.push_back(u);
		}
		else {
			red_vertices_sorted_by_degree.push_back(u);
		}
	}
	const auto sort_nodes = [&](std::vector<node>& nodes)
	{
		detail::sorting::
			counting_sort<node, detail::sorting::sort_type::non_decreasing>(
				nodes.begin(),
				nodes.end(),
				n - 1,
				nodes.size(),
				[&](const node u) -> std::size_t
				{
					return t.get_degree(u);
				}
			);
	};
	sort_nodes(blue_vertices_sorted_by_degree);
	sort_nodes(red_vertices_sorted_by_degree);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "-----------------\n";
	std::cout << "Color 0:";
	for (node u : blue_vertices_sorted_by_degree) {
		std::cout << ' ' << u;
	}
	std::cout << '\n';
	std::cout << "Color 1:";
	for (node u : red_vertices_sorted_by_degree) {
		std::cout << ' ' << u;
	}
	std::cout << '\n';
	std::cout << "-----------------\n";
#endif
}

void retrieve_leave_sets(
	const graphs::free_tree& t,
	detail::array<std::vector<node>>& leaves_per_vertex
) noexcept
{
	const uint64_t n = t.get_num_nodes();

	leaves_per_vertex.resize(n);
	for (node u = 0; u < n; ++u) {
		// retrieve leaves of vertex u
		leaves_per_vertex[u].reserve(t.get_degree(u));
		for (node v : t.get_neighbors(u)) {
			if (t.get_degree(v) == 1) {
				leaves_per_vertex[u].push_back(v);
			}
		}
		// sort the leaves by vertex index
		std::sort(leaves_per_vertex[u].begin(), leaves_per_vertex[u].end());
	}
}

void relate_vertices_to_paths(
	const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& branchless_paths_in_tree,
	detail::array<std::size_t>& internal_path_node_to_path_idx,
	detail::array<std::vector<node>>& incident_antennas
) noexcept
{
	const uint64_t n = t.get_num_nodes();
	internal_path_node_to_path_idx.resize(n, n + 1);
	incident_antennas.resize(n);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "Num paths: " << branchless_paths_in_tree.size() << '\n';
	for (const auto& path : branchless_paths_in_tree) {
		std::cout << "-----------------\n";
		std::cout << "Path: " << path.get_h1() << ' ' << path.get_h2() << '\n';
		std::cout << "Sequence:";
		for (node u : path.get_vertex_sequence()) {
			std::cout << ' ' << u;
		}
		std::cout << '\n';
		std::cout << "Set:";
		for (node u = 0; u < n; ++u) {
			std::cout << ' ' << int(path.has_node(u));
		}
		std::cout << '\n';
		if (path.has_lowest_lexicographic()) {
			std::cout << "Lowest: " << path.get_lowest_lexicographic() << '\n';
		}
	}
	std::cout << "-----------------\n";
#endif

	// Relate every vertex to the path it belongs to.
	// Only vertices of degree <= 2 are taken into account.
	for (std::size_t i = 0; i < branchless_paths_in_tree.size(); ++i) {
		const auto& p = branchless_paths_in_tree[i];
		for (std::size_t j = 1; j < p.get_num_nodes() - 1; ++j) {
			internal_path_node_to_path_idx[p[j]] = i;
		}
		if (const node h = p.get_h1(); t.get_degree(h) == 1) {
			internal_path_node_to_path_idx[h] = i;
		}
		if (const node h = p.get_h2(); t.get_degree(h) == 1) {
			internal_path_node_to_path_idx[h] = i;
		}
	}
	for (node u = 0; u < n; ++u) {
		if (t.get_degree(u) <= 2) {
			continue;
		}

		for (node v : t.get_neighbors(u)) {
			if (t.get_degree(v) > 2) {
				continue;
			}

			const std::size_t idx_v = internal_path_node_to_path_idx[v];
			const auto& p = branchless_paths_in_tree[idx_v];
			if (p.is_antenna(t)) {
				incident_antennas[u].push_back(v);
			}
		}
	}

#if defined LAL_DEBUG_DMax_Unc_BnB
	for (node u = 0; u < n; ++u) {
		std::cout << "Node '" << u << "' belongs to path '"
				  << internal_path_node_to_path_idx[u] << "'.\n";
	}
	std::cout << "-----------------\n";
#endif
}

void relate_vertices_to_orbits(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	detail::array<std::size_t>& vertex_to_orbit
) noexcept
{
#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "Computing orbits...\n";
#endif

	const uint64_t n = t.get_num_nodes();
	vertex_to_orbit.resize(n);

	// relate vertices to their orbit
	for (std::size_t i = 0; i < orbits.size(); ++i) {
		for (node u : orbits[i]) {
			vertex_to_orbit[u] = i;
		}
	}

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "    Orbits: " << orbits.size() << '\n';
	for (const auto& orbit : orbits) {
		std::cout << "    ->";
		for (node u : orbit) {
			std::cout << ' ' << u;
		}
		std::cout << '\n';
	}
#endif
}

void calculate_initial_solution(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& vertex_colors,
	const std::vector<properties::branchless_path>& branchless_paths_in_tree,
	const detail::array<std::size_t>& internal_path_node_to_path_idx,

	detail::DMax::unconstrained::set_maximum_arrangements& max_arrs,
	std::pair<uint64_t, linear_arrangement>& initial_DMax
) noexcept
{
	max_arrs.init();

	std::pair<uint64_t, linear_arrangement> Bipartite_MaxLA =
		detail::DMax::bipartite::AEF<true>(t, vertex_colors);
	max_arrs.add(Bipartite_MaxLA.first, Bipartite_MaxLA.second);

	std::pair<uint64_t, linear_arrangement> OneThistle_MaxLA = {0, {}};

	if (t.get_num_nodes() >= 3) {
		OneThistle_MaxLA = detail::DMax::thistle_1::AEF<true>(
			t, branchless_paths_in_tree, internal_path_node_to_path_idx
		);
		max_arrs.add(OneThistle_MaxLA.first, OneThistle_MaxLA.second);
	}

	if (Bipartite_MaxLA.first > OneThistle_MaxLA.first) {
		initial_DMax = std::move(Bipartite_MaxLA);
	}
	else {
		initial_DMax = std::move(OneThistle_MaxLA);
	}
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const properties::bipartite_graph_coloring& vertex_colors,
	const std::vector<properties::branchless_path>& branchless_paths_in_tree,
	const std::size_t number_of_threads
) noexcept
{
#if defined LAL_DEBUG_DMax_Unc_BnB
	assert(number_of_threads == 1);
#endif

	// annyoing, base case
	if (t.get_num_nodes() == 1) {
		linear_arrangement arr(1);
		arr.assign(0ull, 0ull);
		return {0, {{std::move(arr)}}};
	}

	std::vector<node> blue_vertices_sorted_by_degree;
	std::vector<node> red_vertices_sorted_by_degree;
	split_vertices_by_color(
		t,
		vertex_colors,
		blue_vertices_sorted_by_degree,
		red_vertices_sorted_by_degree
	);

	// leaves_per_vertex[u] := set of vertices of degree 1 adjacent to u
	detail::array<std::vector<node>> leaves_per_vertex;
	retrieve_leave_sets(t, leaves_per_vertex);

	detail::array<std::size_t> internal_path_node_to_path_idx;
	detail::array<std::vector<node>> incident_antennas;
	relate_vertices_to_paths(
		t,
		branchless_paths_in_tree,
		internal_path_node_to_path_idx,
		incident_antennas
	);

	detail::array<std::size_t> vertex_to_orbit;
	relate_vertices_to_orbits(t, orbits, vertex_to_orbit);

	detail::DMax::unconstrained::set_maximum_arrangements max_arrs(t);
	std::pair<uint64_t, linear_arrangement> initial_DMax;
	calculate_initial_solution(
		t,
		vertex_colors,
		branchless_paths_in_tree,
		internal_path_node_to_path_idx,
		max_arrs,
		initial_DMax
	);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "Making runners...\n";
#endif

	// The list of objects that run DMax. No two objects are ever run in the
	// same thread at the same time.
	std::vector<detail::DMax::unconstrained::AEF_BnB> BnB_runners;

	// initialize runners
	BnB_runners = std::vector<detail::DMax::unconstrained::AEF_BnB>(
		number_of_threads,
		detail::DMax::unconstrained::AEF_BnB(
			t,
			leaves_per_vertex,
			// colors of vertices
			vertex_colors,
			blue_vertices_sorted_by_degree.size(),
			red_vertices_sorted_by_degree.size(),
			// paths
			branchless_paths_in_tree,
			internal_path_node_to_path_idx,
			incident_antennas,
			// orbits
			orbits,
			vertex_to_orbit
		)
	);

	// ---------------------------------------------------------------------- //

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "Initializing runners...\n";
#endif

	for (std::size_t i = 0; i < BnB_runners.size(); ++i) {
		BnB_runners[i].initialize(initial_DMax);
	}

#if defined LAL_DEBUG_DMax_Unc_BnB
	// execute BnB sequentially
	for (const auto& orbit : orbits) {
		BnB_runners[0].exe(orbit[0]);
	}
#else
#pragma omp parallel for schedule(dynamic) num_threads(number_of_threads)
	for (std::size_t i = 0; i < orbits.size(); ++i) {
		const auto u = orbits[i][0];

		const uint64_t tid = static_cast<uint64_t>(omp_get_thread_num());
		BnB_runners[tid].exe(u);
	}
#endif

	for (auto& runner : BnB_runners) {
		max_arrs.merge(std::move(runner.m_max_arrs));
	}

	std::pair<uint64_t, std::vector<linear_arrangement>> res;
	res.first = max_arrs.get_max_value();
	res.second = max_arrs.retrieve_all_representatives();
	return res;
}

// -------------------------------------------------------------------------- //

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const std::vector<properties::branchless_path>& bps,
	std::size_t nuthreads
) noexcept
{
	const properties::bipartite_graph_coloring c =
		detail::color_vertices_graph(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const std::vector<properties::branchless_path>& bps,
	std::size_t nuthreads
) noexcept
{
	const std::vector<std::vector<node>> orbits =
		properties::vertex_orbits_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const properties::bipartite_graph_coloring& c,
	std::size_t nuthreads
) noexcept
{
	const std::vector<properties::branchless_path> bps =
		detail::branchless_paths_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& bps,
	std::size_t nuthreads
) noexcept
{
	const properties::bipartite_graph_coloring c =
		detail::color_vertices_graph(t);
	const std::vector<std::vector<node>> orbits =
		properties::vertex_orbits_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	std::size_t nuthreads
) noexcept
{
	const std::vector<properties::branchless_path> bps =
		detail::branchless_paths_compute(t);
	const std::vector<std::vector<node>> orbits =
		properties::vertex_orbits_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	std::size_t nuthreads
) noexcept
{
	const properties::bipartite_graph_coloring c =
		detail::color_vertices_graph(t);
	const std::vector<properties::branchless_path> bps =
		detail::branchless_paths_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t, std::size_t nuthreads
) noexcept
{
	const properties::bipartite_graph_coloring c =
		detail::color_vertices_graph(t);
	const std::vector<properties::branchless_path> bps =
		detail::branchless_paths_compute(t);
	const std::vector<std::vector<node>> orbits =
		properties::vertex_orbits_compute(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, nuthreads);
}

} // namespace linarr
} // namespace lal
