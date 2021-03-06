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
 
#include <lal/numeric/rational.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
using namespace std;

// lal includes
#include <lal/internal/gmp/utils.hpp>

namespace lal {
namespace numeric {

// PUBLIC

rational::rational(integer&& i) noexcept {
	internal::move_mpz_to_mpq(i.m_val, m_val);
	i.m_initialized = false;
}

rational::rational(integer&& n, integer&& d) noexcept {
	// move n's and d's contents
	internal::move_mpz_to_mpq(n.m_val, d.m_val, m_val);
	mpq_canonicalize(m_val);

	n.m_initialized = false;
	d.m_initialized = false;
}

rational::rational(rational&& r) noexcept {
	internal::move_mpq_to_mpq(r.m_val, m_val);
	r.m_initialized = false;
}

/* OPERATORS */

rational& rational::operator= (integer&& i) noexcept {
	mpq_clear(m_val);

	internal::move_mpz_to_mpq(i.m_val, m_val);
	m_initialized = true;

	i.m_initialized = false;
	return *this;
}

rational& rational::operator= (rational&& r) noexcept {
	mpq_clear(m_val);

	// move r's contents
	internal::move_mpq_to_mpq(r.m_val, m_val);
	m_initialized = true;

	r.m_initialized = false;
	return *this;
}

// -- DIVISION

rational& rational::operator/= (int64_t I) noexcept {
	integer i(I);
	internal::mpz_divide_mpq(m_val, i.get_raw_value());
	return *this;
}

rational& rational::operator/= (const integer& i) noexcept {
	internal::mpz_divide_mpq(m_val, i.get_raw_value());
	return *this;
}
rational& rational::operator/= (const rational& r) noexcept {
	internal::mpq_divide_mpq(m_val, r.m_val);
	return *this;
}

// -- EXPONENTIATION

rational rational::operator^ (uint64_t p) const noexcept {
	rational r(*this);
	internal::operate_power(r.m_val, p);
	return r;
}

rational rational::operator^ (const integer& p) const noexcept {
	rational r(*this);
	internal::operate_power(r.m_val, p.get_raw_value());
	return r;
}

rational& rational::operator^= (uint64_t p) noexcept {
	internal::operate_power(m_val, p);
	return *this;
}

rational& rational::operator^= (const integer& p) noexcept {
	internal::operate_power(m_val, p.get_raw_value());
	return *this;
}

/* GETTERS */

size_t rational::bytes() const noexcept {
	const size_t bs =
	internal::mpz_bytes({m_val[0]._mp_num}) +
	internal::mpz_bytes({m_val[0]._mp_den});
	return bs;
}

/* CONVERTERS */

void rational::as_integer(integer& i) const noexcept {
	mpz_t numerator, denominator;
	mpz_inits(numerator, denominator, NULL);

	// COPY the contents of this rational into 'num' and 'den'
	mpq_get_num(numerator, m_val);
	mpq_get_den(denominator, m_val);

	// integer division (numerator/denominator)
	mpz_div(numerator, numerator, denominator);

	// free the contents of integer 'i' if needed
	if (i.is_initialized()) { mpz_clear(i.m_val); }
	// MOVE 'numerator' into 'i'
	internal::move_mpz_to_mpz(numerator, i.m_val);

	// there is no need to clear 'numerator' since the function
	// 'move_mpz_to_mpz' does it for us (it really doesn't -- it just
	// moves the contens!).
	mpz_clear(denominator);
}

} // -- namespace numeric
} // -- namespace lal
