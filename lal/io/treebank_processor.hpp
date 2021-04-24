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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
#include <string>

// lal includes
#include <lal/io/treebank_error.hpp>
#include <lal/io/process_treebank_base.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatic processing of treebank files.
 *
 * This class, the objects of which will be referred to as the "processors",
 * has the goal to ease the processing a whole treebank collection and produce
 * data for a fixed set of features available in the library. See the enumeration
 * @ref lal::io::treebank_feature for details on the features available.
 *
 * This class is meant to process a single treebank only. A treebank file is
 * a plain text file that contains several syntactic dependency trees of sentences.
 *
 * Every processor must be initialised prior to processing the treebank file.
 * This is done via method @ref init, which requires the path to the treebank file
 * and the name of the output file (the treebank result) where the results are
 * going to be stored. It also requires a Boolean value indicating whether all
 * (or none) of the features should be used. Processing a treebank file with this
 * class will produce a single file, with as many columns as features added to
 * the processor. The columns are separated with a separating character (see
 * method @ref set_separator); this file will contain a header only if method
 * @ref set_output_header has been called with true). Progress and error messages
 * can be controlled via method @ref set_verbosity.
 *
 * When initialised, features can be added to or removed from the processor: when
 * the number of features to calculate is low, it can be initialised with no
 * features, and then be added some via method @ref add_feature. Conversely, if
 * the number of features is high, but not all features are needed, a processer
 * can be initialised with all features, and then be removed some of them via
 * method @ref remove_feature.
 *
 * Finally, the treebank file is processed via method @ref process. This method
 * returns a value of the enumeration @ref treebank_error.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_collection_reader. For example:
 * @code
 *		treebank_processor tbproc;
 *		// initialise the processor without features (remmeber to check for errors)
 *		tbproc.init(treebank_input_file, result_filename, "Book_1");
 *		tbproc.add_feature(treebank_processor::tree_feature::num_crossings);
 *		tbproc.add_feature(treebank_processor::tree_feature::var_num_crossings);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_processor : public process_treebank_base {
public:
	// PROCESS THE TREEBANK collection

	/**
	 * @brief Initialise the processor with a new collection.
	 * @param treebank_input_file File listing all the treebanks.
	 * @param output_file File where the results are to be stored.
	 * @param treebank_id A nickname for this treebank (for example, an ISO code).
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error::treebank_file_does_not_exist
	 */
	treebank_error init(
		const std::string& treebank_input_file,
		const std::string& output_file,
		const std::string& treebank_id = ""
	)
	noexcept;

	/**
	 * @brief Process the treebank file.
	 *
	 * This method produces the information as explained in this class'
	 * description. However, it may fail to do so. In this case it will return
	 * a value different from @ref lal::io::treebank_error::no_error.
	 *
	 * This function uses attributes @ref m_separator, @ref m_output_header to
	 * format the output data. It also outputs the current progress if
	 * @ref m_be_verbose is set to true.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error::no_features
	 * - @ref lal::io::treebank_error::output_file_could_not_be_opened
	 * - @ref lal::io::treebank_error::treebank_file_could_not_be_opened
	 * @pre Initialisation did not return any errors.
	 */
	treebank_error process() noexcept;

private:
	/// Process a single tree in a treebank.
	template<class TREE, class OUT_STREAM>
	void process_tree
	(const TREE& rT, OUT_STREAM& out_lab_file)
	const noexcept;

	// HEADER

	/// Output the header for the tree types
	template<class OUT_STREAM>
	void output_tree_type_header
	(OUT_STREAM& out_lab_file)
	const noexcept;

	/// Output the header for the tree types
	template<class OUT_STREAM>
	void output_syndepstruct_type_header
	(OUT_STREAM& out_lab_file)
	const noexcept;

	// VALUES

	/// Output the values for the tree types
	template<class TREE_TYPE, class OUT_STREAM>
	void output_tree_type_values
	(TREE_TYPE& t, OUT_STREAM& out_lab_file)
	const noexcept;

	/// Output the values for the syntactic dependency tree types
	template<class TREE_TYPE, class OUT_STREAM>
	void output_syndepstruct_type_values
	(const TREE_TYPE& t, OUT_STREAM& out_lab_file)
	const noexcept;

private:
	/// File containing the list of languages and their treebanks.
	std::string m_treebank_filename = "none";
	/// Output directory.
	std::string m_output_file = "none";
	/// Treebank identifier.
	std::string m_treebank_id = "";
};

} // -- namespace io
} // -- namespace lal
