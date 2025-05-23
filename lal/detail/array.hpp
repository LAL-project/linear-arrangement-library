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

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <iterator>

namespace lal {
namespace detail {

/**
 * @brief Wrapper of a C array for automatic deallocation of memory.
 *
 * Automatically manage deallocation of memory via destructors.
 */
template <typename T>
struct array {
public:

	/// Default constructor
	array() noexcept = default;

	/// Constructor from initializer_list.
	array(std::initializer_list<T> l) noexcept
		: m_size(l.size())
	{
		// (un?)fortunately, this will call the constructor of T
		// for every element in m_data.
		alloc_data();
		// array reference
		const T * const list_data = std::data(l);
		for (std::size_t i = 0; i < l.size(); ++i) {
			// the data has to be be copied, it cannot be moved
			// unless 'list_data' is const_cast<T *>-ed.
			m_data[i] = list_data[i];
		}
	}

	/**
	 * @brief Constructor with size
	 * @param n Size.
	 */
	array(const std::size_t n) noexcept
		: m_size(n)
	{
		alloc_data();
	}
	/**
	 * @brief Constructor with size
	 * @param n Size.
	 * @param v Value to initialize the array with.
	 */
	array(const std::size_t n, const T& v) noexcept
		: array(n)
	{
		fill(v);
	}
	/// Copy constructor
	array(const array& d) noexcept
		: array(d.m_size)
	{
		if (m_size > 0) {
			std::copy(d.begin(), d.end(), begin());
		}
	}
	/// Copy assignment operator
	array& operator= (const array& d) noexcept
	{
		if (m_size != d.m_size) {
			delete[] m_data;
			m_size = d.m_size;
			alloc_data();
		}
		if (m_size > 0) {
			std::copy(d.begin(), d.end(), begin());
		}
		return *this;
	}

	/// Move constructor
	array(array&& d) noexcept
	{
		m_data = d.m_data;
		m_size = d.m_size;
		d.m_data = nullptr;
		d.m_size = 0;
	}
	/// Move assignment operator
	array& operator= (array&& d) noexcept
	{
		delete[] m_data;
		m_data = d.m_data;
		m_size = d.m_size;
		d.m_data = nullptr;
		d.m_size = 0;
		return *this;
	}

	/*
	/// Copy constructor from generic container
	template <template <typename... Args> class container, typename... Types>
	array(const container<Types...>& v) noexcept : array(v.size()) {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		std::copy(v.begin(), v.end(), begin());
	}

	/// Copy assignment operator from generic container
	template <template <typename... Args> class container, typename... Types>
	array& operator= (const container<Types...>& v) noexcept {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		resize(v.size());
		std::copy(v.begin(), v.end(), begin());
		return *this;
	}
	*/

	/// Destructor
	~array() noexcept
	{
		clear();
	}

	/// Comparison of equal data arrays
	[[nodiscard]] bool operator== (const array& d) const noexcept
	{
		if (size() != d.size()) {
			return false;
		}

		for (std::size_t i = 0; i < size(); ++i) {
			if (m_data[i] != d.m_data[i]) {
				return false;
			}
		}
		return true;
	}

	/// Clear the contents of the array.
	void clear() noexcept
	{
		delete[] m_data;
		m_size = 0;
		m_data = nullptr;
	}

	/**
	 * @brief Resize the array.
	 *
	 * Does nothing if @e new_size is the same as the current size.
	 * @tparam keep_current_data Resize the array while keeping the existing data.
	 * @param new_size The new size of the array.
	 */
	template <bool keep_current_data = true>
	void resize(const std::size_t new_size) noexcept
	{
		if (new_size != m_size or m_data == nullptr) {
			if constexpr (keep_current_data) {
				array<T> old_data(new_size);
				for (std::size_t i = 0; i < std::min(new_size, m_size); ++i) {
					old_data.m_data[i] = std::move(m_data[i]);
				}
				*this = std::move(old_data);
			}
			else {
				delete[] m_data;
				m_size = new_size;
				alloc_data();
			}
		}
	}

	/**
	 * @brief Resize-initialize the array
	 *
	 * Resizes and initializes the array in the same function.
	 * @param new_size New size of the array.
	 * @param v Value to initialize the array with.
	 */
	void resize(const std::size_t new_size, const T& v) noexcept
	{
		resize<false>(new_size);
		if (m_size > 0) {
			fill(v);
		}
	}

	/**
	 * @brief Size of the array.
	 *
	 * Imitate the vector::size() method.
	 * @returns The size of the array.
	 */
	[[nodiscard]] std::size_t size() const noexcept
	{
		return m_size;
	}

	/**
	 * @brief Element at position @e i.
	 *
	 * Same as std::vector::operator[]
	 * @returns A non-constant reference to the @e i-th element.
	 */
	[[nodiscard]] T& operator[] (const std::size_t i) noexcept
	{
#if defined DEBUG
		assert(i < size());
#endif
		return m_data[i];
	}
	/**
	 * @brief Element at position @e i.
	 *
	 * Same as std::vector::operator[]
	 * @returns A constant reference to the @e i-th element.
	 */
	[[nodiscard]] const T& operator[] (const std::size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < size());
#endif
		return m_data[i];
	}

	/// Non-constant reference to the first element in the array.
	[[nodiscard]] T& first() noexcept
	{
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *m_data;
	}
	/// Constant reference to the first element in the array.
	[[nodiscard]] const T& first() const noexcept
	{
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *m_data;
	}
	/// Non-constant reference to the last element in the array.
	[[nodiscard]] T& back() noexcept
	{
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *(m_data + m_size - 1);
	}
	/// Constant reference to the first element in the array.
	[[nodiscard]] const T& back() const noexcept
	{
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *(m_data + m_size - 1);
	}

	/// Assign the same value to every element in the data
	void fill(const T& v) noexcept
	{
		std::fill(begin(), end(), v);
	}

	/**
	 * @brief Pointer at a specific location of the array.
	 * @param i Position.
	 * @returns A non-constant raw pointer.
	 */
	[[nodiscard]] T *at(const std::size_t i) noexcept
	{
#if defined DEBUG
		assert(i < m_size);
#endif
		return &m_data[i];
	}
	/**
	 * @brief Pointer at a specific location of the array.
	 * @param i Position.
	 * @returns A constant raw pointer.
	 */
	[[nodiscard]] const T *at(const std::size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < m_size);
#endif
		return &m_data[i];
	}

	/// Non-constant raw pointer to first element.
	[[nodiscard]] T *begin() noexcept
	{
		return m_data;
	}
	/// Non-constant raw pointer to last+1 element.
	[[nodiscard]] T *end() noexcept
	{
		return m_data + m_size;
	}

	/// Constant raw pointer to first element.
	[[nodiscard]] const T *begin() const noexcept
	{
		return m_data;
	}
	/// Constant raw pointer to last+1 element.
	[[nodiscard]] const T *end() const noexcept
	{
		return m_data + m_size;
	}

private:

	/// Allocate memory for array @ref m_data only when @ref m_size is greater
	/// than 0.
	void alloc_data() noexcept
	{
		m_data = m_size == 0 ? nullptr : new T[m_size];
	}

protected:

	/// Pointer to the memory allocated by this array.
	T *m_data = nullptr;
	/// The size of this array in number of elements.
	std::size_t m_size = 0;
};

} // namespace detail
} // namespace lal
