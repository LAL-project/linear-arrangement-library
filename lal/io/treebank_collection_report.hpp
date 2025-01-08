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
#include <cstdint>
#include <vector>

// lal includes
#include <lal/io/treebank_file_report.hpp>
#include <lal/io/treebank_file_error.hpp>
#include <lal/io/treebank_collection_report_location.hpp>

namespace lal {
namespace io {

/**
 * @brief Report on a treebank collection.
 *
 * Stores a list of reports on the treebank files within a collection. If the
 * main file of the collection could not be opened, then the treebank error is
 * set (see @ref m_treebank_error and @ref lal::io::treebank_file_error::is_error).
 *
 * Only when the treebank could be opened (and thus the treebank error is never
 * set) does this class contain a valid (possibly empty) list of reports on
 * its treebank files (see @ref m_reports).
 */
class treebank_collection_report {
public:

	/// The list of reports of errors of the treebanks within the collection.
	typedef std::vector<treebank_collection_report_location> error_list;

public:
	/// Default constructor.
	treebank_collection_report() noexcept = default;
	/// Default copy constructor.
	treebank_collection_report(const treebank_collection_report&) noexcept = default;
	/// Default move constructor.
	treebank_collection_report(treebank_collection_report&&) noexcept = default;

	/// Constructor with treebank error.
	treebank_collection_report(const treebank_file_error& err) noexcept {
		m_treebank_error = err;
	}
	/// Constructor with a single head vector error.
	treebank_collection_report(treebank_file_error&& err) noexcept {
		m_treebank_error = std::move(err);
	}

	/// Default destructor
	~treebank_collection_report() noexcept = default;

	/* ASSIGNMENT OPERATORS */

	/// Default copy assignment.
	treebank_collection_report& operator= (const treebank_collection_report&) noexcept = default;
	/// Default move assignment.
	treebank_collection_report& operator= (treebank_collection_report&&) noexcept = default;

	/* MODIFIERS */

	/// Adds a report on a treebank file.
	void add_report(
		const uint64_t line_number,
		const std::string& treebank_file_name,
		const std::string& treebank_id,
		const treebank_file_report& err
	)
	noexcept
	{
		m_reports.emplace_back(
			line_number,
			treebank_file_name,
			treebank_id,
			err
		);
	}
	/// Adds a report on a treebank file.
	void add_report(
		const uint64_t line_number,
		std::string&& treebank_file_name,
		std::string&& treebank_id,
		treebank_file_report&& err
	)
	noexcept
	{
		m_reports.emplace_back(
			line_number,
			std::move(treebank_file_name),
			std::move(treebank_id),
			std::move(err)
		);
	}

	/// Sets the error concerning the main file of the collection.
	void set_treebank_error(const treebank_file_error& err) noexcept {
		m_treebank_error = err;
	}
	/// Sets the error concerning the main file of the collection.
	void set_treebank_error(treebank_file_error&& err) noexcept {
		m_treebank_error = std::move(err);
	}

	/* GETTERS */

	/// Returns the number of errors in this report.
	[[nodiscard]] std::size_t get_num_errors() const noexcept {
		std::size_t c =
			(m_treebank_error.get_error_type() != io::treebank_file_error_type::no_error);

		for (const auto& rep : m_reports) {
			c += rep.report.get_num_errors();
		}
		return c;
	}

	/**
	 * @brief Returns the list of error reports for every treebank file.
	 *
	 * If the list returned is empty, then there were no errors.
	 */
	[[nodiscard]] const error_list& get_treebank_reports() const noexcept
	{ return m_reports; }

	/**
	 * @brief Returns the only treebank error concerning the file (if any).
	 * @returns An error that informs that the file does not exist or that it
	 * could not be opened.
	 */
	[[nodiscard]] const treebank_file_error& get_treebank_error() const noexcept
	{ return m_treebank_error; }

private:
	/// The error in the head vector and the line number where it happened.
	error_list m_reports;

	/// A treebank error.
	treebank_file_error m_treebank_error;
};

} // -- namespace io
} // -- namespace lal
