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
 
#include <lal/io/treebank_processor.hpp>

// C includes
#include <sys/types.h>
#include <sys/stat.h>

// C++ includes
#include <iostream>
#include <cstring>
#include <numeric>
#include <cmath>
using namespace std;

// lal includes
#include <lal/io/treebank_dataset.hpp>
#include <lal/io/treebank_reader.hpp>
#include <lal/definitions.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/D.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>

#define NUM_TREE_FEATURES 14
#define to_double(x) static_cast<double>(x)

namespace lal {
using namespace graphs;

namespace io {

inline string tree_feature_string(const treebank_processor::tree_feature& tf) {
	switch (tf) {
	case treebank_processor::tree_feature::n: return "n";
	case treebank_processor::tree_feature::k2: return "k2";
	case treebank_processor::tree_feature::k3: return "k3";
	case treebank_processor::tree_feature::size_Q: return "size_Q";
	case treebank_processor::tree_feature::C: return "C";
	case treebank_processor::tree_feature::C_exp_1: return "C_exp_1";
	case treebank_processor::tree_feature::C_exp_2: return "C_exp_2";
	case treebank_processor::tree_feature::C_var: return "C_var";
	case treebank_processor::tree_feature::C_z: return "C_z";
	case treebank_processor::tree_feature::D: return "D";
	case treebank_processor::tree_feature::D_exp_1: return "D_exp_1";
	case treebank_processor::tree_feature::D_exp_2: return "D_exp_2";
	case treebank_processor::tree_feature::D_var: return "D_var";
	case treebank_processor::tree_feature::D_z: return "D_z";
	}
	// should never happen
	return "???";
}

inline string processor_error_string(const treebank_processor::processor_error& pe) {
	switch (pe) {
	case treebank_processor::processor_error::none: return "No error";
	case treebank_processor::processor_error::need_main_file: return "Main file not given";
	case treebank_processor::processor_error::need_output_directory: return "Output directory not given";
	case treebank_processor::processor_error::missing_main_file: return "Main file not found";
	case treebank_processor::processor_error::missing_output_directory: return "Output directory not found";
	case treebank_processor::processor_error::missing_treebank_file: return "Some tree bank file could not be found";
	case treebank_processor::processor_error::no_features: return "No features to be output";
	}
	// should never happen
	return "Wrong value for process_error parameter";
}

inline size_t enum2index(const treebank_processor::tree_feature& tf) {
	switch (tf) {
	case treebank_processor::tree_feature::n: return 0;
	case treebank_processor::tree_feature::k2: return 1;
	case treebank_processor::tree_feature::k3: return 2;
	case treebank_processor::tree_feature::size_Q: return 3;
	case treebank_processor::tree_feature::C: return 4;
	case treebank_processor::tree_feature::C_exp_1: return 5;
	case treebank_processor::tree_feature::C_exp_2: return 6;
	case treebank_processor::tree_feature::C_var: return 7;
	case treebank_processor::tree_feature::C_z: return 8;
	case treebank_processor::tree_feature::D: return 9;
	case treebank_processor::tree_feature::D_exp_1: return 10;
	case treebank_processor::tree_feature::D_exp_2: return 11;
	case treebank_processor::tree_feature::D_var: return 12;
	case treebank_processor::tree_feature::D_z: return 13;
	}
	// should never happen
	return NUM_TREE_FEATURES;
}

inline treebank_processor::tree_feature index2enum(size_t i) {
	switch (i) {
	case 0: return treebank_processor::tree_feature::n;
	case 1: return treebank_processor::tree_feature::k2;
	case 2: return treebank_processor::tree_feature::k3;
	case 3: return treebank_processor::tree_feature::size_Q;
	case 4: return treebank_processor::tree_feature::C;
	case 5: return treebank_processor::tree_feature::C_exp_1;
	case 6: return treebank_processor::tree_feature::C_exp_2;
	case 7: return treebank_processor::tree_feature::C_var;
	case 8: return treebank_processor::tree_feature::C_z;
	case 9: return treebank_processor::tree_feature::D;
	case 10: return treebank_processor::tree_feature::D_exp_1;
	case 11: return treebank_processor::tree_feature::D_exp_2;
	case 12: return treebank_processor::tree_feature::D_var;
	case 13: return treebank_processor::tree_feature::D_z;
	}
	// should never happen
	cerr << "=========================================================" << endl;
	cerr << "LAL internal error:" << endl;
	cerr << "    File: 'treebank_processor.cpp', line: " << __LINE__ << "'" << endl;
	cerr << "    Function: 'treebank_processor::tree_feature index2enum(size_t i)'" << endl;
	cerr << "    Value of parameter 'i' is: " << i << endl;
	cerr << "    This is an invalid value." << endl;
	cerr << "    The value should be less than " << NUM_TREE_FEATURES << endl;
	cerr << "=========================================================" << endl;
	return treebank_processor::tree_feature::D_z;
}

#define n_idx enum2index(treebank_processor::tree_feature::n)
#define k2_idx enum2index(treebank_processor::tree_feature::k2)
#define k3_idx enum2index(treebank_processor::tree_feature::k3)
#define size_Q_idx enum2index(treebank_processor::tree_feature::size_Q)
#define C_idx enum2index(treebank_processor::tree_feature::C)
#define C_exp1_idx enum2index(treebank_processor::tree_feature::C_exp_1)
#define C_exp2_idx enum2index(treebank_processor::tree_feature::C_exp_2)
#define C_var_idx enum2index(treebank_processor::tree_feature::C_var)
#define C_z_idx enum2index(treebank_processor::tree_feature::C_z)
#define D_idx enum2index(treebank_processor::tree_feature::D)
#define D_exp1_idx enum2index(treebank_processor::tree_feature::D_exp_1)
#define D_exp2_idx enum2index(treebank_processor::tree_feature::D_exp_2)
#define D_var_idx enum2index(treebank_processor::tree_feature::D_var)
#define D_z_idx enum2index(treebank_processor::tree_feature::D_z)

// CLASS METHODS

// PRIVATE

inline void process_tree(
	const vector<bool>& what_fs, char sep,
	const rtree& rT, ofstream& out_lang_file
)
{
	const ftree t = rT.to_undirected();

	// default linear arrangement
	vector<node> __linarr(rT.n_nodes());
	iota(__linarr.begin(), __linarr.end(), 0);

	// -----------------------------------------------------------
	// compute features in a way that does not repeat computations
	double props[NUM_TREE_FEATURES];

	// <k^2>, <k^3>, |Q|
	if (what_fs[k2_idx]) {
		props[k2_idx] = properties::mmt_degree(t, 2);
	}
	if (what_fs[k3_idx]) {
		props[k3_idx] = properties::mmt_degree(t, 3);
	}
	if (what_fs[size_Q_idx]) {
		props[size_Q_idx] = static_cast<double>(properties::size_Q(t));
	}

	// ------------
	// V[C], E[C^2]
	if (what_fs[C_var_idx] or what_fs[C_exp2_idx]) {
		props[C_exp1_idx] = properties::expectation_C(t);
		props[C_var_idx] = properties::variance_C_tree(t);
		props[C_exp2_idx] = props[C_var_idx] + props[C_exp1_idx]*props[C_exp1_idx];
	}
	else {
		// none of V[C], E[C^2] were asked, but if...
		if (what_fs[C_exp1_idx]) {
			props[C_exp1_idx] = properties::expectation_C(t);
		}
	}

	// z-score of C
	if (what_fs[C_z_idx]) {
		// we need V[C]
		if (not what_fs[C_var_idx]) {
			props[C_var_idx] = properties::variance_C_tree(t);
		}
		// we need E[C]
		if (not what_fs[C_exp1_idx] and not what_fs[C_var_idx]) {
			props[C_exp1_idx] = properties::expectation_C(t);
		}
		// we need to compute C, whether we like it or not
		props[C_idx] = to_double(linarr::__n_crossings_ladder(t, __linarr));
		props[C_z_idx] = (props[C_idx] - props[C_exp1_idx])/std::sqrt(props[C_var_idx]);
	}
	else {
		if (what_fs[C_idx]) {
			props[C_idx] = to_double(linarr::__n_crossings_ladder(t, __linarr));
		}
	}

	// ------------
	// V[D], E[D^2]
	if (what_fs[D_var_idx] or what_fs[D_exp2_idx]) {
		props[D_exp1_idx] = properties::expectation_D(t);
		props[D_var_idx] = properties::variance_D(t);
		props[D_exp2_idx] = props[D_var_idx] + props[D_exp1_idx]*props[D_exp1_idx];
	}
	else {
		// none of V[D], E[D^2] were asked, but if...
		if (what_fs[D_exp1_idx]) {
			props[D_exp1_idx] = properties::expectation_D(t);
		}
	}

	// z-score of D
	if (what_fs[D_z_idx]) {
		// we need V[D]
		if (not what_fs[D_var_idx]) {
			props[D_var_idx] = properties::variance_D(t);
		}
		// we need E[D]
		if (not what_fs[D_exp1_idx] and not what_fs[D_var_idx]) {
			props[D_exp1_idx] = properties::expectation_D(t);
		}
		// we need to compute D, whether we like it or not
		props[D_idx] = to_double(linarr::sum_length_edges(rT, __linarr));
		props[D_z_idx] = (props[D_idx] - props[D_exp1_idx])/std::sqrt(props[D_var_idx]);
	}
	else {
		if (what_fs[D_idx]) {
			props[D_idx] = to_double(linarr::sum_length_edges(rT, __linarr));
		}
	}

	// ---------------
	// output features
	if (what_fs[0]) {
		out_lang_file << rT.n_nodes();
	}
	for (size_t i = 1; i < what_fs.size(); ++i) {
		if (what_fs[i]) {
			out_lang_file << sep << props[i];
		}
	}
	out_lang_file << endl;
}

// PUBLIC

treebank_processor::treebank_processor(bool all_fs) {
	m_out_dir = "none";
	m_main_list = "none";

	m_what_fs = vector<bool>(NUM_TREE_FEATURES, all_fs);
}

treebank_processor::treebank_processor
(const string& file, const string& odir, bool all_fs)
{
	m_main_list = file;
	m_out_dir = odir;

	m_what_fs = vector<bool>(NUM_TREE_FEATURES, all_fs);
}

treebank_processor::~treebank_processor() {}

void treebank_processor::init
(const string& file, const string& odir, bool all_fs)
{
	m_main_list = file;
	m_out_dir = odir;

	m_what_fs = vector<bool>(NUM_TREE_FEATURES, all_fs);
}

void treebank_processor::add_feature(const tree_feature& fs) {
	m_what_fs[ enum2index(fs) ] = true;
}
void treebank_processor::remove_feature(const tree_feature& fs) {
	m_what_fs[ enum2index(fs) ] = false;
}

treebank_processor::processor_error treebank_processor::process
(char sep, bool header, bool v) const
{
	// first, check that there is something to be computed
	if (m_what_fs.size() == 0) {
		return processor_error::no_features;
	}

	// second, check that the reader was initialised
	if (m_main_list == "none") {
		return processor_error::need_main_file;
	}
	if (m_out_dir == "none") {
		return processor_error::need_output_directory;
	}

	treebank_dataset tbds;
	dataset_error dserr = tbds.init(m_main_list);

	if (dserr == dataset_error::missing_main_file) {
		return processor_error::missing_main_file;
	}

	// make sure output directory exists
	struct stat info;
	if (m_out_dir != "." and stat(m_out_dir.c_str(), &info) != 0) {
		return processor_error::missing_output_directory;
	}

	// process dataset using treebank_dataset class
	while (tbds.has_language()) {
		dserr = tbds.next_language();
		if (dserr == dataset_error::missing_treebank_file) {
			return processor_error::missing_treebank_file;
		}

		// iterate to next language
		treebank_reader& tbread = tbds.get_treebank_reader();

		const string lang = tbread.what_language();
		if (v) {
			cout << "Processing language: " << lang
				 << " (file: '" << tbread.get_treebank_filename()
				 << "')" << endl;
		}

		const string full_out_file = m_out_dir + "/" + lang + ".txt";

		// output file stream
		ofstream out_lang_file;
		out_lang_file.open(full_out_file);
		// since the output directory exists there is no
		// need to check for is_open()

		// output header
		if (header) {
			if (m_what_fs[0]) {
				out_lang_file << tree_feature_string(index2enum(0));
			}
			for (size_t i = 1; i < m_what_fs.size(); ++i) {
				if (m_what_fs[i]) {
					out_lang_file << sep << tree_feature_string(index2enum(i));
				}
			}
			out_lang_file << endl;
		}

		rtree rT;
		// process the current treebank
		while (tbread.has_tree()) {
			dataset_error err = tbread.next_tree();
			if (err == dataset_error::empty_line) {
				// empty line, skip...
			}
			else {
				rT = tbread.get_tree();
				process_tree(m_what_fs, sep, rT, out_lang_file);
			}
		}

		if (v) {
			cout << "    processed "
				 << tbread.get_num_trees() << " trees" << endl;
		}
	}

	return processor_error::none;
}

} // -- namespace io
} // -- namespace lal
