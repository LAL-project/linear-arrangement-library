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

// C++ includes
#include <filesystem>
#include <optional>
#include <fstream>
#include <string>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/conversions.hpp>

namespace lal {
namespace io {

static inline
std::optional<head_vector> read_head_vector
(const std::string& filename)
noexcept
{
	if (not std::filesystem::exists(filename)) { return {}; }

	head_vector heads;

	std::ifstream fin;
	fin.open(filename);
	node head;
	while (fin >> head) { heads.push_back(head); }
	fin.close();

	// this moves!
	return heads;
}

std::optional<graphs::free_tree> read_head_vector_free_tree
(const std::string& filename, bool norm, bool check_norm)
noexcept
{
	const auto heads = read_head_vector(filename);
	if (not heads) { return {}; }
	// move, please
	return {std::move(
		detail::from_head_vector_to_tree<graphs::free_tree>
				(*heads, norm, check_norm).first
	)};
}

std::optional<graphs::rooted_tree> read_head_vector_rooted_tree
(const std::string& filename, bool norm, bool check_norm)
noexcept
{
	const auto heads = read_head_vector(filename);
	if (not heads) { return {}; }
	return detail::from_head_vector_to_tree<graphs::rooted_tree>
			(*heads, norm, check_norm);
}

} // -- namespace io
} // -- namespace lal
