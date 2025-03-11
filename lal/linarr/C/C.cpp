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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/linarr/C/algorithms_C.hpp>
#include <lal/detail/linarr/C/brute_force.hpp>
#include <lal/detail/linarr/C/dyn_prog.hpp>
#include <lal/detail/linarr/C/ladder.hpp>
#include <lal/detail/linarr/C/stack_based.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace linarr {

#define lal_ident_arr(arr) detail::identity_arr(arr)
#define lal_nonident_arr(arr) detail::nonidentity_arr(arr)

template <graphs::Graph graph_t>
uint64_t num_crossings(
	const graph_t& g, const linear_arrangement& arr, const algorithms_C& A
) noexcept
{
	if (arr.size() == 0) {
		switch (A) {
		case algorithms_C::brute_force:
			return detail::crossings::n_C_brute_force(g, lal_ident_arr(arr));
		case algorithms_C::dynamic_programming:
			return detail::crossings::n_C_dynamic_programming(g, lal_ident_arr(arr));
		case algorithms_C::ladder:
			return detail::crossings::n_C_ladder(g, lal_ident_arr(arr));
		case algorithms_C::stack_based:
			return detail::crossings::n_C_stack_based(g, lal_ident_arr(arr));
		}
	}
	else {
		switch (A) {
		case algorithms_C::brute_force:
			return detail::crossings::n_C_brute_force(g, lal_nonident_arr(arr));
		case algorithms_C::dynamic_programming:
			return detail::crossings::n_C_dynamic_programming(
				g, lal_nonident_arr(arr)
			);
		case algorithms_C::ladder:
			return detail::crossings::n_C_ladder(g, lal_nonident_arr(arr));
		case algorithms_C::stack_based:
			return detail::crossings::n_C_stack_based(g, lal_nonident_arr(arr));
		}
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return g.get_num_edges() * g.get_num_edges();
}

uint64_t
num_crossings(const graphs::directed_graph& g, const algorithms_C& A) noexcept
{
	return num_crossings<graphs::directed_graph>(g, {}, A);
}

uint64_t
num_crossings(const graphs::undirected_graph& g, const algorithms_C& A) noexcept
{
	return num_crossings<graphs::undirected_graph>(g, {}, A);
}

uint64_t num_crossings(
	const graphs::directed_graph& g,
	const linear_arrangement& arr,
	const algorithms_C& A
) noexcept
{
	return num_crossings<graphs::directed_graph>(g, arr, A);
}

uint64_t num_crossings(
	const graphs::undirected_graph& g,
	const linear_arrangement& arr,
	const algorithms_C& A
)
{
	return num_crossings<graphs::undirected_graph>(g, arr, A);
}

// -----------------------------------------------------------------------------

template <graphs::Graph graph_t>
std::vector<uint64_t> num_crossings_list(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const algorithms_C& A
) noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::crossings::n_C_brute_force(g, arrs);
	case algorithms_C::dynamic_programming:
		return detail::crossings::n_C_dynamic_programming(g, arrs);
	case algorithms_C::ladder: return detail::crossings::n_C_ladder(g, arrs);
	case algorithms_C::stack_based:
		return detail::crossings::n_C_stack_based(g, arrs);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(
		arrs.size(), g.get_num_edges() * g.get_num_edges()
	);
}

std::vector<uint64_t> num_crossings_list(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& arrs,
	const algorithms_C& A
) noexcept
{
	return num_crossings_list<graphs::directed_graph>(g, arrs, A);
}

std::vector<uint64_t> num_crossings_list(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& arrs,
	const algorithms_C& A
) noexcept
{
	return num_crossings_list<graphs::undirected_graph>(g, arrs, A);
}

// -----------------------------------------------------------------------------

template <graphs::Graph graph_t>
uint64_t is_num_crossings_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	if (arr.size() == 0) {
		switch (A) {
		case algorithms_C::brute_force:
			return detail::crossings::is_n_C_brute_force_lesseq_than(
				g, lal_ident_arr(arr), upper_bound
			);
		case algorithms_C::dynamic_programming:
			return detail::crossings::is_n_C_dynamic_programming_lesseq_than(
				g, lal_ident_arr(arr), upper_bound
			);
		case algorithms_C::ladder:
			return detail::crossings::is_n_C_ladder_lesseq_than(
				g, lal_ident_arr(arr), upper_bound
			);
		case algorithms_C::stack_based:
			return detail::crossings::is_n_C_stack_based_lesseq_than(
				g, lal_ident_arr(arr), upper_bound
			);
		}
	}
	else {
		switch (A) {
		case algorithms_C::brute_force:
			return detail::crossings::is_n_C_brute_force_lesseq_than(
				g, lal_nonident_arr(arr), upper_bound
			);
		case algorithms_C::dynamic_programming:
			return detail::crossings::is_n_C_dynamic_programming_lesseq_than(
				g, lal_nonident_arr(arr), upper_bound
			);
		case algorithms_C::ladder:
			return detail::crossings::is_n_C_ladder_lesseq_than(
				g, lal_nonident_arr(arr), upper_bound
			);
		case algorithms_C::stack_based:
			return detail::crossings::is_n_C_stack_based_lesseq_than(
				g, lal_nonident_arr(arr), upper_bound
			);
		}
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return false;
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& g,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than<graphs::directed_graph>(
		g, {}, upper_bound, A
	);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& g,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than<graphs::undirected_graph>(
		g, {}, upper_bound, A
	);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::directed_graph& g,
	const linear_arrangement& arr,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than<graphs::directed_graph>(
		g, arr, upper_bound, A
	);
}

uint64_t is_num_crossings_lesseq_than(
	const graphs::undirected_graph& g,
	const linear_arrangement& arr,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than<graphs::undirected_graph>(
		g, arr, upper_bound, A
	);
}

// -----------------------------------------------------------------------------

template <graphs::Graph graph_t>
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::crossings::is_n_C_brute_force_lesseq_than(
			g, arrs, upper_bound
		);
	case algorithms_C::dynamic_programming:
		return detail::crossings::is_n_C_dynamic_programming_lesseq_than(
			g, arrs, upper_bound
		);
	case algorithms_C::ladder:
		return detail::crossings::is_n_C_ladder_lesseq_than(
			g, arrs, upper_bound
		);
	case algorithms_C::stack_based:
		return detail::crossings::is_n_C_stack_based_lesseq_than(
			g, arrs, upper_bound
		);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(
		arrs.size(), g.get_num_edges() * g.get_num_edges()
	);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& g,
	const std::vector<linear_arrangement>& arrs,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::directed_graph>(
		g, arrs, upper_bound, A
	);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& g,
	const std::vector<linear_arrangement>& arrs,
	const uint64_t upper_bound,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::undirected_graph>(
		g, arrs, upper_bound, A
	);
}

// -----------------------------------------------------------------------------

template <graphs::Graph graph_t>
std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
) noexcept
{
	switch (A) {
	case algorithms_C::brute_force:
		return detail::crossings::is_n_C_brute_force_lesseq_than(
			g, arrs, upper_bounds
		);
	case algorithms_C::dynamic_programming:
		return detail::crossings::is_n_C_dynamic_programming_lesseq_than(
			g, arrs, upper_bounds
		);
	case algorithms_C::ladder:
		return detail::crossings::is_n_C_ladder_lesseq_than(
			g, arrs, upper_bounds
		);
	case algorithms_C::stack_based:
		return detail::crossings::is_n_C_stack_based_lesseq_than(
			g, arrs, upper_bounds
		);
	}

#if defined DEBUG
	// wrong value of enumeration
	assert(false);
#endif
	return std::vector<uint64_t>(
		arrs.size(), g.get_num_edges() * g.get_num_edges()
	);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::directed_graph& G,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::directed_graph>(
		G, arrs, upper_bounds, A
	);
}

std::vector<uint64_t> is_num_crossings_lesseq_than_list(
	const graphs::undirected_graph& G,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds,
	const algorithms_C& A
) noexcept
{
	return is_num_crossings_lesseq_than_list<graphs::undirected_graph>(
		G, arrs, upper_bounds, A
	);
}

} // namespace linarr
} // namespace lal
