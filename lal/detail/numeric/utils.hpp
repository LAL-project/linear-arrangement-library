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
 
#pragma once

// C++ includes
#include <cstdint>

// gmp includes
#include <gmp.h>

namespace lal {
namespace detail {

/* Other arithmetic operations */

/**
 * @brief Computes the exponentiation of a big integer to another big integer.
 *
 * Fast exponentiation algorithm.
 *
 * This function has, as an exception, its output parameter as its first
 * parameter.
 * @param[out] r Result. \f$r = b^e\f$.
 * @param[in] b Base.
 * @param[in] e Exponent.
 */
inline
void mpz_pow_mpz(mpz_t& r, const mpz_t& b, const mpz_t& e) noexcept  {
	if (mpz_fits_ulong_p(e)) {
		mpz_pow_ui(r, b, mpz_get_ui(e));
		return;
	}

	if (mpz_even_p(e)) {
		// r = (b^(e/2))^2

		mpz_t e_half;
		mpz_init(e_half);

		// e_half = e/2
		mpz_div_ui(e_half, e, 2);

		// r = b^(e/2)
		mpz_pow_mpz(r, b, e_half);

		// r = (b^(e/2))^2 = b^e
		mpz_mul(r, r, r);
		mpz_clear(e_half);
		return;
	}

	// r = (b^(e - 1))*b

	mpz_t e_minus_one;
	mpz_init(e_minus_one);
	// e_minus_one = e - 1
	mpz_sub_ui(e_minus_one, e, 1);

	// r = b^(e - 1)
	mpz_pow_mpz(r, b, e_minus_one);

	// r = (b^(e - 1))*b = b^e
	mpz_mul(r, r, b);
	mpz_clear(e_minus_one);
}

/**
 * @brief Rational-Integer division.
 *
 * Divide a rational \f$r\f$ by an integer \f$k\f$.
 * @param[out] r The rational to be divided by \f$k\f$. Result is \f$r := r/k\f$.
 * @param[in] k The integer that divides the rational.
 */
inline
void mpz_divide_mpq(mpq_t& r, const mpz_t& k) noexcept {
	mpz_t b;
	mpz_init(b);

	mpq_get_den(b, r);	// r = a/b

	mpz_mul(b, b, k); // b <- b*k

	mpq_set_den(r, b); // r <- a/(b*k)
	mpq_canonicalize(r);

	mpz_clear(b);
}

/**
 * @brief Rational-Rational division.
 *
 * Divide a rational \f$r_1\f$ by another rational \f$r_2\f$.
 * @param[out] num The rational to be divided by \f$k\f$. Result is \f$r_1 := r_1/r_2\f$.
 * @param[in] den The rational that divides the rational.
 */
inline
void mpq_divide_mpq(mpq_t& num, const mpq_t& den) noexcept {
	mpz_t a, b, c, d;
	mpz_inits(a, b, c, d, nullptr);

	mpq_get_num(a, num);	// num = a/b
	mpq_get_den(b, num);
	mpq_get_num(c, den);	// den = c/d
	mpq_get_den(d, den);

	mpz_mul(a, a, d);
	mpz_mul(b, b, c);

	mpq_set_num(num, a);
	mpq_set_den(num, b);
	mpq_canonicalize(num);

	mpz_clears(a, b, c, d, nullptr);
}

/**
 * @brief Power operation.
 *
 * Raise a rational value \f$r\f$ to a certain power \f$p\f$.
 * @param[out] r Rational value. Result is \f$r = r^p\f$.
 * @param[in] p Exponent.
 */
inline
void operate_power(mpq_t& r, uint64_t p) noexcept {
	if (p == 0) {
		mpq_set_si(r, 1, 1);
		return;
	}
	if (p == 1) {
		return;
	}

	mpz_t num, den;
	mpz_inits(num, den, nullptr);

	// get numerator and denominator of 'res'
	mpq_get_num(num, r);
	mpq_get_den(den, r);

	// operate power
	mpz_pow_ui(num, num, p);
	mpz_pow_ui(den, den, p);

	// set value into 'res'
	mpq_set_num(r, num);
	mpq_set_den(r, den);

	// canonicalise
	mpq_canonicalize(r);

	mpz_clears(num, den, nullptr);
}

/**
 * @brief Power operation.
 *
 * Raise a rational value \f$r\f$ to a certain power \f$p\f$.
 * @param[out] r Rational value. Result is \f$r = r^p\f$.
 * @param[in] p Exponent.
 */
inline
void operate_power(mpq_t& r, const mpz_t& p) noexcept {
	if (mpz_cmp_ui(p, 0) == 0) {
		mpq_set_si(r, 1, 1);
		return;
	}
	if (mpz_cmp_ui(p, 1) == 0) {
		return;
	}

	mpz_t num, den;
	mpz_inits(num, den, nullptr);

	// get numerator and denominator of 'res'
	mpq_get_num(num, r);
	mpq_get_den(den, r);

	// operate power
	mpz_pow_mpz(num, num, p);
	mpz_pow_mpz(den, den, p);

	// set value into 'res'
	mpq_set_num(r, num);
	mpq_set_den(r, den);

	// canonicalise
	mpq_canonicalize(r);

	mpz_clears(num, den, nullptr);
}

/* Getters of mpz_t objects */

/// Returns the amount of bytes of a gmp's integer value.
[[nodiscard]] inline std::size_t mpz_bytes(const mpz_t& v) noexcept
{
	const std::size_t alloc = static_cast<std::size_t>(v[0]._mp_alloc);
	return sizeof(mp_limb_t)*alloc;
}

} // -- namespace detail
} // -- namespace lal
