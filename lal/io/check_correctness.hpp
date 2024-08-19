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
#include <string>
#include <vector>

// lal includes
#include <lal/io/head_vector_error.hpp>
#include <lal/io/treebank_file_report.hpp>
#include <lal/io/treebank_collection_report.hpp>
#include <lal/basic_types.hpp>

namespace lal {
namespace io {

/**
 * @brief Checks the correctness of a head vector.
 * @param head_vector A head vector.
 * @returns A list of error messages.
 */
[[nodiscard]] std::vector<head_vector_error>
check_correctness_head_vector(const head_vector& head_vector)
noexcept;

/**
 * @brief Checks the correctness of a head vector.
 *
 * The head vector is given in a string.
 * @param head_vector_str A string containing a head vector.
 * @returns A list of error messages.
 */
[[nodiscard]] std::vector<head_vector_error>
check_correctness_head_vector(const std::string& head_vector_str)
noexcept;

/**
 * @brief Checks the correctness of a treebank collection.
 * @param treebank_filename Name of the treebank file.
 * @returns A list of errors.
 */
[[nodiscard]] treebank_file_report
check_correctness_treebank(const std::string& treebank_filename)
noexcept;

/**
 * @brief Checks the correctness of a treebank collection.
 * @param main_file_name Name of the main file.
 * @param n_threads Number of threads to use.
 * @returns A list of errors.
 */
[[nodiscard]] treebank_collection_report
check_correctness_treebank_collection
(const std::string& main_file_name, const std::size_t n_threads = 1)
noexcept;

} // -- namespace io
} // -- namespace lal
