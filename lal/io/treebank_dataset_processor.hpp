/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <string>
#include <array>

// lal includes
#include <lal/io/treebank_dataset_reader.hpp>

namespace lal {
namespace io {

/**
 * @brief Treebank dataset processor.
 *
 * This class, the objects of which will be referred to as the "processor", is
 * an interface for processing a whole treebank dataset and produce data for
 * a fixed set of features. It is meant to ease the processing of treebanks if
 * the features to be calculated are to be only those available in the library.
 * See the enumeration @ref tree_feature for details on the features available.
 * Each feature can be set to be calculated via method @ref add_feature. However,
 * the processor can be initialised with all features set (see @ref init), and
 * then have some of them removed via method @ref remove_feature.
 *
 * A treebank dataset is made up of a set of files, each containing several
 * syntactic dependency trees of sentences of the corresponding language.
 * Each file is referenced within a "main file list", henceforth called the
 * main file. The main file indicates, for each language, a file with the
 * syntactic dependency trees. For example, a main file \a stanford.txt
 * could contain:
 *
 *		arb path/to/file/ar-all.heads2
 *		eus path/to/file/eu-all.heads2
 *		ben path/to/file/bn-all.heads2
 *		...
 *
 * where the first column contains a string referencing the language (e.g., an
 * ISO code, or simply the name of the language), and the second column contains
 * the full path to the file with the syntactic dependency trees. The processor
 * has to be initialised with such file via method @ref init. Then, the treebank
 * is processed via method @ref process.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_dataset_reader. See code for details.
 * @code
 *		treebank_processor tbproc;
 *		// initialise the processor without features (check for errors)
 *		tbproc.init(main_file, output_dir, false);
 *		tbproc.add_feature(treebank_processor::tree_feature::C);
 *		tbproc.add_feature(treebank_processor::tree_feature::D_var);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_dataset_processor {
public:
	/// @brief Features that can be computed for each tree.
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
		Dmin_Projective
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
		none,
		/// Main file could not be found.
		main_file_does_not_exist,
		/// Output directory could not be found.
		output_directory_does_not_exist,
		/// A treebank file could not be opened.
		treebank_file_could_not_be_opened,
		/// No features at all were given to the processor.
		no_features
	};

public:
	/**
	 * @brief Initialise the processor with a new dataset.
	 *
	 * If the parameter @e all_fs is true, the list is initialised with all
	 * features possible.
	 * @param file Main file.
	 * @param odir Output directory.
	 * @param all_fs Should the feature list contain all possible features?
	 */
	treebank_dataset_processor::processor_error init
	(const std::string& file, const std::string& odir, bool all_fs);

	/**
	 * @brief Adds a feature to the processor.
	 * @param fs Feature to be added.
	 */
	void add_feature(const tree_feature& fs);
	/**
	 * @brief Removes a feature from the processor.
	 * @param fs Feature to be removed.
	 */
	void remove_feature(const tree_feature& fs);

	/**
	 * @brief Process the dataset.
	 *
	 * This method outputs the information as explained in this class'
	 * description.
	 *
	 * However, it may fail to do so. In this case it will return a value
	 * different from @ref processor_error::none.
	 * @param sep Separator character.
	 * @param header Should a header be written?
	 * @param verbose Output progress on standard output.
	 * @return Returns a value describing the error (if any) that occurred
	 * while processing the dataset.
	 */
	processor_error process
	(char sep = '\t', bool header = true, bool verbose = false);

private:
	/// The number of total features available.
	static constexpr size_t NUM_TREE_FEATURES =
		static_cast<size_t>(tree_feature::Dmin_Projective) + 1;

private:

	/// Process a single tree in the treebank.
	template<class TREE, class OUT_STREAM>
	void process_tree(
		char sep, const TREE& rT, OUT_STREAM& out_lab_file
	) const;

private:
	/// Output directory.
	std::string m_out_dir = "none";
	/// File containing the list of languages and their treebanks.
	std::string m_main_list = "none";
	
	/// The list of features to be computed.
	std::array<bool, NUM_TREE_FEATURES> m_what_fs;

	/// The dataset reader used to process the trees.
	treebank_dataset_reader m_treebank_dataset_reader;
};

} // -- namespace io
} // -- namespace lal
