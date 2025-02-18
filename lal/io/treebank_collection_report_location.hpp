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

// lal includes
#include <lal/io/treebank_file_report.hpp>

namespace lal {
namespace io {

/// An auxiliary struct in replacement of std::tuple.
struct treebank_collection_report_location {
	/// The line number within the main file.
	uint64_t line_number;
	/// The name of the treebank file where the report comes from.
	std::string treebank_file_name;
	/// The id of the treebank file where the report comes from.
	std::string treebank_id;
	/// The report of errors in the treebank file.
	treebank_file_report report;

	/// Default constructor.
	treebank_collection_report_location() noexcept = default;

	/// Constructor with data.
	treebank_collection_report_location(
		const uint64_t ln,
		const std::string& tfn,
		const std::string& tid,
		const treebank_file_report& rep
	) noexcept
		: line_number(ln),
		  treebank_file_name(tfn),
		  treebank_id(tid),
		  report(rep)
	{ }

	/// Constructor with data.
	treebank_collection_report_location(
		const uint64_t ln,
		std::string&& tfn,
		std::string&& tid,
		treebank_file_report&& rep
	) noexcept
		: line_number(ln),
		  treebank_file_name(std::move(tfn)),
		  treebank_id(std::move(tid)),
		  report(std::move(rep))
	{ }

	/// Default destructor.
	~treebank_collection_report_location() noexcept = default;
};

} // namespace io
} // namespace lal
