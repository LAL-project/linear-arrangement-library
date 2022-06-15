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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <cinttypes>
#include <utility>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/linarr/DMax_Projective_AEF.hpp>
#include <lal/detail/linarr/DMax_Planar_AEF.hpp>

namespace lal {
namespace linarr {

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_planar
(const graphs::free_tree& t) noexcept
{
	return detail::DMax::planar::AEF<true>(t);
}

std::pair<std::vector<uint64_t>, node> max_sum_edge_lengths_projective_roots
(const graphs::free_tree& t) noexcept
{
	static constexpr auto choice =
		detail::DMax::planar::return_type_all_maxs::both_DMax_value_and_max_root;
	return detail::DMax::planar::all_max_sum_lengths_values<choice>(t);
}

std::pair<uint64_t, linear_arrangement> max_sum_edge_lengths_projective
(const graphs::rooted_tree& t) noexcept
{
	return detail::DMax::projective::AEF<true>(t);
}

} // -- namespace linarr
} // -- namespace lal
