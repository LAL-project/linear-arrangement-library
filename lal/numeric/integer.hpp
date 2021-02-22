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
	/// Empty constructor.
	integer() noexcept;
	/// Constructor with unsigned integer value.
	integer(int64_t i) noexcept;
	/// Constructor with string.
	integer(const std::string& i) noexcept;
#ifndef SWIG
	/// Move constructor.
	integer(integer&& i) noexcept;
#endif
	/// Copy constructor.
	integer(const integer& i) noexcept;
	/// Destructor.
	~integer() noexcept;

	/* SET VALUE */

	/// Overwrites the value of this integer with @e i.
	void set_si(int64_t i) noexcept;
	/// Overwrites the value of this integer with @e i.
	void set_ui(uint64_t i) noexcept;
	/// Overwrites the value of this integer with @e i.
	void set_integer(const integer& i) noexcept;
	/// Overwrites the value of this integer with the contents in @e s.
	void set_str(const std::string& s) noexcept;

	/* OPERATORS */

#ifndef SWIG
	/// Assignment operator.
	integer& operator= (int64_t i) noexcept;
	/// Assignment operator.
	integer& operator= (const integer& i) noexcept;
	/// Move assignment operator.
	integer& operator= (integer&& i) noexcept;
#endif

	/// Equality operator.
	bool operator== (int64_t i) const noexcept;
#ifndef SWIG
	/// Equality operator.
	inline bool friend operator== (int64_t i, const integer& ii) noexcept
	{ return ii == i; }
#endif
	/// Equality operator.
	bool operator== (const std::string& i) const noexcept;
#ifndef SWIG
	/// Equality operator.
	inline bool friend operator== (const std::string& i, const integer& ii) noexcept
	{ return ii == i; }
#endif
	/// Equality operator.
	bool operator== (const integer& i) const noexcept;

	/// Non-equality operator.
	bool operator!= (int64_t i) const noexcept;
#ifndef SWIG
	/// Non-equality operator.
	inline bool friend operator!= (int64_t i, const integer& ii) noexcept
	{ return ii != i; }
#endif
	/// Non-equality operator.
	bool operator!= (const std::string& i) const noexcept;
#ifndef SWIG
	/// Non-equality operator.
	inline bool friend operator!= (const std::string& i, const integer& ii) noexcept
	{ return ii != i; }
#endif
	/// Non-equality operator.
	bool operator!= (const integer& i) const noexcept;

	/// Less than operator.
	bool operator< (int64_t i) const noexcept;
#ifndef SWIG
	/// Less than operator.
	inline friend bool operator< (int64_t i, const integer& ii) noexcept
	{ return ii > i; }
#endif
	/// Less than operator.
	bool operator< (const integer& i) const noexcept;

	/// Less than or equal to operator.
	bool operator<= (int64_t i) const noexcept;
#ifndef SWIG
	/// Less than or equal to operator.
	inline friend bool operator<= (int64_t i, const integer& ii) noexcept
	{ return ii >= i; }
#endif
	/// Less than or equal to operator.
	bool operator<= (const integer& i) const noexcept;

	/// Greater than operator.
	bool operator> (int64_t i) const noexcept;
#ifndef SWIG
	/// Greater than operator.
	inline friend bool operator> (int64_t i, const integer& ii) noexcept
	{ return ii < i; }
#endif
	/// Greater than operator.
	bool operator> (const integer& i) const noexcept;

	/// Greater than or equal to operator.
	bool operator>= (int64_t i) const noexcept;
#ifndef SWIG
	/// Greater than or equal to operator.
	inline friend bool operator>= (int64_t i, const integer& ii) noexcept
	{ return ii <= i; }
#endif
	/// Greater than or equal to operator.
	bool operator>= (const integer& i) const noexcept;

	/// Addition operator. Returns a new object of type 'integer'.
	integer operator+ (int64_t i) const noexcept;
#ifndef SWIG
	/// Addition operator. Returns a new object of type 'integer'.
	inline friend integer operator+ (int64_t i, const integer& ii) noexcept
	{ return ii + i; }
#endif
	/// Addition operator. Returns a new object of type 'integer'.
	integer operator+ (const integer& i) const noexcept;

	/// Addition operator. Modifies the current instance.
	integer& operator+= (int64_t i) noexcept;
	/// Addition operator. Modifies the current instance.
	integer& operator+= (const integer& i) noexcept;

	/// Minus unary operator. Returns a new object of type 'integer'.
	integer operator- () const noexcept;
	/// Substraction operator. Returns a new object of type 'integer'.
	integer operator- (int64_t i) const noexcept;
#ifndef SWIG
	/// Substraction operator. Returns a new object of type 'integer'.
	inline friend integer operator- (int64_t i, const integer& ii) noexcept
	{ return -ii + i; }
#endif
	/// Substraction operator. Returns a new object of type 'integer'.
	integer operator- (const integer& i) const noexcept;

	/// Minus unary operator. Modifies the current instance.
	integer& operator- () noexcept;
	/// Substraction operator. Modifies the current instance.
	integer& operator-= (int64_t i) noexcept;
	/// Substraction operator. Modifies the current instance.
	integer& operator-= (const integer& i) noexcept;

	/// Product operator. Returns a new object of type 'integer'.
	integer operator* (int64_t i) const noexcept;
#ifndef SWIG
	/// Product operator. Returns a new object of type 'integer'.
	inline friend integer operator* (int64_t i, const integer& ii) noexcept
	{ return ii*i; }
#endif
	/// Product operator. Returns a new object of type 'integer'.
	integer operator* (const integer& i) const noexcept;

	/// Product operator. Modifies the current instance.
	integer& operator*= (int64_t i) noexcept;
	/// Product operator. Modifies the current instance.
	integer& operator*= (const integer& i) noexcept;

	/// Quotient operator. Returns a new object of type 'integer'.
	integer operator/ (int64_t i) const noexcept;
#ifndef SWIG
	/// Quotient operator. Returns a new object of type 'integer'.
	inline friend int64_t operator/ (int64_t i, const integer& ii) noexcept
	{ return i/ii.to_int(); }
#endif
	/// Quotient operator. Returns a new object of type 'integer'.
	integer operator/ (const integer& i) const noexcept;

	/// Quotient operator. Modifies the current instance.
	integer& operator/= (int64_t i) noexcept;
	/// Quotient operator. Modifies the current instance.
	integer& operator/= (const integer& i) noexcept;

	/// Power operator. Returns a new object of type 'integer'.
	integer operator^ (uint64_t i) const noexcept;
	/// Power operator. Returns a new object of type 'integer'.
	integer operator^ (const integer& i) const noexcept;

	/// Power operator. Modifies the current instance.
	integer& operator^= (uint64_t i) noexcept;
	/// Power operator. Modifies the current instance.
	integer& operator^= (const integer& i) noexcept;

	/// Modulus operator.
	uint64_t operator% (uint64_t i) const noexcept;
	/// Modulus operator.
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
	/// Converts this integer to a string.
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
	 */
	void swap(integer& i) noexcept;

#ifndef SWIG
	/**
	 * @brief Swaps two integers.
	 * @param i Input integer.
	 * @param j Input integer.
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

/// Make an integer from a 64-bit unsigned integer value.
inline integer integer_from_ui(uint64_t n) noexcept {
	integer i;
	i.set_ui(n);
	return i;
}

} // -- namespace numeric
} // -- namespace lal
