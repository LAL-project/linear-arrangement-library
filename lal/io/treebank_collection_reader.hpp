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
#include <lal/io/treebank_error.hpp>
#include <lal/io/treebank_reader.hpp>

namespace lal {
namespace io {

/**
 * @brief A reader for a collection of treebanks.
 *
 * This class, the objects of which will be referred to as the "collection readers",
 * is an interface to help you do a custom processing of a set of treebanks. A
 * treebank collection is a set of files, each of which is a treebank. A treebank
 * is a file containing one or more lines, each describing a syntactic dependency
 * tree. If you want to output any of the features already calculated by
 * the library, use class @ref treebank_collection_processor instead; said class
 * is much easier to use and can process treebanks in parallel.
 *
 * Each tree in a treebank file is formatted as a list of whole positive numbers
 * (including zero), each representing a node of the tree. The number 0 denotes
 * the root of the tree, and a number at a certain position indicates its parent
 * node. For example, when number 4 is at position 9 it means that node 9 has
 * parent node 4. Therefore, if number 0 is at position 1 it means that node 1
 * is the root of the tree. A complete example of such a tree's representation
 * is the following
 *
 *       0 3 4 1 6 3
 *
 * which should be interpreted as
 *
 *		predecessor:       0 3 4 1 6 3
 *		node of the tree:  1 2 3 4 5 6
 *
 * Note that lines like these are not valid:
 *
 *		(1) 0 2 2 2 2 2
 *		(2) 2 0 0
 *
 * Line (1) is not valid due to a self-reference in the second position, and (2)
 * not being valid due to containing two '0' (i.e., two roots).
 *
 * A treebank collection reader helps you navigate through a treebank collection. It
 * does the job of initialising the @ref treebank_reader class, which is what
 * you need to do a custom process of a treebank file.
 *
 * Now, the treebank files are referenced within a "main file list", henceforth
 * called the main file. The main file's lines contain only two strings describing
 * a treebank. The first string is a self-descriptive name of the treebank (e.g.,
 * the ISO code of a language), and the second is the relative path to the file
 * containing the syntactic dependency trees (e.g., the syntactic dependency trees
 * of a language in a collection). The path is relative to the directory that contains
 * the main file.
 *
 * For example, the main file could be called \a stanford.txt, representing the
 * Stanford treebank collection, and could contain:
 *
 *		arb path/to/file/ar-all.heads2
 *		eus path/to/file/eu-all.heads2
 *		ben path/to/file/bn-all.heads2
 *		...
 *
 * where the first column contains a string referencing the language (in this case,
 * an ISO code), and the second column contains the relative path to the file
 * with the syntactic dependency trees.
 *
 * The user has to initialise a collection reader with the main file (the main file
 * list). For example, to read the Stanford collection the reader has to be initialised
 * with the main file \a stanford.txt which could contain the contents given
 * above. Bear in mind that a collection reader only processes the main file: it
 * iterates through the list of files within the main file using the method
 * @ref next_treebank. This method can be called as long as method
 * @ref has_treebank returns true. Each call to @ref next_treebank builds an
 * object of class @ref treebank_reader which allows the user to iterate
 * through the trees within the corresponding file. This object can be retrieved
 * by calling method @ref get_treebank_reader.
 *
 * An example of usage of this class is given in the following piece of code.
 * @code
 *		treebank_collection tbds;
 *		// it is advisable to check for errors
 *		tbds.init(mainf)
 *		while (tbds.has_language()) {
 *			// errors are not likely, but it is advisable to check for errors
 *			tbds.next_language();
 *			treebank_reader& tbread = tbds.get_treebank_reader();
 *			// here goes your custom processing of the treebank
 *			// ...
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
	 * - @ref lal::io::treebank_error::main_file_does_not_exist
	 * - @ref lal::io::treebank_error::main_file_could_not_be_opened
	 */
	treebank_error init(const std::string& main_file) noexcept;

	/// Returns true or false depending on whether there is a next treebank
	/// to be read.
	bool has_treebank() const noexcept { return not m_reached_end; }

	/**
	 * @brief Opens the file of the next treebank in the main file.
	 *
	 * This method can be called even after it has returned an error.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error::treebank_file_could_not_be_opened
	 * @pre Method @ref init did not return any errors.
	 * @post In case it returns an error, then method
	 * @ref get_treebank_reader will return an instance of
	 * @ref lal::io::treebank_reader that can't be used.
	 */
	treebank_error next_treebank() noexcept;

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

	/// Has this treebank collection reader reached the end?
	bool m_reached_end = false;

private:
	/// Consumes one line of the main file @ref m_main_file.
	void step_line() noexcept {
		if (m_list >> m_cur_treebank_name >> m_cur_treebank_filename) {
			// do nothing, there are more trees
		}
		else {
			m_reached_end = true;
			m_cur_treebank_name = m_cur_treebank_filename = "!";
		}
	}
};

} // -- namespace io
} // -- namespace lal
