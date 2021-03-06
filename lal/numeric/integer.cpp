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
 
#include <lal/numeric/integer.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <string>
using namespace std;

// lal includes
#include <lal/internal/gmp/utils.hpp>

/* This macro takes a signed integer and converts it into an unsigned
 * integer. First, drops the negative sign (if appropriate) and then
 * converts the result into an actual uint64_t (using static cast).
 */
#define my_abs(x) static_cast<uint64_t>((x < 0 ? -x : x))
#define to_uint64(x) static_cast<uint64_t>(x)

namespace lal {
namespace numeric {

// PUBLIC

integer::integer(integer&& i) noexcept {
	// move i's contents
	internal::move_mpz_to_mpz(i.m_val, m_val);
	i.m_initialized = false;
}

/* OPERATORS */

integer& integer::operator= (integer&& i) noexcept {
	mpz_clear(m_val);

	// move i's contents
	internal::move_mpz_to_mpz(i.m_val, m_val);
	m_initialized = true;

	i.m_initialized = false;
	return *this;
}

/* ARITHMETIC OPERATORS */

// -- ADDITION

integer& integer::operator+= (int64_t i) noexcept {
	if (i > 0) {	mpz_add_ui(m_val, m_val, to_uint64(i));	}
	else {			mpz_sub_ui(m_val, m_val, my_abs(i));	}
	return *this;
}

// -- SUBSTRACTION

integer& integer::operator-= (int64_t i) noexcept {
	if (i > 0) {	mpz_sub_ui(m_val, m_val, to_uint64(i));	}
	else {			mpz_add_ui(m_val, m_val, my_abs(i));	}
	return *this;
}

// -- DIVISION

integer& integer::operator/= (int64_t i) noexcept {
	mpz_div_ui(m_val, m_val, my_abs(i));
	mpz_mul_si(m_val, m_val, (i<0 ? -1 : 1));
	return *this;
}

// -- EXPONENTIATION

integer& integer::operator^= (const integer& i) noexcept {
	internal::mpz_pow_mpz(m_val, m_val, i.m_val);
	return *this;
}

/* GETTERS */

size_t integer::bytes() const noexcept
{ return internal::mpz_bytes(m_val); }

} // -- namespace numeric
} // -- namespace lal
