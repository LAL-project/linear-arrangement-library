/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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
#include <lal/io/treebank_error.hpp>
#include <lal/io/treebank_reader.hpp>

namespace lal {
namespace io {

/**
 * @brief A reader for a collection of treebanks.
 *
 * This class, the objects of which will be referred to as the "collection readers",
 * is an interface to help you do a custom processing of a set of treebanks (see
 * @ref LAL_concepts__treebank_collection and @ref LAL_concepts__treebank for
 * further details on treebanks and treebank collections).
 *
 * The user has to initialise a collection reader with the main file (the main file
 * list). For example, to read the Stanford collection the reader has to be initialised
 * with the main file \a stanford.txt which could contain the contents given
 * above. Bear in mind that a collection reader only processes the main file: it
 * iterates through the list of files within the main file using the method
 * @ref next_treebank. This method can be called as long as method
 * @ref end returns false. Each call to @ref next_treebank builds an
 * object of class @ref treebank_reader which allows the user to iterate
 * through the trees within the corresponding file. This object can be retrieved
 * by calling method @ref get_treebank_reader.
 *
 * An example of usage of this class is given in the following piece of code.
 * @code
 *		lal::io::treebank_collection tbcolreader;
 *		// it is advisable to check for errors
 *		auto err = tbcolreader.init(mainf)
 *		while (not tbcolreader.end()) {
 *			lal::io::treebank_reader& tbreader = tbcolreader.get_treebank_reader();
 *			if (not tbreader.is_open()) {
 *				tbcolreader.next_treebank();
 *				continue;
 *			}
 *			// here goes your custom processing of the treebank
 *			// ...
 *
 *			tbcolreader.next_treebank();
 *		}
 * @endcode
 */
class treebank_collection_reader {
public:

	/**
	 * @brief Initialise the reader with a new collection.
	 *
	 * Objects of this class can't be used to read a treebank until this method
	 * returns no error.
	 * @param main_file Main file of the collection.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error_type::main_file_does_not_exist
	 * - @ref lal::io::treebank_error_type::main_file_could_not_be_opened
	 */
	treebank_error init(const std::string& main_file) noexcept;

	/// Returns true or false depending on whether there is a next treebank
	/// to be read.
	bool end() const noexcept { return m_reached_end; }

	/**
	 * @brief Opens the file of the next treebank in the main file.
	 *
	 * This method can be called even after it has returned an error.
	 */
	void next_treebank() noexcept;

	/// Returns a treebank reader class instance for processing a treebank.
	treebank_reader& get_treebank_reader() noexcept
	{ return m_treebank_reader; }

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

	/// Did we reach the end of the file?
	bool m_reached_end = false;
	/// Have all trees in the file been consumed?
	bool m_no_more_treebanks = false;

private:
	/// Consumes one line of the main file @ref m_main_file.
	void step_line() noexcept {
		if (m_list >> m_cur_treebank_name >> m_cur_treebank_filename) {
			// do nothing, there are more trees
		}
		else {
			m_no_more_treebanks = true;
			m_cur_treebank_name = m_cur_treebank_filename = "!";
		}
	}
};

} // -- namespace io
} // -- namespace lal
