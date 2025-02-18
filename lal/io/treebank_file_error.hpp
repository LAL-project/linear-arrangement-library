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
#include <lal/io/treebank_file_error_type.hpp>

namespace lal {
namespace io {

/**
 * @brief Treebank file error report class.
 *
 * This class is used only to be able to display errors arising in treebank
 * processing more clearly.
 *
 * Use method @ref get_error_message to retrieve the error message.
 *
 * Method @ref get_error_type returns a value from the enumeration
 * @ref lal::io::treebank_file_error_type.
 */
class treebank_file_error {
public:

	/// Default constructor
	treebank_file_error() noexcept = default;

	/// Constructor with error message and error type.
	treebank_file_error(
		const std::string& msg, const treebank_file_error_type& tet
	) noexcept
		: m_error_msg(msg),
		  m_error_type(tet)
	{ }

	/// Destructor
	~treebank_file_error() = default;

	/// Copy constructor.
	treebank_file_error(const treebank_file_error& te) noexcept
		: m_error_msg(te.m_error_msg),
		  m_error_type(te.m_error_type)
	{ }

	/// Move constructor.
	treebank_file_error(treebank_file_error&& te) noexcept
		: m_error_msg(std::move(te.m_error_msg)),
		  m_error_type(te.m_error_type)
	{ }

	/* ASSIGNMENT OPERATOR */

	/// Copy assignment operator.
	treebank_file_error& operator= (const treebank_file_error& te) noexcept
	{
		m_error_msg = te.m_error_msg;
		m_error_type = te.m_error_type;
		return *this;
	}

	/// Move assignment operator.
	treebank_file_error& operator= (treebank_file_error&& te) noexcept
	{
		m_error_msg = std::move(te.m_error_msg);
		m_error_type = te.m_error_type;
		return *this;
	}

	/* COMPARISON OPERATOR */

	/// Compares the treebank error with a treebank error type.
	[[nodiscard]] bool operator== (const treebank_file_error_type& tet
	) const noexcept
	{
		return m_error_type == tet;
	}
	/// Compares the treebank error with a treebank error type.
	[[nodiscard]] bool operator!= (const treebank_file_error_type& tet
	) const noexcept
	{
		return m_error_type != tet;
	}

	/* GETTERS */

	/// Returns the error message.
	[[nodiscard]] const std::string& get_error_message() const noexcept
	{
		return m_error_msg;
	}

	/// Returns the error type.
	[[nodiscard]] treebank_file_error_type get_error_type() const noexcept
	{
		return m_error_type;
	}

	/// Returns whether or not this is an actual error.
	[[nodiscard]] bool is_error() const noexcept
	{
		return m_error_type != treebank_file_error_type::no_error;
	}

private:

	/// Error message
	std::string m_error_msg;
	/// Error type.
	treebank_file_error_type m_error_type;
};

} // namespace io
} // namespace lal
