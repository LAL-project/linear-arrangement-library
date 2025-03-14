/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
#include <utility>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

#include <lal/linarr/D/algorithms_Dmin_projective.hpp>
#include <lal/linarr/D/algorithms_Dmin_planar.hpp>
#include <lal/linarr/D/algorithms_Dmin.hpp>

#include <lal/properties/bipartite_graph_colorability.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>

#include <lal/detail/linarr/D/Dmin/Bipartite_AEF.hpp>
#include <lal/detail/linarr/D/Dmin/Projective_AEF.hpp>
#include <lal/detail/linarr/D/Dmin/Projective_HS.hpp>
#include <lal/detail/linarr/D/Dmin/Planar_AEF.hpp>
#include <lal/detail/linarr/D/Dmin/Planar_HS.hpp>
#include <lal/detail/linarr/D/Dmin/Unconstrained_FC.hpp>
#include <lal/detail/linarr/D/Dmin/Unconstrained_YS.hpp>

namespace lal {
namespace linarr {

std::pair<uint64_t, linear_arrangement> min_sum_edge_lengths(
	const graphs::free_tree& t, const algorithms_Dmin& a
) noexcept
{
	if (a == algorithms_Dmin::Shiloach) {
		return detail::Dmin::unconstrained::YossiShiloach<true>(t);
	}

	return detail::Dmin::unconstrained::FanChung_2<true>(t);
}

std::pair<uint64_t, linear_arrangement> min_sum_edge_lengths_bipartite(
	const graphs::free_tree& t, const properties::bipartite_graph_coloring& c
) noexcept
{
	return detail::Dmin::bipartite::AEF<true>(t, c);
}

std::pair<uint64_t, linear_arrangement>
min_sum_edge_lengths_bipartite(const graphs::free_tree& t) noexcept
{
	const auto c = properties::bipartite_coloring(t);
	return min_sum_edge_lengths_bipartite(t, c);
}

std::pair<uint64_t, linear_arrangement> min_sum_edge_lengths_planar(
	const graphs::free_tree& t, const algorithms_Dmin_planar& a
) noexcept
{
	if (a == algorithms_Dmin_planar::AlemanyEstebanFerrer) {
		return detail::Dmin::planar::AEF<true>(t);
	}

	return detail::Dmin::planar::HS<true>(t);
}

std::pair<uint64_t, linear_arrangement> min_sum_edge_lengths_projective(
	const graphs::rooted_tree& t, const algorithms_Dmin_projective& a
) noexcept
{
	if (a == algorithms_Dmin_projective::AlemanyEstebanFerrer) {
		return detail::Dmin::projective::AEF<true>(t);
	}

	return detail::Dmin::projective::HS<true>(t);
}

} // namespace linarr
} // namespace lal
