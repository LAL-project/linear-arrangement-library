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
#include <bits/stl_algobase.h>

namespace lal {
namespace internal {

/*
 * @brief Wrapper of an array for autmatic deallocation of memory.
 *
 * Automatically manage deallocation of memory via destructors.
 * Objects of this class are not to be moved nor copied.
 */
template<typename T>
struct data_array {
	// the data of this array
	T *data = nullptr;
	// the size of this array -- making this const forces
	// users to use the non-empty constructor
	const std::size_t _size;

	data_array(const std::size_t n) noexcept : _size(n) {
		data = new T[n];
	}
	data_array(const std::size_t n, const T& v) noexcept : _size(n) {
		data = new T[n];
		fill(v);
	}
	~data_array() noexcept {
		delete[] data;
	}

	// do not allow copies
	data_array(const data_array& d) = delete;
	data_array& operator= (const data_array& d) = delete;
	// nor moves
	data_array(data_array&& d) noexcept = delete;
	data_array& operator= (data_array&& d) = delete;

	// imitate the vector::size() method
	[[nodiscard]]
	inline constexpr std::size_t size() const noexcept { return _size; }

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
		std::fill(&data[0], &data[_size], v);
	}

	// non-constant pointer to first element and last+1 element
	[[nodiscard]] inline T *begin() { return &data[0]; }
	[[nodiscard]] inline T *end() { return &data[_size]; }

	// constant pointer to first element and last+1 element
	[[nodiscard]] inline const T *begin() const { return &data[0]; }
	[[nodiscard]] inline const T *end() const { return &data[_size]; }
};

} // -- namespace internal
} // -- namespace lal
