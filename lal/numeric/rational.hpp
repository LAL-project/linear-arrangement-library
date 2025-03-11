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

// C includes
#include <gmp.h>

// C++ includes
#include <cstdint>
#include <string>

// lal includes
#include <lal/numeric/integer.hpp>

namespace lal {
namespace numeric {

/**
 * @brief Exact rational number.
 *
 * This class wraps the @e mpq_t data structure from GMP with operators for an
 * easy usage.
 */
class rational {
public:

	/* CONSTRUCTORS */

	/// Empty constructor.
	rational() noexcept
	{
		m_initialized = true;
		mpq_init(m_val);
	}
	/**
	 * @brief Constructor with numerator and denominator.
	 * @param n Numerator, signed integer (basic type).
	 * @param d Denominator, unsigned integer (basic type).
	 */
	template <std::integral T>
	rational(const T n, const uint64_t d = 1) noexcept
	{
		set_number(n, d);
	}
	/**
	 * @brief Constructor with numerator and denominator.
	 * @param n Numerator, a @ref lal::numeric::integer.
	 * @param d Denominator, a @ref lal::numeric::integer.
	 */
	rational(const integer& n, const integer& d = 1) noexcept
	{
		set_integer(n, d);
	}
	/**
	 * @brief Constructor with string.
	 * @param s A string.
	 */
	rational(const std::string& s) noexcept
	{
		set_str(s);
	}
	/**
	 * @brief Copy constructor.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational(const rational& r) noexcept
	{
		m_initialized = true;
		mpq_init(m_val);
		mpq_set(m_val, r.m_val);
	}

	/**
	 * @brief Move constructor.
	 * @param i A @ref lal::numeric::integer
	 */
	rational(integer&& i) noexcept
	{
		move_into(std::move(i));
	}
	/**
	 * @brief Move constructor with numerator and denominator.
	 * @param n Numerator, a @ref lal::numeric::integer.
	 * @param d Denominator, a @ref lal::numeric::integer.
	 * @post Objects @e n and @e d are not initialized.
	 */
	rational(integer&& n, integer&& d) noexcept
	{
		move_into(std::move(n), std::move(d));
	}
	/**
	 * @brief Move constructor.
	 * @param r A @ref lal::numeric::rational.
	 * @post Object @e r is not initialized.
	 */
	rational(rational&& r) noexcept
	{
		move_into(std::move(r));
	}

	/// Destructor.
	~rational() noexcept
	{
		mpq_clear(m_val);
		m_initialized = false;
	}

	/* SETTERS */

	/**
	 * @brief Overwrites the value of this rational with \f$n/d\f$.
	 * @param n Numerator, signed integer (basic type).
	 * @param d Denominator, unsigned integer (basic type).
	 */
	template <std::integral T>
	void set_number(const T n, const uint64_t d = 1) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpq_init(m_val);
		}
		if constexpr (std::is_signed_v<T>) {
			mpq_set_si(m_val, n, d);
		}
		else {
			mpq_set_ui(m_val, n, d);
		}
		if (d > 1) {
			mpq_canonicalize(m_val);
		}
	}
	/**
	 * @brief Overwrites the value in the string @e s.
	 * @param s A string.
	 */
	void set_str(const std::string& s) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpq_init(m_val);
		}
		mpq_set_str(m_val, s.c_str(), 10);
		mpq_canonicalize(m_val);
	}
	/**
	 * @brief Overwrites the value of this rational with the value \f$n/d\f$.
	 * @param n Numerator, a @ref lal::numeric::integer.
	 * @param d Denominator, a @ref lal::numeric::integer.
	 */
	void set_integer(const integer& n, const integer& d) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpq_init(m_val);
		}
		mpq_set_num(m_val, n.m_val);
		mpq_set_den(m_val, d.m_val);
		mpq_canonicalize(m_val);
	}
	/**
	 * @brief Overwrites the value of this rational with the value \f$n/d\f$.
	 * @param n Numerator, a @ref lal::numeric::integer.
	 * @param d Denominator, a @ref lal::numeric::integer.
	 */
	void set_integer(integer&& n, integer&& d) noexcept
	{
		move_into(std::move(n), std::move(d));
	}
	/**
	 * @brief Overwrites the value of this rational with the value \f$n/d\f$.
	 * @param r A lal::numeric::rational.
	 */
	void set_rational(const rational& r) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpq_init(m_val);
		}
		mpq_set(m_val, r.m_val);
	}

	/**
	 * @brief Changes numerator and denominator.
	 *
	 * If this rational value was \f$n/d\f$ then after calling this
	 * method it will be \f$d/n\f$.
	 */
	void invert() noexcept
	{
		mpq_inv(m_val, m_val);
	}

	/* OPERATORS */

	// -- ASSIGNMENT

	/**
	 * @brief Assignment operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	rational& operator= (const T i) noexcept
	{
		set_number(i);
		return *this;
	}
	/**
	 * @brief Assignment operator.
	 * @param s A string.
	 */
	rational& operator= (const std::string& s) noexcept
	{
		set_str(s);
		return *this;
	}
	/**
	 * @brief Assignment operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& operator= (const integer& i) noexcept
	{
		set_integer(i, 1);
		return *this;
	}
	/**
	 * @brief Assignment operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational& operator= (const rational& r) noexcept
	{
		set_rational(r);
		return *this;
	}

	/**
	 * @brief Move assignment operator.
	 * @param i A @ref lal::numeric::integer.
	 * @post Object @e i is not initialized.
	 */
	rational& operator= (integer&& i) noexcept
	{
		mpq_clear(m_val);
		move_into(std::move(i));
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param r A @ref lal::numeric::rational.
	 * @post Object @e i is not initialized.
	 */
	rational& operator= (rational&& r) noexcept
	{
		mpq_clear(m_val);
		move_into(std::move(r));
		return *this;
	}

	// -- COMPARISON OPERATORS

	/**
	 * @brief Three-way comparison operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] std::strong_ordering operator<=> (const rational& r
	) const noexcept
	{
		return mpq_cmp(m_val, r.m_val) <=> 0;
	}
	/**
	 * @brief Equality operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] bool operator== (const rational& r) const noexcept
	{
		return mpq_equal(m_val, r.m_val) != 0;
	}

	/**
	 * @brief Three-way comparison operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] std::strong_ordering operator<=> (const T i) const noexcept
	{
		return (std::is_signed_v<T> ? mpq_cmp_si(m_val, i, 1)
									: mpq_cmp_ui(m_val, i, 1)) <=> 0;
	}
	/**
	 * @brief Equality operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] bool operator== (const T i) const noexcept
	{
		return (std::is_signed_v<T> ? mpq_cmp_si(m_val, i, 1)
									: mpq_cmp_ui(m_val, i, 1)) == 0;
	}

	/**
	 * @brief Three-way comparison operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] std::strong_ordering operator<=> (const integer& i
	) const noexcept
	{
		rational r(i);
		return mpq_cmp(m_val, r.m_val) <=> 0;
	}
	/**
	 * @brief Equality operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] bool operator== (const integer& i) const noexcept
	{
		rational r(i);
		return mpq_equal(m_val, r.m_val);
	}

	// -- ADDITION

	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] rational operator+ (const T i) const noexcept
	{
		rational r = *this;
		r += i;
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 * @param r A @ref lal::numeric::rational.
	 */
	template <std::integral T>
	[[nodiscard]] friend rational
	operator+ (const T i, const rational& r) noexcept
	{
		return r + i;
	}
#endif
	/**
	 * @brief Addition operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] rational operator+ (const integer& i) const noexcept
	{
		rational r = *this;
		r += i;
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Addition operator.
	 * @param i A @ref lal::numeric::integer.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] friend rational
	operator+ (const integer& i, const rational& r) noexcept
	{
		return r + i;
	}
#endif
	/**
	 * @brief Addition operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] rational operator+ (const rational& r) const noexcept
	{
		rational k;
		mpq_add(k.m_val, m_val, r.m_val);
		return k;
	}

	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	rational& operator+= (const T i) noexcept
	{
		rational r(i);
		mpq_add(m_val, m_val, r.m_val);
		return *this;
	}
	/**
	 * @brief Addition operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& operator+= (const integer& i) noexcept
	{
		rational r(i);
		mpq_add(m_val, m_val, r.m_val);
		return *this;
	}
	/**
	 * @brief Addition operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational& operator+= (const rational& r) noexcept
	{
		mpq_add(m_val, m_val, r.m_val);
		return *this;
	}

	// -- SUBSTRACTION

	/// Substraction unary operator.
	[[nodiscard]] rational operator- () const noexcept
	{
		rational r;
		mpq_neg(r.m_val, m_val);
		return r;
	}
	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] rational operator- (const T i) const noexcept
	{
		rational r = -i;
		mpq_add(r.m_val, r.m_val, m_val);
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 * @param r A @ref lal::numeric::rational.
	 */
	template <std::integral T>
	[[nodiscard]] friend rational
	operator- (const T i, const rational& r) noexcept
	{
		return -r + i;
	}
#endif
	/**
	 * @brief Substraction operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] rational operator- (const integer& i) const noexcept
	{
		rational r = *this;
		r -= i;
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Substraction operator.
	 * @param i A @ref lal::numeric::integer.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] friend rational
	operator- (const integer& i, const rational& r) noexcept
	{
		return -r + i;
	}
#endif
	/**
	 * @brief Substraction operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] rational operator- (const rational& r) const noexcept
	{
		rational k;
		mpq_sub(k.m_val, m_val, r.m_val);
		return k;
	}

	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	rational& operator-= (const T i) noexcept
	{
		rational r(i);
		mpq_sub(m_val, m_val, r.m_val);
		return *this;
	}
	/**
	 * @brief Substraction operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& operator-= (const integer& i) noexcept
	{
		rational r(i);
		mpq_sub(m_val, m_val, r.m_val);
		return *this;
	}
	/**
	 * @brief Substraction operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational& operator-= (const rational& r) noexcept
	{
		mpq_sub(m_val, m_val, r.m_val);
		return *this;
	}

	// -- MULTIPLICATION

	/**
	 * @brief Multiplication operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] rational operator* (const T i) const noexcept
	{
		rational r;
		if constexpr (std::is_signed_v<T>) {
			mpz_mul_si(&r.m_val->_mp_num, &m_val->_mp_num, i);
		}
		else {
			mpz_mul_ui(&r.m_val->_mp_num, &m_val->_mp_num, i);
		}
		mpz_set(&r.m_val->_mp_den, &m_val->_mp_den);
		mpq_canonicalize(r.m_val);
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Multiplication operator.
	 * @param i An integer (basic type) number.
	 * @param r A @ref lal::numeric::rational.
	 */
	template <std::integral T>
	[[nodiscard]] friend rational
	operator* (const T i, const rational& r) noexcept
	{
		return r * i;
	}
#endif
	/**
	 * @brief Multiplication operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] rational operator* (const integer& i) const noexcept
	{
		rational r;
		mpz_mul(&r.m_val->_mp_num, &m_val->_mp_num, i.m_val);
		mpz_set(&r.m_val->_mp_den, &m_val->_mp_den);
		mpq_canonicalize(r.m_val);
		return r;
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Multiplication operator.
	 * @param i A @ref lal::numeric::integer.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] friend rational
	operator* (const integer& i, const rational& r) noexcept
	{
		return r * i;
	}
#endif
	/**
	 * @brief Multiplication operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] rational operator* (const rational& r) const noexcept
	{
		rational k;
		mpq_mul(k.m_val, m_val, r.m_val);
		return k;
	}

	/**
	 * @brief Multiplication operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	rational& operator*= (const T i) noexcept
	{
		if constexpr (std::is_signed_v<T>) {
			mpz_mul_si(&m_val->_mp_num, &m_val->_mp_num, i);
		}
		else {
			mpz_mul_ui(&m_val->_mp_num, &m_val->_mp_num, i);
		}
		mpq_canonicalize(m_val);
		return *this;
	}
	/**
	 * @brief Multiplication operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& operator*= (const integer& i) noexcept
	{
		mpz_mul(&m_val->_mp_num, &m_val->_mp_num, i.m_val);
		mpq_canonicalize(m_val);
		return *this;
	}
	/**
	 * @brief Multiplication operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational& operator*= (const rational& r) noexcept
	{
		mpq_mul(m_val, m_val, r.m_val);
		return *this;
	}

	// -- DIVISION

	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] rational operator/ (const T i) const noexcept
	{
		if constexpr (std::is_signed_v<T>) {
			if (i < 0) {
				rational r(-1, static_cast<uint64_t>(-i));
				r *= (*this);
				return r;
			}
			else {
				rational r(1, static_cast<uint64_t>(i));
				r *= (*this);
				return r;
			}
		}
		else {
			rational r(1, i);
			r *= (*this);
			return r;
		}
	}
#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 * @param r Rational value.
	 */
	template <std::integral T>
	[[nodiscard]] friend rational
	operator/ (const T i, const rational& r) noexcept
	{
		rational inv(r);
		inv.invert();
		return inv * i;
	}
#endif

	/**
	 * @brief Division operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] rational operator/ (const integer& i) const noexcept
	{
		rational r;
		mpz_set_ui(&r.m_val->_mp_num, 1);
		mpz_mul(&r.m_val->_mp_den, &r.m_val->_mp_den, i.m_val);
		mpq_canonicalize(r.m_val);
		return r;
	}
	/**
	 * @brief Division operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] friend rational
	operator/ (const integer& i, const rational& r) noexcept
	{
		rational R(i, 1);
		return R / r;
	}
	/**
	 * @brief Division operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	[[nodiscard]] rational operator/ (const rational& r) const noexcept
	{
		rational k;
		mpq_div(k.m_val, m_val, r.m_val);
		return k;
	}

	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	rational& operator/= (const T i) noexcept
	{
		rational I(i);
		*this /= I;
		return *this;
	}
	/**
	 * @brief Division operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& operator/= (const integer& i) noexcept
	{
		mpz_mul(&m_val->_mp_den, &m_val->_mp_den, i.m_val);
		mpq_canonicalize(m_val);
		return *this;
	}
	/**
	 * @brief Division operator.
	 * @param r A @ref lal::numeric::rational.
	 */
	rational& operator/= (const rational& r) noexcept
	{
		mpq_div(m_val, m_val, r.m_val);
		return *this;
	}

	// -- EXPONENTIATION

	/**
	 * @brief Exponentiation operator.
	 * @param i An unsigned integer (basic type).
	 */
	[[nodiscard]] rational power(const uint64_t i) const noexcept;
	/**
	 * @brief Exponentiation operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] rational power(const integer& i) const noexcept;

	/**
	 * @brief Exponentiation operator.
	 * @param i An unsigned integer (basic type).
	 */
	rational& powt(const uint64_t i) noexcept;
	/**
	 * @brief Exponentiation operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	rational& powt(const integer& i) noexcept;

	/* GETTERS */

	/// Returns whether this object is initialized or not.
	[[nodiscard]] bool is_initialized() const noexcept
	{
		return m_initialized;
	}
	/// Returns the sign of this rational.
	[[nodiscard]] int64_t get_sign() const noexcept
	{
		return mpq_sgn(m_val);
	}

	/// Returns the amount of bytes this integer occupies.
	[[nodiscard]] std::size_t bytes() const noexcept;

	/* CONVERTERS */

	/**
	 * @brief Converts this rational to an integer value.
	 *
	 * This function returns \f$\left\lfloor n/d \right\rfloor\f$ where
	 * \f$n,d\f$ are, respectively, the numerator and denominator.
	 * @returns The floor of this rational.
	 */
	[[nodiscard]] integer to_integer() const noexcept
	{
		integer i;
		as_integer(i);
		return i;
	}
	/**
	 * @brief Converts this rational to an integer value.
	 *
	 * This function returns \f$\left\lfloor n/d \right\rfloor\f$ where
	 * \f$n,d\f$ are, respectively, the numerator and denominator.
	 * @param[out] i Integer representation of this rational.
	 */
	void as_integer(integer& i) const noexcept;

	/// Converts this rational to a double-precision floating-point value.
	[[nodiscard]] double to_double() const noexcept
	{
		return mpq_get_d(m_val);
	}
	/// Converts this rational to a double-precision floating-point value.
	void as_double(double& d) const noexcept
	{
		d = mpq_get_d(m_val);
	}

	/// Converts this integer to a string.
	[[nodiscard]] std::string to_string() const noexcept
	{
		std::string k;
		as_string(k);
		return k;
	}
	/// Converts this integer to a string.
	void as_string(std::string& s) const noexcept
	{
		char *buf = nullptr;
		buf = mpq_get_str(buf, 10, m_val);
		s = std::string(buf);
		free(buf);
	}

	/// Returns the numerator of this rational number
	[[nodiscard]] integer get_numerator() const noexcept
	{
		mpz_t numerator;
		mpz_init(numerator);
		mpq_get_num(numerator, m_val);
		return integer(std::move(numerator));
	}

	/// Returns the denominator of this rational number
	[[nodiscard]] integer get_denominator() const noexcept
	{
		mpz_t denominator;
		mpz_init(denominator);
		mpq_get_den(denominator, m_val);
		return integer(std::move(denominator));
	}

	/* OTHERS */

	/**
	 * @brief Swaps the value of this rational with rational @e r's value.
	 * @param[inout] r Rational value.
	 */
	void swap(rational& r) noexcept
	{
		mpq_swap(m_val, r.m_val);
		std::swap(m_initialized, r.m_initialized);
	}

#if !defined LAL_SWIG_PYTHON
	/**
	 * @brief Swaps two rationals.
	 * @param r1 Input rational.
	 * @param r2 Input rational.
	 */
	friend void swap(rational& r1, rational& r2) noexcept
	{
		r1.swap(r2);
	}
#endif

private:

	/**
	 * @brief Moves an @ref lal::numeric::integer into this object.
	 * @param i Input integer.
	 */
	void move_into(integer&& i) noexcept;

	/**
	 * @brief Moves two @ref lal::numeric::integer into this object.
	 *
	 * The two integers represent a fraction.
	 * @param n Input integer.
	 * @param d Input integer.
	 */
	void move_into(integer&& n, integer&& d) noexcept;

	/**
	 * @brief Moves a @ref lal::numeric::rational into this object.
	 * @param r Input rational.
	 */
	void move_into(rational&& r) noexcept;

private:

	/// Structure from GMP storing the rational's value.
	mpq_t m_val;
	/// Is this rational initialized?
	bool m_initialized = false;
};

} // namespace numeric
} // namespace lal
