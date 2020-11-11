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
 
#include <lal/numeric/rational.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/internal/gmp/utils.hpp>

namespace lal {
namespace numeric {

// PUBLIC

//rational::rational() { }
rational::rational(int64_t n, uint64_t d) {
	init_si(n, d);
}

rational::rational(const integer& n, const integer& d) {
	if (not n.is_initialized() or not d.is_initialized()) { clear(); return; }
	init_integer(n, d);
}

rational::rational(const std::string& s) {
	init_str(s);
}

rational::rational(integer&& n) {
	*this = std::move(static_cast<integer&>(n));
}

rational::rational(integer&& n, integer&& d) {
	if (not n.is_initialized() or not d.is_initialized()) { clear(); return; }

	if (is_initialized()) {
		// if this rational was initialised (and quite likely,
		// given a value) the mpq_t must be freed
		clear();
	}

	// move 'n's contents and initialise denominator to 1
	internal::move_mpz_to_mpq(n.m_val, d.m_val, m_val);
	mpq_canonicalize(m_val);
	m_initialized = true;
	n.m_initialized = false;
	d.m_initialized = false;
}

rational::rational(rational&& r) {
	*this = std::move(static_cast<rational&>(r));
}

rational::rational(const rational& r) {
	if (not r.is_initialized()) { clear(); return; }
	init_mpq(r.m_val);
}

rational::~rational() {
	clear();
}

/* ALLOC AND DEALLOC */

void rational::init() {
	if (is_initialized()) { return; }
	mpq_init(m_val);
	m_initialized = true;
}

void rational::init_si(int64_t n, uint64_t d) {
	init();
	set_si(n, d);
}

void rational::init_ui(uint64_t n, uint64_t d) {
	init();
	set_ui(n, d);
}

void rational::init_str(const std::string& s) {
	init();
	set_str(s);
}

void rational::init_integer(const integer& n, const integer& d) {
	assert(n.is_initialized() and d.is_initialized());

	init();
	set_integer(n, d);
}

void rational::clear() {
	if (is_initialized()) {
		mpq_clear(m_val);
	}
	m_initialized = false;
}

/* SET VALUE */

void rational::set_si(int64_t n, uint64_t d) {
	mpq_set_si(m_val, n, d);
	mpq_canonicalize(m_val);
}
void rational::set_ui(uint64_t n, uint64_t d) {
	mpq_set_ui(m_val, n, d);
	mpq_canonicalize(m_val);
}
void rational::set_str(const std::string& s) {
	mpq_set_str(m_val, s.c_str(), 10);
	mpq_canonicalize(m_val);
}
void rational::set_integer(const integer& n, const integer& d) {
	assert(n.is_initialized() and d.is_initialized());
	mpq_set_num(m_val, n.get_raw_value());
	mpq_set_den(m_val, d.get_raw_value());
	mpq_canonicalize(m_val);
}

void rational::invert() {
	mpq_inv(m_val, m_val);
}

/* OPERATORS */

rational& rational::operator= (int64_t i) {
	init_si(i);
	return *this;
}

rational& rational::operator= (const integer& i) {
	if (not i.is_initialized()) {
		clear();
		return *this;
	}

	init_integer(i);
	return *this;
}

rational& rational::operator= (const rational& r) {
	if (not r.is_initialized()) {
		clear();
		return *this;
	}

	init();
	mpq_set(m_val, r.m_val);
	return *this;
}

rational& rational::operator= (integer&& i) {
	if (not i.is_initialized()) {
		clear();
		return *this;
	}

	if (is_initialized()) {
		// if this rational was initialised (and quite likely,
		// given a value) the mpq_t must be freed
		clear();
	}
	// move 'r's contents
	internal::move_mpz_to_mpq(i.m_val, m_val);
	mpq_canonicalize(m_val);
	m_initialized = true;
	i.m_initialized = false; // i is no longer initialised
	return *this;
}

rational& rational::operator= (rational&& r) {
	if (not r.is_initialized()) {
		clear();
		return *this;
	}

	if (is_initialized()) {
		// if this rational was initialised (and quite likely,
		// given a value) the mpq_t must be freed
		clear();
	}
	// move 'r's contents
	internal::move_mpq_to_mpq(r.m_val, m_val);
	mpq_canonicalize(m_val);
	m_initialized = true;
	r.m_initialized = false; // r is no longer initialised
	return *this;
}

bool rational::operator== (int64_t i) const				{ rational r(i); 	return mpq_equal(m_val, r.m_val) != 0; }
bool rational::operator== (const integer& i) const		{ rational r(i); 	return mpq_equal(m_val, r.m_val) != 0; }
bool rational::operator== (const rational& r) const		{					return mpq_equal(m_val, r.m_val) != 0; }

bool rational::operator!= (int64_t i) const				{ return not (*this == i); }
bool rational::operator!= (const integer& i) const		{ return not (*this == i); }
bool rational::operator!= (const rational& r) const		{ return not (*this == r); }

bool rational::operator< (int64_t i) const				{ rational r(i); 	return mpq_cmp(m_val, r.m_val) < 0; }
bool rational::operator< (const integer& i) const		{ rational r(i); 	return mpq_cmp(m_val, r.m_val) < 0; }
bool rational::operator< (const rational& r) const		{					return mpq_cmp(m_val, r.m_val) < 0; }

bool rational::operator<= (int64_t i) const				{ rational r(i); 	return mpq_cmp(m_val, r.m_val) <= 0; }
bool rational::operator<= (const integer& i) const		{ rational r(i); 	return mpq_cmp(m_val, r.m_val) <= 0; }
bool rational::operator<= (const rational& r) const		{ 					return mpq_cmp(m_val, r.m_val) <= 0; }

bool rational::operator> (int64_t i) const				{ rational r(i); 	return mpq_cmp(m_val, r.m_val) > 0; }
bool rational::operator> (const integer& i) const		{ rational r(i); 	return mpq_cmp(m_val, r.m_val) > 0; }
bool rational::operator> (const rational& r) const		{ 					return mpq_cmp(m_val, r.m_val) > 0; }

bool rational::operator>= (int64_t i) const				{ rational r(i); 	return mpq_cmp(m_val, r.m_val) >= 0; }
bool rational::operator>= (const integer& i) const		{ rational r(i); 	return mpq_cmp(m_val, r.m_val) >= 0; }
bool rational::operator>= (const rational& r) const		{					return mpq_cmp(m_val, r.m_val) >= 0; }

rational rational::operator+ (int64_t i) const			{ rational r(*this); r += i; return r; }
rational rational::operator+ (const integer& i) const	{ rational r(*this); r += i; return r; }
rational rational::operator+ (const rational& r) const	{ rational k(*this); k += r; return k; }

rational& rational::operator+= (int64_t i)				{ rational r(i); 	mpq_add(m_val, m_val, r.m_val); return *this; }
rational& rational::operator+= (const integer& i)		{ rational r(i); 	mpq_add(m_val, m_val, r.m_val); return *this; }
rational& rational::operator+= (const rational& r)		{					mpq_add(m_val, m_val, r.m_val); return *this; }

rational rational::operator- () const 					{ rational r(*this); mpq_neg(r.m_val, r.m_val); return r; }
rational rational::operator- (int64_t i) const			{ rational r(*this); r -= i; return r; }
rational rational::operator- (const integer& i) const	{ rational r(*this); r -= i; return r; }
rational rational::operator- (const rational& r) const	{ rational k(*this); k -= r; return k; }

rational& rational::operator- ()						{					mpq_neg(m_val, m_val); return *this; }
rational& rational::operator-= (int64_t i)				{ rational r; r.init_si(i);
																			mpq_sub(m_val, m_val, r.m_val); return *this; }
rational& rational::operator-= (const integer& i)		{ rational r(i);	mpq_sub(m_val, m_val, r.m_val); return *this; }
rational& rational::operator-= (const rational& r)		{					mpq_sub(m_val, m_val, r.m_val); return *this; }

rational rational::operator* (int64_t i) const			{ rational r(*this); r *= i; return r; }
rational rational::operator* (const integer& i) const	{ rational r(*this); r *= i; return r; }
rational rational::operator* (const rational& r) const	{ rational k(*this); k *= r; return k; }

rational& rational::operator*= (int64_t i)				{ rational r(i);	mpq_mul(m_val, m_val, r.m_val); return *this; }
rational& rational::operator*= (const integer& i)		{ rational r(i);	mpq_mul(m_val, m_val, r.m_val); return *this; }
rational& rational::operator*= (const rational& r)		{					mpq_mul(m_val, m_val, r.m_val); return *this; }

rational rational::operator/ (int64_t i) const			{ rational r(*this); r /= i; return r; }
rational rational::operator/ (const integer& i) const	{ rational r(*this); r /= i; return r; }
rational rational::operator/ (const rational& r) const	{ rational k(*this); k /= r; return k; }

rational& rational::operator/= (int64_t I) {
	integer i(I);
	internal::mpz_divide_mpq(m_val, i.get_raw_value());
	return *this;
}

rational& rational::operator/= (const integer& i) {
	internal::mpz_divide_mpq(m_val, i.get_raw_value());
	return *this;
}
rational& rational::operator/= (const rational& r) {
	internal::mpq_divide_mpq(m_val, r.m_val);
	return *this;
}

rational rational::operator^ (uint64_t p) const {
	rational r(*this);
	internal::operate_power(r.m_val, p);
	return r;
}

rational rational::operator^ (const integer& p) const {
	rational r(*this);
	internal::operate_power(r.m_val, p.get_raw_value());
	return r;
}

rational& rational::operator^= (uint64_t p) {
	internal::operate_power(m_val, p);
	return *this;
}

rational& rational::operator^= (const integer& p) {
	internal::operate_power(m_val, p.get_raw_value());
	return *this;
}

/* GETTERS */

int rational::get_sign() const {
	return mpq_sgn(m_val);
}

size_t rational::bytes() const {
	if (not is_initialized()) {
		return 0;
	}

	mpz_t num, den;
	mpz_inits(num, den, NULL);

	mpq_get_num(num, m_val);
	mpq_get_den(den, m_val);

	size_t bs = internal::mpz_bytes(num) +
				internal::mpz_bytes(den);

	mpz_clears(num, den, NULL);

	return bs;
}

/* CONVERTERS */

integer rational::to_integer() const {
	integer i(0);
	as_integer(i);
	return i;
}

void rational::as_integer(integer& i) const {
	mpz_t num, den;
	mpz_inits(num, den, NULL);

	mpq_get_num(num, m_val);
	mpq_get_den(den, m_val);

	// if numerator is not 1 then divide numerator by denominator.
	if (mpz_cmp_si(num, 1) != 0) {
		mpz_div(num, num, den);
	}

	i.init_mpz(num);

	mpz_clears(num, den, NULL);
}

double rational::to_double() const {
	return mpq_get_d(m_val);
}

void rational::as_double(double& d) const {
	d = mpq_get_d(m_val);
}

string rational::to_string() const {
	string k;
	as_string(k);
	return k;
}

void rational::as_string(string& s) const {
	if (not is_initialized()) {
		s = "uninitialized";
		return;
	}

	char *buf = nullptr;
	buf = mpq_get_str(buf, 10, m_val);
	s = string(buf);
	free(buf);
}

void rational::swap(rational& r) {
	const bool this_ini = is_initialized();
	const bool i_ini = r.is_initialized();

	if (not this_ini and not i_ini) {
		// do nothing
		return;
	}
	else if (not this_ini and i_ini) {
		*this = std::move(r);
	}
	else if (this_ini and not i_ini) {
		r = std::move(*this);
	}
	else {
		// simply swap the values
		mpq_swap(m_val, r.m_val);
	}
}

// PRIVATE

void rational::init_mpq(const mpq_t& mpq) {
	init();
	mpq_set(m_val, mpq);
}

} // -- namespace numeric
} // -- namespace lal
