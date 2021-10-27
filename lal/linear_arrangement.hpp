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
 * This is further explained in page @ref LAL_concepts__linear_arrangement.
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
		from_data<true>(dir_arr);
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
		from_data<true>(dir_arr);
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
	~linear_arrangement() noexcept = default;

	/**
	 * @brief Construct a linear arrangement from a direct arrangement.
	 * @param v A direct arrangement. This gives the position of every node.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	static linear_arrangement
	from_direct(const std::vector<position>& v) noexcept
	{
		linear_arrangement arr(v.size());
		arr.from_data<true>(v);
		return arr;
	}

	/**
	 * @brief Construct a linear arrangement from an inverse arrangement.
	 * @param v An inverse arrangement. This gives the node for every position.
	 * @returns A linear arrangement object constructed from @e v.
	 */
	static linear_arrangement
	from_inverse(const std::vector<node>& v) noexcept
	{
		linear_arrangement arr(v.size());
		arr.from_data<false>(v);
		return arr;
	}

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

	/// Access to linear arrangement using a type-safe node.
	node operator[] (const node_t& u) const noexcept
	{ return get_position_of(u.value); }

	/// Access to linear arrangement using a type-safe position.
	position operator[] (const position_t& p) const noexcept
	{ return get_node_at(p.value); }

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
	 * @post This overwrites the corresponding positions of both
	 * @ref m_direct and @ref m_inverse.
	 */
	template<
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
			m_direct[u.value] = p.value;
			m_inverse[p.value] = u.value;
		}
		else if constexpr (std::is_same_v<NODE, node_t>) {
			m_direct[u.value] = p;
			m_inverse[p] = u.value;
		}
		else if constexpr (std::is_same_v<POSITION, position_t>) {
			m_direct[u] = p.value;
			m_inverse[p.value] = u;
		}
		else {
			m_direct[u] = p;
			m_inverse[p] = u;
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
		for (std::size_t i = 0; i < n; ++i) { arr.assign(i,i); }
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
	 * @param v Data to construct the arrangement from.
	 * @pre Pointers @ref m_direct and @ref m_inverse must have been set
	 * appropriately.
	 */
	template<bool from_direct_arr>
	void from_data(const std::vector<uint64_t>& v) noexcept
	{
#if defined DEBUG
		assert(m_direct != nullptr);
		assert(m_inverse != nullptr);
#endif
		if constexpr (from_direct_arr) {
			for (size_t i = 0; i < v.size(); ++i) {
				m_direct[i] = v[i];
				m_inverse[v[i]] = i;
			}
		}
		else {
			for (size_t i = 0; i < v.size(); ++i) {
				m_direct[v[i]] = i;
				m_inverse[i] = v[i];
			}
		}
	}

private:
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
