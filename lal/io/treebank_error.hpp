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
#include <string>

// lal includes
#include <lal/io/treebank_error_type.hpp>

namespace lal {
namespace io {

/**
 * @brief Treebank error report class.
 *
 * This class is used only to be able to display errors arising in treebank
 * processing more clearly.
 *
 * Use method @ref get_error_message to retrieve an error message.
 *
 * Method @ref get_error_type returns a value from the enumeration
 * @ref lal::io::treebank_error_type.
 */
class treebank_error {
public:
	/// Constructor with error message and error type.
	treebank_error(const std::string& msg, const treebank_error_type& tet) noexcept
		: m_error_msg(msg), m_error_type(tet)
	{ }

	/// 
	~treebank_error() = default;

	/// Copy constructor.
	treebank_error(const treebank_error& te) noexcept
		: m_error_msg(te.m_error_msg), m_error_type(te.m_error_type)
	{ }

#ifndef SWIG
	/// Move constructor.
	treebank_error(treebank_error&& te) noexcept
		: m_error_msg(std::forward<std::string>(te.m_error_msg)),
		  m_error_type(te.m_error_type)
	{ }
#endif

	/// Copy assignment operator.
	treebank_error& operator= (const treebank_error& te) noexcept {
		m_error_msg = te.m_error_msg;
		m_error_type = te.m_error_type;
		return *this;
	}

#ifndef SWIG
	/// Move assignment operator.
	treebank_error& operator= (treebank_error&& te) noexcept {
		m_error_msg = std::move(te.m_error_msg);
		m_error_type = te.m_error_type;
		return *this;
	}
#endif

	/// Compares a treebank error with a treebank error type.
	bool operator== (const treebank_error_type& tet) const noexcept
	{ return m_error_type == tet; }

	/// Retrieve the error message.
	const std::string& get_error_message() const noexcept
	{ return m_error_msg; }

	/// Retrieve the error type.
	treebank_error_type get_error_type() const noexcept
	{ return m_error_type; }

private:
	/// Error message
	std::string m_error_msg;
	/// Error type.
	treebank_error_type m_error_type;
};

} // -- namespace io
} // -- namespace lal
