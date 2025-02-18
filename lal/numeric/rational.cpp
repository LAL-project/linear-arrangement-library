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

#include <lal/numeric/rational.hpp>

// lal includes
#include <lal/detail/numeric/utils.hpp>

namespace lal {
namespace numeric {

// PUBLIC

/* ARITHMETIC OPERATORS */

// -- DIVISION

rational& rational::operator/= (const integer& i) noexcept
{
	detail::mpz_divide_mpq(m_val, i.get_raw_value());
	return *this;
}
rational& rational::operator/= (const rational& r) noexcept
{
	detail::mpq_divide_mpq(m_val, r.m_val);
	return *this;
}

// -- EXPONENTIATION

rational rational::power(uint64_t p) const noexcept
{
	rational r(*this);
	detail::operate_power(r.m_val, p);
	return r;
}

rational rational::power(const integer& p) const noexcept
{
	rational r(*this);
	detail::operate_power(r.m_val, p.get_raw_value());
	return r;
}

rational& rational::powt(uint64_t p) noexcept
{
	detail::operate_power(m_val, p);
	return *this;
}

rational& rational::powt(const integer& p) noexcept
{
	detail::operate_power(m_val, p.get_raw_value());
	return *this;
}

/* GETTERS */

std::size_t rational::bytes() const noexcept
{
	const std::size_t bs = detail::mpz_bytes({m_val[0]._mp_num}) +
						   detail::mpz_bytes({m_val[0]._mp_den});
	return bs;
}

/* CONVERTERS */

void rational::as_integer(integer& i) const noexcept
{
	mpz_t numerator, denominator;
	mpz_inits(numerator, denominator, NULL);

	// COPY the contents of this rational into 'num' and 'den'
	mpq_get_num(numerator, m_val);
	mpq_get_den(denominator, m_val);

	// integer division (numerator/denominator)
	mpz_div(i.m_val, numerator, denominator);

	mpz_clears(numerator, denominator, NULL);
}

} // namespace numeric
} // namespace lal
