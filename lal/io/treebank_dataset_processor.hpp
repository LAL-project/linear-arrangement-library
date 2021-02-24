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
#include <lal/io/dataset_error.hpp>

namespace lal {
namespace io {

/**
 * @brief Treebank dataset processor.
 *
 * This class, the objects of which will be referred to as the "processors", has
 * the goal to ease the processing a whole treebank dataset and produce data for
 * a fixed set of features. This is most useful when the features to be calculated
 * are only those available in the library. See the enumeration @ref tree_feature
 * for details on the features available. Since not all features available have
 * to be computed, this class offers the possibility to compute only a subset of
 * them. Each feature can be set to be calculated via method @ref add_feature.
 *
 * A treebank dataset is made up of a set of files, each containing several
 * syntactic dependency trees of sentences. Each file is referred to as a treebank
 * file. Each of these files is referenced within a "main file list", henceforth
 * called the "main file". The main file is a two-column formatted file, the first
 * of which contains a self-descriptive name of the treebank that is indicated in
 * the next column.
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
 * Every processor must be initialised prior to processing the files. This is done
 * via method @ref init, which requires the path to the main file and the output
 * directory where the results are going to be stored. Besides these, it also
 * requires a Boolean value indicating whether all (or none) of the features should
 * be used. Moreover, it also admits an optional parameter indicating the number
 * of threads to be used to parallelise the processing of the files. Finally,
 * the treebank dataset is processed via method @ref process which returns a
 * value of the enumeration @ref processor_error. Further errors can be checked
 * via methods @ref get_num_processor_errors, @ref get_error_type,
 * @ref get_error_treebank_filename, @ref get_error_treebank_name.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_dataset_reader. See code for details.
 * @code
 *		treebank_processor tbproc;
 *		// initialise the processor without features (remmeber to check for errors)
 *      // and 4 threads for faster processing.
 *		tbproc.init(main_file, output_dir, false, 4);
 *		tbproc.add_feature(treebank_processor::tree_feature::C);
 *		tbproc.add_feature(treebank_processor::tree_feature::D_var);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_dataset_processor {
public:
	/// Features that can be computed for each tree.
	enum class tree_feature {
		/// Number of nodes of the tree.
		n = 0,
		/**
		 * @brief Second moment of degree \f$\langle k^2 \rangle\f$.
		 *
		 * See @ref properties::mmt_degree for details.
		 */
		k2,
		/**
		 * @brief Third moment of degree \f$\langle k^3 \rangle\f$.
		 *
		 * See @ref properties::mmt_degree for details.
		 */
		k3,
		/**
		 * @brief Size of the set \f$Q(T)\f$ of this tree \f$T\f$.
		 *
		 * See @ref properties::size_Q for details.
		 */
		size_Q,
		/**
		 * @brief Headedness of the tree.
		 *
		 * See @ref linarr::headedness for details.
		 */
		headedness,
		/**
		 * @brief Mean hierarchical distance of the tree.
		 *
		 * See @ref properties::mean_hierarchical_distance for details.
		 */
		mean_hierarchical_distance,
		/**
		 * @brief Mean dependency distance of the tree.
		 *
		 * See @ref linarr::mean_dependency_distance for details.
		 */
		mean_dependency_distance,
		/**
		 * @brief Number of edge crossings \f$C\f$.
		 *
		 * See @ref linarr::algorithms_C for details.
		 */

		// C

		C,
		/**
		 * @brief First moment of expectation of \f$C\f$, \f$E[C]\f$.
		 *
		 * See @ref properties::expectation_C for details.
		 */
		C_exp_1,
		/**
		 * @brief Second moment of expectation of \f$C\f$, \f$E[C^2]\f$.
		 *
		 * This is calculated as \f$E[C^2]=V[C] + E[C]^2\f$. See
		 * @ref properties::variance_C_tree for details on how the variance
		 * of \f$C\f$, \f$V[C]\f$, is calculated.
		 */
		C_exp_2,
		/**
		 * @brief Variance of \f$C\f$, \f$V[C]\f$.
		 *
		 * @ref properties::variance_C_tree for details.
		 */
		C_var,
		/**
		 * @brief z-score of \f$C\f$, \f$\frac{C - E[C]}{\sqrt{V[C]}}\f$.
		 *
		 * See @ref properties::variance_C_tree for details on how the
		 * variance of \f$C\f$, \f$V[C]\f$, is calculated.
		 */
		C_z,
		/**
		 * @brief Sum of length of edges \f$D\f$.
		 *
		 * See @ref linarr::sum_length_edges for details.
		 */

		// D

		D,
		/**
		 * @brief First moment of expectation of \f$D\f$, \f$E[D]\f$.
		 *
		 * See @ref properties::expectation_D for details.
		 */
		D_exp_1,
		/**
		 * @brief Second moment of expectation of \f$D\f$, \f$E[D^2]\f$.
		 *
		 * This is calculated as \f$E[D^2]=V[D] + E[D]^2\f$. See
		 * @ref properties::variance_D for details on how the variance
		 * of \f$D\f$, \f$V[D]\f$, is calculated.
		 */
		D_exp_2,
		/**
		 * @brief Variance of \f$D\f$, \f$V[D]\f$.
		 *
		 * @ref properties::variance_D for details.
		 */
		D_var,
		/**
		 * @brief z-score of \f$D\f$, \f$\frac{D - E[D]}{\sqrt{V[D]}}\f$.
		 *
		 * See @ref properties::variance_D for details on how the
		 * variance of \f$D\f$, \f$V[D]\f$, is calculated.
		 */
		D_z,
		/**
		 * @brief Unconstrained minimum sum of length of edges.
		 *
		 * See @ref linarr::algorithms_Dmin::Unconstrained_YS, or
		 * @ref linarr::algorithms_Dmin::Unconstrained_FC for details.
		 */
		Dmin_Unconstrained,
		/**
		 * @brief Minimum sum of length of edges under the planary constraint.
		 *
		 * See @ref linarr::algorithms_Dmin::Planar for details.
		 */
		Dmin_Planar,
		/**
		 * @brief Minimum sum of length of edges under the planary constraint.
		 *
		 * See @ref linarr::algorithms_Dmin::Projective for details.
		 */
		Dmin_Projective,

		// FLUXES

		/**
		 * @brief Maximum flux weight.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_flux_weight,
		/**
		 * @brief Mean flux weight.
		 *
		 * This is the sum of weights averaged by the number of fluxes (the number
		 * of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of weight.
		 */
		mean_flux_weight,
		/**
		 * @brief Minimum flux weight.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_flux_weight,

		/**
		 * @brief Maximum left span.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_left_span,
		/**
		 * @brief Mean left span.
		 *
		 * This is the sum of left spans averaged by the number of fluxes (the
		 * number of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of left span.
		 */
		mean_left_span,
		/**
		 * @brief Minimum left span.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_left_span,

		/**
		 * @brief Maximum right span.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_right_span,
		/**
		 * @brief Mean right span.
		 *
		 * This is the sum of right spans averaged by the number of fluxes (the
		 * number of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of right span.
		 */
		mean_right_span,
		/**
		 * @brief Minimum right span.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_right_span,

		/**
		 * @brief Maximum flux size.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_size,
		/**
		 * @brief Mean flux size.
		 *
		 * This is the sum of flux sizes averaged by the number of fluxes (the
		 * number of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of flux size.
		 */
		mean_size,
		/**
		 * @brief Minimum flux size.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_size,

		/**
		 * @brief Maximum R/L ratio.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_RL_ratio,
		/**
		 * @brief Mean R/L ratio.
		 *
		 * This is the sum of R/L ratios averaged by the number of fluxes (the
		 * number of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of R/L ratio.
		 */
		mean_RL_ratio,
		/**
		 * @brief Minimum R/L ratio.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_RL_ratio,

		/**
		 * @brief Maximum W/S ratio.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		max_WS_ratio,
		/**
		 * @brief Mean W/S ratio.
		 *
		 * This is the sum of W/S ratios averaged by the number of fluxes (the
		 * number of vertices of the tree minus 1). See @ref linarr::dependency_flux
		 * for details on the definition of W/S ratio.
		 */
		mean_WS_ratio,
		/**
		 * @brief Minimum W/S ratio.
		 *
		 * See @ref linarr::dependency_flux for details.
		 */
		min_WS_ratio,


		/**
		 * @brief The last value of the enumeration.
		 *
		 * This is used for internal purposes only.
		 */
		__last_value
	};

	/**
	 * @brief Possible errors that can arise while processing a dataset.
	 *
	 * There are several reasons why a dataset could not be processed.
	 * Because of this, the method @ref process will return a value
	 * describing what went wrong.
	 *
	 * Some of these errors are common to @ref dataset_error.
	 */
	enum class processor_error {
		/// The dataset was processed successfully.
		no_error,
		/// Main file could not be found.
		main_file_does_not_exist,
		/// Output directory could not be found.
		output_directory_does_not_exist,
		/// A treebank file could not be opened.
		treebank_file_could_not_be_opened,
		/// No features at all were given to the processor.
		no_features,
		/// Processing one or more of the treebanks failed.
		some_treebank_file_failed
	};

public:
	// MODIFIERS

	/**
	 * @brief Initialise the processor with a new dataset.
	 *
	 * If the parameter @e all_fs is true, the list is initialised with all
	 * features possible.
	 * @param file Main file.
	 * @param odir Output directory.
	 * @param all_fs Should the feature list contain all possible features?
	 * @param n_threads Number of threads to use in a parallel application.
	 */
	processor_error init(
		const std::string& file,
		const std::string& odir,
		bool all_fs,
		size_t n_threads = 1
	)
	noexcept;

	/**
	 * @brief Adds a feature to the processor.
	 * @param fs Feature to be added.
	 */
	void add_feature(const tree_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = true; }
	/**
	 * @brief Removes a feature from the processor.
	 * @param fs Feature to be removed.
	 */
	void remove_feature(const tree_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = false; }

	// SETTERS

	/**
	 * @brief Sets the separator character.
	 *
	 * Default is '\\t'.
	 * @param c The separator character.
	 */
	void set_separator(char c) noexcept { m_separator = c; }
	/**
	 * @brief Sets the level of verbosity of the @ref process method.
	 *
	 * Default is 0 (i.e., no verbosity at all).
	 * Verbosity is organised by levels:
	 * - Level 1: outputs progress messages.
	 * - Level 2: outputs error messages.
	 * @param k Verbosity level.
	 */
	void set_verbosity(int k) noexcept { m_be_verbose = k; }
	/**
	 * @brief Output a hedaer for each processed treebank file.
	 *
	 * Default is true.
	 * @param h Output header or not.
	 */
	void set_output_header(bool h) noexcept { m_output_header = h; }

	// GETTERS

	/// Returns the number of errors that arised during processing.
	size_t get_num_processor_errors() const noexcept
	{ return m_errors_from_processing.size(); }

	/// Get the @e ith error. This is a value of the enumeration @ref processor_error.
	processor_error get_error_type(size_t i) const noexcept
	{ return std::get<0>(m_errors_from_processing[i]); }

	/// Get the treebank's file name for which the @e ith error happened.
	const std::string& get_error_treebank_filename(size_t i) const noexcept
	{ return std::get<1>(m_errors_from_processing[i]); }

	/// Get the treebank's name for which the @e ith error happened.
	const std::string& get_error_treebank_name(size_t i) const noexcept
	{ return std::get<2>(m_errors_from_processing[i]); }

	// PROCESS THE TREEBANK DATASET

	/**
	 * @brief Process the dataset.
	 *
	 * This method produces the information as explained in this class'
	 * description. However, it may fail to do so. In this case it will return
	 * a value different from @ref processor_error::no_error.
	 *
	 * This function uses attributes @ref m_separator, @ref m_output_header to format
	 * the output data. It also outputs the current progress if @ref m_be_verbose
	 * is set to true.
	 *
	 * Moreover, it gathers the errors thay may have occurred during processing.
	 * If so, see methods @ref get_num_processor_errors, @ref get_error_type,
	 * @ref get_error_treebank_name.
	 * @return Returns a value describing the error (if any) that occurred
	 * while processing the dataset. If any error ocurred, see methods
	 */
	processor_error process() noexcept;

private:
	/// Character used as separator
	char m_separator = '\t';
	/// Output a header for each file
	bool m_output_header = true;
	/**
	 * @brief The verbosity of the processor.
	 *
	 * When set to true, method @ref process will output progress messages.
	 */
	int m_be_verbose = 0;

	/// Number of threads to use.
	size_t m_num_threads = 1;

	/// Set of errors resulting from processing the treebank dataset.
	std::vector<std::tuple<processor_error, std::string, std::string>>
	m_errors_from_processing;

	/// The number of total features available.
	static constexpr size_t NUM_TREE_FEATURES =
		static_cast<size_t>(tree_feature::__last_value);

private:
	/// Process a single tree in the treebank.
	template<class TREE, class OUT_STREAM>
	void process_tree(const TREE& rT, OUT_STREAM& out_lab_file) const;

	dataset_error process_treebank
	(const std::string& treebank_filename, const std::string& treebank_name)
	noexcept;

private:
	/// Output directory.
	std::string m_out_dir = "none";
	/// File containing the list of languages and their treebanks.
	std::string m_main_file = "none";
	
	/// The list of features to be computed.
	std::array<bool, NUM_TREE_FEATURES> m_what_fs;
};

} // -- namespace io
} // -- namespace lal
