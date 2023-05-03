/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <ostream>

// lal includes
#include <lal/linarr/chunking/chunk.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Standard output operator for chunks.
 *
 * Usable by @ref lal::linarr::chunk.
 * @param os ostream C++ object.
 * @param g Input chunk.
 * @returns The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const chunk& c) noexcept {
	os << "parent: ";
	if (c.has_parent_node()) {
		os << c.get_parent_node();
	}
	else {
		os << '*';
	}
	os << "; nodes:";
	for (node u : c.get_nodes()) {
		os << ' ' << u;
	}
	return os;
}

/**
 * @brief Standard output operator for chunk sequences.
 *
 * Usable by @ref lal::linarr::chunk_sequence.
 * @param os ostream C++ object.
 * @param g Input chunk sequence.
 * @returns The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const chunk_sequence& seq)
noexcept
{
	for (std::size_t i = 0; i < seq.size(); ++i) {
		os << i << ": " << seq.get_chunk(i) << '\n';
	}
	return os;
}


} // -- namespace linarr
} // -- namespace lal
