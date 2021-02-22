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
 
#pragma once

// C++ includes
#include <cstdint>

// gmp includes
#include <gmp.h>

namespace lal {
namespace internal {

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
size_t mpz_bytes(const mpz_t& v);

/* Moving gmp types */

/*
 * @brief Move the contents from 'source' to 'target'.
 *
 * The contents are moved in a way that 'source' no longer has them.
 * @param[in] source The mpz_t whose contents we want.
 * @param[out] target The mpz_t with the contents of 'source'.
 * @pre @e source must be initialised.
 * @pre Optionally, target can be initialised, but should not have been
 * assigned any value (otherwise its contents are never going to be freed,
 * thus causing memory leaks).
 * @post @e source does not hold any value.
 */
void move_mpz_to_mpz(mpz_t& source, mpz_t& target);

/*
 * @brief Move the contents from 'source' to 'target'.
 *
 * The contents are moved in a way that 'source' no longer has them.
 * @param[in] source The mpq_t whose contents we want.
 * @param[out] target The mpq_t with the contents of 'source'.
 * @pre @e source must be initialised.
 * @pre @e target should not be initialised (otherwise its contents
 * are never going to be freed, thus causing memory leaks).
 * @post @e source does not hold any value.
 */
void move_mpq_to_mpq(mpq_t& source, mpq_t& target);

/*
 * @brief Move the contents from 'source' to 'target'.
 *
 * The contents are moved in a way that 'source' no longer has them.
 * @param[in] source The mpz_t whose contents we want (numerator).
 * @param[out] target The mpq_t with the contents of 'source'.
 * @pre @e source must be initialised.
 * @pre @e target should not be initialised (otherwise its contents
 * are never going to be freed, thus causing memory leaks).
 * @post @e source does not hold any value.
 * @post The denominator of @e target is initialised to 1
 */
void move_mpz_to_mpq(mpz_t& source, mpq_t& target);

/*
 * @brief Move the contents from 'source' to 'target'.
 *
 * The contents are moved in a way that 'source' no longer has them.
 * @param[in] source_n The mpz_t whose contents we want (numerator).
 * @param[in] source_d The mpz_t whose contents we want (denominator).
 * @param[out] target The mpq_t with the contents of 'source'.
 * @pre @e source must be initialised.
 * @pre @e target should not be initialised (otherwise its contents
 * are never going to be freed, thus causing memory leaks).
 * @post @e source does not hold any value.
 */
void move_mpz_to_mpq(mpz_t& source_n, mpz_t& source_d, mpq_t& target);

} // -- namespace internal
} // -- namespace lal
