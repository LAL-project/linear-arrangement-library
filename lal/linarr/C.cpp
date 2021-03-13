/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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
#include <vector>
using namespace std;

// lal includes
#include <lal/linarr/algorithms_C.hpp>
#include <lal/internal/graphs/algorithms_crossings.hpp>

namespace lal {
using namespace graphs;

namespace linarr {

template<class G>
inline uint32_t number_of_crossings(
	const G& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
{
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

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return g.num_edges()*g.num_edges();
}

uint32_t number_of_crossings(
	const directed_graph& g,
	const algorithms_C& A
)
{
	return number_of_crossings<directed_graph>(g, {}, A);
}

uint32_t number_of_crossings(
	const undirected_graph& g,
	const algorithms_C& A
)
{
	return number_of_crossings<undirected_graph>(g, {}, A);
}

uint32_t number_of_crossings(
	const directed_graph& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
{
	return number_of_crossings<directed_graph>(g, pi, A);
}

uint32_t number_of_crossings(
	const undirected_graph& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
{
	return number_of_crossings<undirected_graph>(g, pi, A);
}

// -----------------------------------------------------------------------------

template<class G>
inline vector<uint32_t> number_of_crossings(
	const G& g,
	const vector<linear_arrangement>& pis,
	const algorithms_C& A
)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::n_C_brute_force(g, pis);
	case algorithms_C::dynamic_programming:
		return internal::n_C_dynamic_programming(g, pis);
	case algorithms_C::ladder:
		return internal::n_C_ladder(g, pis);
	case algorithms_C::stack_based:
		return internal::n_C_stack_based(g, pis);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return vector<uint32_t>(pis.size(), g.num_edges()*g.num_edges());
}

vector<uint32_t> number_of_crossings(
	const directed_graph& g,
	const vector<linear_arrangement>& pis,
	const algorithms_C& A
)
{
	return number_of_crossings<directed_graph>(g, pis, A);
}

vector<uint32_t> number_of_crossings(
	const undirected_graph& g,
	const vector<linear_arrangement>& pis,
	const algorithms_C& A
)
{
	return number_of_crossings<undirected_graph>(g, pis, A);
}

// -----------------------------------------------------------------------------

template<class G>
uint32_t is_number_of_crossings_lesseq_than(
	const G& g,
	const linear_arrangement& pi,
	uint32_t upper_bound,
	const algorithms_C& A
)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::is_n_C_brute_force_lesseq_than(g, pi, upper_bound);
	case algorithms_C::dynamic_programming:
		return internal::is_n_C_dynamic_programming_lesseq_than(g, pi, upper_bound);
	case algorithms_C::ladder:
		return internal::is_n_C_ladder_lesseq_than(g, pi, upper_bound);
	case algorithms_C::stack_based:
		return internal::is_n_C_stack_based_lesseq_than(g, pi, upper_bound);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return false;
}

uint32_t is_number_of_crossings_lesseq_than(
	const directed_graph& g,
	const linear_arrangement& pi,
	uint32_t upper_bound,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<directed_graph>(g, pi, upper_bound, A);
}

uint32_t is_number_of_crossings_lesseq_than(
	const undirected_graph& g,
	const linear_arrangement& pi,
	uint32_t upper_bound,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<undirected_graph>(g, pi, upper_bound, A);
}

// -----------------------------------------------------------------------------

template<class G>
inline vector<uint32_t> is_number_of_crossings_lesseq_than(
	const G& g,
	const vector<linear_arrangement>& pis,
	uint32_t upper_bound,
	const algorithms_C& A
)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::is_n_C_brute_force_lesseq_than(g, pis, upper_bound);
	case algorithms_C::dynamic_programming:
		return internal::is_n_C_dynamic_programming_lesseq_than(g, pis, upper_bound);
	case algorithms_C::ladder:
		return internal::is_n_C_ladder_lesseq_than(g, pis, upper_bound);
	case algorithms_C::stack_based:
		return internal::is_n_C_stack_based_lesseq_than(g, pis, upper_bound);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return vector<uint32_t>(pis.size(), g.num_edges()*g.num_edges());
}

vector<uint32_t> is_number_of_crossings_lesseq_than(
	const directed_graph& g,
	const vector<linear_arrangement>& pis,
	 uint32_t upper_bound,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<directed_graph>
			(g, pis, upper_bound, A);
}

vector<uint32_t> is_number_of_crossings_lesseq_than(
	const undirected_graph& g,
	const vector<linear_arrangement>& pis,
	uint32_t upper_bound,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<undirected_graph>
			(g, pis, upper_bound, A);
}

// -----------------------------------------------------------------------------

template<class G>
vector<uint32_t> is_number_of_crossings_lesseq_than
(
	const G& g,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds,
	const algorithms_C& A
)
{
	switch (A) {
	case algorithms_C::brute_force:
		return internal::is_n_C_brute_force_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::dynamic_programming:
		return internal::is_n_C_dynamic_programming_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::ladder:
		return internal::is_n_C_ladder_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::stack_based:
		return internal::is_n_C_stack_based_lesseq_than(g, pis, upper_bounds);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return vector<uint32_t>(pis.size(), g.num_edges()*g.num_edges());
}

vector<uint32_t> is_number_of_crossings_lesseq_than(
	const directed_graph& G,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<directed_graph>
			(G, pis, upper_bounds, A);
}

vector<uint32_t> is_number_of_crossings_lesseq_than(
	const undirected_graph& G,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds,
	const algorithms_C& A
)
{
	return is_number_of_crossings_lesseq_than<undirected_graph>
			(G, pis, upper_bounds, A);
}

} // -- namespace linarr
} // -- namespace lal
