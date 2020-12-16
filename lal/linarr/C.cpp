/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <cassert>
using namespace std;

// lal includes
#include <lal/linarr/algorithms_C.hpp>
#include <lal/internal/graphs/algorithms_crossings.hpp>

namespace lal {
using namespace graphs;

namespace linarr {

uint32_t n_crossings
(const directed_graph& g, const linear_arrangement& pi, const algorithms_C& A) {
	switch (A) {
	case algorithms_C::brute_force:
		return internal::n_C_brute_force(g, pi);
	case algorithms_C::dynamic_programming:
		return internal::n_C_dynamic_programming(g, pi);
	case algorithms_C::ladder:
		return internal::n_C_ladder(g, pi);
	case algorithms_C::stack_based:
		return internal::n_C_stack_based(g, pi);
	}

	// wrong value of enumeration
	assert(false);
	return g.n_edges()*g.n_edges();
}

uint32_t n_crossings
(const undirected_graph& g, const linear_arrangement& pi, const algorithms_C& A) {
	switch (A) {
	case algorithms_C::brute_force:
		return internal::n_C_brute_force(g, pi);
	case algorithms_C::dynamic_programming:
		return internal::n_C_dynamic_programming(g, pi);
	case algorithms_C::ladder:
		return internal::n_C_ladder(g, pi);
	case algorithms_C::stack_based:
		return internal::n_C_stack_based(g, pi);
	}

	// wrong value of enumeration
	assert(false);
	return g.n_edges()*g.n_edges();
}

// -----------------------------------------------------------------------------

vector<uint32_t> n_crossings_list
(const directed_graph& g, const vector<linear_arrangement>& pis, const algorithms_C& A)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::n_C_brute_force_list(g, pis);
	case algorithms_C::dynamic_programming:
		return internal::n_C_dynamic_programming_list(g, pis);
	case algorithms_C::ladder:
		return internal::n_C_ladder_list(g, pis);
	case algorithms_C::stack_based:
		return internal::n_C_stack_based_list(g, pis);
	}

	// wrong value of enumeration
	assert(false);
	return vector<uint32_t>(pis.size(), g.n_edges()*g.n_edges());
}

vector<uint32_t> n_crossings_list
(const undirected_graph& g, const vector<linear_arrangement>& pis, const algorithms_C& A)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::n_C_brute_force_list(g, pis);
	case algorithms_C::dynamic_programming:
		return internal::n_C_dynamic_programming_list(g, pis);
	case algorithms_C::ladder:
		return internal::n_C_ladder_list(g, pis);
	case algorithms_C::stack_based:
		return internal::n_C_stack_based_list(g, pis);
	}

	// wrong value of enumeration
	assert(false);
	return vector<uint32_t>(pis.size(), g.n_edges()*g.n_edges());
}

} // -- namespace linarr
} // -- namespace lal
