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
#include <string>

// lal includes
#include <lal/io/head_vector_error_type.hpp>

namespace lal {
namespace io {

/**
 * @brief Head vector error report class.
 *
 * This class is used only to be able to display errors found in head vectors
 * more clearly.
 *
 * Use method @ref get_error_message to retrieve the error message.
 *
 * Method @ref get_error_type returns a value from the enumeration
 * @ref lal::io::head_vector_error_type.
 */
class head_vector_error {
public:

	/// Default constructor.
	head_vector_error() noexcept = default;

	/// Constructor with error message and error type.
	head_vector_error
	(const std::string& msg, const head_vector_error_type& tet)
	noexcept
		:
		m_error_msg(msg),
		m_error_type(tet)
	{ }
	/// Constructor with error message and error type.
	head_vector_error
	(std::string&& msg, const head_vector_error_type& tet)
	noexcept
		:
		m_error_msg(msg),
		m_error_type(tet)
	{ }

	/// Copy constructor.
	head_vector_error(const head_vector_error& te) noexcept :
		m_error_msg(te.m_error_msg),
		m_error_type(te.m_error_type)
	{ }

	/// Move constructor.
	head_vector_error(head_vector_error&& te) noexcept :
		m_error_msg(std::move(te.m_error_msg)),
		m_error_type(te.m_error_type)
	{ }

	/// Destructor
	~head_vector_error() = default;

	/* ASSIGNMENT OPERATORS */

	/// Default copy assignment operator.
	head_vector_error& operator= (const head_vector_error& ) noexcept = default;
	/// Default move assignment operator.
	head_vector_error& operator= (head_vector_error&& ) noexcept = default;

	/* COMPARISON OPERATORS */

	/// Compares this head vector error with a head vector error type.
	[[nodiscard]] bool operator== (const head_vector_error_type& tet) const noexcept
	{ return m_error_type == tet; }
	/// Compares this head vector error with a head vector error type.
	[[nodiscard]] bool operator!= (const head_vector_error_type& tet) const noexcept
	{ return m_error_type != tet; }

	/* GETTERS */

	/// Returns the error message.
	[[nodiscard]] const std::string& get_error_message() const noexcept { return m_error_msg; }

	/// Returns the error type.
	[[nodiscard]] head_vector_error_type get_error_type() const noexcept
	{ return m_error_type; }

private:
	/// Error message
	std::string m_error_msg;
	/// Error type.
	head_vector_error_type m_error_type;
};

} // -- namespace io
} // -- namespace lal
