/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
#include <utility>

// lal includes
#include <lal/linarr/algorithms_Dmin_projective.hpp>
#include <lal/linarr/algorithms_Dmin_planar.hpp>
#include <lal/linarr/algorithms_Dmin.hpp>

#include <lal/internal/linarr/Dmin_Projective_AEF.hpp>
#include <lal/internal/linarr/Dmin_Projective_HS.hpp>

#include <lal/internal/linarr/Dmin_Planar_AEF.hpp>
#include <lal/internal/linarr/Dmin_Planar_HS.hpp>

#include <lal/internal/linarr/Dmin_Unconstrained_FC.hpp>
#include <lal/internal/linarr/Dmin_Unconstrained_YS.hpp>

namespace lal {
using namespace graphs;

namespace linarr {

std::pair<uint64_t, linear_arrangement>
min_sum_edge_lengths(const free_tree& t, const algorithms_Dmin& a)
noexcept
{
	if (a == algorithms_Dmin::Shiloach) {
		return internal::Dmin_Unconstrained_YS(t);
	}

	return internal::Dmin_Unconstrained_FC(t);
}

std::pair<uint64_t, linear_arrangement>
min_sum_edge_lengths_planar(const free_tree& t, const algorithms_Dmin_planar& a)
noexcept
{
	if (a == algorithms_Dmin_planar::AlemanyEstebanFerrer) {
		return internal::Dmin_Planar_AEF(t);
	}

	return internal::Dmin_Planar_HS(t);
}

std::pair<uint64_t, linear_arrangement>
min_sum_edge_lengths_projective(const rooted_tree& t, const algorithms_Dmin_projective& a)
noexcept
{
	if (a == algorithms_Dmin_projective::AlemanyEstebanFerrer) {
		return internal::Dmin_Projective_AEF(t);
	}

	return internal::Dmin_Projective_HS(t);
}

} // -- namespace linarr
} // -- namespace lal
