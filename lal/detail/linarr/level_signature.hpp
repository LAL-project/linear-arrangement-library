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
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/linear_arrangement.hpp>
#include <lal/properties/branchless_path.hpp>

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

	/// Initializes this level signature.
	void init(std::size_t n) noexcept {
		m_data.resize(n, 0);
	}

	/// Access position 'i'
	template <typename T>
	[[nodiscard]] int64_t operator[] (T i) const noexcept {
		if constexpr (t == level_signature_type::per_position) {
			static_assert(std::is_same_v<T, position_t>);
		}
		else {
			static_assert(std::is_same_v<T, node_t>);
		}
		return m_data[*i];
	}
	/// Access position 'i'
	template <typename T>
	[[nodiscard]] int64_t& operator[] (T i) noexcept {
		if constexpr (t == level_signature_type::per_position) {
			static_assert(std::is_same_v<T, position_t>);
		}
		else {
			static_assert(std::is_same_v<T, node_t>);
		}
		return m_data[*i];
	}

	/**
	 * @brief Equality test
	 *
	 * Two level signatures of two arrangements of the same tree are equal if
	 * their values are equal position-wise, regardless of the type of level
	 * signature.
	 * @param L Other level signature.
	 * @returns Whether this and @e L are equal or not.
	 * @pre Both level signatures must be of arrangements of the same tree.
	 */
	template <level_signature_type st = t>
	bool operator== (const level_signature<st>& L) const noexcept {
		static_assert(st == t);
		for (std::size_t i = 0; i < m_data.size(); ++i) {
			if (m_data[i] != L.m_data[i]) { return false; }
		}
		return true;
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

	/**
	 * @brief Mirrors this level signature.
	 *
	 * The operation is equivalent to recalculating the level signature for the
	 * mirrored arrangement.
	 */
	void mirror() noexcept {
		if constexpr (t == level_signature_type::per_position) {
			const std::size_t n = m_data.size();
			for (position p = 0ull; p < n/2; ++p) {
				m_data[p] = -m_data[p];
				m_data[n - 1ull - p] = -m_data[n - 1ull - p];
				std::swap( m_data[p], m_data[n - 1ull - p] );
			}
			if (n%2 == 1) {
				m_data[n/2] = -m_data[n/2];
			}
		}
		else {
			for (node u = 0ull; u < m_data.size(); ++u) {
				m_data[u] = -m_data[u];
			}
		}
	}

private:
	/// The signature of level values.
	array<int64_t> m_data;
};

/// Returns true if the template parameter is @ref lal::detail::level_signature_type::per_vertex.
inline constexpr bool is_per_vertex(const level_signature_type& t) noexcept {
	return t == level_signature_type::per_vertex;
}
/// Returns true if the template parameter is @ref lal::detail::level_signature_type::per_position.
inline constexpr bool is_per_position(const level_signature_type& t) noexcept {
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
		const auto [u, v] = it.yield_edge_t();
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
				++L[pu];
				--L[pv];
			}
			else {
				--L[pu];
				++L[pv];
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
level_signature<t> calculate_level_signature
(const graph_t& g, const linear_arrangement& arr)
noexcept
{
	const auto n = g.get_num_nodes();
	level_signature<t> L(n);
	if constexpr (t == level_signature_type::per_vertex) {
		for (node_t u = 0ull; u < n; ++u) { L[u] = 0; }
	}
	else {
		for (position_t p = 0ull; p < n; ++p) { L[p] = 0; }
	}

	calculate_level_signature(g, arr, L);
	return L;
}

/**
 * @brief Mirrors a level signature.
 * @tparam level_signature_t Level signature type.
 * @param L Input level signature.
 * @returns A mirrored copy of the input level signature.
 */
template <class level_signature_t>
level_signature_t mirror_level_signature(const level_signature_t& L)
noexcept
{
	level_signature_t L2 = L;
	L2.mirror();
	return L2;
}

} // -- namespace detail
} // -- namespace lal
