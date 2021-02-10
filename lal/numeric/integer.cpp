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
 
#include <lal/numeric/integer.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <iostream>
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

integer::integer() noexcept {
	mpz_init(m_val);
}

integer::integer(int64_t i) noexcept {
	mpz_init_set_si(m_val, i);
}

integer::integer(const std::string& s) noexcept {
	mpz_init_set_str(m_val, s.c_str(), 10);
}

integer::integer(integer&& i) noexcept {
	// move i's contents
	internal::move_mpz_to_mpz(i.m_val, m_val);
	i.m_initialized = false;
}

integer::integer(const integer& i) noexcept {
	mpz_init_set(m_val, i.m_val);
}

integer::~integer() noexcept {
	mpz_clear(m_val);
}

/* SET VALUE */

void integer::set_si(int64_t i) noexcept {
	if (not is_initialized()) { mpz_init(m_val); }
	mpz_set_si(m_val, i);
	m_initialized = true;
}
void integer::set_ui(uint64_t i) noexcept {
	if (not is_initialized()) { mpz_init(m_val); }
	mpz_set_ui(m_val, i);
	m_initialized = true;
}
void integer::set_integer(const integer &i) noexcept {
	if (not is_initialized()) { mpz_init(m_val); }
	mpz_set(m_val, i.m_val);
	m_initialized = true;
}
void integer::set_str(const std::string& s) noexcept {
	if (not is_initialized()) { mpz_init(m_val); }
	mpz_set_str(m_val, s.c_str(), 10);
	m_initialized = true;
}

/* OPERATORS */

integer& integer::operator= (int64_t i) noexcept {
	set_si(i);
	return *this;
}

integer& integer::operator= (const integer& i) noexcept {
	set_integer(i);
	return *this;
}

integer& integer::operator= (integer&& i) noexcept {
	mpz_clear(m_val);

	// move i's contents
	internal::move_mpz_to_mpz(i.m_val, m_val);
	m_initialized = true;

	i.m_initialized = false;
	return *this;
}

bool integer::operator== (int64_t i) const noexcept			{ return mpz_cmp_si(m_val, i) == 0; }
bool integer::operator== (const string& s) const noexcept	{ return *this == integer(s); }
bool integer::operator== (const integer& i) const noexcept	{ return mpz_cmp(m_val, i.m_val) == 0; }

bool integer::operator!= (int64_t i) const noexcept			{ return not (*this == i); }
bool integer::operator!= (const string& s) const noexcept	{ return not (*this == s); }
bool integer::operator!= (const integer& i) const noexcept	{ return not (*this == i); }

bool integer::operator< (int64_t i) const noexcept			{ return mpz_cmp_si(m_val, i)  < 0; }
bool integer::operator< (const integer& i) const noexcept	{ return mpz_cmp(m_val, i.m_val) < 0; }

bool integer::operator<= (int64_t i) const noexcept			{ return mpz_cmp_si(m_val, i)  <= 0; }
bool integer::operator<= (const integer& i) const noexcept	{ return mpz_cmp(m_val, i.m_val) <= 0; }

bool integer::operator> (int64_t i) const noexcept			{ return mpz_cmp_si(m_val, i)  > 0; }
bool integer::operator> (const integer& i) const noexcept	{ return mpz_cmp(m_val, i.m_val) > 0; }

bool integer::operator>= (int64_t i) const noexcept			{ return mpz_cmp_si(m_val, i)  >= 0; }
bool integer::operator>= (const integer& i) const noexcept	{ return mpz_cmp(m_val, i.m_val) >= 0; }

integer integer::operator+ (int64_t i) const noexcept		{ integer a(*this); a += i; return a; }
integer integer::operator+ (const integer& i) const noexcept{ integer a(*this); a += i;	return a; }

integer& integer::operator+= (int64_t i) noexcept {
	if (i > 0) {	mpz_add_ui(m_val, m_val, to_uint64(i));	}
	else {			mpz_sub_ui(m_val, m_val, my_abs(i));	}
	return *this;
}
integer& integer::operator+= (const integer& i) noexcept		{ mpz_add(m_val, m_val, i.m_val); return *this; }

integer integer::operator- () const noexcept					{ integer a(*this);	mpz_neg(a.m_val, a.m_val); return a; }
integer integer::operator- (int64_t i) const noexcept			{ integer a(*this); a -= i;	return a; }
integer integer::operator- (const integer& i) const noexcept	{ integer a(*this); a -= i;	return a; }

integer& integer::operator- () noexcept							{ mpz_neg(m_val, m_val); return *this; }
integer& integer::operator-= (int64_t i) noexcept {
	if (i > 0) {	mpz_sub_ui(m_val, m_val, to_uint64(i));	}
	else {			mpz_add_ui(m_val, m_val, my_abs(i));	}
	return *this;
}
integer& integer::operator-= (const integer& i) noexcept		{ mpz_sub(m_val, m_val, i.m_val); return *this; }

integer integer::operator* (int64_t i) const noexcept			{ integer a(*this); a *= i;	return a; }
integer integer::operator* (const integer& i) const noexcept	{ integer a(*this); a *= i;	return a; }

integer& integer::operator*= (int64_t i) noexcept				{ mpz_mul_si(m_val, m_val, i);		return *this; }
integer& integer::operator*= (const integer& i) noexcept		{ mpz_mul(m_val, m_val, i.m_val);	return *this; }

integer integer::operator/ (int64_t i) const noexcept 			{ integer a(*this); a /= i;	return a; }
integer integer::operator/ (const integer& i) const noexcept	{ integer a(*this); a /= i;	return a; }

integer& integer::operator/= (int64_t i) noexcept {
	mpz_div_ui(m_val, m_val, my_abs(i));
	mpz_mul_si(m_val, m_val, (i<0 ? -1 : 1));
	return *this;
}
integer& integer::operator/= (const integer& i) noexcept		{ mpz_div(m_val, m_val, i.m_val);	return *this; }

integer integer::operator^ (uint64_t i)	 const noexcept			{ integer r(*this); r ^= i;	return r; }
integer integer::operator^ (const integer& i) const noexcept	{ integer r(*this); r ^= i;	return r; }

integer& integer::operator^= (uint64_t i) noexcept {
	mpz_pow_ui(m_val, m_val, i);
	return *this;
}

integer& integer::operator^= (const integer& i) noexcept {
	internal::mpz_pow_mpz(m_val, m_val, i.m_val);
	return *this;
}

uint64_t integer::operator% (uint64_t i) const noexcept {
	mpz_t r;
	mpz_init(r);
	const uint64_t m = mpz_mod_ui(r, m_val, i);
	mpz_clear(r);
	return m;
}
integer integer::operator% (const integer& i) const noexcept {
	integer r;
	mpz_mod(r.m_val, m_val, i.m_val);
	return r;
}

/* GETTERS */

int32_t integer::get_sign() const noexcept {
	return mpz_sgn(m_val);
}

size_t integer::bytes() const noexcept {
	return (internal::mpz_bytes(m_val));
}

const mpz_t& integer::get_raw_value() const noexcept {
	return m_val;
}

/* CONVERTERS */

int64_t integer::to_int() const noexcept {
	return mpz_get_si(m_val);
}

uint64_t integer::to_uint() const noexcept {
	return mpz_get_ui(m_val);
}

double integer::to_double() const noexcept {
	return mpz_get_d(m_val);
}

string integer::to_string() const noexcept {
	std::string k;
	as_string(k);
	return k;
}

void integer::as_string(string& s) const noexcept {
	char *buf = nullptr;
	buf = mpz_get_str(buf, 10, m_val);
	s = std::string(buf);
	free(buf);
}

/* OTHERS */

void integer::swap(integer& i) noexcept {
	// swap the values
	mpz_swap(m_val, i.m_val);
}

} // -- namespace numeric
} // -- namespace lal
