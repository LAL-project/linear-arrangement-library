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

// lal includes
#include <lal/io/treebank_collection_processor.hpp>
#include <lal/io/treebank_processor.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatically process a treebank.
 *
 * This function is an utility to process easily a single treebank file.
 * This function uses the class @ref lal::io::treebank_processor in order to
 * process such a file. The default values of the processor are used, i.e.,
 * all features available in @ref lal::io::treebank_feature are computed.
 * @param treebank_file The treebank file name.
 * @param output_file The output file name.
 * @returns A treebank error (see @ref lal::io::treebank_error) if any.
 */
treebank_error process_treebank(
	const std::string& treebank_file,
	const std::string& output_file
)
noexcept;

/**
 * @brief Automatically process a treebank collection.
 *
 * This function is an utility to process easily a collection of treebank files.
 * This function uses the class @ref lal::io::treebank_collection_processor in
 * order to process such a collection, with all its options set to their default
 * value. The default options are:
 * - All features in @ref lal::io::treebank_feature are computed,
 * - All files produced are joined into a single file,
 * - The individual files are deleted.
 * @param treebank_collection_main_file The main file of the treebank collection.
 * @param output_directory The output .
 * @param num_threads The number of threads.
 * @returns A treebank error (see @ref lal::io::treebank_error) if any.
 */
treebank_error process_treebank_collection(
	const std::string& treebank_collection_main_file,
	const std::string& output_directory,
	std::size_t num_threads = 1
)
noexcept;

} // -- namespace io
} // -- namespace lal
