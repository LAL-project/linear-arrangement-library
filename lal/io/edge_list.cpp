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
#include <filesystem>
#include <optional>
#include <fstream>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace io {

template <class graph_t>
inline std::optional<graph_t> __read_edge_list
(const std::string& filename, const bool norm, const bool check)
noexcept
{
	if (not std::filesystem::exists(filename)) {
		return {};
	}

	std::ifstream fin;
	fin.open(filename);

	std::vector<edge> edge_list;
	node max_vert_idx = 0;

	node u, v;
	while (fin >> u >> v) {
		edge_list.push_back(edge(u, v));
		max_vert_idx = std::max(max_vert_idx, u);
		max_vert_idx = std::max(max_vert_idx, v);
	}
	fin.close();

	graph_t g(max_vert_idx + 1);
	g.set_edges(edge_list, norm, check);
	return g;
}

std::optional<graphs::undirected_graph> read_edge_list_undirected_graph
(const std::string& filename, const bool norm, const bool check)
noexcept
{
	return __read_edge_list<graphs::undirected_graph>(filename, norm, check);
}

std::optional<graphs::directed_graph> read_edge_list_directed_graph
(const std::string& filename, const bool norm, const bool check)
noexcept
{
	return __read_edge_list<graphs::directed_graph>(filename, norm, check);
}

std::optional<graphs::free_tree> read_edge_list_free_tree
(const std::string& filename, const bool norm, const bool check)
noexcept
{
	return __read_edge_list<graphs::free_tree>(filename, norm, check);
}

std::optional<graphs::rooted_tree> read_edge_list_rooted_tree
(const std::string& filename, const bool norm, const bool check)
noexcept
{
	return __read_edge_list<graphs::rooted_tree>(filename, norm, check);
}

} // -- namespace io
} // -- namespace lal
