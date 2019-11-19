/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <ostream>

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/graphs/drtree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Standard output operator for undirected graphs.
 *
 * Usable by: @ref ugraph, @ref dgraph, @ref utree, @ref dtree
 * @param os ostream C++ object
 * @param g Input graph.
 * @returns Returns the output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const ugraph& g)
{
	const auto N = g.n_nodes();
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
 * Usable by: @ref ugraph, @ref dgraph, @ref utree, @ref dtree
 * @param os ostream C++ object
 * @param g Input graph.
 * @returns Returns the output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const dgraph& g)
{
	const auto N = g.n_nodes();
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
 * @brief Standard output operator for undirected rooted trees.
 *
 * Usable by: @ref drtree, @ref urtree.
 * @param os ostream C++ object
 * @param g Input graph.
 * @returns Returns the output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const urtree& g) {
	const auto N = g.n_nodes();
	const std::string pad = (g.has_root() ? " " : "");
	for (node u = 0; u < N; ++u) {
		os << (g.has_root() and u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/**
 * @brief Standard output operator for directed rooted trees.
 *
 * Usable by: @ref drtree, @ref urtree.
 * @param os ostream C++ object
 * @param g Input graph.
 * @returns Returns the output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const drtree& g) {
	const auto N = g.n_nodes();
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
