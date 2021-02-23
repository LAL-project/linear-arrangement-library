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
#include <lal/io/dataset_error.hpp>
#include <lal/io/treebank_reader.hpp>

namespace lal {
namespace io {

/**
 * @brief A reader for a set of treebank language files.
 *
 * This class, the objects of which will be referred to as the "readers", is
 * an interface for processing a set of treebanks. It offers the possibility
 * of processing each tree in every treebank file individually, as opposed to
 * class @ref treebank_dataset_processor, which also processes a whole dataset
 * but the information produced is limited to the features available in this
 * library.
 *
 * A treebank dataset is made up of a set of files. Each file contains several
 * syntactic dependency trees (of, e.g., sentences of its corresponding language).
 * These files are referenced within a "main file list", henceforth called the
 * main file. The main file's lines contain only two strings. The first is an
 * identifier (e.g., the ISO code of a language), and the second is the relative
 * path to the file containing the syntactic dependency trees (e.g., the syntactic
 * dependency trees of Arabic in the Stanford dataset). For example, the main
 * file could be called \a stanford.txt and could contain:
 *
 *		arb path/to/file/ar-all.heads2
 *		eus path/to/file/eu-all.heads2
 *		ben path/to/file/bn-all.heads2
 *		...
 *
 * where, again, the first column contains a string referencing the language
 * (e.g., an ISO code, or simply the name of the language), and the second column
 * contains the relative path to the file with the syntactic dependency trees.
 *
 * This reader works as follows: the user has to initialise it with the
 * main file (the main file list). For example, to read the Stanford dataset the
 * reader has to be initialised with the main file \a stanford.txt which could
 * contain the contents examplified above.
 *
 * This class only processes the main file: it iterates through the list
 * of files within the main file using the method @ref next_treebank(). This
 * method can be called as long as method @ref has_treebank() returns true.
 * Each call to @ref next_treebank() builds an internal object of type
 * @ref treebank_reader which allows the user to iterate through the trees
 * within the corresponding file. This object can be retrieved by calling
 * method @ref get_treebank_reader().
 *
 * The correct usage of this class is given in the following piece of code.
 * @code
 *		treebank_dataset tbds;
 *		// it is advisable to check for errors
 *		tbds.init(mainf)
 *		while (tbds.has_language()) {
 *			// errors are not likely, but it is advisable to check for errors
 *			tbds.next_language();
 *			treebank_reader& tbread = tbds.get_treebank_reader();
 *			// ....
 *		}
 * @endcode
 */
class treebank_dataset_reader {
public:

	/**
	 * @brief Initialise the reader with a new dataset.
	 * @param main_file Main file of the dataset.
	 * @return If any occurred then returns its type.
	 */
	dataset_error init(const std::string& main_file) noexcept;

	/// Returns whether there is a next treebank to be read.
	bool has_treebank() const noexcept { return m_cur_treebank_name != "none"; }

	/**
	 * @brief Opens the file of the next treebank in the main file.
	 * @return In case of error, returns a value different from
	 * dataset_error::none.
	 */
	dataset_error next_treebank() noexcept;

	/// Returns a treebank reader class instance for processing a treebank.
	treebank_reader& get_treebank_reader() noexcept { return m_treebank_reader; }

private:
	/**
	 * @brief File containing the list of languages and their treebanks.
	 *
	 * This file's lines contain two strings, the first being the language
	 * name (used mainly for debugging purposes), and the name of the file
	 * containing the syntactic dependency trees of that language.
	 */
	std::string m_main_file = "none";

	/// The name of the current treebank file.
	std::string m_cur_treebank_name = "none";
	/// The name of the current treebank file.
	std::string m_cur_treebank_filename = "none";

	/// Handler for main file reading.
	std::ifstream m_list;
	/// Object to process a language's treebank.
	treebank_reader m_treebank_reader;

private:
	/// Consumes one line of the main file @ref m_main_file.
	void step_line() noexcept {
		if (m_list >> m_cur_treebank_name >> m_cur_treebank_filename) {
			// do nothing, there are more trees
		}
		else {
			m_cur_treebank_name = m_cur_treebank_filename = "none";
		}
	}
};

} // -- namespace io
} // -- namespace lal
