/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
 */
template<typename T>
struct data_array {
	T *data = nullptr;
	const std::size_t size;

	data_array(const std::size_t n) noexcept : size(n) {
		data = new T[n];
	}
	data_array(const std::size_t n, const T& v) noexcept : size(n) {
		data = new T[n];
		fill(v);
	}
	~data_array() { delete[] data; }

	[[nodiscard]]
	inline T& operator[] (const std::size_t i) noexcept {
#if defined DEBUG
		assert(i < size);
#endif
		return data[i];
	}

	[[nodiscard]]
	inline const T& operator[] (const std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < size);
#endif
		return data[i];
	}

	inline void fill(const T& v) noexcept {
		std::fill(&data[0], &data[size], v);
	}
};

} // -- namespace internal
} // -- namespace lal
