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
#include <array>

// lal includes
#include <lal/io/treebank_error.hpp>
#include <lal/io/treebank_feature.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatic processing of treebank files.
 *
 * This class, the objects of which will be referred to as the "processors",
 * has the goal to ease the processing a whole treebank dataset and produce
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
 * @ref treebank_dataset_reader. For example:
 * @code
 *		treebank_processor tbproc;
 *		// initialise the processor without features (remmeber to check for errors)
 *		tbproc.init(treebank_input_file, result_filename, false, "Book_1");
 *		tbproc.add_feature(treebank_processor::tree_feature::C);
 *		tbproc.add_feature(treebank_processor::tree_feature::D_var);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_processor {
public:
	// MODIFIERS

	/**
	 * @brief Adds a feature to the processor.
	 * @param fs Feature to be added.
	 */
	void add_feature(const treebank_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = true; }
	/**
	 * @brief Removes a feature from the processor.
	 * @param fs Feature to be removed.
	 */
	void remove_feature(const treebank_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = false; }

	// SETTERS

	/**
	 * @brief Sets the separator character.
	 *
	 * The default seprator is a tabulator character '\\t'.
	 * @param c The separator character.
	 */
	void set_separator(char c) noexcept { m_separator = c; }
	/**
	 * @brief Sets the level of verbosity of the @ref process method.
	 *
	 * Default is 0 (i.e., no verbosity at all).
	 * Verbosity is classified by levels:
	 * - Level 1: outputs progress messages.
	 * - Level 2: outputs error messages.
	 * @param k Verbosity level.
	 */
	void set_verbosity(int k) noexcept { m_be_verbose = k; }
	/**
	 * @brief Output a hedaer for the treebank result file.
	 *
	 * Default is true.
	 * @param h Output header or not.
	 */
	void set_output_header(bool h) noexcept { m_output_header = h; }

	// PROCESS THE TREEBANK DATASET

	/**
	 * @brief Initialise the processor with a new dataset.
	 * @param treebank_input_file File listing all the treebanks.
	 * @param output_file File where the results are to be stored.
	 * @param all_features Should the feature list contain all possible features?
	 * @param treebank_id A nickname for this treebank (for example, an ISO code).
	 */
	treebank_error init(
		const std::string& treebank_input_file,
		const std::string& output_file,
		bool all_features,
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
	 * @returns A value describing the error (if any) that may have occurred
	 * while processing the treebank file. If any error ocurred, see methods
	 * @pre Initialisation did not return any errors.
	 */
	treebank_error process() noexcept;

private:
	/// Character used as separator
	char m_separator = '\t';
	/// Output a header for each file
	bool m_output_header = true;
	/**
	 * @brief The verbosity of the processor.
	 *
	 * When set to a value greater than or equal to 1, method @ref process
	 * will output progress messages.
	 */
	int m_be_verbose = 0;

private:
	/**
	 * @brief Process a single tree in a treebank.
	 *
	 */
	template<class TREE, class OUT_STREAM>
	void process_tree(const TREE& rT, OUT_STREAM& out_lab_file) const;

private:
	/// File containing the list of languages and their treebanks.
	std::string m_treebank_filename = "none";
	/// Output directory.
	std::string m_output_file = "none";
	/// Treebank identifier.
	std::string m_treebank_id = "";

	/// The list of features to be computed.
	std::array<bool, __treebank_feature_size> m_what_fs;
};

} // -- namespace io
} // -- namespace lal
