/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#include <lal/graphs/tree_literals.hpp>

// C++ includes
#include <sstream>

// lal includes
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

std::pair<lal::graphs::free_tree, lal::node>
operator""_pfree_tree (const char *hv_str, std::size_t)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return lal::graphs::from_head_vector_to_free_tree(hv);
}

lal::graphs::free_tree
operator""_free_tree (const char *hv_str, std::size_t)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return std::move(lal::graphs::from_head_vector_to_free_tree(hv).first);
}

lal::graphs::rooted_tree
operator""_rooted_tree (const char *hv_str, std::size_t)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return lal::graphs::from_head_vector_to_rooted_tree(hv);
}
