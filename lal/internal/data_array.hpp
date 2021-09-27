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
	// the data of this array
	T *data = nullptr;

private:
	// the size of this array in number of elements
	std::size_t m_size;

public:
	// Constructor with size
	data_array(const std::size_t n) noexcept : m_size(n) {
		data = m_size == 0 ? nullptr : new T[m_size];
	}
	// Constructor with size and initial value
	data_array(const std::size_t n, const T& v) noexcept : m_size(n) {
		data = m_size == 0 ? nullptr : new T[m_size];
		fill(v);
	}
	// Destructor
	~data_array() noexcept {
		delete[] data;
		// this is for those who like calling the destructor...
		data = nullptr;
	}

	// Copy constructor
	data_array(const data_array& d) noexcept : m_size(d.m_size) {
		data = m_size == 0 ? nullptr : new T[m_size];
		std::copy(d.begin(), d.end(), begin());
	}
	// Copy assignment operator
	data_array& operator= (const data_array& d) noexcept {
		if (m_size != d.m_size) {
			delete[] data;
			m_size = d.m_size;
			data = new T[m_size];
		}
		std::copy(d.begin(), d.end(), begin());
		return *this;
	}

	// Move constructor
	data_array(data_array&& d) noexcept : m_size(d.m_size) {
		// steal data
		data = d.data;
		// invalidate data
		d.data = nullptr;
		d.m_size = 0;
	}
	// Move assignment operator
	data_array& operator= (data_array&& d) noexcept {
		// free yourself
		delete[] data;
		// steal from others
		data = d.data;
		m_size = d.m_size;
		// invalidate data
		d.data = nullptr;
		d.m_size = 0;
		return *this;
	}

	// resize the array
	inline void resize(std::size_t new_size) noexcept {
		if (new_size != m_size) {
			m_size = new_size;
			delete[] data;
			data = new T[m_size];
		}
	}

	// imitate the vector::size() method
	[[nodiscard]]
	inline constexpr std::size_t size() const noexcept { return m_size; }

	// operator[]
	[[nodiscard]] inline T& operator[] (const std::size_t i) noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return data[i];
	}
	[[nodiscard]]
	inline const T& operator[] (const std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size());
#endif
		return data[i];
	}

	// assign the same value to every element in the data
	inline void fill(const T& v) noexcept {
		std::fill(&data[0], &data[m_size], v);
	}

	// pointer to non-constant first element and last+1 element
	[[nodiscard]] inline T *begin() { return &data[0]; }
	[[nodiscard]] inline T *end() { return &data[m_size]; }

	// pointer to constant first element and last+1 element
	[[nodiscard]] inline const T *begin() const { return &data[0]; }
	[[nodiscard]] inline const T *end() const { return &data[m_size]; }
};

} // -- namespace internal
} // -- namespace lal
