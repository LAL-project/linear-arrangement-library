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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
#include <string>
#include <tuple>

// lal includes
#include <lal/io/treebank_error.hpp>
#include <lal/io/process_treebank_base.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatic processing of treebank collections.
 *
 * This class, the objects of which will be referred to as the "processors",
 * has the goal to ease the processing a whole treebank collection and produce
 * data for a fixed set of features available in the library. See the enumeration
 * @ref lal::io::treebank_feature for details on the features available, and
 * see @ref LAL_concepts__treebank_collection and @ref LAL_concepts__treebank for
 * further details on treebank collections and treebanks.
 *
 * Every processor must be initialised prior to processing the collection. This is done
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
 * Processing a treebank collection with this class will produce a file for every
 * treebank in the collection. These files can be merged together by indicating so
 * via method @ref set_join_files. A new file will be created, regardless of the
 * number of treebanks in the collection. See method @ref set_join_to_file_name
 * to set the name of the file that merges the result. Said file will contain
 * an extra column (besides the columns corresponding to the features computed)
 * indicating the treebank each line pertains. The name of this column can be
 * changed using method @ref set_treebank_column_name.
 *
 * Finally, the treebank collection is processed via method @ref process. Said
 * method returns a value of the enumeration @ref treebank_error. Further errors
 * can be checked via methods @ref get_num_errors, @ref get_error_type,
 * @ref get_error_treebank_filename, @ref get_error_treebank_name.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_collection_reader. For example:
 * @code
 *		treebank_collection_processor tbproc;
 *		// initialise the processor without features (remember to check for errors)
 *      // and 4 threads for faster processing.
 *		tbproc.init(main_file, output_dir, 4);
 *		tbproc.add_feature(lal::io::treebank_feature::num_crossings);
 *		tbproc.add_feature(lal::io::treebank_feature::var_num_crossings);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_collection_processor : public _process_treebank_base {
public:
	// SETTERS

	/**
	 * @brief Join the resulting files into a single file.
	 * @param v A Boolean value.
	 */
	void set_join_files(bool v) noexcept { m_join_files = v; }

	/// Set the number of threads
	void set_number_threads(std::size_t n_threads) noexcept {
#if defined DEBUG
		assert(n_threads != 0);
#endif
		m_num_threads = n_threads;
	}

	// GETTERS

	/// Returns the number of errors that arised during processing.
	std::size_t get_num_errors() const noexcept
	{ return m_errors_from_processing.size(); }

	/**
	 * @brief Get the @e ith error.
	 * @param i The index of the error, an unsigned integer.
	 * @returns A value of the enumeration @ref lal::io::treebank_error.
	 */
	const treebank_error& get_error_type(std::size_t i) const noexcept
	{ return std::get<0>(m_errors_from_processing[i]); }

	/**
	 * @brief Get the treebank's file name where the @e ith error happened.
	 * @param i The index of the error, an unsigned integer.
	 * @returns The name of the treebank file where the @e i-th error happened
	 * as a string.
	 */
	const std::string& get_error_treebank_filename(std::size_t i) const noexcept
	{ return std::get<1>(m_errors_from_processing[i]); }

	/**
	 * @brief Get the treebank's name for where  the @e ith error happened.
	 * @param i The index of the error, an unsigned integer.
	 * @returns The name of the treebank where the @e i-th error happened
	 * as a string.
	 */
	const std::string& get_error_treebank_name(std::size_t i) const noexcept
	{ return std::get<2>(m_errors_from_processing[i]); }

	/**
	 * @brief Sets the name of the file where all values are going to be stored.
	 *
	 * If this name is not given set, the class will construct one using the name
	 * of the main file and appending the string "_full" to it. If, on the contrary,
	 * some path is given and is not a full path then the path is considered to
	 * be relative to the working directory path.
	 * @param join_to String
	 */
	void set_join_to_file_name(const std::string& join_to) noexcept {
		m_join_to_file = join_to;
	}

	/// Sets the name of the column used to group lines according to the treebank.
	void set_treebank_column_name(const std::string& name) noexcept {
		m_treebank_column_name = name;
	}

	// PROCESS THE TREEBANK COLLECTION

	/**
	 * @brief Initialise the processor with a new collection.
	 * @param main_file File listing all the treebanks.
	 * @param output_directory Directory where the result files are to be stored.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error_type::main_file_does_not_exist
	 * - @ref lal::io::treebank_error_type::output_directory_could_not_be_created
	 */
	treebank_error init
	(const std::string& main_file, const std::string& output_directory)
	noexcept;

	/**
	 * @brief Process the treebank collection.
	 *
	 * This method produces the information as explained in this class'
	 * description. However, it may fail to do so. In this case it will return
	 * a value different from @ref lal::io::treebank_error_type::no_error.
	 *
	 * This function uses attributes @ref m_separator, @ref m_output_header to
	 * format the output data. It also outputs the current progress if
	 * @ref m_be_verbose is set to true.
	 *
	 * Moreover, it gathers the errors thay may have occurred during processing.
	 * If so, see methods @ref get_num_errors, @ref get_error_type,
	 * @ref get_error_treebank_name.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error_type::no_features
	 * - @ref lal::io::treebank_error_type::main_file_could_not_be_opened
	 * - @ref lal::io::treebank_error_type::some_treebank_file_failed
	 * - @ref lal::io::treebank_error_type::output_join_file_could_not_be_opened
	 * - @ref lal::io::treebank_error_type::treebank_result_file_could_not_be_opened
	 *
	 * See methods @ref get_num_errors, @ref get_error_treebank_filename,
	 * @ref get_error_treebank_name to know how to retrieve these errors.
	 * @pre Initialisation did not return any errors.
	 */
	treebank_error process() noexcept;

private:
	/**
	 * @brief Joins all resulting files into a single file
	 * @returns An error code, if any.
	 */
	treebank_error join_all_files() const noexcept;

private:
	/// The list of names of the treebanks.
	std::vector<std::string> m_all_individual_treebank_ids;
	/// The name of the file that joins all result files.
	std::string m_join_to_file = "";
	/// Join the files into a single file.
	bool m_join_files = true;
	/// Name of the column that identifies each treebank.
	std::string m_treebank_column_name = "treebank";
	/// Number of threads to use.
	std::size_t m_num_threads = 1;
	/// The name of the column in the join file
	std::string m_column_join_name = "";

	/// Set of errors resulting from processing the treebank collection.
	std::vector<std::tuple<treebank_error, std::string, std::string>>
	m_errors_from_processing;

	/// Output directory.
	std::string m_out_dir = "none";
	/// File containing the list of languages and their treebanks.
	std::string m_main_file = "none";
};

/**
 * @brief Automatically process a treebank collection.
 *
 * This function is an utility to process easily a collection of treebank files.
 * This function uses the class @ref lal::io::treebank_collection_processor in
 * order to process such a collection, with all its options set to their default
 * value. The default options are:
 * - All features in @ref lal::io::treebank_feature are computed,
 * - All files produced are joined into a single file,
 * - The individual files are deleted.
 * @param treebank_collection_main_file The main file of the treebank collection.
 * @param output_directory The output .
 * @param num_threads The number of threads.
 * @returns A treebank error (see @ref lal::io::treebank_error) if any.
 */
inline
treebank_error process_treebank_collection(
	const std::string& treebank_collection_main_file,
	const std::string& output_directory,
	std::size_t num_threads = 1
)
noexcept
{
	treebank_collection_processor tbcolproc;
	auto err = tbcolproc.init(treebank_collection_main_file, output_directory);
	tbcolproc.set_number_threads(num_threads);
	if (err != treebank_error_type::no_error) {
		return err;
	}
	return tbcolproc.process();
}

} // -- namespace io
} // -- namespace lal
