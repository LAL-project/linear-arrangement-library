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
#include <vector>

// lal includes
#include <lal/linarr/algorithms_C.hpp>
#include <lal/detail/linarr/C_brute_force.hpp>
#include <lal/detail/linarr/C_dyn_prog.hpp>
#include <lal/detail/linarr/C_ladder.hpp>
#include <lal/detail/linarr/C_stack_based.hpp>

namespace lal {
namespace linarr {

template<class G>
inline uint64_t num_crossings(
	const G& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::n_C_brute_force(g, pi);
	case algorithms_C::dynamic_programming:
		return detail::n_C_dynamic_programming(g, pi);
	case algorithms_C::ladder:
		return detail::n_C_ladder(g, pi);
	case algorithms_C::stack_based:
		return detail::n_C_stack_based(g, pi);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return g.get_num_edges()*g.get_num_edges();
}

uint64_t num_crossings(
	const graphs::directed_graph& g,
	const algorithms_C& A
)
noexcept
{
	return num_crossings<graphs::directed_graph>(g, {}, A);
}

uint64_t num_crossings(
	const graphs::undirected_graph& g,
	const algorithms_C& A
)
noexcept
{
	return num_crossings<graphs::undirected_graph>(g, {}, A);
}

uint64_t num_crossings(
	const graphs::directed_graph& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
noexcept
{
	return num_crossings<graphs::directed_graph>(g, pi, A);
}

uint64_t num_crossings(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi,
	const algorithms_C& A
)
{
	return num_crossings<graphs::undirected_graph>(g, pi, A);
}

// -----------------------------------------------------------------------------

template<class G>
inline std::vector<uint64_t> num_crossings_list(
	const G& g,
	const std::vector<linear_arrangement>& pis,
	const algorithms_C& A
)
noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::n_C_brute_force(g, pis);
	case algorithms_C::dynamic_programming:
		return detail::n_C_dynamic_programming(g, pis);
	case algorithms_C::ladder:
		return detail::n_C_ladder(g, pis);
	case algorithms_C::stack_based:
		return detail::n_C_stack_based(g, pis);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(pis.size(), g.get_num_edges()*g.get_num_edges());
}

std::vector<uint64_t> num_crossings_list(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pis,
	const algorithms_C& A
)
noexcept
{
	return num_crossings_list<graphs::directed_graph>(g, pis, A);
}

std::vector<uint64_t> num_crossings_list(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pis,
	const algorithms_C& A
)
noexcept
{
	return num_crossings_list<graphs::undirected_graph>(g, pis, A);
}

// -----------------------------------------------------------------------------

template<class G>
uint64_t is_num_crossings_lesseq_than(
	const G& g,
	const linear_arrangement& pi,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::is_n_C_brute_force_lesseq_than(g, pi, upper_bound);
	case algorithms_C::dynamic_programming:
		return detail::is_n_C_dynamic_programming_lesseq_than(g, pi, upper_bound);
	case algorithms_C::ladder:
		return detail::is_n_C_ladder_lesseq_than(g, pi, upper_bound);
	case algorithms_C::stack_based:
		return detail::is_n_C_stack_based_lesseq_than(g, pi, upper_bound);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return false;
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& g,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than<graphs::directed_graph>(g, {}, upper_bound, A);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& g,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than<graphs::undirected_graph>(g, {}, upper_bound, A);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than<graphs::directed_graph>(g, pi, upper_bound, A);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than<graphs::undirected_graph>(g, pi, upper_bound, A);
}

// -----------------------------------------------------------------------------

template<class G>
inline std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const G& g,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::is_n_C_brute_force_lesseq_than(g, pis, upper_bound);
	case algorithms_C::dynamic_programming:
		return detail::is_n_C_dynamic_programming_lesseq_than(g, pis, upper_bound);
	case algorithms_C::ladder:
		return detail::is_n_C_ladder_lesseq_than(g, pis, upper_bound);
	case algorithms_C::stack_based:
		return detail::is_n_C_stack_based_lesseq_than(g, pis, upper_bound);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(pis.size(), g.get_num_edges()*g.get_num_edges());
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& pis,
	 uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::directed_graph>
			(g, pis, upper_bound, A);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::undirected_graph>
			(g, pis, upper_bound, A);
}

// -----------------------------------------------------------------------------

template<class G>
std::vector<uint64_t> is_num_crossings_lesseq_than_list
(
	const G& g,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
)
noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::is_n_C_brute_force_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::dynamic_programming:
		return detail::is_n_C_dynamic_programming_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::ladder:
		return detail::is_n_C_ladder_lesseq_than(g, pis, upper_bounds);
	case algorithms_C::stack_based:
		return detail::is_n_C_stack_based_lesseq_than(g, pis, upper_bounds);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(pis.size(), g.get_num_edges()*g.get_num_edges());
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& G,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::directed_graph>
			(G, pis, upper_bounds, A);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& G,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
)
noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::undirected_graph>
			(G, pis, upper_bounds, A);
}

} // -- namespace linarr
} // -- namespace lal
