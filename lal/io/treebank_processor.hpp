/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
#include <vector>

namespace lal {
namespace io {

/**
 * @brief Treebank dataset processor.
 *
 * This class, the objects of which will be referred to as the "processor", is
 * an interface for processing a whole treebank dataset. This class offers an
 * automatic way of processing a corpora of treebanks by computing a series of
 * values, available within this library, automatically and outputting all these
 * values into a file, which can be read the same way a csv file can be read.
 *
 * Assume a treebank dataset is a set of files, each containing several syntactic
 * dependency trees of a language. Each file is referenced inside a "main file list",
 * henceforth called the main file. The main file indicates, for each language,
 * its file with the syntactic dependency trees. For example, the main file
 * \a stanford.txt could contain:
 *
 *		arabic stanford/stanford-ar-all.heads2
 *		eus stanford/stanford-eu-all.heads2
 *		ben stanford/stanford-bn-all.heads2
 *		...
 *
 * This processor works as follows: the user has to give the directory
 * in which the main file is located at (henceforth, the parent directory),
 * and the name of this file, without the leading path. For example, to read
 * the dataset \a Documents/datasets/stanford.txt the processor has to be
 * initialised with the parent directory \a Documents/datasets and the
 * main file \a stanford.txt.
 *
 * It is important to notice that the files referenced inside the main
 * must be done so with paths relative to the parent directory. Using the
 * previous example, we could have the following file directory structure:
 *
 *		Documents/
 *		| ...
 *		| datasets/
 *			| ...
 *			| stanford.txt
 *			| stanford/
 *				| stanford-ar-all.heads2
 *				| stanford-eu-all.heads2
 *				| stanford-bn-all.heads2
 *				| ...
 *
 * Therefore, if \a D is the parent directory, \a F is the main file then
 * all the files containing the syntactic dependency trees \a T in the main
 * file \a F must be reachable by concatenating \a D and \a T.
 *
 * The processor will read every language's tree bank, indicated in the main
 * file, and process all trees found. For each language, the processor will
 * compute a list of features (fully described in @ref tree_feature) of every
 * tree in the order that the user provided them via method @ref add_feature.
 * The class will produce an output file for each language inside an output
 * directory, provided by the user. These files will contain as many rows as
 * there are trees in the corresponding tree banks, with one optional row at
 * the begining of the file: the header.
 *
 * The list of features to be computed is, by default, empty, unless stated
 * otherwise in the constructor methods, or in the @ref init.
 *
 * Each output file will be stored in the output directory, which must exist
 * prior to processing the dataset, and they will be named after the language
 * the treebank belongs to.
 *
 * The output files are organised in columns, each corresponding to a feature.
 * The columns will be separated by either a default tab character '\\t', or by
 * any other character chosen by the user. Also, the user may choose not to
 * have a header output at the begining of each file.
 *
 * In order to produce an output, use method @ref process.
 */
class treebank_processor {
	public:
		/**
		 * @brief Features to be output.
		 *
		 * Features that can be computed for each tree.
		 */
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
			/// Parent directory was not given.
			missing_parent,
			/// Main file was not given.
			missing_main,
			/// Output directory was not given.
			missing_output,
			/// Parent directory could not be found.
			no_parent,
			/// Main file could not be found.
			no_main,
			/// Output directory could not be found.
			no_output,
			/// One of the treebank files could not be found.
			no_treebank,
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
		 * @param pdir Parent directory.
		 * @param file Main file's name.
		 * @param odir Output directory.
		 * @param all_fs Should the feature list contain all possible features?
		 */
		treebank_processor
		(const std::string& pdir, const std::string& file,
		 const std::string& odir, bool all_fs = false);
		/// Destructor.
		~treebank_processor();

		/**
		 * @brief Initialise the processor with a new dataset.
		 *
		 * If the parameter @e all_fs is true, the list is initialised with all
		 * features possible.
		 * @param pdir Parent directory.
		 * @param file Main file.
		 * @param odir Output directory.
		 * @param all_fs Should the feature list contain all possible features?
		 */
		void init
		(const std::string& pdir, const std::string& file,
		 const std::string& odir, bool all_fs = false);

		/**
		 * @brief Add another feature to be computed.
		 * @param fs Feature to be added.
		 */
		void add_feature(const tree_feature& fs);

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
		/// Parent directory.
		std::string m_par_dir;
		/// Output directory.
		std::string m_out_dir;
		/// File containing the list of languages and their treebanks.
		std::string m_main_list;

		/// The list of features to be computed.
		std::vector<bool> m_what_fs;
};

} // -- namespace io
} // -- namespace lal
