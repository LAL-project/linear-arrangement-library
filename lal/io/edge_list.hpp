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

#pragma once

// C++ includes
#include <optional>
#include <string>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace io {

// **DEVELOPER NOTE**
// These functions were not implemented as a templated function in this header
// file so as to avoid including <filesystem> and <ifstream> in this header.

// **DEVELOPER NOTE**
// These function's documentation has to be updated manually in the io.i
// interface file.

/**
 * @brief Reads a undirected graph in edge list format.
 *
 * See @ref LAL_concepts__edge_list_file for further details on the format.
 *
 * @param filename Name of the file.
 * @param norm Should the graph be normalized?
 * See @ref lal::graphs::graph::is_normalized()
 * @param check_norm If the graph is not to be normalized check whether or
 * not the graph read is normalized.
 * @returns Nothing in case the file does not exist. A free graph if otherwise.
 */
std::optional<graphs::undirected_graph>
read_edge_list_undirected_graph
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;

/**
 * @brief Reads a directed graph in edge list format.
 *
 * See @ref LAL_concepts__edge_list_file for further details on the format.
 *
 * @param filename Name of the file.
 * @param norm Should the graph be normalized?
 * See @ref lal::graphs::graph::is_normalized()
 * @param check_norm If the graph is not to be normalized check whether or
 * not the graph read is normalized.
 * @returns Nothing in case the file does not exist. A free graph if otherwise.
 */
std::optional<graphs::directed_graph>
read_edge_list_directed_graph
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;

/**
 * @brief Reads a free tree in edge list format.
 *
 * See @ref LAL_concepts__edge_list_file for further details on the format.
 *
 * @param filename Name of the file.
 * @param norm Should the graph be normalized?
 * See @ref lal::graphs::graph::is_normalized()
 * @param check_norm If the graph is not to be normalized check whether or
 * not the graph read is normalized.
 * @returns Nothing in case the file does not exist. A free graph if otherwise.
 */
std::optional<graphs::free_tree>
read_edge_list_free_tree
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;

/**
 * @brief Reads a rooted tree in edge list format.
 *
 * See @ref LAL_concepts__edge_list_file for further details on the format.
 *
 * @param filename Name of the file.
 * @param norm Should the graph be normalized?
 * See @ref lal::graphs::graph::is_normalized()
 * @param check_norm If the graph is not to be normalized check whether or
 * not the graph read is normalized.
 * @returns Nothing in case the file does not exist. A free graph if otherwise.
 */
std::optional<graphs::rooted_tree>
read_edge_list_rooted_tree
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept;

/**
 * @brief Reads a graph in edge list format.
 *
 * See @ref LAL_concepts__edge_list_file for further details on the format.
 *
 * This function calls
 * @ref lal::io::read_edge_list_undirected_graph,
 * @ref lal::io::read_edge_list_directed_graph,
 * @ref lal::io::read_edge_list_free_tree or
 * @ref lal::io::read_edge_list_rooted_tree
 * depending on the type of the template
 * parameter @e G.
 * @param filename Name of the file.
 * @param norm Should the graph be normalized?
 * See @ref lal::graphs::graph::is_normalized()
 * @param check_norm If the graph is not to be normalized check whether or
 * not the graph read is normalized.
 * @tparam graph_t A graph type. A class that derives from @ref lal::graphs::graph.
 * @returns Nothing in case the file does not exist. A free graph if otherwise.
 */
template <
	class graph_t,
	std::enable_if_t<std::is_base_of_v<graphs::graph, graph_t>, bool> = true
>
inline
std::optional<graph_t> read_edge_list
(const std::string& filename, bool norm = true, bool check_norm = true)
noexcept
{
	if constexpr (std::is_base_of_v<graphs::free_tree, graph_t>) {
		return read_edge_list_free_tree(filename, norm, check_norm);
	}
	else if constexpr (std::is_base_of_v<graphs::rooted_tree, graph_t>) {
		return read_edge_list_rooted_tree(filename, norm, check_norm);
	}
	else if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_t>) {
		return read_edge_list_undirected_graph(filename, norm, check_norm);
	}
	else if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
		return read_edge_list_directed_graph(filename, norm, check_norm);
	}
}

} // -- namespace io
} // -- namespace lal
