/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

#pragma once

// C++ includes
#include <cmath>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/basic_types.hpp>
#include <lal/linarr/level_signature_type.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {
namespace linarr {

/**
 * @brief A class that implements level signatures of an array.
 *
 * This class is just an array of signed integer values, each corresponding
 * to a level value. In case the template parameter @e t is equal to
 * @ref lal::linarr::level_signature_type::per_vertex, the level values are
 * defined per vertex, that is
 */
template <level_signature_type t>
class level_signature {
public:
	/// Default constructor.
	level_signature() noexcept = default;
	/// Default copy constructor.
	level_signature(const level_signature&) noexcept = default;
	/// Default move constructor.
	level_signature(level_signature&&) noexcept = default;
	/// Default copy assignment operator.
	level_signature& operator=(const level_signature&) noexcept = default;
	/// Default move assignment operator.
	level_signature& operator=(level_signature&&) noexcept = default;

	/**
	 * @brief Constructor with size.
	 *
	 * All level values are initialized at 0.
	 */
	level_signature(std::size_t n) noexcept : m_data(n, 0) { }

	/// Access position 'i'
	template <typename T> [[nodiscard]] int64_t operator[] (T i) const noexcept {
		static_assert(
			(t == level_signature_type::per_position and std::is_same_v<T, position_t>) or
			(t == level_signature_type::per_vertex and std::is_same_v<T, node_t>)
		);
		return m_data[*i];
	}
	/// Access position 'i'
	template <typename T> [[nodiscard]] int64_t& operator[] (T i) noexcept {
		static_assert(
			(t == level_signature_type::per_position and std::is_same_v<T, position_t>) or
			(t == level_signature_type::per_vertex and std::is_same_v<T, node_t>)
		);
		return m_data[*i];
	}

	/**
	 * @brief Gets the level value of a vertex.
	 *
	 * Only usable when the type of this level signature is
	 * @ref lal::linarr::level_signature_type::per_vertex.
	 * @param u Input vertex.
	 * @returns The level value of the input vertex.
	 */
	template <
		level_signature_type st = t,
		std::enable_if_t<st == level_signature_type::per_vertex, bool> = true
	>
	int64_t get_vertex_level(node u) const noexcept { return m_data[u]; }
	/**
	 * @brief Sets the level value of a vertex.
	 *
	 * Only usable when the type of this level signature is
	 * @ref lal::linarr::level_signature_type::per_vertex.
	 * @param u Input vertex.
	 * @param l Level value.
	 */
	template <
		level_signature_type st = t,
		std::enable_if_t<st == level_signature_type::per_vertex, bool> = true
	>
	void set_vertex_level(node u, int64_t l) noexcept { m_data[u] = l; }

	/**
	 * @brief Gets the level value of a position.
	 *
	 * Only usable when the type of this level signature is
	 * @ref lal::linarr::level_signature_type::per_position.
	 * @param p Input position.
	 * @returns The level value of the input position.
	 */
	template <
		level_signature_type st = t,
		std::enable_if_t<st == level_signature_type::per_position, bool> = true
	>
	int64_t get_position_level(position p) const noexcept { return m_data[p]; }
	/**
	 * @brief Sets the level value of a position.
	 *
	 * Only usable when the type of this level signature is
	 * @ref lal::linarr::level_signature_type::per_position.
	 * @param p Input position.
	 * @param l Level value.
	 */
	template <
		level_signature_type st = t,
		std::enable_if_t<st == level_signature_type::per_position, bool> = true
	>
	void set_position_level(position p, int64_t l) noexcept { m_data[p] = l; }

private:
	/// The signature of level values.
	detail::data_array<int64_t> m_data;
};

/// Returns true if the template parameter is @ref lal::linarr::level_signature_type::per_vertex.
static constexpr bool is_per_vertex(const level_signature_type& t) noexcept {
	return t == level_signature_type::per_vertex;
}
/// Returns true if the template parameter is @ref lal::linarr::level_signature_type::per_position.
static constexpr bool is_per_position(const level_signature_type& t) noexcept {
	return t == level_signature_type::per_position;
}

typedef level_signature<level_signature_type::per_vertex> level_signature_per_vertex;
typedef level_signature<level_signature_type::per_position> level_signature_per_position;

/**
 * @brief Returns whether or not the input vertex is a thistle vertex.
 *
 * A thistle vertex is a vertex whose value is different from its degree in the
 * given arrangmeent.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param levels Level signature of the arrangement.
 * @param u Vertex to check.
 * @returns Whether or not the input vertex is a thistle.
 */
template <class graph_t>
bool is_thistle_vertex
(const graph_t& g, const level_signature_per_vertex& levels, node_t u)
noexcept
{
	const int64_t l = levels[u];
	return static_cast<uint64_t>( std::abs(l) ) != g.get_degree(*u);
}

/**
 * @brief Returns whether or not the input vertex is a thistle vertex.
 *
 * A thistle vertex is a vertex whose value is different from its degree in the
 * given arrangmeent.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels Level signature of the arrangement.
 * @param u Vertex to check.
 * @returns Whether or not the input vertex is a thistle.
 */
template <class graph_t>
bool is_thistle_vertex(
	const graph_t& g, const level_signature_per_position& levels, node_t u,
	const linear_arrangement& arr = {}
)
noexcept
{
	const int64_t l = (arr.size() == 0 ? levels[position_t{*u}] : levels[position_t{arr[u]}]);
	return static_cast<uint64_t>( std::abs(l) ) != g.get_degree(*u);
}

/**
 * @brief Calculates the level signature of an arrangement of a graph.
 *
 * Level values are given per position or per vertex according to @e t.
 *
 * As defined by \cite Nurse2018a \cite Nurse2019a.
 * @tparam t Type of level signature.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param[out] L Level signature of the arrangement of the input graph.
 * @pre Parameter @e L is initialized at 0.
 */
template <level_signature_type t, class graph_t, class arrangement_t>
void calculate_level_signature
(const graph_t& g, const arrangement_t& arr, level_signature<t>& L)
noexcept
{
	iterators::E_iterator it(g);
	while (not it.end()) {
		const auto [u,v] = it.yield_edge_t();
		const position_t pu = arr[u];
		const position_t pv = arr[v];
		if constexpr (lal::linarr::is_per_position(t)) {
			if (pu < pv) {
				++L[pu];
				--L[pv];
			}
			else {
				--L[pu];
				++L[pv];
			}
		}
		else {
			if (pu < pv) {
				++L[u];
				--L[v];
			}
			else {
				--L[u];
				++L[v];
			}
		}
	}
}

/**
 * @brief Calculates the level signature of an arrangement of a graph.
 *
 * Level values are given per position or per vertex according to @e t.
 *
 * As defined by \cite Nurse2018a \cite Nurse2019a.
 * @tparam stype Type of level signature.
 * @tparam container_t The type of the container where to store the values. The
 * contained type should be a signed integer value.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns The level sequence of an arrangement per vertex.
 */
template <level_signature_type t, class graph_t, class arrangement_t>
level_signature<t> calculate_level_signature
(const graph_t& g, const arrangement_t& arr)
noexcept
{
	const auto n = g.get_num_nodes();
	linarr::level_signature<t> L(n);
	for (position p = 0ull; p < n; ++p) { L[p] = 0; }
	calculate_level_signature(g, arr, L);
	return L;
}

/**
 * @brief Returns true if the level sequence follows that of a maximum arrangement.
 *
 * Checks the conditions defined by Nurse and De Vos \cite Nurse2018a \cite Nurse2019a,
 * defined as:
 * - The sequence of level values has to be non-decreasing.
 * - No two adjacent vertices in the graph can have equal level value.
 *
 * Level values are given per position or per vertex according to @e t.
 * @tparam stype Type of level signature.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param Level signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the two conditions.
 */
template <level_signature_type t, class graph_t, class arrangement_t>
bool is_level_signature_appropriate_NDV
(const graph_t& g, const arrangement_t& arr, const level_signature<t>& levels)
noexcept
{
	const auto n = g.get_num_nodes();

	// the sequence of level values must be non-increasing
	if constexpr (linarr::is_per_position(t)) {
		// check constraint per position
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			if (levels[p] < levels[p + 1ull]) {
				return false;
			}
		}
	}
	else {
		// check constraint per vertex
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			const node_t u = arr[p];
			const node_t v = arr[p + 1ull];
			if (levels[u] < levels[v]) {
				return false;
			}
		}
	}

	// no two adjacent vertices can have equal level value
	for (iterators::E_iterator it(g); not it.end(); it.next()) {
		if constexpr (linarr::is_per_position(t)) {
			// check constraint per position
			const auto [u,v] = it.yield_edge_t();
			const position_t pu = arr[u];
			const position_t pv = arr[v];
			if (levels[pu] == levels[pv]) {
				return false;
			}
		}
		else {
			// check constraint per vertex
			const auto [u,v] = it.yield_edge_t();
			if (levels[u] == levels[v]) {
				return false;
			}
		}
	}
	return true;
}

} // -- namespace linarr
} // -- namespace lal
