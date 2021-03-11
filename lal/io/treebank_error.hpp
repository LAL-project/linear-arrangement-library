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

namespace lal {
namespace io {

/**
 * @brief Possible errors that can arise while processing a dataset.
 *
 * There are several reasons why a treebank dataset or a single treebank file
 * could not be processed. Because of this, certain methods return one of these
 * values instead of a plain 'false' value.
 */
enum class treebank_error {
	/// No error.
	no_error,

	/**
	 * @brief No features at all were given to the processor.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_processor
	 * - @ref lal::io::treebank_dataset_processor
	 */
	no_features,

	/**
	 * @brief A treebank was not found in disk.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_processor
	 * - @ref lal::io::treebank_dataset_processor
	 */
	treebank_file_does_not_exist,
	/**
	 * @brief A treebank file could not be opened.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_reader
	 * - @ref lal::io::treebank_processor
	 * - @ref lal::io::treebank_dataset_processor
	 */
	treebank_file_could_not_be_opened,
	/**
	 * @brief An empty line in the file being processed.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_reader
	 * - @ref lal::io::treebank_processor
	 * - @ref lal::io::treebank_dataset_processor
	 */
	empty_line_found,
	/**
	 * @brief Output file could not be opened.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_processor
	 * - @ref lal::io::treebank_dataset_processor
	 */
	output_file_could_not_be_opened,

	/**
	 * @brief Main file does not exist.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_dataset_processor
	 * - @ref lal::io::treebank_dataset_reader
	 */
	main_file_does_not_exist,
	/**
	 * @brief Main file could not be opened.
	 *
	 * Returned by:
	 * - @ref lal::io::treebank_dataset_reader
	 * - @ref lal::io::treebank_dataset_processor
	 */
	main_file_could_not_be_opened,
	/**
	 * @brief Output directory could not be found.
	 *
	 * Returned by @ref lal::io::treebank_dataset_processor.
	 */
	output_directory_does_not_exist,
	/**
	 * @brief The file containing the result of processing a treebank dataset could not be opened.
	 *
	 * Returned by @ref lal::io::treebank_dataset_processor.
	 */
	output_join_file_could_not_be_opened,
	/**
	 * @brief The resulting file of processing a treebank could not be opened
	 *
	 * Returned by @ref lal::io::treebank_dataset_processor.
	 */
	treebank_result_file_could_not_be_opened,
	/**
	 * @brief Processing one or more of the treebanks failed.
	 *
	 * Returned by @ref lal::io::treebank_dataset_processor.
	 */
	some_treebank_file_failed,
};

} // -- namespace io
} // -- namespace lal
