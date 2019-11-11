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
#include <fstream>

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/graphs/drtree.hpp>

/**
 * @brief Standard output operator for most graphs.
 *
 * Usable by: @ref ugraph, @ref dgraph, @ref utree, @ref dtree
 * @param os ostream C++ object
 * @param g Input graph.
 * @returns Returns the output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const lal::graphs::graph& g) {
	const uint32_t N = g.n_nodes();
	for (lal::node u = 0; u < N; ++u) {
		os << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/// Standard output operator for undirected rooted graphs (see @ref lal::graphs::urtree).
inline std::ostream& operator<< (std::ostream& os, const lal::graphs::urtree& g) {
	const uint32_t N = g.n_nodes();
	const std::string pad = (g.has_root() ? " " : "");
	for (lal::node u = 0; u < N; ++u) {
		os << (u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/// Standard output operator for directed rooted graphs (see @ref lal::graphs::drtree).
inline std::ostream& operator<< (std::ostream& os, const lal::graphs::drtree& g) {
	const uint32_t N = g.n_nodes();
	const std::string pad = (g.has_root() ? " " : "");
	for (lal::node u = 0; u < N; ++u) {
		os << (u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

/// Standard output operator for the @ref integer class.
inline std::ostream& operator<< (std::ostream& os, const lal::numeric::integer& i) {
	os << i.to_string();
	return os;
}

/// Standard output operator for the @ref rational class.
inline std::ostream& operator<< (std::ostream& os, const lal::numeric::rational& r) {
	os << r.to_string();
	return os;
}
