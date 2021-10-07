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
#include <algorithm>

namespace lal {
namespace internal {

/*
 * @brief Wrapper of an array for autmatic deallocation of memory.
 *
 * Automatically manage deallocation of memory via destructors.
 */
template<typename T>
struct data_array {
public:
	// Default constructor
	data_array() noexcept = default;

	// Constructor with size
	data_array(const std::size_t n) noexcept : m_size(n) {
		alloc_data();
	}
	// Constructor with size and initial value
	data_array(const std::size_t n, const T& v) noexcept : data_array(n) {
		fill(v);
	}

	// Copy constructor
	data_array(const data_array& d) noexcept : data_array(d.m_size) {
		if (m_size > 0) {
			std::copy(d.begin(), d.end(), begin());
		}
	}
	// Copy assignment operator
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

	// Move constructor
	data_array(data_array&& d) noexcept {
		// steal data
		m_data = d.m_data;
		m_size = d.m_size;
		// invalidate data
		d.m_data = nullptr;
		d.m_size = 0;
	}
	// Move assignment operator
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

	// COPY constructor from generic container
	template<template<typename... Args> class container, typename... Types>
	data_array(const container<Types...>& v) noexcept : data_array(v.size()) {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		std::copy(v.begin(), v.end(), begin());
	}

	// COPY assignment from generic container
	template<template<typename... Args> class container, typename... Types>
	data_array& operator= (const container<Types...>& v) noexcept {
		// assert first type in Types... is 'T'
		static_assert(std::is_same_v<T, std::tuple_element_t<0, std::tuple<Types...>>>);

		resize(v.size());
		std::copy(v.begin(), v.end(), begin());
		return *this;
	}

	// Destructor
	~data_array() noexcept {
		clear();
	}

	void clear() noexcept {
		delete[] m_data;
		// this is for those who like calling the destructor...
		m_data = nullptr;
	}

	// resize the array
	void resize(std::size_t new_size) noexcept {
		if (new_size != m_size or m_data == nullptr) {
			delete[] m_data;
			m_size = new_size;
			alloc_data();
		}
	}

	// resize-initialize the array
	void resize(std::size_t new_size, const T& v) noexcept {
		resize(new_size);
		if (m_size > 0) {
			fill(v);
		}
	}

	// imitate the vector::size() method
	[[nodiscard]]
	std::size_t size() const noexcept { return m_size; }

	// operator[]
	[[nodiscard]]
	T& operator[] (const std::size_t i) noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_data[i];
	}
	[[nodiscard]]
	const T& operator[] (const std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return m_data[i];
	}

	// assign the same value to every element in the data
	void fill(const T& v) noexcept {
		std::fill(begin(), end(), v);
	}

	// pointer to data (same as begin)
	[[nodiscard]] T *data() noexcept { return m_data; }
	[[nodiscard]] T *data() const noexcept { return m_data; }

	// pointer to non-constant first element and last+1 element
	[[nodiscard]] T *begin() noexcept { return m_data; }
	[[nodiscard]] T *end() noexcept { return &m_data[m_size]; }

	// pointer to constant first element and last+1 element
	[[nodiscard]] const T *begin() const noexcept { return m_data; }
	[[nodiscard]] const T *end() const noexcept { return &m_data[m_size]; }

private:
	// allocate memory for array m_data only when m_size > 0
	void alloc_data() noexcept {
		m_data = m_size == 0 ? nullptr : new T[m_size];
	}

private:
	// the data of this array
	T *m_data = nullptr;
	// the size of this array in number of elements
	std::size_t m_size = 0;
};

} // -- namespace internal
} // -- namespace lal
