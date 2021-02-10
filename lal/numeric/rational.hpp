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
	/// Empty constructor.
	rational() noexcept;
	/// Constructor with numerator and denominator.
	rational(int64_t n, uint64_t d = 1) noexcept;
	/// Constructor with integer.
	rational(const integer& n, const integer& d = 1) noexcept;
	/// Constructor with string.
	rational(const std::string& s) noexcept;
	/// Copy constructor.
	rational(const rational& r) noexcept;
#ifndef SWIG
	/// Move constructor with one integer.
	rational(integer&& n) noexcept;
	/// Move constructor with two integers.
	rational(integer&& n, integer&& d) noexcept;
	/// Move constructor.
	rational(rational&& r) noexcept;
#endif
	/// Destructor.
	~rational() noexcept;

	/* SET VALUE */

	/// Overwrites the value of this rational with \f$n/d\f$.
	void set_si(int64_t n, uint64_t d = 1) noexcept;
	/// Overwrites the value of this rational with \f$n/d\f$.
	void set_ui(uint64_t n, uint64_t d = 1) noexcept;
	/// Overwrites the value in the string @e s.
	void set_str(const std::string& s) noexcept;
	/// Overwrites the value of this rational with the value \f$n/d\f$.
	void set_integer(const integer& n, const integer& d = 1) noexcept;

	/**
	 * @brief Changes numerator and denominator.
	 *
	 * If this rational value was \f$n/d\f$ then after calling this
	 * method it will be \f$d/n\f$.
	 */
	void invert() noexcept;

	/* OPERATORS */

#ifndef SWIG
	/// Assignment operator.
	rational& operator= (int64_t i) noexcept;
	/// Assignment operator.
	rational& operator= (const std::string& i) noexcept;
	/// Assignment operator.
	rational& operator= (const integer& i) noexcept;
	/// Assignment operator.
	rational& operator= (const rational& i) noexcept;
	/// Move assignment operator.
	rational& operator= (integer&& i) noexcept;
	/// Move assignment operator.
	rational& operator= (rational&& r) noexcept;
#endif

	/// Equality operator.
	bool operator== (int64_t i) const noexcept;
#ifndef SWIG
	/// Equality operator.
	inline friend bool operator== (int64_t i, const rational& r) noexcept
	{ return r == i; }
#endif
	/// Equality operator.
	bool operator== (const std::string& s) const noexcept;
#ifndef SWIG
	/// Equality operator.
	inline friend bool operator== (const std::string& s, const rational& r) noexcept
	{ return r == s; }
#endif
	/// Equality operator.
	bool operator== (const integer& i) const noexcept;
#ifndef SWIG
	/// Equality operator.
	inline friend bool operator== (const integer& i, const rational& r) noexcept
	{ return r == i; }
#endif
	/// Equality operator.
	bool operator== (const rational& r) const noexcept;

	/// Non-equatility operator.
	bool operator!= (int64_t i) const noexcept;
#ifndef SWIG
	/// Non-equatility operator.
	inline friend bool operator!= (int64_t i, const rational& r) noexcept
	{ return r != i; }
#endif
	/// Non-equatility operator.
	bool operator!= (const std::string& s) const noexcept;
#ifndef SWIG
	/// Non-equatility operator.
	inline friend bool operator!= (const std::string& s, const rational& r) noexcept
	{ return r != s; }
#endif
	/// Non-equatility operator.
	bool operator!= (const integer& i) const noexcept;
#ifndef SWIG
	/// Non-equatility operator.
	inline friend bool operator!= (const integer& i, const rational& r) noexcept
	{ return r != i; }
#endif
	/// Non-equatility operator.
	bool operator!= (const rational& r) const noexcept;

	/// Less than operator.
	bool operator< (int64_t i) const noexcept;
#ifndef SWIG
	/// Less than operator.
	inline friend bool operator< (int64_t i, const rational& r) noexcept
	{ return r > i; }
#endif
	/// Less than operator.
	bool operator< (const integer& i) const noexcept;
#ifndef SWIG
	/// Less than operator.
	inline friend bool operator< (const integer& i, const rational& r) noexcept
	{ return r > i; }
#endif
	/// Less than operator.
	bool operator< (const rational& r) const noexcept;

	/// Less than or equal to operator.
	bool operator<= (int64_t i) const noexcept;
#ifndef SWIG
	/// Less than or equal to operator.
	inline friend bool operator<= (int64_t i, const rational& r) noexcept
	{ return r >= i; }
#endif
	/// Less than or equal to operator.
	bool operator<= (const integer& i) const noexcept;
#ifndef SWIG
	/// Less than or equal to operator.
	inline friend bool operator<= (const integer& i, const rational& r) noexcept
	{ return r >= i; }
#endif
	/// Less than or equal to operator.
	bool operator<= (const rational& r) const noexcept;

	/// Greater than operator.
	bool operator> (int64_t i) const noexcept;
#ifndef SWIG
	/// Greater than operator.
	inline friend bool operator> (int64_t i, const rational& r) noexcept
	{ return r < i; }
#endif
	/// Greater than operator.
	bool operator> (const integer& i) const noexcept;
#ifndef SWIG
	/// Greater than operator.
	inline friend bool operator> (const integer& i, const rational& r) noexcept
	{ return r < i; }
#endif
	/// Greater than operator.
	bool operator> (const rational& r) const noexcept;

	/// Greater than or equal to operator.
	bool operator>= (int64_t i) const noexcept;
#ifndef SWIG
	/// Greater than or equal to operator.
	inline friend bool operator>= (int64_t i, const rational& r) noexcept
	{ return r <= i; }
#endif
	/// Greater than or equal to operator.
	bool operator>= (const integer& i) const noexcept;
#ifndef SWIG
	/// Greater than or equal to operator.
	inline friend bool operator>= (const integer& i, const rational& r) noexcept
	{ return r <= i; }
#endif
	/// Greater than or equal to operator.
	bool operator>= (const rational& r) const noexcept;

	/// Addition operator. Returns a new object of type 'rational'.
	rational operator+ (int64_t i) const noexcept;
#ifndef SWIG
	/// Addition operator. Returns a new object of type 'rational'.
	inline friend rational operator+ (int64_t i, const rational& r) noexcept
	{ return r + i; }
#endif
	/// Addition operator. Returns a new object of type 'rational'.
	rational operator+ (const integer& i) const noexcept;
#ifndef SWIG
	/// Addition operator. Returns a new object of type 'rational'.
	inline friend rational operator+ (const integer& i, const rational& r) noexcept
	{ return r + i; }
#endif
	/// Addition operator. Returns a new object of type 'rational'.
	rational operator+ (const rational& r) const noexcept;

	/// Addition operator. Modifies the current instance.
	rational& operator+= (int64_t i) noexcept;
	/// Addition operator. Modifies the current instance.
	rational& operator+= (const integer& i) noexcept;
	/// Addition operator. Modifies the current instance.
	rational& operator+= (const rational& r) noexcept;

	/// Substraction unary operator. Returns a new object of type 'rational'.
	rational operator- () const noexcept;
	/// Substraction operator. Returns a new object of type 'rational'.
	rational operator- (int64_t i) const noexcept;
#ifndef SWIG
	/// Substraction operator. Returns a new object of type 'rational'.
	inline friend rational operator- (int64_t i, const rational& r) noexcept
	{ return -r + i; }
#endif
	/// Substraction operator. Returns a new object of type 'rational'.
	rational operator- (const integer& i) const noexcept;
#ifndef SWIG
	/// Substraction operator. Returns a new object of type 'rational'.
	inline friend rational operator- (const integer& i, const rational& r) noexcept
	{ return -r + i; }
#endif
	/// Substraction operator. Returns a new object of type 'rational'.
	rational operator- (const rational& r) const noexcept;

	/// Substraction unary operator. Modifies the current instance.
	rational& operator- () noexcept;
	/// Substraction operator. Modifies the current instance.
	rational& operator-= (int64_t i) noexcept;
	/// Substraction operator. Modifies the current instance.
	rational& operator-= (const integer& i) noexcept;
	/// Substraction operator. Modifies the current instance.
	rational& operator-= (const rational& r) noexcept;

	/// Product operator. Returns a new object of type 'rational'.
	rational operator* (int64_t i) const noexcept;
#ifndef SWIG
	/// Product operator. Returns a new object of type 'rational'.
	inline friend rational operator* (int64_t i, const rational& r) noexcept
	{ return r*i; }
#endif
	/// Product operator. Returns a new object of type 'rational'.
	rational operator* (const integer& i) const noexcept;
#ifndef SWIG
	/// Product operator. Returns a new object of type 'rational'.
	inline friend rational operator* (const integer& i, const rational& r) noexcept
	{ return r*i; }
#endif
	/// Product operator. Returns a new object of type 'rational'.
	rational operator* (const rational& r) const noexcept;

	/// Product operator. Modifies the current instance.
	rational& operator*= (int64_t i) noexcept;
	/// Product operator. Modifies the current instance.
	rational& operator*= (const integer& i) noexcept;
	/// Product operator. Modifies the current instance.
	rational& operator*= (const rational& r) noexcept;

	/// Quotient operator. Returns a new object of type 'rational'.
	rational operator/ (int64_t i) const noexcept;
	/// Quotient operator. Returns a new object of type 'rational'.
	rational operator/ (const integer& i) const noexcept;
	/// Quotient operator. Returns a new object of type 'rational'.
	rational operator/ (const rational& r) const noexcept;

	/// Quotient operator. Modifies the current instance.
	rational& operator/= (int64_t i) noexcept;
	/// Quotient operator. Modifies the current instance.
	rational& operator/= (const integer& i) noexcept;
	/// Quotient operator. Modifies the current instance.
	rational& operator/= (const rational& r) noexcept;

	/// Power operator. Returns a new object of type 'rational'.
	rational operator^ (uint64_t i) const noexcept;
	/// Power operator. Returns a new object of type 'rational'.
	rational operator^ (const integer& r) const noexcept;

	/// Power operator. Modifies the current instance.
	rational& operator^= (uint64_t i) noexcept;
	/// Power operator. Modifies the current instance.
	rational& operator^= (const integer& r) noexcept;

	/* GETTERS */

	/// Returns whether this object is initialised or not.
	constexpr bool is_initialized() const noexcept { return m_initialized; }
	/// Returns the sign of this rational.
	int32_t get_sign() const noexcept;

	/// Returns the amount of bytes this integer occupies.
	size_t bytes() const noexcept;

	/* CONVERTERS */

	/**
	 * @brief Converts this rational to an integer value.
	 * @return Returns \f$ \left\lfloor n/d \right\rfloor \f$ where
	 * \f$n,d\f$ are, respectively, the numerator and denominator.
	 */
	integer to_integer() const noexcept;
	/**
	 * @brief Converts this rational to an integer value.
	 * @return Returns \f$ \left\lfloor n/d \right\rfloor \f$ where
	 * \f$n,d\f$ are, respectively, the numerator and denominator.
	 */
	void as_integer(integer& i) const noexcept;

	/// Converts this rational to a double-precision floating-point value.
	double to_double() const noexcept;
	/// Converts this rational to a double-precision floating-point value.
	void as_double(double& d) const noexcept;

	/// Converts this integer to a string.
	std::string to_string() const noexcept;
	/// Converts this integer to a string.
	void as_string(std::string& s) const noexcept;

	/* OTHERS */

	/**
	 * @brief Swaps the value of this rational with rational @e r's value.
	 *
	 * - If none of the rationals is initialised, it does nothing.
	 * - If only one of the rationals is initialised, moves the contents
	 * of the initialised rational to the other. At the end, one of the two
	 * rationals is left uninitiliased.
	 * - If both rationals are initialised, swaps the values they contain.
	 */
	void swap(rational& r) noexcept;

#ifndef SWIG
	/**
	 * @brief Swaps two rationals.
	 * @param r1 Input rational.
	 * @param r2 Input rational.
	 */
	friend void swap(rational& r1, rational& r2) noexcept {
		r1.swap(r2);
	}
#endif

private:

private:
	/// Structure from GMP storing the rational's value.
	mpq_t m_val;
	/// Is this rational initialised?
	bool m_initialized = true;
};

/// Make a rational value from two 64-bit unsigned integers
inline rational rational_from_ui(uint64_t n, uint64_t d = 1) noexcept {
	rational R;
	R.set_ui(n, d);
	return R;
}

} // -- namespace numeric
} // -- namespace lal
