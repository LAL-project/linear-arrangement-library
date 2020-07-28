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
#include <string>
#include <vector>

namespace lal {
namespace io {

/**
 * @brief Treebank dataset processor.
 *
 * This class, the objects of which will be referred to as the "processor", is
 * an interface for processing a whole treebank dataset and produce data for
 * a fixed set of features. It is meant to ease processing treebanks if the
 * features to be calculated are considered in this class. See the enumeration
 * @ref tree_feature for details on the features available. Each feature
 * can be set to be calculated via method @ref add_feature. However, the processor
 * can be initialised with all features set (see @ref init), and then have some
 * of them removed via method @ref remove_feature.
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
 * The usage of this class is a lot simpler than that of class @ref treebank_dataset.
 * See code for details.
 * @code
 *		treebank_processor tbproc = treebank_processor();
 *		// initialise the processor without features
 *		tbproc.init(main_file, output_dir, false);
 *		tbproc.add_feature(treebank_processor::tree_feature::C);
 *		tbproc.add_feature(treebank_processor::tree_feature::D_var);
 *		// it is advisable to check for errors
 *		tbproc.process();
 * @endcode
 */
class treebank_processor {
	public:
		/// @brief Features that can be computed for each tree.
		enum class tree_feature {
			/// Number of nodes of the tree.
			n,
			/// Second moment of degree \f$\langle k^2 \rangle\f$.
			k2,
			/// Third moment of degree \f$\langle k^3 \rangle\f$.
			k3,
			/// Size of the set Q \f$|Q|\f$.
			size_Q,
			/// Number of crossings \f$C\f$.
			C,
			/// First moment of expectation of C \f$E[C]\f$.
			C_exp_1,
			/// Second moment of expectation of C \f$E[C^2]\f$.
			C_exp_2,
			/// Variance of C \f$V[C]\f$.
			C_var,
			/// z-score of C: \f$\frac{C - E[C]}{\sqrt{V[C]}}\f$.
			C_z,
			/// Sum of length of edges \f$D\f$.
			D,
			/// First moment of expectation of D \f$E[D]\f$.
			D_exp_1,
			/// Second moment of expectation of D \f$E[D^2]\f$.
			D_exp_2,
			/// Variance of D \f$V[D]\f$.
			D_var,
			/// z-score of D: \f$\frac{D - E[D]}{\sqrt{V[D]}}\f$.
			D_z
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
			/// Main file was not given.
			need_main_file,
			/// Output directory was not given.
			need_output_directory,
			/// Main file could not be found.
			missing_main_file,
			/// Output directory could not be found.
			missing_output_directory,
			/// One of the treebank files could not be found.
			missing_treebank_file,
			/// No features at all were given to the processor.
			no_features
		};

	public:
		/**
		 * @brief Default constructor
		 *
		 * If the parameter is true, the list is initialised with all features
		 * possible.
		 * @param all_fs Should the feature list contain all possible features?
		 */
		treebank_processor(bool all_fs = false);
		/**
		 * @brief Constructor with directories.
		 *
		 * If the parameter is true, the list is initialised with all
		 * features possible.
		 * @param file Main file's name.
		 * @param odir Output directory.
		 * @param all_fs Should the feature list contain all possible features?
		 */
		treebank_processor
		(const std::string& file, const std::string& odir, bool all_fs = false);
		/// Destructor.
		~treebank_processor();

		/**
		 * @brief Initialise the processor with a new dataset.
		 *
		 * If the parameter @e all_fs is true, the list is initialised with all
		 * features possible.
		 * @param file Main file.
		 * @param odir Output directory.
		 * @param all_fs Should the feature list contain all possible features?
		 */
		void init
		(const std::string& file, const std::string& odir, bool all_fs = false);

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
		 * @param v Output progress on standard output.
		 * @return Returns a value describing the error (if any) that occurred
		 * while processing the dataset.
		 */
		processor_error process
		(char sep = '\t', bool header = true, bool v = false) const;

	private:
		/// Output directory.
		std::string m_out_dir;
		/// File containing the list of languages and their treebanks.
		std::string m_main_list;

		/// The list of features to be computed.
		std::vector<bool> m_what_fs;
};

} // -- namespace io
} // -- namespace lal
