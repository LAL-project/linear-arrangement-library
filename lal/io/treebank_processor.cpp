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
 
#include <lal/io/treebank_processor.hpp>

// C includes
#include <sys/types.h>
#include <sys/stat.h>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <string_view>
#include <iostream>
#include <numeric>
#include <cmath>
using namespace std;

// lal includes
#include <lal/io/treebank_dataset.hpp>
#include <lal/io/treebank_reader.hpp>
#include <lal/definitions.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/headedness.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>

#define to_double(x) static_cast<double>(x)
#define square(x) ((x)*(x))

namespace lal {
using namespace graphs;

namespace io {

inline
constexpr string_view tree_feature_string(const treebank_processor::tree_feature& tf) {
	switch (tf) {
	case treebank_processor::tree_feature::n: return "n";
	case treebank_processor::tree_feature::k2: return "k2";
	case treebank_processor::tree_feature::k3: return "k3";
	case treebank_processor::tree_feature::size_Q: return "size_Q";
	case treebank_processor::tree_feature::headedness: return "headedness";
	case treebank_processor::tree_feature::mean_hierarchical_distance: return "mean_hierarchical_distance";
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
	case treebank_processor::tree_feature::Dmin_Unconstrained: return "Dmin_Unconstrained";
	case treebank_processor::tree_feature::Dmin_Planar: return "Dmin_Planar";
	case treebank_processor::tree_feature::Dmin_Projective: return "Dmin_Projective";
	}
	// should never happen
	return "???";
}

inline
constexpr string_view processor_error_string(const treebank_processor::processor_error& pe) {
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

#define n_idx enum2index(treebank_processor::tree_feature::n)
#define k2_idx enum2index(treebank_processor::tree_feature::k2)
#define k3_idx enum2index(treebank_processor::tree_feature::k3)
#define size_Q_idx enum2index(treebank_processor::tree_feature::size_Q)
#define headedness_idx enum2index(treebank_processor::tree_feature::headedness)
#define mean_hierarchical_distance_idx enum2index(treebank_processor::tree_feature::mean_hierarchical_distance)
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
#define Dmin_Unconstrained_idx enum2index(treebank_processor::tree_feature::Dmin_Unconstrained)
#define Dmin_Planar_idx enum2index(treebank_processor::tree_feature::Dmin_Planar)
#define Dmin_Projective_idx enum2index(treebank_processor::tree_feature::Dmin_Projective)

// CLASS METHODS

// PUBLIC

treebank_processor::treebank_processor(bool all_fs) {
	m_out_dir = "none";
	m_main_list = "none";

	std::fill(m_what_fs.begin(), m_what_fs.end(), all_fs);
}

treebank_processor::treebank_processor
(const string& file, const string& odir, bool all_fs)
{
	m_main_list = file;
	m_out_dir = odir;

	std::fill(m_what_fs.begin(), m_what_fs.end(), all_fs);
}

void treebank_processor::init
(const string& file, const string& odir, bool all_fs)
{
	m_main_list = file;
	m_out_dir = odir;
	
	std::fill(m_what_fs.begin(), m_what_fs.end(), all_fs);
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

		rooted_tree rT;
		// process the current treebank
		while (tbread.has_tree()) {
			dataset_error err = tbread.next_tree();
			if (err == dataset_error::empty_line) {
				// empty line, skip...
			}
			else {
				rT = tbread.get_tree();
				process_tree<rooted_tree, ofstream>(
					sep, rT, out_lang_file
				);
			}
		}

		if (v) {
			cout << "    processed "
				 << tbread.get_num_trees() << " trees" << endl;
		}
	}

	return processor_error::none;
}

// PRIVATE

template<class TREE, class OUT_STREAM>
void treebank_processor::process_tree(
	char sep, const TREE& rT, OUT_STREAM& out_lang_file
)
const
{
	const free_tree fT = rT.to_undirected();

	// -----------------------------------------------------------
	// compute features in a way that does not repeat computations
	double props[NUM_TREE_FEATURES]{0.0};
	bool prop_set[NUM_TREE_FEATURES]{false};

	const auto set_prop =
	[&](size_t idx, double val) -> void {
		props[idx] = val;
		prop_set[idx] = true;
	};

	// <k^2>, <k^3>, |Q|, headedness
	if (m_what_fs[k2_idx]) {
		set_prop(k2_idx, properties::mmt_degree(fT, 2));
	}
	if (m_what_fs[k3_idx]) {
		set_prop(k3_idx, properties::mmt_degree(fT, 3));
	}
	if (m_what_fs[size_Q_idx]) {
		set_prop(size_Q_idx, static_cast<double>(properties::size_Q(fT)));
	}
	if (m_what_fs[headedness_idx]) {
		set_prop(headedness_idx, linarr::headedness(rT));
	}
	if (m_what_fs[mean_hierarchical_distance_idx]) {
		set_prop(mean_hierarchical_distance_idx, properties::mean_hierarchical_distance(rT));
	}

	// -----------------------------------------------------------------
	// C
	
	if (m_what_fs[C_idx]) {
		set_prop(C_idx, linarr::n_crossings(fT));
	}
	if (m_what_fs[C_var_idx]) {
		set_prop(C_var_idx, properties::variance_C_tree(fT));
	}
	if (m_what_fs[C_exp1_idx]) {
		set_prop(C_exp1_idx, properties::expectation_C(fT));
	}

	if (m_what_fs[C_exp2_idx]) {
		if (not m_what_fs[C_exp1_idx]) {
			set_prop(C_exp1_idx, properties::expectation_C(fT));
		}
		if (not m_what_fs[C_var_idx]) {
			set_prop(C_var_idx, properties::variance_C_tree(fT));
		}

#if defined DEBUG
		assert(prop_set[C_exp1_idx]);
		assert(prop_set[C_var_idx]);
#endif
		set_prop(C_exp2_idx, props[C_var_idx] + square(props[C_exp1_idx]));
	}

	// z-score of C
	if (m_what_fs[C_z_idx]) {
		// we need C
		if (not m_what_fs[C_idx]) {
			set_prop(C_idx, linarr::n_crossings(fT));
		}
		// we need V[C]
		if (not m_what_fs[C_var_idx]) {
			set_prop(C_var_idx, properties::variance_C_tree(fT));
		}
		// we need E[C]
		if (not m_what_fs[C_exp1_idx]) {
			set_prop(C_exp1_idx, properties::expectation_C(fT));
		}
		
#if defined DEBUG
		assert(prop_set[C_idx]);
		assert(prop_set[C_var_idx]);
		assert(prop_set[C_exp1_idx]);
#endif
		set_prop(C_z_idx,
			(props[C_idx] - props[C_exp1_idx])/std::sqrt(props[C_var_idx])
		);
	}

	// -----------------------------------------------------------------
	// D
	
	if (m_what_fs[D_idx]) {
		set_prop(D_idx, linarr::sum_length_edges(fT));
	}
	if (m_what_fs[D_var_idx]) {
		set_prop(D_var_idx, properties::variance_D(fT));
	}
	if (m_what_fs[D_exp1_idx]) {
		set_prop(D_exp1_idx, properties::expectation_D(fT));
	}
	
	if (m_what_fs[D_exp2_idx]) {
		if (not m_what_fs[D_exp1_idx]) {
			set_prop(D_exp1_idx, properties::expectation_D(fT));
		}
		if (not m_what_fs[D_var_idx]) {
			set_prop(D_var_idx, properties::variance_D(fT));
		}

#if defined DEBUG
		assert(prop_set[D_exp1_idx]);
		assert(prop_set[D_var_idx]);
#endif
		set_prop(D_exp2_idx, props[D_var_idx] + square(props[D_exp1_idx]));
	}

	// z-score of D
	if (m_what_fs[D_z_idx]) {
		// we need D
		if (not m_what_fs[D_idx]) {
			set_prop(D_idx, linarr::sum_length_edges(fT));
		}
		// we need V[D]
		if (not m_what_fs[D_var_idx]) {
			set_prop(D_var_idx, properties::variance_D(fT));
		}
		// we need E[D]
		if (not m_what_fs[D_exp1_idx]) {
			set_prop(D_exp1_idx, properties::expectation_D(fT));
		}
		
#if defined DEBUG
		assert(prop_set[D_idx]);
		assert(prop_set[D_var_idx]);
		assert(prop_set[D_exp1_idx]);
#endif
		set_prop(D_z_idx,
			(props[D_idx] - props[D_exp1_idx])/std::sqrt(props[D_var_idx])
		);
	}

	// -----------------
	// Optimisation of D
	if (m_what_fs[Dmin_Unconstrained_idx]) {
		set_prop(Dmin_Unconstrained_idx, linarr::Dmin(fT, linarr::algorithms_Dmin::Unconstrained_YS).first);
	}
	if (m_what_fs[Dmin_Planar_idx]) {
		set_prop(Dmin_Planar_idx, linarr::Dmin(fT, linarr::algorithms_Dmin::Planar).first);
	}
	if (m_what_fs[Dmin_Projective_idx]) {
		set_prop(Dmin_Projective_idx, linarr::Dmin(rT, linarr::algorithms_Dmin::Projective).first);
	}

	// ---------------
	// output features
	if (m_what_fs[0]) {
		out_lang_file << rT.n_nodes();
	}
	for (size_t i = 1; i < m_what_fs.size(); ++i) {
		out_lang_file << sep;
		if (m_what_fs[i]) {
			out_lang_file << props[i];
		}
	}
	out_lang_file << "\n";
}

} // -- namespace io
} // -- namespace lal
