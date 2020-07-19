/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
#include <cassert>
using namespace std;

// lal includes
#include <lal/utils/gmp/utils.hpp>

/* This macro takes a signed integer and converts it into an unsigned
 * integer. First, drops the negative sign (if appropriate) and then
 * converts the result into an actual uint64_t (using static cast).
 */
#define my_abs(x) static_cast<uint64_t>((x < 0 ? -x : x))

namespace lal {
namespace numeric {

// PUBLIC

//integer::integer() { }

integer::integer(int64_t i) {
	init_si(i);
}

integer::integer(const std::string& s) {
	init_str(s);
}

integer::integer(integer&& i) {
	// If 'i' is not initialized then 'this->m_val[0]' attributes
	// will contain unitialised attributes, which can cause
	// undefined behaviour when clearing *this. Also, valgrind
	// will complain when testing.
	assert(i.is_initialized());

	// '*this' need not be initialised using mpz_init()
	m_initialized = true;
	utils::steal_from(m_val, i.m_val);

	i.m_initialized = false; // better be safe than sorry
}

integer::integer(const integer& i) {
	*this = i;
}

integer::~integer() {
	clear();
}

/* ALLOC AND DEALLOC */

void integer::init() {
	if (is_initialized()) {
		return;
	}
	mpz_init(m_val);
	m_initialized = true;
}

void integer::init_si(int64_t i) {
	init();
	set_si(i);
}

void integer::init_ui(uint64_t i) {
	init();
	set_ui(i);
}

void integer::init_str(const std::string& s) {
	init();
	set_str(s);
}

void integer::init_mpz(const mpz_t& mpz) {
	init();
	mpz_set(m_val, mpz);
}

void integer::clear() {
	if (not is_initialized()) {
		return;
	}
	mpz_clear(m_val);
	m_initialized = false;
}

/* SET VALUE */

void integer::set_si(int64_t i) {
	assert(is_initialized());
	mpz_set_si(m_val, i);
}
void integer::set_ui(uint64_t i) {
	assert(is_initialized());
	mpz_set_ui(m_val, i);
}
void integer::set_str(const std::string& s)	{
	assert(is_initialized());
	mpz_set_str(m_val, s.c_str(), 10);
}
void integer::set_mpz(const mpz_t& mpz) {
	assert(is_initialized());
	mpz_set(m_val, mpz);
}

/* OPERATORS */

integer& integer::operator= (int64_t i) {
	init_si(i);
	return *this;
}

integer& integer::operator= (const integer& i) {
	assert(i.is_initialized());

	init();
	mpz_set(m_val, i.m_val);
	return *this;
}

integer& integer::operator= (integer&& i) {
	assert(i.is_initialized());

	init();
	mpz_set(m_val, i.m_val);
	i.clear();
	return *this;
}

bool integer::operator== (int64_t i) const			{ return mpz_cmp_si(m_val, i) == 0; }
bool integer::operator== (const integer& i) const	{ return mpz_cmp(m_val, i.m_val) == 0; }

bool integer::operator!= (int64_t i) const			{ return not (*this == i); }
bool integer::operator!= (const integer& i) const	{ return not (*this == i); }

bool integer::operator< (int64_t i) const			{ return mpz_cmp_si(m_val, i)  < 0; }
bool integer::operator< (const integer& i) const	{ return mpz_cmp(m_val, i.m_val) < 0; }

bool integer::operator<= (int64_t i) const			{ return mpz_cmp_si(m_val, i)  <= 0; }
bool integer::operator<= (const integer& i) const	{ return mpz_cmp(m_val, i.m_val) <= 0; }

bool integer::operator> (int64_t i) const			{ return mpz_cmp_si(m_val, i)  > 0; }
bool integer::operator> (const integer& i) const	{ return mpz_cmp(m_val, i.m_val) > 0; }

bool integer::operator>= (int64_t i) const			{ return mpz_cmp_si(m_val, i)  >= 0; }
bool integer::operator>= (const integer& i) const	{ return mpz_cmp(m_val, i.m_val) >= 0; }

integer integer::operator+ (int64_t i) const		{ integer a(*this); a += i; return a; }
integer integer::operator+ (const integer& i) const	{ integer a(*this); a += i;	return a; }

integer& integer::operator+= (int64_t i) {
	if (i > 0) {	mpz_add_ui(m_val, m_val, i);			}
	else {			mpz_sub_ui(m_val, m_val, my_abs(i));	}
	return *this;
}
integer& integer::operator+= (const integer& i)		{ mpz_add(m_val, m_val, i.m_val); return *this; }

integer integer::operator- () const					{ integer a(*this);	mpz_neg(a.m_val, a.m_val); return a; }
integer integer::operator- (int64_t i) const		{ integer a(*this); a -= i;	return a; }
integer integer::operator- (const integer& i) const	{ integer a(*this); a -= i;	return a; }

integer& integer::operator- ()						{ mpz_neg(m_val, m_val); return *this; }
integer& integer::operator-= (int64_t i)			{
	if (i > 0) {	mpz_sub_ui(m_val, m_val, i);			}
	else {			mpz_add_ui(m_val, m_val, my_abs(i));	}
	return *this;
}
integer& integer::operator-= (const integer& i)		{ mpz_sub(m_val, m_val, i.m_val); return *this; }

integer integer::operator* (int64_t i) const		{ integer a(*this); a *= i;	return a; }
integer integer::operator* (const integer& i) const	{ integer a(*this); a *= i;	return a; }

integer& integer::operator*= (int64_t i)			{ mpz_mul_si(m_val, m_val, i);		return *this; }
integer& integer::operator*= (const integer& i)		{ mpz_mul(m_val, m_val, i.m_val);	return *this; }

integer integer::operator/ (int64_t i) const		{ integer a(*this); a /= i;	return a; }
integer integer::operator/ (const integer& i) const	{ integer a(*this); a /= i;	return a; }

integer& integer::operator/= (int64_t i) {
	mpz_div_ui(m_val, m_val, my_abs(i));
	mpz_mul_si(m_val, m_val, (i<0 ? -1 : 1));
	return *this;
}
integer& integer::operator/= (const integer& i)		{ mpz_div(m_val, m_val, i.m_val);	return *this; }

integer integer::operator^ (uint64_t i)	 const		{ integer r(*this); r ^= i;	return r; }
integer integer::operator^ (const integer& i) const	{ integer r(*this); r ^= i;	return r; }

integer& integer::operator^= (uint64_t i) {
	mpz_pow_ui(m_val, m_val, i);
	return *this;
}

integer& integer::operator^= (const integer& i) {
	utils::mpz_pow_mpz(m_val, m_val, i.m_val);
	return *this;
}

uint64_t integer::operator% (uint64_t i) const {
	mpz_t r;
	mpz_init(r);
	uint64_t m = mpz_mod_ui(r, m_val, i);
	mpz_clear(r);
	return m;
}
integer integer::operator% (const integer& i) const {
	integer r;
	r.init();
	mpz_mod(r.m_val, m_val, i.m_val);
	return r;
}

/* GETTERS */

bool integer::is_initialized() const {
	return m_initialized;
}

int32_t integer::get_sign() const {
	return mpz_sgn(m_val);
}

size_t integer::bytes() const {
	return (is_initialized() ? utils::mpz_bytes(m_val) : 0);
}

const mpz_t& integer::get_raw_value() const	{
	return m_val;
}

int64_t integer::to_int() const {
	return mpz_get_si(m_val);
}

uint64_t integer::to_uint() const {
	return mpz_get_ui(m_val);
}

double integer::to_double() const {
	return mpz_get_d(m_val);
}

void integer::swap(integer& i) {
	if (is_initialized() or i.is_initialized()) {
		init();
		i.init();
		mpz_swap(m_val, i.m_val);
	}
}

/* CONVERTERS */

string integer::to_string() const {
	std::string k;
	as_string(k);
	return k;
}

void integer::as_string(string& s) const {
	if (!is_initialized()) {
		s = "integer uninitialized";
		return;
	}

	char *buf = nullptr;
	buf = mpz_get_str(buf, 10, m_val);
	s = std::string(buf);
	free(buf);
}

} // -- namespace numeric
} // -- namespace lal
