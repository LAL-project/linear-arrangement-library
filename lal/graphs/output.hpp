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

#pragma once

// C++ includes
#include <ostream>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Standard output operator for undirected graphs.
 *
 * Usable by @ref lal::graphs::undirected_graph, @ref lal::graphs::free_tree.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const undirected_graph& g)
noexcept
{
	const auto N = g.get_num_nodes();
	for (node u = 0; u < N; ++u) {
		os << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/**
 * @brief Standard output operator for directed graphs.
 *
 * Usable only by @ref lal::graphs::directed_graph.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const directed_graph& g)
noexcept
{
	const auto N = g.get_num_nodes();
	os << "out:" << "\n";
	for (node u = 0; u < N; ++u) {
		os << u << ":";
		for (auto v : g.get_out_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	os << "\n" << "in:" << "\n";
	for (node u = 0; u < N; ++u) {
		os << u << ":";
		for (auto v : g.get_in_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/**
 * @brief Standard output operator for directed rooted trees.
 *
 * Usable by @ref lal::graphs::rooted_tree.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const rooted_tree& g)
noexcept
{
	const auto N = g.get_num_nodes();
	const std::string pad = (g.has_root() ? " " : "");
	os << "out:" << "\n";
	for (node u = 0; u < N; ++u) {
		os << (g.has_root() and u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_out_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	os << "\n" << "in:" << "\n";
	for (node u = 0; u < N; ++u) {
		os << (g.has_root() and u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_in_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

} // -- namespace graphs
} // -- namespace lal
