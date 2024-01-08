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

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/linear_arrangement.hpp>

namespace lal {
namespace detail {

/// Types of level signature.
enum class level_signature_type {
	/**
	 * @brief Given per vertex.
	 *
	 * The level value is queried via a vertex u, "L[u]".
	 */
	per_vertex,
	/**
	 * @brief Given per position.
	 *
	 * The level value is queried via a position p, "L[p]".
	 */
	per_position
};

/**
 * @brief A class that implements level signatures of an array.
 *
 * This class is just an array of signed integer values, each corresponding
 * to a level value.
 *
 * In case the template parameter @e t is equal to
 * @ref lal::detail::level_signature_type::per_vertex, the level values
 * are defined per vertex, that is, level values have to be accessed using a
 * vertex 'u' via the operator [], @e L[u].
 *
 * In case the template parameter @e t is equal to
 * @ref lal::detail::level_signature_type::per_position, the level values
 * are defined per position, that is, level values have to be accessed using a
 * position 'p' via the operator [], @e L[p].
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
	 * @ref lal::detail::level_signature_type::per_vertex.
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
	 * @ref lal::detail::level_signature_type::per_vertex.
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
	 * @ref lal::detail::level_signature_type::per_position.
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
	 * @ref lal::detail::level_signature_type::per_position.
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
	data_array<int64_t> m_data;
};

/// Returns true if the template parameter is @ref lal::detail::level_signature_type::per_vertex.
static constexpr bool is_per_vertex(const level_signature_type& t) noexcept {
	return t == level_signature_type::per_vertex;
}
/// Returns true if the template parameter is @ref lal::detail::level_signature_type::per_position.
static constexpr bool is_per_position(const level_signature_type& t) noexcept {
	return t == level_signature_type::per_position;
}

/// A useful typedef for level signatures per vertex.
typedef level_signature<level_signature_type::per_vertex> level_signature_per_vertex;
/// A useful typedef for level signatures per position.
typedef level_signature<level_signature_type::per_position> level_signature_per_position;

/**
 * @brief Returns whether or not the input vertex is a thistle vertex.
 * @tparam t Level signature type
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels Level signature of the arrangement.
 * @param u Vertex to check.
 * @returns Whether or not the input vertex is a thistle.
 */
template <level_signature_type t, class graph_t>
bool is_thistle_vertex(
	const graph_t& g, const level_signature<t>& levels, node_t u,
	const linear_arrangement& arr = {}
)
noexcept
{
	int64_t l;
	if constexpr (t == level_signature_type::per_vertex) {
		l = levels[u];
	}
	else {
		l = (arr.size() == 0 ? levels[position_t{*u}] : levels[position_t{arr[u]}]);
	}
	return static_cast<uint64_t>( std::abs(l) ) != g.get_degree(*u);
}

/**
 * @brief Returns true if the level sequence follows that of a maximum arrangement.
 *
 * Checks the conditions defined by Nurse and De Vos \cite Nurse2018a \cite Nurse2019a,
 * defined as:
 * - The sequence of level values has to be non-decreasing.
 * - No two adjacent vertices in the graph can have equal level value.
 *
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the two conditions.
 */
template <class graph_t, level_signature_type t>
bool is_level_signature_maximum(
	const graph_t& g,
	const linear_arrangement& arr,
	const level_signature<t>& levels
)
noexcept
{
	const auto n = g.get_num_nodes();

	if constexpr (t == level_signature_type::per_position) {
		// (1) the sequence of level values must be non-increasing
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			if (levels[p] < levels[p + 1ull]) {
				return false;
			}
		}

		// (2) no two adjacent vertices can have equal level value
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u,v] = it.yield_edge_t();
			const position_t pu = (arr.size() == 0 ? *u : arr[u]);
			const position_t pv = (arr.size() == 0 ? *v : arr[v]);
			if (levels[pu] == levels[pv]) {
				return false;
			}
		}
	}
	else {
		// (1) the sequence of level values must be non-increasing
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			const node_t u = (arr.size() == 0 ? *p : arr[p]);
			const node_t v = (arr.size() == 0 ? *p + 1 : arr[p + 1ull]);
			if (levels[u] < levels[v]) {
				return false;
			}
		}

		// (2) no two adjacent vertices can have equal level value
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u,v] = it.yield_edge_t();
			if (levels[u] == levels[v]) {
				return false;
			}
		}
	}

	return true;
}


/**
 * @brief Calculates the level signature of an arrangement of a graph.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param[out] L Level signature of the arrangement of the input graph.
 * @pre Parameter @e L is initialized at 0.
 */
template <level_signature_type t, class graph_t>
void calculate_level_signature
(const graph_t& g, const linear_arrangement& arr, level_signature<t>& L)
noexcept
{
	iterators::E_iterator it(g);
	while (not it.end()) {
		const auto [u,v] = it.yield_edge_t();
		const position_t pu = (arr.size() == 0 ? *u : arr[u]);
		const position_t pv = (arr.size() == 0 ? *v : arr[v]);
		if constexpr (t == level_signature_type::per_vertex) {
			if (pu < pv) {
				++L[u];
				--L[v];
			}
			else {
				--L[u];
				++L[v];
			}
		}
		else {
			if (pu < pv) {
				++L[*pu];
				--L[*pv];
			}
			else {
				--L[*pu];
				++L[*pv];
			}
		}
	}
}

/**
 * @brief Calculates the level signature of an arrangement of a graph.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns The level sequence of an arrangement per vertex.
 */
template <level_signature_type t, class graph_t>
level_signature<t> calculate_level_signature_per_vertex
(const graph_t& g, const linear_arrangement& arr)
noexcept
{
	const auto n = g.get_num_nodes();
	level_signature<t> L(n);
	for (position p = 0ull; p < n; ++p) { L[p] = 0; }
	calculate_level_signature(g, arr, L);
	return L;
}

} // -- namespace detail
} // -- namespace lal