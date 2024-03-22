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
#include <lal/detail/data_array.hpp>

namespace lal {
namespace detail {

/**
 * @brief A set-like data structure implemented with an array.
 *
 * It is actually a simplified unordered hash map implemented using 4 arrays. The
 * goal is to implement a set-like data structure. Elements contained in the set
 * are added to array @ref m_values which is known to hold a maximum number of
 * elements \f$M\f$; its actual size is stored in @ref m_size.
 *
 * Every time an element \f$E\f$ is added to the set, its position (in @ref m_values)
 * is recorded in @ref m_position via a function that maps \f$E\f$ to an integer
 * value between \f$0\f$ and \f$M-1\f$. This function is implemented via a so-called
 * @e indexer object, the only requirement of which is to have an operator
 * (@e value_t) that returns said index. After the position is recorded, its
 * existence in the set is also recorded, but in @ref m_exists.
 *
 * If two different elements \f$E_1\f$ and \f$E_2\f$ are mapped to the same index
 * value via the indexer, then they will be treated as the same object.
 *
 * If the contained elements are integer values (char, short, int, ...) there is
 * no need to implement an indexer object and the type of the indexer value needs
 * not be provided. If the elements are more complex, a requirement to use this
 * object is to implement an indexer object.
 *
 * When adding an element, the indexer object is called only once. When removing
 * an element, the indexer object is called twice.
 *
 * This set has to be initialized with a maximum size (which is the maximum number
 * of elements this array would contain in the worst case) and a maximum index
 * value (the highest index that an element could have in the worst case).
 *
 * Now follows an example that shows how to use this set with integer types,
 * @code
 *		linear_set<int> s(10, 10);
 *		s.add(3);
 *		s.add(4);
 *		s.remove(3);
 * @endcode
 * Now follows an example that shows how to use this set with pairs of integers,
 * @code
 *		struct indexer {
 *			std::size_t operator()(const lal::edge& p) const noexcept {
 *				return p.first + p.second*10;
 *			}
 *		};
 *		indexer I;
 *		linear_set<std::pair<int, int>> s(10, 100, std::move(I));
 *		s.add({3,4});
 *		s.add({4,4});
 *		s.remove({3,4});
 * @endcode
 *
 * @tparam value_t The type of the contained elements
 * @tparam indexer_t The type of the indexer object
 */
template<typename value_t, class indexer_t = value_t>
class linear_set {
public:

	/**
	 * @brief Initialize the set with no indexer object.
	 * @param max_num_elems Maximum number of elements in this set.
	 * @param max_index_value Maximum index value in the worst case.
	 */
	template <
		typename vt = value_t, typename it = indexer_t,
		std::enable_if_t< std::is_same_v<vt, it>, bool > = true
	>
	void init(std::size_t max_num_elems, std::size_t max_index_value) noexcept
	{
		m_size = 0;
		m_values.resize(max_num_elems);
		m_exists.resize(max_index_value, NOT_EXISTS);
		m_position.resize(max_index_value);
	}

	/**
	 * @brief Initialize the set with an indexer object.
	 * @param max_num_elems Maximum number of elements in this set.
	 * @param max_index_value Maximum index value in the worst case.
	 * @param i Indexer object.
	 */
	template <
		typename vt = value_t, typename it = indexer_t,
		std::enable_if_t< not std::is_same_v<vt, it>, bool > = true
	>
	void init(
		std::size_t max_num_elems,
		std::size_t max_index_value,
		indexer_t&& i
	)
	noexcept
	{
		m_I = std::move(i);
		m_size = 0;
		m_values.resize(max_num_elems);
		m_exists.resize(max_index_value, NOT_EXISTS);
		m_position.resize(max_index_value);
	}

	/**
	 * @brief Operator to access values in a given position.
	 * @param i Index value.
	 * @returns The value at the @e i-th position of @ref m_values.
	 */
	const value_t& operator[] (std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_values[i];
	}

	/// Maximum size of this set.
	std::size_t capacity() const noexcept { return m_exists.size(); }
	/// Actual size of this set.
	std::size_t size() const noexcept { return m_size; }

	/// Does an element exist?
	bool exists(const value_t& v) const noexcept
	{ return m_exists[index(v)] == EXISTS; }

	/// Where is an element located?
	std::size_t position(const value_t& v) const noexcept
	{ return m_position[index(v)]; }

	/// Add a new element to the set.
	void add(const value_t& v) noexcept {
		const std::size_t idx_v = index(v);

		if (not m_exists[idx_v]) {
			m_exists[idx_v] = EXISTS;

#if defined DEBUG
			assert(m_size < m_values.size());
#endif

			m_position[idx_v] = m_size;
			m_values[m_size++] = std::move(v);
		}
	}

	/// Add a new element to the set.
	void add(value_t&& v) noexcept {
		const std::size_t idx_v = index(v);

		if (not m_exists[idx_v]) {
			m_exists[idx_v] = EXISTS;

#if defined DEBUG
			assert(m_size < m_values.size());
#endif

			m_position[idx_v] = m_size;
			m_values[m_size++] = std::move(v);
		}
	}

	/// Remove an element from the set.
	void remove(const value_t& v) noexcept {
		const std::size_t idx_v = index(v);
		if (m_exists[idx_v]) {

			m_exists[idx_v] = NOT_EXISTS;

#if defined DEBUG
			assert(m_size > 0);
#endif

			const auto pos_v = m_position[idx_v];
			const auto idx_last_value = index(m_values[m_size - 1]);

#if defined DEBUG
			assert(m_position[idx_last_value] == m_size - 1);
#endif

			std::swap(m_position[idx_v], m_position[idx_last_value]);
			std::swap(m_values[pos_v], m_values[m_size - 1]);

			--m_size;
		}
	}

	/// Begin iterator to @ref m_values.
	const value_t* begin_values() const noexcept { return m_values.begin(); }
	/// Begin iterator to @ref m_values.
	value_t* begin_values() noexcept { return m_values.begin(); }
	/// End iterator to @ref m_values.
	const value_t* end_values() const noexcept { return m_values.end(); }
	/// End iterator to @ref m_values.
	value_t* end_values() noexcept { return m_values.end(); }

	/// Begin iterator to @ref m_position.
	const value_t* begin_position() const noexcept { return m_position.begin(); }
	/// Begin iterator to @ref m_position.
	value_t* begin_position() noexcept { return m_position.begin(); }
	/// End iterator to @ref m_position.
	const value_t* end_position() const noexcept { return m_position.end(); }
	/// End iterator to @ref m_position.
	value_t* end_position() noexcept { return m_position.end(); }

private:
	/// Does an element exist in the set?
	static constexpr char EXISTS = 1;
	/// Does an element not exist in the set?
	static constexpr char NOT_EXISTS = 0;

private:
	/// The indexer object.
	indexer_t m_I;

	/// The unique values in this set.
	data_array<value_t> m_values;
	/// The number of values in @ref m_values.
	std::size_t m_size;

	/// Does a value exist in the set?
	data_array<char> m_exists;
	/**
	 * @brief The position of every value in the set.
	 *
	 * This position is an index that points to a cell of @ref m_values.
	 */
	data_array<std::size_t> m_position;

private:
	/// Calculate the index of an element using the indexer object @ref m_I.
	std::size_t index(const value_t& v) const noexcept {
		if constexpr (not std::is_same_v<value_t, indexer_t>) {
			return m_I(v);
		}
		else {
			return v;
		}
	}
};


} // -- namespace detail
} // -- namespace lal
