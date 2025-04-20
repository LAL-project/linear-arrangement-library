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

#if defined LAL_REGISTER_BIBLIOGRAPHY

/// The list of bibliographic entries. Activated or not.
static std::array<bool, num_bib_entries> m_entry_used = []()
{
	std::array<bool, num_bib_entries> res;
	res.fill(false);
	return res;
}();

/// Set a particular bibliographic entry to used.
inline void set_entry(const bib_entries& entry) noexcept
{
	m_entry_used[static_cast<std::size_t>(entry)] = true;
}

/// Set a particular bibliographic entry to not used.
inline void unset_entry(const bib_entries& entry) noexcept
{
	m_entry_used[static_cast<std::size_t>(entry)] = false;
}

/// Is a particular bibliographic entry set?
inline bool is_entry_set(const bib_entries& entry) noexcept
{
	return m_entry_used[static_cast<std::size_t>(entry)];
}

/**
 * @brief Register a specific bibliography entry into the manager.
 * @param entry The specific entry to set.
 */
inline void register_bib_entry(const bib_entries& entry) noexcept
{
	if (not is_entry_set(entry)) [[unlikely]] {
		set_entry(entry);
	}
}

#endif

} // namespace lal
