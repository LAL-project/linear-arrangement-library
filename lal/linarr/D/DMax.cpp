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
#include <utility>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

#include <lal/detail/linarr/D/DMax/Projective_AEF.hpp>
#include <lal/detail/linarr/D/DMax/Planar_AEF.hpp>
#include <lal/detail/linarr/D/DMax/Bipartite_AEF.hpp>
#include <lal/detail/linarr/D/DMax/1_eq_thistle_AEF.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/properties/branchless_path_compute.hpp>

namespace lal {
namespace linarr {

/* ------------------------------- 1 THISTLE -------------------------------- */

// *** LE ***

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_le_thistle(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const std::vector<properties::branchless_path>& all_paths
)
noexcept
{
	const auto max_1_thistle = detail::DMax::thistle_1::AEF<true>(t, all_paths);
	const auto max_bipartite = detail::DMax::bipartite::AEF<true>(t, c);
	return (max_1_thistle.first > max_bipartite.first ? max_1_thistle : max_bipartite);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_le_thistle(
	const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& all_paths
)
noexcept
{
	const properties::bipartite_graph_coloring c
		= detail::color_vertices_graph(t);
	return max_sum_edge_lengths_1_le_thistle(t, c, all_paths);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_le_thistle
(const graphs::free_tree& t, const properties::bipartite_graph_coloring& c)
noexcept
{
	const std::vector<properties::branchless_path> all_paths
		= lal::detail::branchless_paths_compute(t);
	return max_sum_edge_lengths_1_le_thistle(t, c, all_paths);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_le_thistle
(const graphs::free_tree& t)
noexcept
{
	const properties::bipartite_graph_coloring c
		= detail::color_vertices_graph(t);
	const std::vector<properties::branchless_path> all_paths
		= lal::detail::branchless_paths_compute(t);
	return max_sum_edge_lengths_1_le_thistle(t, c, all_paths);
}

// *** EQ ***

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_eq_thistle(
	const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& all_paths
)
noexcept
{
	return detail::DMax::thistle_1::AEF<true>(t, all_paths);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_1_eq_thistle
(const graphs::free_tree& t)
noexcept
{
	const std::vector<properties::branchless_path> all_paths
		= lal::detail::branchless_paths_compute(t);
	return max_sum_edge_lengths_1_eq_thistle(t, all_paths);
}

/* -------------------------- BIPARTITE CONSTRAINT -------------------------- */

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::undirected_graph& g, const properties::bipartite_graph_coloring& c)
noexcept
{
	return detail::DMax::bipartite::AEF<true>(g, c);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::undirected_graph& g)
noexcept
{
	const auto c = detail::color_vertices_graph(g);
	return max_sum_edge_lengths_bipartite(g, c);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::directed_graph& g, const properties::bipartite_graph_coloring& c)
noexcept
{
	return detail::DMax::bipartite::AEF<true>(g, c);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_bipartite
(const graphs::directed_graph& g)
noexcept
{
	const auto c = detail::color_vertices_graph(g);
	return max_sum_edge_lengths_bipartite(g, c);
}

/* ------------------- PROJECTIVE AND PLANAR CONSTRAINTS -------------------- */

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::free_tree& t) noexcept
{
	return detail::DMax::planar::AEF<true>(t);
}

std::pair<std::vector<uint64_t>, node> max_sum_edge_lengths_projective_roots
(const graphs::free_tree& t) noexcept
{
	static constexpr auto choice =
		detail::DMax::planar::return_type_all_maxs::DMax_value_vertex_and_max_root;
	return detail::DMax::planar::all_max_sum_lengths_values<choice>(t);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_projective
(const graphs::rooted_tree& t) noexcept
{
	return detail::DMax::projective::AEF<true>(t);
}

} // -- namespace linarr
} // -- namespace lal
