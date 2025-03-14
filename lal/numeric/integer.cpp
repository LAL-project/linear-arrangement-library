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

#include <lal/numeric/integer.hpp>

// lal includes
#include <lal/detail/numeric/utils.hpp>

namespace lal {
namespace numeric {

// PUBLIC

integer::integer(mpz_t&& raw) noexcept
{
	m_initialized = true;
	detail::move_mpz(*m_val, std::move(*raw));
}

/* ARITHMETIC OPERATORS */

// -- EXPONENTIATION

integer& integer::powt(const integer& i) noexcept
{
	detail::mpz_pow_mpz(m_val, m_val, i.m_val);
	return *this;
}

/* GETTERS */

std::size_t integer::bytes() const noexcept
{
	return detail::mpz_bytes(m_val);
}

// PRIVATE

void integer::move_into(integer&& i) noexcept
{
	m_initialized = true;
	detail::move_mpz(*m_val, std::move(*i.m_val));
	i.m_initialized = false;
}

} // namespace numeric
} // namespace lal
