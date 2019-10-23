/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
 * There are several reasons why a dataset could not be processed.
 * Because of this, certain methods return one of these values instead
 * of a plain 'false' value.
 */
enum dataset_error {
	/// No error.
	no_error,
	/// Parent directory could not be found.
	no_parent_dir,
	/// Main file could not be found.
	no_main_file,
	/// One of the treebank files could not be found.
	no_treebank_file,
	/// The reader found an empty line.
	empty_line
};

/**
 * @brief Returns a description of each processing error.
 *
 * There are several types of problems that can arise while processing
 * a dataset. These are described in @ref dataset_error. This method
 * provides a more human-readable description of these errors.
 * @param pe The type of processing error.
 * @return Returns a string describing the error.
 */
inline std::string dataset_error_string(const dataset_error& pe) {
	switch (pe) {
	case dataset_error::no_error: return "No error";
	case dataset_error::no_parent_dir: return "Parent directory not found";
	case dataset_error::no_main_file: return "Main file not found";
	case dataset_error::empty_line: return "An empty line was found";
	case dataset_error::no_treebank_file: return "Some treebank file could not be found";
	}
	return "Wrong value for process_error parameter";
}

} // -- namespace io
} // -- namespace lal
