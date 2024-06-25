/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
#include <cstdint>
#include <string>

namespace lal {
namespace io {

/**
 * @brief Report on a treebank file.
 *
 * Stores a simple error message and the line within the file where it is found.
 */
class report_treebank_file {
public:
	/// Default constructor.
	report_treebank_file() = default;
	/// Default copy constructor.
	report_treebank_file(const report_treebank_file&) = default;

	/// Default move constructor.
	report_treebank_file(report_treebank_file&&) = default;

	/// Default destructor
	~report_treebank_file() = default;

	/**
	 * @brief Constructor
	 * @param line_number Line number within the treebank file.
	 * @param error_message Error message.
	 */
	report_treebank_file(
		uint64_t line_number,
		const std::string& error_message
	)
		: m_treebank_file_line_number(line_number),
		  m_error_message(error_message)
	{}

	/// Default copy assignment.
	report_treebank_file& operator= (const report_treebank_file&) = default;
	/// Default move assignment.
	report_treebank_file& operator= (report_treebank_file&&) = default;

	/* GETTERS */

	/// Returns the line number within the treebank file where the error is found.
	uint64_t get_line_number() const noexcept { return m_treebank_file_line_number; }

	/// Returns the error message.
	const std::string& get_error_message() const noexcept { return m_error_message; }

private:
	/// The line number within the treebank file
	uint64_t m_treebank_file_line_number;
	/// Error message
	std::string m_error_message;
};

/**
 * @brief Report on a treebank collection.
 *
 * Stores a simple error message and the line within the file where it is found
 * (see @ref lal::io::report_treebank_file), and the name of the treebank
 * where the error is found and the line within the main file where the treebank
 * was read at.
 */
class report_treebank_collection {
public:
	/// Default constructor.
	report_treebank_collection() = default;
	/// Default copy constructor.
	report_treebank_collection(const report_treebank_collection&) = default;
	/// Default move constructor.
	report_treebank_collection(report_treebank_collection&&) = default;
	/// Default destructor
	~report_treebank_collection() = default;

	/**
	 * @brief Constructor with data
	 * @param treebank_file_path Name of the treebank where the error is found.
	 * @param main_file_line Line within the main file.
	 * @param treebank_line_number Line within the treebank file.
	 * @param error_message Error message.
	 */
	report_treebank_collection(
		const std::string& treebank_file_path,
		uint64_t main_file_line,
		uint64_t treebank_line_number,
		const std::string& error_message
	) noexcept
		: m_treebank_file_path(treebank_file_path),
		  m_main_file_line(main_file_line),
		  m_treebank_file_line_number(treebank_line_number),
		  m_error_message(error_message)
	{}

	/// Default copy assignment.
	report_treebank_collection& operator= (const report_treebank_collection&) = default;
	/// Default move assignment.
	report_treebank_collection& operator= (report_treebank_collection&&) = default;

	/// Returns the line number within the treebank file where the error is found.
	const std::string& get_treebank_file_name() const noexcept
	{ return m_treebank_file_path; }

	/// Returns the error message.
	uint64_t get_line_within_main_file() const noexcept
	{ return m_main_file_line; }

	/// Returns the line number within the treebank file where the error is found.
	uint64_t get_treebank_file_line() const noexcept
	{ return m_treebank_file_line_number; }

	/// Returns the error message.
	const std::string& get_error_message() const noexcept
	{ return m_error_message; }

private:
	/// Filename to the treebank file.
	std::string m_treebank_file_path;
	/// Line within the main file where the treebank file is found.
	uint64_t m_main_file_line;
	/// The line number within the treebank file.
	uint64_t m_treebank_file_line_number;
	/// Error message.
	std::string m_error_message;
};

} // -- namespace io
} // -- namespace lal
