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
#include <tuple>
#include <array>

// lal includes
#include <lal/io/treebank_error.hpp>
#include <lal/io/process_treebank_base.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatic processing of treebank datasets.
 *
 * This class, the objects of which will be referred to as the "processors",
 * has the goal to ease the processing a whole treebank dataset and produce
 * data for a fixed set of features available in the library. See the enumeration
 * @ref lal::io::treebank_feature for details on the features available.
 *
 * This class is meant to process a treebank dataset only. A treebank dataset is
 * a set of treebank files, each containing several syntactic dependency trees
 * of sentences. Each file is referred to as a treebank file. Each of these
 * files is referenced within a "main file list", henceforth called the "main
 * file". The main file is a two-column formatted file, the first of which
 * contains a self-descriptive name of the treebank that is indicated in the next
 * column.
 *
 * For example, the main file of a treebank dataset of languages could contain:
 *
 *		arb path/to/file/ar-all.heads2
 *		eus path/to/file/eu-all.heads2
 *		ben path/to/file/bn-all.heads2
 *		...
 *
 * where the first column contains a string referencing the treebank of a language
 * (in this case, by giving an ISO code of a language), and the second column
 * contains the full path to the file with the syntactic dependency trees.
 *
 * Every processor must be initialised prior to processing the dataset. This is done
 * via method @ref init, which requires the path to the main file and the output
 * directory where the results are going to be stored. It also requires a Boolean
 * value indicating whether all (or none) of the features should be used. Moreover,
 * it also admits an optional parameter indicating the number of threads to be
 * used to parallelise the processing of the files.
 *
 * When initialised, a processor can be removed or added features: when the number
 * of features to calculate is low, it can be initialised with no features, and
 * then be added some via method @ref add_feature. Conversely, if the number of
 * features is high, but not all features are needed, a processer can be initialised
 * with all features, and then be removed some of them via method @ref remove_feature.
 *
 * Processing a treebank dataset with this class will produce a file for every
 * treebank in the dataset. These files can be merged together by indicating so
 * via method @ref join_files. A new file will be created, regardless of the number
 * of treebanks in the dataset.
 *
 * Finally, the treebank dataset is processed via method @ref process. If all the
 * files produced (one for each treebank) are to be joined in a single file, users
 * can give this new file a name by passing it to method @ref process as a string.
 * Also, users can indicate via an optional Boolean parameter whether the individual
 * files are to be removed or not.
 *
 * Method @ref process returns a value of the enumeration @ref treebank_error.
 * Further errors can be checked via methods @ref get_num_errors,
 * @ref get_error_type, @ref get_error_treebank_filename, @ref get_error_treebank_name.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_dataset_reader. For example:
 * @code
 *		treebank_dataset_processor tbproc;
 *		// initialise the processor without features (remmeber to check for errors)
 *      // and 4 threads for faster processing.
 *		tbproc.init(main_file, output_dir, false, 4);
 *		tbproc.add_feature(lal::io::treebank_feature::tree_feature::C);
 *		tbproc.add_feature(lal::io::treebank_feature::tree_feature::D_var);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_dataset_processor : public process_treebank_base {
public:
	// SETTERS

	/**
	 * @brief Join the resulting files into a single file.
	 * @param v A Boolean value.
	 */
	void set_join_files(bool v) noexcept { m_join_files = v; }

	// GETTERS

	/// Returns the number of errors that arised during processing.
	size_t get_num_errors() const noexcept
	{ return m_errors_from_processing.size(); }

	/**
	 * @brief Get the @e ith error.
	 * @param i The index of the error, an unsigned integer.
	 * @returns A value of the enumeration @ref lal::io::treebank_error.
	 */
	treebank_error get_error_type(size_t i) const noexcept
	{ return std::get<0>(m_errors_from_processing[i]); }

	/**
	 * @brief Get the treebank's file name where the @e ith error happened.
	 * @param i The index of the error, an unsigned integer.
	 * @returns The name of the treebank file where the @e i-th error happened
	 * as a string.
	 */
	const std::string& get_error_treebank_filename(size_t i) const noexcept
	{ return std::get<1>(m_errors_from_processing[i]); }

	/**
	 * @brief Get the treebank's name for where  the @e ith error happened.
	 * @param i The index of the error, an unsigned integer.
	 * @returns The name of the treebank where the @e i-th error happened
	 * as a string.
	 */
	const std::string& get_error_treebank_name(size_t i) const noexcept
	{ return std::get<2>(m_errors_from_processing[i]); }

	// PROCESS THE TREEBANK DATASET

	/**
	 * @brief Initialise the processor with a new dataset.
	 * @param main_file File listing all the treebanks.
	 * @param output_directory Directory where the result files are to be stored.
	 * @param all_features Should the feature list contain all possible features?
	 * @param n_threads Number of threads to use in a parallel application.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error::main_file_does_not_exist
	 * - @ref lal::io::treebank_error::output_directory_does_not_exist
	 */
	treebank_error init(
		const std::string& main_file,
		const std::string& output_directory,
		bool all_features,
		size_t n_threads = 1
	)
	noexcept;

	/**
	 * @brief Process the treebank dataset.
	 *
	 * This method produces the information as explained in this class'
	 * description. However, it may fail to do so. In this case it will return
	 * a value different from @ref lal::io::treebank_error::no_error.
	 *
	 * This function uses attributes @ref m_separator, @ref m_output_header to
	 * format the output data. It also outputs the current progress if
	 * @ref m_be_verbose is set to true.
	 *
	 * Moreover, it gathers the errors thay may have occurred during processing.
	 * If so, see methods @ref get_num_errors, @ref get_error_type,
	 * @ref get_error_treebank_name.
	 * @param res Name of the file where all values are going to be stored.
	 * @param remove Removes all individual files. The default value is true.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error::no_features
	 * - @ref lal::io::treebank_error::main_file_could_not_be_opened
	 * - @ref lal::io::treebank_error::some_treebank_file_failed
	 * - @ref lal::io::treebank_error::output_join_file_could_not_be_opened
	 * - @ref lal::io::treebank_error::treebank_result_file_could_not_be_opened
	 * @ref get_num_errors, @ref get_error_treebank_filename,
	 * @ref get_error_treebank_name.
	 * @pre Initialisation did not return any errors.
	 */
	treebank_error process(
		const std::string& res = "",
		bool remove = true
	) noexcept;

private:
	/// The list of names of the treebanks.
	std::vector<std::string> m_all_individual_treebank_names;

	/// Join the files into a single file.
	bool m_join_files = false;

	/// Number of threads to use.
	size_t m_num_threads = 1;

	/// Set of errors resulting from processing the treebank dataset.
	std::vector<std::tuple<treebank_error, std::string, std::string>>
	m_errors_from_processing;

private:
	/**
	 * @brief Joins all resulting files into a single file
	 * @param resname Name of the result file.
	 * @param remove Are the individual files to be removed?
	 * @returns An error, if any.
	 */
	treebank_error join_all_files(const std::string& resname, bool remove) const noexcept;

private:
	/// Output directory.
	std::string m_out_dir = "none";
	/// File containing the list of languages and their treebanks.
	std::string m_main_file = "none";
};

} // -- namespace io
} // -- namespace lal
