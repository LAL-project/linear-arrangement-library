/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
 * @brief Treebank language dataset reader.
 *
 * This class, the objects of which will be referred to as the "readers", is
 * an interface for processing a list of treebanks. It offers the possibility
 * of processing each tree in every language treebank individually, as opposed
 * to class @ref treebank_processor, which also processes a whole dataset but
 * the information produced is limited to the capabilities of this library.
 *
 * A treebank dataset is made up of a set of files, each containing several
 * syntactic dependency trees of sentences of the corresponding language.
 * Each file is referenced within a "main file list", henceforth called the
 * main file. The main file indicates, for each language, a file with the
 * syntactic dependency trees. For example, the main file \a stanford.txt
 * could contain:
 *
 *		arb path/to/file/ar-all.heads2
 *		eus path/to/file/eu-all.heads2
 *		ben path/to/file/bn-all.heads2
 *		...
 *
 * where the first column contains a string referencing the language (e.g., an
 * ISO code, or simply the name of the language), and the second column contains
 * the full path to the file with the syntactic dependency trees.
 *
 *
 *
 * This reader works as follows: the user has to initialise the reader with the
 * main file (the main file list). For example, to read the Stanford dataset the
 * reader has to be initialised with the main file \a stanford.txt which could
 * contain the contents examplified above.
 *
 * It is important to notice that the files referenced within the main file
 * must be done so with full paths, i.e., paths relative to the system's root.
 *
 * This class only processes the main file: it iterates through the list
 * of files within the main file using the method @ref next_language(). This
 * method can be called as long as method @ref has_language() returns true.
 * Each call to @ref next_language() builds an internal object of type
 * @ref treebank_reader which allows the user to iterate through the trees
 * within the corresponding file. This object can be retrieved by calling
 * method @ref get_treebank_reader().
 *
 * The correct usage of this class is given in the following piece of code.
 * @code
 *		treebank_dataset tbds = treebank_dataset();
 *		// remember to check for errors
 *		tbds.init(mainf)
 *		while (tbds.has_language()) {
 *			// errors are not likely, but it is
 *			// advisable remember to check for errors
 *			tbds.next_language();
 *			treebank_reader tbread = tbds.get_treebank_reader();
 *			while (tbread.has_tree()) {
 *				// again, check for errors
 *				tbread.next_tree();
 *				lal::graphs::urtree t = tbread.get_tree();
 *				// custom processing of tree 't'
 *				// ....
 *			}
 *		}
 * @endcode
 */
class treebank_dataset {
	public:
		/// Constructor
		treebank_dataset();
		/// Destructor.
		~treebank_dataset();

		/**
		 * @brief Initialise the reader with a new dataset.
		 * @param main_file Main file.
		 * @return If any occurred then returns its type.
		 */
		dataset_error init( const std::string& main_file);

		/// Returns whether there is a next treebank to be read.
		bool has_language() const;

		/**
		 * @brief Opens the file of the next language in the main file.
		 * @return In case of error, returns a value different from
		 * dataset_error::none.
		 */
		dataset_error next_language();

		/// Returns a treebank reader class instance for processing a treebank.
		treebank_reader& get_treebank_reader();

	private:
		/// File containing the list of languages and their treebanks.
		std::string m_main_list;

		/// Handler for main file reading.
		std::ifstream m_list;
		/// Object to process a language's treebank.
		treebank_reader m_tree_read;

		/// Current language.
		std::string m_lang;
		/// Current treebank file.
		std::string m_tbf;
};

} // -- namespace io
} // -- namespace lal
