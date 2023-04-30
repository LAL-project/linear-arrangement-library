/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {

/**
 * @brief Linear arrangement of vertices
 *
 * A linear arrangement is a pair of two functions that relate vertices to a
 * distinct position in a linear ordering.
 *
 * This concept is further explained in page @ref LAL_concepts__linear_arrangement.
 *
 * Now, this class's usage is simple enough. Declare a linear arrangement with a
 * given number of vertices
 * @code
 lal::linear_arrangement arr(n);
 * @endcode
 * or initialize it
 * @code
 lal::linear_arrangement arr;
 arr.resize(n);
 * @endcode
 * Assign a vertex to a given position using the method @ref assign. Retrieving
 * a vertex's position can be done using either the method @ref get_position_of
 * or using the @ref operator[] passing to it a @ref lal::node_t object. Likewise,
 * use the method @ref get_node_at or the @ref operator[] with a @ref lal::position_t
 * to retrieve the vertex at a given position. Therefore, the following loops are
 * equivalent
 * @code
 lal::linear_arrangement(n);
 // ...
 for (lal::node u = 0; u < n; ++u) {
	const lal::position p = arr.get_position_of(u);
	// ...
 }
 for (lal::node_t u = 0; u < n; ++u) {
	const lal::position p = arr[u];
 }
 * @endcode
 * Types @ref lal::node_t and @ref lal::position_t are useful also in swapping
 * vertices in the arrangement (see @ref swap).
 *
 * Linear arrangements can be transformed. For example, an arrangement can be
 * - shifted to the left (see @ref lal::linear_arrangement::shift_left),
 * - shifted to the right (see @ref lal::linear_arrangement::shift_right),
 * - mirrored (see @ref lal::linear_arrangement::mirror).
 * - in case the inverse arrangement was manipulated, the directed can be updated
 * (see @ref lal::linear_arrangement::update_direct).
 * - in case the direct arrangement was manipulated, the inverse can be updated
 * (see @ref lal::linear_arrangement::update_inverse).
 */
class linear_arrangement {
public:
	/// Default constructor.
	linear_arrangement() noexcept = default;
	/**
	 * @brief Constructor with size.
	 * @param n Number of nodes in the arrangement.
	 */
	linear_arrangement(std::size_t n) noexcept
		: m_memory(2*n, n+1),
		  m_n(n),
		  m_direct(m_memory.begin()),
		  m_inverse(m_memory.begin() + m_n)
	{ }

	/**
	 * @brief Constructor with direct arrangement.
	 *
	 * Constructs a linear arrangement assuming that the parameter is a direct
	 * arrangement, i.e., @e dir_arr[u]=p if the position of vertex @e u is @e p.
	 *
	 * This helps the conversion from python's list to this object.
	 * @param dir_arr Direct arrangement.
	 */
	linear_arrangement(const std::vector<position>& dir_arr) noexcept
		: m_memory(2*dir_arr.size()),
		  m_n(dir_arr.size()),
		  m_direct(m_memory.begin()),
		  m_inverse(m_memory.begin() + m_n)
	{
		// construct m_direct and m_inverse
		from_data<true>(dir_arr.begin(), dir_arr.end());
	}

	/// Copy constructor.
	linear_arrangement(const linear_arrangement& arr) noexcept
		: m_memory(arr.m_memory),
		  m_n(arr.m_n),
		  m_direct(m_memory.begin()),
		  m_inverse(m_memory.begin() + m_n)
	{ }
	/// Copy assignment operator.
	linear_arrangement& operator= (const linear_arrangement& arr) noexcept {
		m_memory = arr.m_memory;
		m_n = arr.m_n;
		m_direct = m_memory.begin();
		m_inverse = m_memory.begin() + m_n;
		return *this;
	}

	/// Copy assignment operator.
	linear_arrangement& operator= (const std::vector<position>& dir_arr) noexcept {
		m_memory.resize(2*dir_arr.size());
		m_n = dir_arr.size();
		m_direct = m_memory.begin();
		m_inverse = m_memory.begin() + m_n;
		// construct m_direct and m_inverse
		from_data<true>(dir_arr.begin(), dir_arr.end());
		return *this;
	}

	/// Move constructor
	linear_arrangement(linear_arrangement&& arr) noexcept
		: m_memory(std::move(arr.m_memory)),
		  m_n(arr.m_n),
		  m_direct(std::move(arr.m_direct)),
		  m_inverse(std::move(arr.m_inverse))
	{
		// invalidate data
		arr.m_n = 0;
		arr.m_direct = nullptr;
		arr.m_inverse = nullptr;
	}
	/// Move assignment operator.
	linear_arrangement& operator= (linear_arrangement&& arr) noexcept {
		// steal data
		m_memory = std::move(arr.m_memory);
		m_n = arr.m_n;
		m_direct = std::move(arr.m_direct);
		m_inverse = std::move(arr.m_inverse);
		// invalidate data
		arr.m_n = 0;
		arr.m_direct = nullptr;
		arr.m_inverse = nullptr;
		return *this;
	}

	/// Default destructor.
	virtual ~linear_arrangement() noexcept = default;

	/**
	 * @brief Construct a linear arrangement from a direct arrangement.
	 *
	 * A direct arrangement gives the position of every node.
	 * @param v A direct arrangement. This gives the position of every node.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	static linear_arrangement from_direct(const std::vector<position>& v) noexcept {
		return from_direct(v.begin(), v.end(), v.size());
	}

#ifndef SWIG
	/**
	 * @brief Construct a linear arrangement from a direct arrangement.
	 *
	 * A direct arrangement gives the position of every node.
	 * @param begin A pointer to the beginning of the direct arrangement.
	 * @param end A pointer to the end of the direct arrangement.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	template <typename It>
	static linear_arrangement from_direct(It begin, It end) noexcept {
		return from_direct(begin, end, std::distance(begin, end));
	}

	/**
	 * @brief Construct a linear arrangement from a direct arrangement.
	 *
	 * A direct arrangement gives the position of every node.
	 * @param begin A pointer to the beginning of the direct arrangement.
	 * @param end A pointer to the end of the direct arrangement.
	 * @param size The size of the container pointer by @e begin and @e end.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	template <typename It>
	static linear_arrangement from_direct(It begin, It end, std::size_t size) noexcept {
		linear_arrangement arr(size);
		arr.from_data<true>(begin, end);
		return arr;
	}
#endif

	/**
	 * @brief Construct a linear arrangement from an inverse arrangement.
	 * @param v An inverse arrangement. This gives the node for every position.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	static linear_arrangement from_inverse(const std::vector<node>& v) noexcept {
		return from_inverse(v.begin(), v.end(), v.size());
	}

#ifndef SWIG
	/**
	 * @brief Construct a linear arrangement from an inverse arrangement.
	 *
	 * A direct arrangement gives the node for every position.
	 * @param begin A pointer to the beginning of the inverse arrangement.
	 * @param end A pointer to the end of the inverse arrangement.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	template <typename It>
	static linear_arrangement from_inverse(It begin, It end) noexcept {
		return from_inverse(begin, end, std::distance(begin, end));
	}

	/**
	 * @brief Construct a linear arrangement from an inverse arrangement.
	 *
	 * A direct arrangement gives the node for every position.
	 * @param begin A pointer to the beginning of the inverse arrangement.
	 * @param end A pointer to the end of the inverse arrangement.
	 * @param size The size of the container pointer by @e begin and @e end.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	template <typename It>
	static linear_arrangement from_inverse(It begin, It end, std::size_t size) noexcept {
		linear_arrangement arr(size);
		arr.from_data<false>(begin, end);
		return arr;
	}
#endif

	/// Lexicographic comparison of two linear arrangements
	bool operator< (const linear_arrangement& arr) const noexcept {
		if (size() != arr.size()) { return size() < arr.size(); }

		for (std::size_t i = 0; i < size() - 1; ++i) {
			if (m_direct[i] != arr.m_direct[i]) {
				return m_direct[i] < arr.m_direct[i];
			}
		}
		return m_direct[size() - 1] < arr.m_direct[size() - 1];
	}

	/// Returns the position of node @e u.
	position operator[] (const node_t& u) const noexcept
	{ return get_position_of(*u); }

	/// Returns the node at position @e p.
	node operator[] (const position_t& p) const noexcept
	{ return get_node_at(*p); }

	/// Frees the memory used by the linear arrangement.
	void clear() noexcept {
		m_memory.clear();
		m_n = 0;
		m_direct = nullptr;
		m_inverse = nullptr;
	}

	/// Returns the position of node @e u.
	position get_position_of(const node u) const noexcept
	{ return m_direct[u]; }

	/// Returns the node at position @e p.
	node get_node_at(const position p) const noexcept
	{ return m_inverse[p]; }

	/**
	 * @brief Changes the size of the arrangement.
	 * @param n New size of the arrangement.
	 * @post Sets the position of each node to \f$n+1\f$, and the node at
	 * each position is also \f$n+1\f$.
	 */
	void resize(std::size_t n) noexcept {
		m_memory.resize(2*n, n + 1);
		m_n = n;
		set_pointers();
	}

	/**
	 * @brief Assigns a node @e u to position @e p.
	 * @tparam NODE A type that must be convertible to @ref node_t.
	 * @tparam POSITION A type that must be convertible to @ref position_t.
	 * @param u Node.
	 * @param p Position.
	 * @pre Values @e u and @e p must both be strictly less than the size of
	 * the arrangement (see @ref m_n).
	 */
	template <
		typename NODE, typename POSITION,
		std::enable_if_t<
			(std::is_integral_v<NODE> or std::is_same_v<NODE, node_t>)
			and
			(std::is_integral_v<POSITION> or std::is_same_v<POSITION, position_t>)
		,
		bool> = true
	>
	void assign(const NODE u, const POSITION p) noexcept {
#if defined DEBUG
		assert(u < m_n);
		assert(p < m_n);
#endif
		if constexpr (std::is_same_v<NODE, node_t> and std::is_same_v<POSITION, position_t>) {
			m_direct[*u] = *p;
			m_inverse[*p] = *u;
		}
		else if constexpr (std::is_same_v<NODE, node_t>) {
			m_direct[*u] = p;
			m_inverse[p] = *u;
		}
		else if constexpr (std::is_same_v<POSITION, position_t>) {
			m_direct[u] = *p;
			m_inverse[*p] = u;
		}
		else {
			m_direct[u] = p;
			m_inverse[p] = u;
		}
	}

	/**
	 * @brief Swaps the position of two vertices or of two positions.
	 *
	 * Updates @ref m_direct and @ref m_inverse so that the vertices are
	 * effectively swapped.
	 *
	 * The two parameters have to be of the same type: either @ref lal::node_t
	 * or @ref lal::position_t.
	 * @tparam what Swap either vertices or positions.
	 * @param u_t Value indicating the first object.
	 * @param v_t Value indicating the second object.
	 */
	template <
		typename what,
		std::enable_if_t<
			std::is_same_v<what, node_t> or std::is_same_v<what, position_t>
		,
		bool> = true
	>
	void swap(const what u_t, const what v_t) noexcept {
		if constexpr (std::is_same_v<what, node_t>) {
			// swap vertices
			const position pu = m_direct[*u_t];
			const position pv = m_direct[*v_t];
			assign(u_t, pv);
			assign(v_t, pu);
		}
		else {
			// swap positions
			const node u = m_inverse[*u_t];
			const node v = m_inverse[*v_t];
			assign(u, v_t);
			assign(v, u_t);
		}
	}

	/// Shifts the vertices one position to the left.
	void shift_left() noexcept {
		const node_t u0 = m_inverse[0];
		// shift every vertex one position to the left
		for (position pu = 0; pu < m_n - 1; ++pu) {
			const node u = get_node_at(pu + 1);
			assign(u, pu);
		}
		// put the first vertex at the last position
		assign(u0, m_n - 1);
	}

	/// Shifts the vertices one position to the right.
	void shift_right() noexcept {
		const node_t ulast = m_inverse[m_n - 1];
		// shift every vertex one position to the left
		for (position pu = m_n - 1; pu > 0; --pu) {
			const node u = get_node_at(pu - 1);
			assign(u, pu);
		}
		// put the last vertex at the first position
		assign(ulast, 0ull);
	}

	/**
	 * @brief Mirror the arrangement.
	 *
	 * Swaps the vertices so that the first is placed at the last position,
	 * the second at the second to last, ... More formally, swaps arr[1] with
	 * arr[n], arr[2] with arr[n-1], ...
	 */
	void mirror() noexcept {
		for (position_t i = 0ull; i < m_n/2; ++i) {
			swap(i, m_n - 1ull - i);
		}
	}

	/// Size of the arrangement (number of nodes in the arrangement).
	std::size_t size() const noexcept { return m_n; }

	/**
	 * @brief Constructs a linear arrangement from an inverse arrangement.
	 * @param n Number of vertices.
	 */
	static linear_arrangement identity(std::size_t n) noexcept
	{
		linear_arrangement arr(n);
		arr.identity();
		return arr;
	}

	/// Makes this arrangement an identity arrangement.
	void identity() noexcept {
		for (std::size_t i = 0; i < m_n; ++i) { assign(i,i); }
	}

	/**
	 * @brief Updates the direct arrangement using the inverse arrangement
	 *
	 * This function is only useful when there have been changes to the inverse
	 * arrangement not via the @ref assign function.
	 */
	void update_direct() noexcept {
		for (position pu = 0; pu < m_n; ++pu) {
			m_direct[ m_inverse[pu] ] = pu;
		}
	}

	/**
	 * @brief Updates the inverse arrangement using the direct arrangement
	 *
	 * This function is only useful when there have been changes to the direct
	 * arrangement not via the @ref assign function.
	 */
	void update_inverse() noexcept {
		for (node u = 0; u < m_n; ++u) {
			m_inverse[ m_direct[u] ] = u;
		}
	}

	/// Pointer to beginning of direct arrangement.
	node *begin_direct() noexcept { return m_direct; }
	/// Pointer to beginning of direct arrangement.
	const node *begin_direct() const noexcept { return m_direct; }
	/// Pointer to end of direct arrangement.
	node *end_direct() noexcept { return m_direct + m_n; }
	/// Pointer to end of direct arrangement.
	const node *end_direct() const noexcept { return m_direct + m_n; }

	/// Pointer to beginning of inverse arrangement.
	node *begin_inverse() noexcept { return m_inverse; }
	/// Pointer to beginning of inverse arrangement.
	const node *begin_inverse() const noexcept { return m_inverse; }
	/// Pointer to end of inverse arrangement.
	node *end_inverse() noexcept { return m_inverse + m_n; }
	/// Pointer to end of inverse arrangement.
	const node *end_inverse() const noexcept { return m_inverse + m_n; }

	/// Constructs a std::vector from the direct arrangement.
	std::vector<position> direct_as_vector() const noexcept
	{ return {begin_direct(), end_direct()}; }

	/// Constructs a std::vector from the inverse arrangement.
	std::vector<node> inverse_as_vector() const noexcept
	{ return {begin_inverse(), end_inverse()}; }

private:

	/// Sets the pointers @ref m_direct and @ref m_inverse appropriate
	/// locations of the memory allocated by @ref m_memory.
	void set_pointers() noexcept {
		m_direct = m_memory.begin();
		m_inverse = m_memory.begin() + m_n;
	}

	/**
	 * @brief Initializes this arrangement from a direct or inverse arrangement.
	 * @tparam from_direct_arr If true, parameter @e v is interpreted as a direct
	 * arrangement; as an inverse arrangement if otherwise.
	 * @param begin Pointer to beginning of data.
	 * @param end Pointer to ending of data.
	 * @pre Pointers @ref m_direct and @ref m_inverse must have been set
	 * appropriately.
	 */
	template <bool from_direct_arr, typename It>
	void from_data(const It begin, const It end) noexcept
	{
#if defined DEBUG
		assert(m_direct != nullptr);
		assert(m_inverse != nullptr);
#endif
		std::size_t i = 0;
		if constexpr (from_direct_arr) {
			for (It it = begin; it != end; ++it, ++i) {
				m_direct[i] = *it;
				m_inverse[*it] = i;
			}
		}
		else {
			for (It it = begin; it != end; ++it, ++i) {
				m_direct[*it] = i;
				m_inverse[i] = *it;
			}
		}
	}

protected:
	/// Memory of the linear arrangement. Holds twice as many elements as
	/// vertices there are in the arrangement.
	detail::data_array<uint64_t> m_memory;
	/// Size of the arrangement (number of nodes in the arrangement).
	std::size_t m_n = 0;

	/// Pointer to the direct arrangement values.
	position *m_direct = nullptr;
	/// Pointer to the inverse arrangement values.
	node *m_inverse = nullptr;
};

} // -- namespace lal
