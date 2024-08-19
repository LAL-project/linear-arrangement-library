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
#if defined DEBUG
#include <cassert>
#endif
#include <cstdint>
#include <vector>

// lal includes
#include <lal/io/head_vector_error.hpp>
#include <lal/io/treebank_file_error.hpp>

namespace lal {
namespace io {

/**
 * @brief Report on a treebank file.
 *
 * Stores a list of errors concerning a treebank file and its contents. If the
 * treebank file could not be opened, then the treebank error is set (see
 * @ref m_treebank_error and @ref lal::io::treebank_file_error::is_error).
 *
 * Only when the treebank could be opened (and thus the treebank error is never
 * set) does this class contain a valid (possibly empty) list of errors of head
 * vectors (see @ref m_hv_errors).
 */
class treebank_file_report {
public:

	/**
	 * @brief The error list for a trebank file.
	 *
	 * A list of pairs where
	 * - The first field is the line number where the error is found.
	 * - The second field is the actual error in the head vector.
	 */
	typedef std::vector<std::pair<uint64_t, head_vector_error>> error_list;

public:
	/// Default constructor.
	treebank_file_report() noexcept = default;
	/// Default copy constructor.
	treebank_file_report(const treebank_file_report&) noexcept = default;

	/// Default move constructor.
	treebank_file_report(treebank_file_report&&) noexcept = default;

	/// Constructor with treebank error.
	treebank_file_report(const treebank_file_error& err) noexcept {
		m_treebank_error = err;
	}
	/// Constructor with a single head vector error.
	treebank_file_report(treebank_file_error&& err) noexcept {
		m_treebank_error = std::move(err);
	}

	/// Constructor with a single head vector error.
	treebank_file_report(const uint64_t line_number, const head_vector_error& err) noexcept {
		m_hv_errors.emplace_back(line_number, err);
	}
	/// Constructor with a single head vector error.
	treebank_file_report(const uint64_t line_number, head_vector_error&& err) noexcept {
		m_hv_errors.emplace_back(line_number, std::move(err));
	}

	/// Default destructor
	~treebank_file_report() noexcept = default;

	/* ASSIGNMENT OPERATORS */

	/// Default copy assignment.
	treebank_file_report& operator= (const treebank_file_report&) noexcept = default;
	/// Default move assignment.
	treebank_file_report& operator= (treebank_file_report&&) noexcept = default;

	/* MODIFIERS */

	/**
	 * @brief Adds an error to the list of errors.
	 * @param line_number Line number of the treebank file where the error is.
	 * @param err Error to add.
	 */
	void add_error(const uint64_t line_number, const head_vector_error& err) noexcept {
		m_hv_errors.emplace_back(line_number, err);
	}
	/**
	 * @brief Adds an error to the list of errors.
	 * @param line_number Line number of the treebank file where the error is.
	 * @param err Error to add.
	 */
	void add_error(const uint64_t line_number, head_vector_error&& err) noexcept {
		m_hv_errors.emplace_back(line_number, std::move(err));
	}

	/// Sets the treebank error @ref m_treebank_error.
	void set_treebank_error(const treebank_file_error& err) noexcept {
		m_treebank_error = err;
	}
	/// Sets the treebank error @ref m_treebank_error.
	void set_treebank_error(treebank_file_error&& err) noexcept {
		m_treebank_error = std::move(err);
	}

	/* GETTERS */

	/// Returns the number of errors in this report.
	[[nodiscard]] std::size_t get_num_errors() const noexcept {
		return m_treebank_error.is_error() + m_hv_errors.size();
	}

	/**
	 * @brief Returns the list of errors in the head vectors.
	 *
	 * If the list returned is empty, then there were no errors.
	 */
	[[nodiscard]] const error_list& get_head_vector_errors() const noexcept
	{ return m_hv_errors; }

	/**
	 * @brief Returns the only treebank error concerning the file (if any).
	 * @returns An error that informs that the file does not exist or that it
	 * could not be opened.
	 */
	[[nodiscard]] const treebank_file_error& get_treebank_error() const noexcept
	{ return m_treebank_error; }

private:
	/// The error in the head vector and the line number where it happened.
	error_list m_hv_errors;

	/// A treebank error. Set if the file could not be opened or it does not exist.
	treebank_file_error m_treebank_error;
};

} // -- namespace io
} // -- namespace lal
