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

/* This file is used only to include all
 * other files grouping the i/o functions.
 */

// output of graphs to some std::ostream
#include <lal/io/basic_output.hpp>

// read formatted files
#include <lal/io/edge_list.hpp>
#include <lal/io/head_vector.hpp>

// check correctness of treebanks
#include <lal/io/head_vector_error_type.hpp>
#include <lal/io/head_vector_error.hpp>

#include <lal/io/treebank_file_error_type.hpp>
#include <lal/io/treebank_file_error.hpp>

#include <lal/io/treebank_collection_report_location.hpp>
#include <lal/io/treebank_collection_report.hpp>
#include <lal/io/treebank_file_report.hpp>

#include <lal/io/check_correctness.hpp>

// process treebanks
#include <lal/io/treebank_feature_type.hpp>

#include <lal/io/treebank_reader.hpp>
#include <lal/io/treebank_collection_reader.hpp>

#include <lal/io/treebank_processor.hpp>
#include <lal/io/treebank_collection_processor.hpp>
