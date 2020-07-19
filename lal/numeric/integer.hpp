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
 
#pragma once

// C++ includes
#include <cstdint>
#include <string>

// gmp includes
#include <gmp.h>

namespace lal {
namespace numeric {

/**
 * @brief Arbitrary precision integer.
 *
 * This class wraps the @e mpz_t data structure from GMP
 * with operators for an easy usage.
 *
 * In order to be used, it must first be initialised, with
 * one of the following methods:
 * - @ref init
 * - @ref init_si
 * - @ref init_ui
 * - @ref init_str
 *
 * or via the class constructors. The empty constructor, however,
 * does not initialise the internal memory.
 *
 * Although the internal memory is deallocated automatically
 * the user can do so manually by calling method @ref clear.
 */
class integer {
	public:
		/// Default constructor.
		integer() = default;
		/// Constructor with unsigned integer value.
		integer(int64_t i);
		/// Constructor with string.
		integer(const std::string& i);
		/// Move constructor.
		integer(integer&& i);
		/// Copy constructor.
		integer(const integer& i);
		/// Destructor.
		~integer();

		/* ALLOC AND DEALLOC */

		/// Initialises internal memory.
		void init();
		/// Initialises this integer with the value @e i.
		void init_si(int64_t i);
		/// Initialises this integer with the value @e i.
		void init_ui(uint64_t i);
		/// Initialises this integer with the value in string @e s.
		void init_str(const std::string& s);
		/// Initialises this integer with the value in @e mpz.
		void init_mpz(const mpz_t& mpz);

		/// Clear the memory of this integer.
		void clear();

		/* SET VALUE */

		/// Overwrites the value of this integer with @e i.
		void set_si(int64_t i);
		/// Overwrites the value of this integer with @e i.
		void set_ui(uint64_t i);
		/// Overwrites the value of this integer with the contents in @e s.
		void set_str(const std::string& s);
		/// Overwrites the value of this integer with the value in @e mpz.
		void set_mpz(const mpz_t& mpz);

		/* OPERATORS */

		/// Assignment operator.
		integer& operator= (int64_t i);
		/// Assignment operator.
		integer& operator= (const integer& i);
		/// Move assignment operator.
		integer& operator= (integer&& i);

		/// Equality operator.
		bool operator== (int64_t i) const;
		/// Equality operator.
		bool operator== (const integer& i) const;

		/// Difference operator.
		bool operator!= (int64_t i) const;
		/// Difference operator.
		bool operator!= (const integer& i) const;

		/// Less than operator.
		bool operator< (int64_t i) const;
		/// Less than operator.
		bool operator< (const integer& i) const;

		/// Less than or equal to operator.
		bool operator<= (int64_t i) const;
		/// Less than or equal to operator.
		bool operator<= (const integer& i) const;

		/// Greater than operator.
		bool operator> (int64_t i) const;
		/// Greater than operator.
		bool operator> (const integer& i) const;

		/// Greater than or equal to operator.
		bool operator>= (int64_t i) const;
		/// Greater than or equal to operator.
		bool operator>= (const integer& i) const;

		/// Addition operator. Returns a new object of type 'integer'.
		integer operator+ (int64_t i) const;
		/// Addition operator. Returns a new object of type 'integer'.
		integer operator+ (const integer& i) const;

		/// Addition operator. Modifies the current instance.
		integer& operator+= (int64_t i);
		/// Addition operator. Modifies the current instance.
		integer& operator+= (const integer& i);

		/// Minus unary operator. Returns a new object of type 'integer'.
		integer operator- () const;
		/// Substraction operator. Returns a new object of type 'integer'.
		integer operator- (int64_t i) const;
		/// Substraction operator. Returns a new object of type 'integer'.
		integer operator- (const integer& i) const;

		/// Minus unary operator. Modifies the current instance.
		integer& operator- ();
		/// Substraction operator. Modifies the current instance.
		integer& operator-= (int64_t i);
		/// Substraction operator. Modifies the current instance.
		integer& operator-= (const integer& i);

		/// Product operator. Returns a new object of type 'integer'.
		integer operator* (int64_t i) const;
		/// Product operator. Returns a new object of type 'integer'.
		integer operator* (const integer& i) const;

		/// Product operator. Modifies the current instance.
		integer& operator*= (int64_t i);
		/// Product operator. Modifies the current instance.
		integer& operator*= (const integer& i);

		/// Quotient operator. Returns a new object of type 'integer'.
		integer operator/ (int64_t i) const;
		/// Quotient operator. Returns a new object of type 'integer'.
		integer operator/ (const integer& i) const;

		/// Quotient operator. Modifies the current instance.
		integer& operator/= (int64_t i);
		/// Quotient operator. Modifies the current instance.
		integer& operator/= (const integer& i);

		/// Power operator. Returns a new object of type 'integer'.
		integer operator^ (uint64_t i) const;
		/// Power operator. Returns a new object of type 'integer'.
		integer operator^ (const integer& i) const;

		/// Power operator. Modifies the current instance.
		integer& operator^= (uint64_t i);
		/// Power operator. Modifies the current instance.
		integer& operator^= (const integer& i);

		/// Modulus operator.
		uint64_t operator% (uint64_t i) const;
		/// Modulus operator.
		integer operator% (const integer& i) const;

		/* GETTERS */

		/// Returns whether this object is initialised or not.
		bool is_initialized() const;
		/// Returns the sign of this integer.
		int32_t get_sign() const;

		/// Returns the amount of bytes this integer occupies.
		size_t bytes() const;
		/// Returns the underlying gmp data structure.
		const mpz_t& get_raw_value() const;
		/// Converts this integer to a signed 64-bit integer.
		int64_t to_int() const;
		/// Converts this integer to an unsigned 64-bit integer.
		uint64_t to_uint() const;
		/// Converts this integer to a double-precision floating-point value.
		double to_double() const;

		/// Swaps the value of this integer with integer @e i's value.
		void swap(integer& i);

		/* CONVERTERS */

		/// Converts this integer to a string.
		std::string to_string() const;
		/// Converts this integer to a string.
		void as_string(std::string& s) const;

	private:
		/// Structure from GMP storing the integer's value.
		mpz_t m_val;
		/// Is this integer initialised?
		bool m_initialized = false;
};

/// Make an integer from a 64-bit unsigned integer value.
inline integer integer_from_ui(uint64_t n) {
	integer i;
	i.init_ui(n);
	return i;
}

} // -- namespace numeric
} // -- namespace lal
