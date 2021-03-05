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
	integer() noexcept { mpz_init(m_val); }
	/**
	 * @brief Constructor with unsigned integer value.
	 * @param i Signed integer (basic type) number.
	 */
	integer(int64_t i) noexcept { mpz_init_set_si(m_val, i); }
	/**
	 * @brief Constructor with string.
	 * @param s A string.
	 */
	integer(const std::string& s) noexcept { mpz_init_set_str(m_val, s.c_str(), 10); }
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param i A lal::numeric::integer
	 */
	integer(integer&& i) noexcept;
#endif
	/**
	 * @brief Copy constructor.
	 * @param i A lal::numeric::integer.
	 */
	integer(const integer& i) noexcept { mpz_init_set(m_val, i.m_val); }
	/// Destructor.
	~integer() noexcept { mpz_clear(m_val); }

	/* SETTERS */

	/**
	 * @brief Overwrites the value of this integer with @e i.
	 * @param i Signed integer (basic type) number.
	 */
	void set_si(int64_t i) noexcept {
		if (not is_initialized()) { mpz_init(m_val); }
		mpz_set_si(m_val, i);
		m_initialized = true;
	}
	/**
	 * @brief Overwrites the value of this integer with @e i.
	 * @param i Unsigned integer (basic type) number.
	 */
	void set_ui(uint64_t i) noexcept {
		if (not is_initialized()) { mpz_init(m_val); }
		mpz_set_ui(m_val, i);
		m_initialized = true;
	}
	/**
	 * @brief Overwrites the value of this integer with @e i.
	 * @param i A lal::numeric::integer.
	 */
	void set_integer(const integer& i) noexcept {
		if (not is_initialized()) { mpz_init(m_val); }
		mpz_set(m_val, i.m_val);
		m_initialized = true;
	}
	/**
	 * @brief Overwrites the value of this integer with @e s.
	 * @param s A string.
	 */
	void set_str(const std::string& s) noexcept {
		if (not is_initialized()) { mpz_init(m_val); }
		mpz_set_str(m_val, s.c_str(), 10);
		m_initialized = true;
	}

	/* OPERATORS */

	// -- ASSIGNMENT

#ifndef SWIG
	/**
	 * @brief Assignment operator.
	 * @param i Signed integer (basic type) number.
	 */
	integer& operator= (int64_t i) noexcept {
		set_si(i);
		return *this;
	}
	/**
	 * @brief Copy assignment operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator= (const integer& i) noexcept {
		set_integer(i);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator= (integer&& i) noexcept;
#endif

	// -- EQUALITY

	/**
	 * @brief Equality operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator== (int64_t i) const noexcept
	{ return mpz_cmp_si(m_val, i) == 0; }
#ifndef SWIG
	/**
	 * @brief Equality operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline bool friend operator== (int64_t i, const integer& ii) noexcept
	{ return ii == i; }
#endif
	/**
	 * @brief Equality operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator== (const integer& i) const noexcept
	{ return mpz_cmp(m_val, i.m_val) == 0; }

	// -- NON-EQUALITY

	/**
	 * @brief Non-equality operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator!= (int64_t i) const noexcept
	{ return not (*this == i); }
#ifndef SWIG
	/**
	 * @brief Non-equality operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline bool friend operator!= (int64_t i, const integer& ii) noexcept
	{ return ii != i; }
#endif
	/**
	 * @brief Non-equality operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator!= (const integer& i) const noexcept
	{ return not (*this == i); }

	// -- LESS THAN

	/**
	 * @brief Less than operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator< (int64_t i) const noexcept
	{ return mpz_cmp_si(m_val, i)  < 0; }
#ifndef SWIG
	/**
	 * @brief Less operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend bool operator< (int64_t i, const integer& ii) noexcept
	{ return ii > i; }
#endif
	/**
	 * @brief Less than operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator< (const integer& i) const noexcept
	{ return mpz_cmp(m_val, i.m_val) < 0; }

	// -- LESS THAN OR EQUAL TO

	/**
	 * @brief Less than or equal to operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator<= (int64_t i) const noexcept
	{ return mpz_cmp_si(m_val, i)  <= 0; }
#ifndef SWIG
	/**
	 * @brief Less than or equal to operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend bool operator<= (int64_t i, const integer& ii) noexcept
	{ return ii >= i; }
#endif
	/**
	 * @brief Less than or equal to operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator<= (const integer& i) const noexcept
	{ return mpz_cmp(m_val, i.m_val) <= 0; }

	// -- GREATER THAN

	/**
	 * @brief Greater than operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator> (int64_t i) const noexcept
	{ return mpz_cmp_si(m_val, i)  > 0; }
#ifndef SWIG
	/**
	 * @brief Greater than operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend bool operator> (int64_t i, const integer& ii) noexcept
	{ return ii < i; }
#endif
	/**
	 * @brief Greater than operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator> (const integer& i) const noexcept
	{ return mpz_cmp(m_val, i.m_val) > 0; }

	// -- GREATER THAN OR EQUAL TO

	/**
	 * @brief Greater than or equal to operator.
	 * @param i Signed integer (basic type) number.
	 */
	bool operator>= (int64_t i) const noexcept
	{ return mpz_cmp_si(m_val, i) >= 0; }
#ifndef SWIG
	/**
	 * @brief Greater than or equal to operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend bool operator>= (int64_t i, const integer& ii) noexcept
	{ return ii <= i; }
#endif
	/**
	 * @brief Greater than or equal to operator.
	 * @param i A lal::numeric::integer.
	 */
	bool operator>= (const integer& i) const noexcept
	{ return mpz_cmp(m_val, i.m_val) >= 0; }

	/* ARITHMETIC OPERATORS */

	// -- ADDITION

	/**
	 * @brief Addition operator.
	 * @param i Signed integer (basic type) number.
	 */
	integer operator+ (int64_t i) const noexcept
	{ integer a(*this); a += i; return a; }
#ifndef SWIG
	/**
	 * @brief Addition operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend integer operator+ (int64_t i, const integer& ii) noexcept
	{ return ii + i; }
#endif
	/**
	 * @brief Addition operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator+ (const integer& i) const noexcept
	{ integer a(*this); a += i;	return a; }
	/**
	 * @brief Addition operator.
	 * @param i Signed integer (basic type) number.
	 */
	integer& operator+= (int64_t i) noexcept;
	/**
	 * @brief Addition operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator+= (const integer& i) noexcept
	{ mpz_add(m_val, m_val, i.m_val); return *this; }

	// -- SUBSTRACTION

	/// Minus unary operator. Returns a new object of type 'integer'.
	integer operator- () const noexcept
	{ integer a(*this);	mpz_neg(a.m_val, a.m_val); return a; }
	/**
	 * @brief Substraction operator.
	 * @param i Signed integer (basic type) number.
	 */
	integer operator- (int64_t i) const noexcept
	{ integer a(*this); a -= i; return a; }
#ifndef SWIG
	/**
	 * @brief Substraction operator.
	 * @param i Signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend integer operator- (int64_t i, const integer& ii) noexcept
	{ return -ii + i; }
#endif
	/**
	 * @brief Substraction operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator- (const integer& i) const noexcept
	{ integer a(*this); a -= i;	return a; }
	/// Minus unary operator. Modifies this object.
	integer& operator- () noexcept
	{ mpz_neg(m_val, m_val); return *this; }
	/**
	 * @brief Substraction operator.
	 * @param i Signed integer (basic type) number.
	 */
	integer& operator-= (int64_t i) noexcept;
	/**
	 * @brief Substraction operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator-= (const integer& i) noexcept
	{ mpz_sub(m_val, m_val, i.m_val); return *this; }

	// -- MULTIPLICATION

	/**
	 * @brief Product operator.
	 * @param i A signed integer (basic type) number.
	 */
	integer operator* (int64_t i) const noexcept
	{ integer a(*this); a *= i;	return a; }
#ifndef SWIG
	/**
	 * @brief Product operator.
	 * @param i A signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend integer operator* (int64_t i, const integer& ii) noexcept
	{ return ii*i; }
#endif
	/**
	 * @brief Product operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator* (const integer& i) const noexcept
	{ integer a(*this); a *= i;	return a; }
	/**
	 * @brief Product operator.
	 * @param i A signed integer (basic type) number.
	 */
	integer& operator*= (int64_t i) noexcept
	{ mpz_mul_si(m_val, m_val, i);		return *this; }
	/**
	 * @brief Product operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator*= (const integer& i) noexcept
	{ mpz_mul(m_val, m_val, i.m_val);	return *this; }

	// -- DIVISION

	/**
	 * @brief Division operator.
	 * @param i A signed integer (basic type) number.
	 */
	integer operator/ (int64_t i) const noexcept
	{ integer a(*this); a /= i;	return a; }
#ifndef SWIG
	/**
	 * @brief Division operator.
	 * @param i A signed integer (basic type) number.
	 * @param ii A lal::numeric::integer.
	 */
	inline friend int64_t operator/ (int64_t i, const integer& ii) noexcept
	{ return i/ii.to_int(); }
#endif
	/**
	 * @brief Product operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator/ (const integer& i) const noexcept
	{ integer a(*this); a /= i;	return a; }
	/**
	 * @brief Division operator.
	 * @param i A signed integer (basic type) number.
	 */
	integer& operator/= (int64_t i) noexcept;
	/**
	 * @brief Division operator.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator/= (const integer& i) noexcept
	{ mpz_div(m_val, m_val, i.m_val);	return *this; }

	// -- EXPONENTIATION

	/**
	 * @brief Power operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator^ (uint64_t i) const noexcept
	{ integer r(*this); r ^= i;	return r; }
	/**
	 * @brief Power operator.
	 * @param i A lal::numeric::integer.
	 */
	integer operator^ (const integer& i) const noexcept
	{ integer r(*this); r ^= i;	return r; }

	/**
	 * @brief Power operator.
	 *
	 * Modifies the current object.
	 * @param i Unsigned integer (basic type) number.
	 */
	integer& operator^= (uint64_t i) noexcept
	{ mpz_pow_ui(m_val, m_val, i); return *this; }
	/**
	 * @brief Power operator.
	 *
	 * Modifies the current object.
	 * @param i A lal::numeric::integer.
	 */
	integer& operator^= (const integer& i) noexcept;

	// -- MODULUS

	/**
	 * @brief Modulus operator.
	 * @param i Unsigned integer (basic type) number.
	 */
	uint64_t operator% (uint64_t i) const noexcept;
	/**
	 * @brief Modulus operator.
	 * @param i A lal::numeric::integer
	 */
	integer operator% (const integer& i) const noexcept;

	/* GETTERS */

	/// Returns whether this object is initialised or not.
	constexpr bool is_initialized() const noexcept { return m_initialized; }
	/// Returns the sign of this integer.
	int32_t get_sign() const noexcept;

	/// Returns the amount of bytes this integer occupies.
	size_t bytes() const noexcept;
	/// Returns the underlying gmp data structure.
	const mpz_t& get_raw_value() const noexcept;

	/* CONVERTERS */

	/// Converts this integer to a signed 64-bit integer.
	int64_t to_int() const noexcept;
	/// Converts this integer to an unsigned 64-bit integer.
	uint64_t to_uint() const noexcept;
	/// Converts this integer to a double-precision floating-point value.
	double to_double() const noexcept;

	/// Converts this integer to a string.
	std::string to_string() const noexcept;
	/**
	 * @brief Converts this integer to a string.
	 * @param s A reference to a string.
	 */
	void as_string(std::string& s) const noexcept;

	/* OTHERS */

	/**
	 * @brief Swaps the value of this integer with integer @e i's value.
	 *
	 * - If none of the integers is initialised, it does nothing.
	 * - If only one of the integers is initialised, moves the contents
	 * of the initialised integer to the other. At the end, one of the two
	 * integers is left uninitiliased.
	 * - If both integers are initialised, swaps the values they contain.
	 *
	 * @param i A lal::numeric::integer
	 */
	void swap(integer& i) noexcept;

#ifndef SWIG
	/**
	 * @brief Swaps two integers.
	 * @param i Input lal::numeric::integer.
	 * @param j Input lal::numeric::integer.
	 */
	friend void swap(integer& i, integer& j) noexcept {
		i.swap(j);
	}
#endif

private:
	/// Structure from GMP storing the integer's value.
	mpz_t m_val;
	/// Is this integer initialised?
	bool m_initialized = true;
};

/**
 * @brief Make an integer from a 64-bit unsigned integer value.
 * @param n Unsigned integer (basic type) number.
 */
inline integer integer_from_ui(uint64_t n) noexcept {
	integer i;
	i.set_ui(n);
	return i;
}

} // -- namespace numeric
} // -- namespace lal
