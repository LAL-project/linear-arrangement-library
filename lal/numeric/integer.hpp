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

// gmp includes
#include <gmp.h>

// C++ includes
#include <cstdint>
#include <string>

namespace lal {
namespace numeric {

/**
 * @brief Arbitrary precision integer.
 *
 * This class wraps the @e mpz_t data structure from GMP with operators for an
 * easy usage.
 */
class integer {
public:

	friend class rational;

public:

	/* CONSTRUCTORS */

	/// Empty constructor.
	integer() noexcept
	{
		m_initialized = true;
		mpz_init(m_val);
	}

	/**
	 * @brief Constructor with mpz_t.
	 * @post Object @e raw will have to be initialized by the callee.
	 */
	integer(mpz_t&& raw) noexcept;

	/**
	 * @brief Constructor with unsigned integer value.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	integer(const T i) noexcept
	{
		set_number(i);
	}
	/**
	 * @brief Constructor with string.
	 * @param s A string.
	 */
	integer(const std::string& s) noexcept
	{
		set_str(s);
	}

	/**
	 * @brief Move constructor.
	 * @param i A @ref lal::numeric::integer.
	 * @post Object @e i is not initialized.
	 */
	integer(integer&& i) noexcept
	{
		move_into(std::move(i));
	}

	/**
	 * @brief Copy constructor.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer(const integer& i) noexcept
	{
		m_initialized = true;
		mpz_init_set(m_val, i.m_val);
	}
	/// Destructor.
	~integer() noexcept
	{
		mpz_clear(m_val);
		m_initialized = false;
	}

	/* SETTERS */

	/**
	 * @brief Overwrites the value of this integer with @e i.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	void set_number(const T i) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpz_init(m_val);
		}
		if constexpr (std::is_signed_v<T>) {
			mpz_set_si(m_val, i);
		}
		else {
			mpz_set_ui(m_val, i);
		}
	}

	/**
	 * @brief Overwrites the value of this integer with @e i.
	 * @param i A @ref lal::numeric::integer.
	 */
	void set_integer(const integer& i) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpz_init(m_val);
		}
		mpz_set(m_val, i.m_val);
	}
	/**
	 * @brief Overwrites the value of this integer with @e s.
	 * @param s A string.
	 */
	void set_str(const std::string& s) noexcept
	{
		if (not is_initialized()) {
			m_initialized = true;
			mpz_init(m_val);
		}
		mpz_set_str(m_val, s.c_str(), 10);
	}

	/* OPERATORS */

	// -- ASSIGNMENT

	/**
	 * @brief Assignment operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	integer& operator= (const T i) noexcept
	{
		set_number(i);
		return *this;
	}
	/**
	 * @brief Copy assignment operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& operator= (const integer& i) noexcept
	{
		set_integer(i);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param i A @ref lal::numeric::integer.
	 * @post Object @e i is not initialized.
	 */
	integer& operator= (integer&& i) noexcept
	{
		mpz_clear(m_val);
		move_into(std::move(i));
		return *this;
	}

	// -- COMPARISON OPERATORS

	/**
	 * @brief Three-way comparison operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] std::strong_ordering operator<=> (const integer& i
	) const noexcept
	{
		return mpz_cmp(m_val, i.m_val) <=> 0;
	}
	/**
	 * @brief Equality operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] bool operator== (const integer& i) const noexcept
	{
		return mpz_cmp(m_val, i.m_val) == 0;
	}

	/**
	 * @brief Three-way comparison operator.
	 * @param i A basic integral type.
	 */
	template <std::integral T>
	[[nodiscard]] std::strong_ordering operator<=> (const T& i) const noexcept
	{
		return (std::is_signed_v<T> ? mpz_cmp_si(m_val, i)
									: mpz_cmp_ui(m_val, i)) <=> 0;
	}
	/**
	 * @brief Equality operator.
	 * @param i A basic integral type.
	 */
	template <std::integral T>
	[[nodiscard]] bool operator== (const T& i) const noexcept
	{
		return (std::is_signed_v<T> ? mpz_cmp_si(m_val, i)
									: mpz_cmp_ui(m_val, i)) == 0;
	}

	/* ARITHMETIC OPERATORS */

	// -- ADDITION

	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] integer operator+ (const T i) const noexcept
	{
		integer a;
		if constexpr (not std::is_signed_v<T>) {
			mpz_add_ui(a.m_val, m_val, i);
		}
		else {
			if (i > 0) {
				mpz_add_ui(a.m_val, m_val, static_cast<uint64_t>(i));
			}
			else {
				mpz_sub_ui(a.m_val, m_val, static_cast<uint64_t>(-i));
			}
		}
		return a;
	}
#if !defined LAL_PYTHON
	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 * @param ii A @ref lal::numeric::integer.
	 */
	template <std::integral T>
	[[nodiscard]] friend integer
	operator+ (const T i, const integer& ii) noexcept
	{
		return ii + i;
	}
#endif
	/**
	 * @brief Addition operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] integer operator+ (const integer& i) const noexcept
	{
		integer a;
		mpz_add(a.m_val, m_val, i.m_val);
		return a;
	}
	/**
	 * @brief Addition operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	integer& operator+= (const T i) noexcept
	{
		if constexpr (not std::is_signed_v<T>) {
			mpz_add_ui(m_val, m_val, i);
		}
		else {
			if (i > 0) {
				mpz_add_ui(m_val, m_val, static_cast<uint64_t>(i));
			}
			else {
				mpz_sub_ui(m_val, m_val, static_cast<uint64_t>(-i));
			}
		}
		return *this;
	}
	/**
	 * @brief Addition operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& operator+= (const integer& i) noexcept
	{
		mpz_add(m_val, m_val, i.m_val);
		return *this;
	}

	// -- SUBTRACTION

	/// Minus unary operator. Returns a new object of type 'integer'.
	[[nodiscard]] integer operator- () const noexcept
	{
		integer a;
		mpz_neg(a.m_val, m_val);
		return a;
	}
	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] integer operator- (const T i) const noexcept
	{
		integer a;
		if constexpr (not std::is_signed_v<T>) {
			mpz_sub_ui(a.m_val, m_val, i);
		}
		else {
			if (i > 0) {
				mpz_sub_ui(a.m_val, m_val, static_cast<uint64_t>(i));
			}
			else {
				mpz_add_ui(a.m_val, m_val, static_cast<uint64_t>(-i));
			}
		}
		return a;
	}
#if !defined LAL_PYTHON
	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 * @param ii A @ref lal::numeric::integer.
	 */
	template <std::integral T>
	[[nodiscard]] friend integer
	operator- (const T i, const integer& ii) noexcept
	{
		return -ii + i;
	}
#endif
	/**
	 * @brief Substraction operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] integer operator- (const integer& i) const noexcept
	{
		integer a;
		mpz_sub(a.m_val, m_val, i.m_val);
		return a;
	}
	/**
	 * @brief Substraction operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	integer& operator-= (const T i) noexcept
	{
		if constexpr (not std::is_signed_v<T>) {
			mpz_sub_ui(m_val, m_val, i);
		}
		else {
			if (i > 0) {
				mpz_sub_ui(m_val, m_val, static_cast<uint64_t>(i));
			}
			else {
				mpz_add_ui(m_val, m_val, static_cast<uint64_t>(-i));
			}
		}
		return *this;
	}
	/**
	 * @brief Substraction operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& operator-= (const integer& i) noexcept
	{
		mpz_sub(m_val, m_val, i.m_val);
		return *this;
	}

	// -- MULTIPLICATION

	/**
	 * @brief Product operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] integer operator* (const T i) const noexcept
	{
		integer a;
		if constexpr (std::is_signed_v<T>) {
			mpz_mul_si(a.m_val, m_val, i);
		}
		else {
			mpz_mul_ui(a.m_val, m_val, i);
		}
		return a;
	}
#if !defined LAL_PYTHON
	/**
	 * @brief Product operator.
	 * @param i An integer (basic type) number.
	 * @param ii A @ref lal::numeric::integer.
	 */
	template <std::integral T>
	[[nodiscard]] friend integer
	operator* (const T i, const integer& ii) noexcept
	{
		return ii * i;
	}
#endif

	/**
	 * @brief Product operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] integer operator* (const integer& i) const noexcept
	{
		integer a;
		mpz_mul(a.m_val, m_val, i.m_val);
		return a;
	}
	/**
	 * @brief Product operator.
	 * @param i A signed/unsigned integer (basic type) number.
	 */
	template <std::integral T>
	integer& operator*= (const T i) noexcept
	{
		if constexpr (std::is_signed_v<T>) {
			mpz_mul_si(m_val, m_val, i);
		}
		else {
			mpz_mul_ui(m_val, m_val, i);
		}
		return *this;
	}
	/**
	 * @brief Product operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& operator*= (const integer& i) noexcept
	{
		mpz_mul(m_val, m_val, i.m_val);
		return *this;
	}

	// -- DIVISION

	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	[[nodiscard]] integer operator/ (const T i) const noexcept
	{
		integer a;
		if constexpr (not std::is_signed_v<T>) {
			mpz_tdiv_q_ui(a.m_val, m_val, i);
		}
		else {
			if (i < 0) {
				mpz_tdiv_q_ui(a.m_val, m_val, static_cast<uint64_t>(-i));
				mpz_neg(a.m_val, a.m_val);
			}
			else {
				mpz_tdiv_q_ui(a.m_val, m_val, static_cast<uint64_t>(i));
			}
		}
		return a;
	}
#if !defined LAL_PYTHON
	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 * @param ii A @ref lal::numeric::integer.
	 */
	template <std::integral T>
	[[nodiscard]] friend int64_t
	operator/ (const T i, const integer& ii) noexcept
	{
		return i / ii.to_int();
	}
#endif
	/**
	 * @brief Product operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] integer operator/ (const integer& i) const noexcept
	{
		integer a;
		if (i < 0) {
			mpz_t pos_i;
			mpz_init(pos_i);
			mpz_set(pos_i, i.m_val);
			mpz_neg(pos_i, pos_i);

			mpz_tdiv_q(a.m_val, m_val, pos_i);
			mpz_neg(a.m_val, a.m_val);
		}
		else {
			mpz_tdiv_q(a.m_val, m_val, i.m_val);
		}
		return a;
	}
	/**
	 * @brief Division operator.
	 * @param i An integer (basic type) number.
	 */
	template <std::integral T>
	integer& operator/= (const T i) noexcept
	{
		if constexpr (not std::is_signed_v<T>) {
			mpz_tdiv_q_ui(m_val, m_val, i);
		}
		else {
			if (i < 0) {
				mpz_tdiv_q_ui(m_val, m_val, static_cast<uint64_t>(-i));
				mpz_neg(m_val, m_val);
			}
			else {
				mpz_tdiv_q_ui(m_val, m_val, static_cast<uint64_t>(i));
			}
		}
		return *this;
	}
	/**
	 * @brief Division operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& operator/= (const integer& i) noexcept
	{
		if (i < 0) {
			mpz_t pos_i;
			mpz_init(pos_i);
			mpz_set(pos_i, i.m_val);
			mpz_neg(pos_i, pos_i);

			mpz_fdiv_q(m_val, m_val, pos_i);
			mpz_neg(m_val, m_val);
		}
		else {
			mpz_fdiv_q(m_val, m_val, i.m_val);
		}
		return *this;
	}

	// -- EXPONENTIATION

	/**
	 * @brief Exponentiation operator.
	 * @param i An unsigned integer (basic type).
	 */
	[[nodiscard]] integer power(const uint64_t i) const noexcept
	{
		integer r(*this);
		r.powt(i);
		return r;
	}
	/**
	 * @brief Exponentiation operator.
	 * @param i A @ref lal::numeric::integer.
	 */
	[[nodiscard]] integer power(const integer& i) const noexcept
	{
		integer r(*this);
		r.powt(i);
		return r;
	}

	/**
	 * @brief Exponentiation operator.
	 *
	 * Modifies the current object.
	 * @param i Unsigned integer (basic type) number.
	 */
	integer& powt(const uint64_t i) noexcept
	{
		mpz_pow_ui(m_val, m_val, i);
		return *this;
	}
	/**
	 * @brief Exponentiation operator.
	 *
	 * Modifies the current object.
	 * @param i A @ref lal::numeric::integer.
	 */
	integer& powt(const integer& i) noexcept;

	// -- MODULUS

	/**
	 * @brief Modulus operator.
	 * @param i Unsigned integer (basic type) number.
	 */
	[[nodiscard]] uint64_t operator% (const uint64_t i) const noexcept
	{
		mpz_t r;
		mpz_init(r);
		const uint64_t m = mpz_mod_ui(r, m_val, i);
		mpz_clear(r);
		return m;
	}
	/**
	 * @brief Modulus operator.
	 * @param i A @ref lal::numeric::integer
	 */
	[[nodiscard]] integer operator% (const integer& i) const noexcept
	{
		integer r;
		mpz_mod(r.m_val, m_val, i.m_val);
		return r;
	}

	/* GETTERS */

	/// Returns whether this object is initialized or not.
	[[nodiscard]] constexpr bool is_initialized() const noexcept
	{
		return m_initialized;
	}
	/// Returns the sign of this integer.
	[[nodiscard]] int64_t get_sign() const noexcept
	{
		return mpz_sgn(m_val);
	}

	/// Returns the amount of bytes this integer occupies.
	[[nodiscard]] std::size_t bytes() const noexcept;
	/// Returns the underlying gmp data structure.
	[[nodiscard]] const mpz_t& get_raw_value() const noexcept
	{
		return m_val;
	}

	/* CONVERTERS */

	/// Converts this integer to a signed 64-bit integer.
	[[nodiscard]] int64_t to_int() const noexcept
	{
		return mpz_get_si(m_val);
	}
	/// Converts this integer to an unsigned 64-bit integer.
	[[nodiscard]] uint64_t to_uint() const noexcept
	{
		return mpz_get_ui(m_val);
	}
	/// Converts this integer to a double-precision floating-point value.
	[[nodiscard]] double to_double() const noexcept
	{
		return mpz_get_d(m_val);
	}

	/// Converts this integer to a string.
	[[nodiscard]] std::string to_string() const noexcept
	{
		std::string k;
		as_string(k);
		return k;
	}
	/**
	 * @brief Converts this integer to a string.
	 * @param s A reference to a string.
	 */
	void as_string(std::string& s) const noexcept
	{
		char *buf = nullptr;
		buf = mpz_get_str(buf, 10, m_val);
		s = std::string(buf);
		free(buf);
	}

	/**
	 * @brief Swaps the value of this integer with integer @e i's value.
	 * @param[inout] i A @ref lal::numeric::integer
	 */
	void swap(integer& i) noexcept
	{
		mpz_swap(m_val, i.m_val);
		std::swap(m_initialized, i.m_initialized);
	}

#if !defined LAL_PYTHON
	/**
	 * @brief Swaps two integers.
	 * @param i Input lal::numeric::integer.
	 * @param j Input lal::numeric::integer.
	 */
	friend void swap(integer& i, integer& j) noexcept
	{
		i.swap(j);
	}
#endif

private:

	/**
	 * @brief Moves an @ref lal::numeric::integer into this object.
	 * @param i Input integer.
	 */
	void move_into(integer&& i) noexcept;

private:

	/// Structure from GMP storing the integer's value.
	mpz_t m_val;
	/// Is this integer initialized?
	bool m_initialized = false;
};

} // namespace numeric
} // namespace lal
