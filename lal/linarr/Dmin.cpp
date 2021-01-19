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
#include <bits/stl_pair.h>
using namespace std;

// lal includes
#include <lal/linarr/algorithms_Dmin.hpp>
#include <lal/internal/graphs/trees/Dmin.hpp>

namespace lal {
using namespace graphs;

namespace linarr {

pair<uint32_t, linear_arrangement> Dmin
(const rooted_tree& t, const algorithms_Dmin& a)
{
	switch (a) {
	// call Gildea and Temperly's algorithm
	case algorithms_Dmin::Projective:
		return internal::Dmin_Projective(t);

	// check for invalid choices of algorithm
	case algorithms_Dmin::Planar:
	case algorithms_Dmin::Unconstrained_YS:
	case algorithms_Dmin::Unconstrained_FC:
#if defined DEBUG
		assert(false);
#endif
		break;  // in release compilations the
				// function must return nothing.
	}

	return make_pair(0, linear_arrangement());
}

pair<uint32_t, linear_arrangement> Dmin
(const free_tree& t, const algorithms_Dmin& a)
{
	switch (a) {

	// check for invalid choices of algorithm
	case algorithms_Dmin::Projective:
#if defined DEBUG
		assert(false);
#endif
		break;  // in release compilations the
				// function must return nothing.

	// call Hochberg and Stallman's algorithm
	case algorithms_Dmin::Planar:
		return internal::Dmin_Planar(t);
	// call Yossi Shiloach's algorithm
	case algorithms_Dmin::Unconstrained_YS:
		return internal::Dmin_Unconstrained_YS(t);
	// call Fan Chung's algorithm
	case algorithms_Dmin::Unconstrained_FC:
		return internal::Dmin_Unconstrained_FC(t);
	}

	return make_pair(0, linear_arrangement());
}

} // -- namespace linarr
} // -- namespace lal
