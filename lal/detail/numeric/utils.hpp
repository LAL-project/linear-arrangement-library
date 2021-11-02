﻿/*********************************************************************
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
 
#pragma once

// C++ includes
#include <cstdint>

// gmp includes
#include <gmp.h>

namespace lal {
namespace detail {

/* Other arithmetic operations */

/*
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
void mpz_pow_mpz(mpz_t& r, const mpz_t& b, const mpz_t& e);

/*
 * @brief Rational-Integer division.
 *
 * Divide a rational \f$r\f$ by an integer \f$k\f$.
 * @param[out] r The rational to be divided by \f$k\f$. Result is \f$r := r/k\f$.
 * @param[in] k The integer that divides the rational.
 */
void mpz_divide_mpq(mpq_t& r, const mpz_t& k);

/*
 * @brief Rational-Rational division.
 *
 * Divide a rational \f$r_1\f$ by another rational \f$r_2\f$.
 * @param[out] r1 The rational to be divided by \f$k\f$. Result is \f$r_1 := r_1/r_2\f$.
 * @param[in] r2 The integer that divides the rational.
 */
void mpq_divide_mpq(mpq_t& r1, const mpq_t& r2);

/*
 * @brief Power operation.
 *
 * Raise a rational value \f$r\f$ to a certain power \f$p\f$.
 * @param[out] r Rational value. Result is \f$ r = r^p\f$.
 * @param[in] p Exponent.
 */
void operate_power(mpq_t& r, uint64_t p);

/*
 * @brief Power operation.
 *
 * Raise a rational value \f$r\f$ to a certain power \f$p\f$.
 * @param[out] r Rational value. Result is \f$ r = r^p\f$.
 * @param[in] p Exponent.
 */
void operate_power(mpq_t& r, const mpz_t& p);

/* Getters of mpz_t objects */

// Return the amount of bytes of a gmp's integer value.
inline
std::size_t mpz_bytes(const mpz_t& v) noexcept
{
	const std::size_t alloc = static_cast<std::size_t>(v[0]._mp_alloc);
	return sizeof(mp_limb_t)*alloc;
}

} // -- namespace detail
} // -- namespace lal