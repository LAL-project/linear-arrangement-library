/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
#include <iostream>
#include <cassert>
#endif
#include <vector>
#include <omp.h>

#include <iostream>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/properties/vertex_orbits.hpp>
#include <lal/linarr/D/DMax.hpp>

#include <lal/detail/data_array.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/properties/branchless_path_find.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/maximum_arrangements.hpp>

namespace lal {
namespace linarr {

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const properties::bipartite_graph_coloring& vertex_colors,
	const std::vector<properties::branchless_path>& branchless_paths_in_tree,
	const std::size_t num_threads
)
noexcept
{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	assert(m_num_threads == 1);
#endif

	std::pair<uint64_t, std::vector<linear_arrangement>> res;

	const auto n = t.get_num_nodes();

	// annyoing, base case
	if (n == 1) {
		linear_arrangement arr(1);
		arr.assign(0ull, 0ull);
		res.first = 0;
		res.second = {{std::move(arr)}};
		return res;
	}

	// leaves_per_vertex[u] := set of vertices of degree 1 adjacent to u
	detail::data_array<std::vector<node>> leaves_per_vertex;

	// initial value of DMax used to initialize the workers
	std::pair<uint64_t, linear_arrangement> initial_DMax;

	// paths in the tree
	detail::data_array<std::size_t> internal_path_node_to_path_idx;

	// colors
	std::vector<node> blue_vertices_sorted_by_degree;
	std::vector<node> red_vertices_sorted_by_degree;

	detail::data_array<std::size_t> vertex_to_orbit;

	// The list of objects that run DMax. No two objects are ever run in the
	// same thread at the same time.
	std::vector<detail::DMax::unconstrained::AEF_BnB> BnB_runners;

	// retrieve vertex colors
	{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << "-----------------\n";
	for (node u = 0; u < n; ++u) {
		std::cout << "Node '" << u << "' has color '" << int(vertex_colors[u]) << "'.\n";
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
	const auto sort_nodes =
	[&](std::vector<node>& nodes) {
		detail::sorting::counting_sort
		<node, detail::sorting::non_decreasing_t>
		(
			nodes.begin(), nodes.end(), n - 1, nodes.size(),
			[&](const node u) -> std::size_t
			{ return t.get_degree(u); }
		);
	};
	sort_nodes(blue_vertices_sorted_by_degree);
	sort_nodes(red_vertices_sorted_by_degree);
	}

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
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

	initial_DMax = linarr::max_sum_edge_lengths_bipartite(t, vertex_colors);

	detail::DMax::unconstrained::set_max_arrangements max_arrs(t);
	max_arrs.init();
	max_arrs.add(initial_DMax.first, initial_DMax.second);

	// retrieve leaves
	{
	leaves_per_vertex.resize(t.get_num_nodes());
	for (node u = 0; u < n; ++u) {
		// retrieve leaves of vertex u
		leaves_per_vertex[u].reserve(t.get_degree(u));
		for (node v : t.get_neighbours(u)) {
			if (t.get_degree(v) == 1) { leaves_per_vertex[u].push_back(v); }
		}
		// sort the leaves by vertex index
		std::sort(leaves_per_vertex[u].begin(), leaves_per_vertex[u].end());
	}
	}

	internal_path_node_to_path_idx.resize(n, n + 1);

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
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
		std::cout << "Lowest: " << path.get_lowest_lexicographic() << '\n';
	}
	std::cout << "-----------------\n";
#endif
	// Relate every vertex to the path it belongs to.
	// Only vertices of degree <= 2 are taken into account.
	for (std::size_t i = 0; i < branchless_paths_in_tree.size(); ++i) {
		const auto& p = branchless_paths_in_tree[i];
		for (std::size_t j = 1; j < p.get_vertex_sequence().size() - 1; ++j) {
			internal_path_node_to_path_idx[ p.get_vertex_sequence()[j] ] = i;
		}
	}
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	for (node u = 0; u < n; ++u) {
		std::cout
			<< "Node '" << u << "' belongs to path '"
			<< internal_path_node_to_path_idx[u] << "'.\n";
	}
	std::cout << "-----------------\n";
#endif

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << "Computing orbits...\n";
#endif

	// orbits of this tree
	vertex_to_orbit.resize(n);
	for (std::size_t i = 0; i < orbits.size(); ++i) {
		for (node u : orbits[i]) {
			vertex_to_orbit[u] = i;
		}
	}

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << "    Orbits: " << orbits.size() << '\n';
	for (const auto& orbit : orbits) {
		std::cout << "    ->";
		for (node u : orbit) {
			std::cout << ' ' << u;
		}
		std::cout << '\n';
	}
#endif

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << "Making runners...\n";
#endif

	// initialize runners
	BnB_runners =
		std::vector<detail::DMax::unconstrained::AEF_BnB>(
			num_threads,
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
				// orbits
				orbits,
				vertex_to_orbit
			)
		);

// ---------------------------------------------------------------------- //

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << "Initializing runners...\n";
#endif

	for (std::size_t i = 0; i < BnB_runners.size(); ++i) {
		BnB_runners[i].initialise(initial_DMax);
	}

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	for (const auto& orbit : orbits) {
		BnB_runners[0].exe(orbit[0]);
	}
#else
#pragma omp parallel for schedule(dynamic) num_threads(num_threads)
	for (std::size_t i = 0; i < orbits.size(); ++i) {
		const auto u = orbits[i][0];
		//if (m_t.get_degree(u) != max_degree) { continue; }

		const uint64_t tid = static_cast<uint64_t>(omp_get_thread_num());
		BnB_runners[tid].exe(u);
	}
#endif

	for (auto& runner : BnB_runners) {
		max_arrs.merge(std::move(runner.m_max_arrs));
	}

	res.first = max_arrs.get_max_value();
	res.second = max_arrs.retrieve_all_representatives();
	return res;
}

// -------------------------------------------------------------------------- //

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const std::vector<properties::branchless_path>& bps,
	std::size_t num_threads
)
noexcept
{
	const properties::bipartite_graph_coloring c = detail::color_vertices_graph(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const std::vector<properties::branchless_path>& bps,
	std::size_t num_threads
)
noexcept
{
	const std::vector<std::vector<node>> orbits = properties::compute_vertex_orbits(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	const properties::bipartite_graph_coloring& c,
	std::size_t num_threads
)
noexcept
{
	const std::vector<properties::branchless_path> bps = lal::detail::find_all_branchless_paths(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& bps,
	std::size_t num_threads
)
noexcept
{
	const properties::bipartite_graph_coloring c = detail::color_vertices_graph(t);
	const std::vector<std::vector<node>> orbits = properties::compute_vertex_orbits(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	std::size_t num_threads
)
noexcept
{
	const std::vector<properties::branchless_path> bps = lal::detail::find_all_branchless_paths(t);
	const std::vector<std::vector<node>> orbits = properties::compute_vertex_orbits(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	const std::vector<std::vector<node>>& orbits,
	std::size_t num_threads
)
noexcept
{
	const properties::bipartite_graph_coloring c = detail::color_vertices_graph(t);
	const std::vector<properties::branchless_path> bps = lal::detail::find_all_branchless_paths(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

std::pair<uint64_t, std::vector<linear_arrangement>> max_sum_edge_lengths_all(
	const graphs::free_tree& t,
	std::size_t num_threads
)
noexcept
{
	const properties::bipartite_graph_coloring c = detail::color_vertices_graph(t);
	const std::vector<properties::branchless_path> bps = lal::detail::find_all_branchless_paths(t);
	const std::vector<std::vector<node>> orbits = properties::compute_vertex_orbits(t);
	return max_sum_edge_lengths_all(t, orbits, c, bps, num_threads);
}

} // -- namespace linarr
} // -- namespace lal
