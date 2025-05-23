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
 ********************************************************************/

#pragma once

// C++ includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <array>
#endif

// lal includes
#include <lal/bibliography_entries.hpp>

namespace lal {
namespace bibliography {

#if defined LAL_REGISTER_BIBLIOGRAPHY

/// The list of bibliographic entries. Activated or not.
static std::array<bool, num_entries> entries_used = []()
{
	std::array<bool, num_entries> res;
	res.fill(false);
	return res;
}();

/// Set a particular bibliographic entry to used.
constexpr inline void set_entry(const entries& entry) noexcept
{
	entries_used[static_cast<std::size_t>(entry)] = true;
}

/// Set a particular bibliographic entry to not used.
constexpr inline void unset_entry(const entries& entry) noexcept
{
	entries_used[static_cast<std::size_t>(entry)] = false;
}

/// Is a particular bibliographic entry set?
constexpr inline bool is_entry_set(const entries& entry) noexcept
{
	return entries_used[static_cast<std::size_t>(entry)];
}

/**
 * @brief Register a specific bibliography entry into the manager.
 * @param entry The specific entry to set.
 */
constexpr inline void register_entry(const entries& entry) noexcept
{
	if (not is_entry_set(entry)) [[unlikely]] {
		set_entry(entry);
	}
}

#endif

#if defined LAL_REGISTER_BIBLIOGRAPHY

/// Prints the bibliographic entries set to used.
void print() noexcept;

#else

/**
 * @brief Prints the bibliographic entries set to used.
 *
 * This function is not implemented if the library was not compiled with the
 * symbol 'LAL_REGISTER_BIBLIOGRAPHY'.
 */
inline void print() noexcept { }

#endif

} // namespace bibliography
} // namespace lal
