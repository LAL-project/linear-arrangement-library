/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

// lal includes
#include <lal/detail/data_array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Simple array-like fixed-size queue.
 *
 * This class implements functionalities similar to those of std::queue.
 * This queue, however, can hold only at most a given, fixed amount of elements.
 * If the size of the queue is @e n, then
 * - at most @e n @ref push operations can be done,
 * - at most @e n @ref pop operations can be done.
 *
 * Once the @e n @ref push operations have been done, the queue has exhausted
 * its resources (see @ref is_exhausted) and must be reset (see @ref reset).
 * @tparam T Type of the elements in the queue.
 */
template <class T>
class linear_queue {
public:
	/// Initializes the queue to hold @e n elements.
	void init(std::size_t n) noexcept {
		m_queue.resize(n);
		m_left = 0;
		m_right = 0;
	}

	/// Insert a new element to the queue.
	void push(const T& v) noexcept {
#if defined DEBUG
		assert(not is_full());
#endif
		m_queue[m_right++] = v;
	}
	/// Insert a new element to the queue.
	void push(T&& v) noexcept {
#if defined DEBUG
		assert(not is_exhausted());
#endif
		m_queue[m_right++] = std::move(v);
	}
	/**
	 * @brief Pops the first element of the queue
	 * @returns A copy of the first element.
	 * @post The size of the queue is reduced by one.
	 * @post Pointer @ref m_left is updated.
	 */
	T pop() noexcept {
#if defined DEBUG
		assert(m_left < m_queue.size());
#endif
		return m_queue[m_left++];
	}
	/**
	 * @brief Returns a constant reference to the first element.
	 *
	 */
	const T& front() const noexcept {
#if defined DEBUG
		assert(not is_exhausted());
#endif
		return m_queue[m_left];
	}
	/// Returns the size of the queue.
	std::size_t size() const noexcept {
		return m_right - m_left;
	}

	/**
	 * @brief Makes the queue usable again.
	 *
	 * Memory is not reset, freed, or deallocated.
	 * @post Pointers @ref m_left and @ref m_right are set to 0.
	 */
	void reset() noexcept {
		m_left = m_right = 0;
	}

	/**
	 * @brief Has the queue exhausted its resources?
	 *
	 * The queue has exhausted its resources if @e n pop operations have been
	 * performed. This happens when @ref m_left is equal to the queue size.
	 * @returns A Boolean value (true or false).
	 */
	bool is_exhausted() const noexcept { return m_left == m_queue.size(); }

	/**
	 * @brief Is the queue full?
	 *
	 * The queue is full if @e n push operations have been performed. This
	 * happens when @ref m_right is equal to the queue size.
	 * @returns A Boolean value (true or false).
	 */
	bool is_full() const noexcept { return m_right == m_queue.size(); }

	/// Pointer to begin.
	T *begin() noexcept { return m_queue.begin() + m_left; }
	/// Constant pointer to begin.
	const T *begin() const noexcept { return m_queue.begin() + m_left; }
	/// Pointer to end.
	T *end() noexcept { return m_queue.begin() + m_right; }
	/// Constant pointer to end.
	const T *end() const noexcept { return m_queue.begin() + m_right; }

private:

	/// Data (array) of the queue
	data_array<T> m_queue;
	/// Left pointer to @ref m_queue
	std::size_t m_left;
	/// Right pointer to @ref m_queue
	std::size_t m_right;
};

} // -- namespace detail
} // -- namespace lal
