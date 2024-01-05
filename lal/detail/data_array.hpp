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
#include <algorithm>

namespace lal {
namespace detail {

/**
 * @brief Wrapper of a C array for autmatic deallocation of memory.
 *
 * Automatically manage deallocation of memory via destructors.
 */
template <typename T>
struct data_array {
public:
	/// Default constructor
	data_array() noexcept = default;

	/// Constructor from initializer_list.
	data_array(std::initializer_list<T> l) noexcept : m_size(l.size()) {
		// (un?)fortunately, this will call the constructor of T
		// for every element in m_data.
		alloc_data();
		// array reference
		const T* list_data = std::data(l);
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
	data_array(const std::size_t n) noexcept : m_size(n) {
		alloc_data();
	}
	/**
	 * @brief Constructor with size
	 * @param n Size.
	 * @param v Value to initialize the array with.
	 */
	data_array(const std::size_t n, const T& v) noexcept : data_array(n) {
		fill(v);
	}
	/// Copy constructor
	data_array(const data_array& d) noexcept : data_array(d.m_size) {
		if (m_size > 0) {
			std::copy(d.begin(), d.end(), begin());
		}
	}
	/// Copy assignment operator
	data_array& operator= (const data_array& d) noexcept {
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
	data_array(data_array&& d) noexcept {
		// steal data
		m_data = d.m_data;
		m_size = d.m_size;
		// invalidate data
		d.m_data = nullptr;
		d.m_size = 0;
	}
	/// Move assignment operator
	data_array& operator= (data_array&& d) noexcept {
		// free yourself
		delete[] m_data;
		// steal from others
		m_data = d.m_data;
		m_size = d.m_size;
		// invalidate data
		d.m_data = nullptr;
		d.m_size = 0;
		return *this;
	}

	/*
	/// Copy constructor from generic container
	template <template <typename... Args> class container, typename... Types>
	data_array(const container<Types...>& v) noexcept : data_array(v.size()) {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		std::copy(v.begin(), v.end(), begin());
	}

	/// Copy assignment operator from generic container
	template <template <typename... Args> class container, typename... Types>
	data_array& operator= (const container<Types...>& v) noexcept {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		resize(v.size());
		std::copy(v.begin(), v.end(), begin());
		return *this;
	}
	*/

	/// Destructor
	~data_array() noexcept {
		clear();
	}

	/// Comparison of equal data arrays
	bool operator== (const data_array& d) const noexcept {
		if (size() != d.size()) { return size() < d.size(); }

		for (std::size_t i = 0; i < size(); ++i) {
			if (m_data[i] != d.m_data[i]) {
				return false;
			}
		}
		return true;
	}

	/// Clear the contents of the array.
	void clear() noexcept {
		delete[] m_data;
		m_size = 0;
		// this is for those who like calling the destructor...
		m_data = nullptr;
	}

	/**
	 * @brief Resize the array.
	 *
	 * Does nothing if @e new_size is the same as the current size.
	 * @param new_size The new size of the array.
	 */
	void resize(std::size_t new_size) noexcept {
		if (new_size != m_size or m_data == nullptr) {
			delete[] m_data;
			m_size = new_size;
			alloc_data();
		}
	}

	/**
	 * @brief Resize-initialize the array
	 *
	 * Resizes and initializes the array in the same function.
	 * @param new_size New size of the array.
	 * @param v Value to initialize the array with.
	 */
	void resize(std::size_t new_size, const T& v) noexcept {
		resize(new_size);
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
	[[nodiscard]] std::size_t size() const noexcept { return m_size; }

	/**
	 * @brief Element at position @e i.
	 *
	 * Same as std::vector::operator[]
	 * @returns A non-constant reference to the @e i-th element.
	 */
	[[nodiscard]] T& operator[] (const std::size_t i) noexcept {
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
	[[nodiscard]] const T& operator[] (const std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_data[i];
	}

	/// Non-constant reference to the first element in the array.
	[[nodiscard]] T& first() noexcept {
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *m_data;
	}
	/// Constant reference to the first element in the array.
	[[nodiscard]] const T& first() const noexcept {
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *m_data;
	}
	/// Non-constant reference to the last element in the array.
	[[nodiscard]] T& back() noexcept {
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *(m_data + m_size - 1);
	}
	/// Constant reference to the first element in the array.
	[[nodiscard]] const T& back() const noexcept {
#if defined DEBUG
		assert(m_size > 0);
#endif
		return *(m_data + m_size - 1);
	}

	/// Assign the same value to every element in the data
	void fill(const T& v) noexcept {
		std::fill(begin(), end(), v);
	}

	/**
	 * @brief Pointer at a specific location of the array.
	 * @param i Position.
	 * @returns A non-constant raw pointer.
	 */
	[[nodiscard]] T *at(std::size_t i) noexcept {
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
	[[nodiscard]] T const *at(std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < m_size);
#endif
		return &m_data[i];
	}

	/// Non-constant raw pointer to first element.
	[[nodiscard]] T *begin() noexcept { return m_data; }
	/// Non-constant raw pointer to last+1 element.
	[[nodiscard]] T *end() noexcept { return m_data + m_size; }

	/// Constant raw pointer to first element.
	[[nodiscard]] T const *begin() const noexcept { return m_data; }
	/// Constant raw pointer to last+1 element.
	[[nodiscard]] T const *end() const noexcept { return m_data + m_size; }

private:
	/// Allocate memory for array @ref m_data only when @ref m_size is greater
	/// than 0.
	void alloc_data() noexcept {
		m_data = m_size == 0 ? nullptr : new T[m_size];
	}

protected:
	/// Pointer to the memory allocated by this array.
	T *m_data = nullptr;
	/// The size of this array in number of elements.
	std::size_t m_size = 0;
};

} // -- namespace detail
} // -- namespace lal
