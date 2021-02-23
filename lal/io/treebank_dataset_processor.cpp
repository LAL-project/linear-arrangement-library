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
 
#include <lal/io/treebank_dataset_processor.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <string_view>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <cmath>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/io/treebank_dataset_reader.hpp>
#include <lal/io/treebank_reader.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/headedness.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/linarr/flux.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>

template<typename T>
double to_double(const T& x) {
	if constexpr (std::is_same_v<T, double>) { return x; }
	else { return static_cast<double>(x); }
}
#define square(x) ((x)*(x))

namespace lal {
using namespace graphs;

template<typename T>
void set_average_of(
	const vector<linarr::dependency_flux>& F,
	const size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
{
	const double cumul =
	[&]() -> double {
		double v =
		std::accumulate(
			F.begin(), F.end(),
			0.0, [&](double x, const linarr::dependency_flux& f) {
				return x + to_double( (f.*FUNC)() );
			}
		);
		return v;
	}();
	props[idx] = cumul/to_double(F.size());
}

template<typename T>
void set_maximum_of(
	const vector<linarr::dependency_flux>& F,
	const size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
{
	const double value =
	[&]() -> double {
		double v = 0.0;
		std::for_each(
			F.begin(), F.end(),
			[&](const linarr::dependency_flux& f) {
				v = std::max(v, to_double( (f.*FUNC)() ));
			}
		);
		return v;
	}();
	props[idx] = value;
}
template<typename T>
void set_minimum_of(
	const vector<linarr::dependency_flux>& F,
	const size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
{
	const double value =
	[&]() -> double {
		double v = 9999999.9;
		std::for_each(
			F.begin(), F.end(),
			[&](const linarr::dependency_flux& f) {
				v = std::min(v, to_double( (f.*FUNC)() ));
			}
		);
		return v;
	}();
	props[idx] = value;
}

namespace io {

#define TBPROC_TF treebank_dataset_processor::tree_feature
#define TBPROC_PE treebank_dataset_processor::processor_error

inline constexpr string_view
tree_feature_string(const treebank_dataset_processor::tree_feature& tf) {
	switch (tf) {
	case TBPROC_TF::n: return "n";
	case TBPROC_TF::k2: return "k2";
	case TBPROC_TF::k3: return "k3";
	case TBPROC_TF::size_Q: return "size_Q";
	case TBPROC_TF::headedness: return "headedness";
	case TBPROC_TF::mean_hierarchical_distance: return "mean_hierarchical_distance";
	case TBPROC_TF::mean_dependency_distance: return "mean_dependency_distance";
	case TBPROC_TF::C: return "C";
	case TBPROC_TF::C_exp_1: return "C_exp_1";
	case TBPROC_TF::C_exp_2: return "C_exp_2";
	case TBPROC_TF::C_var: return "C_var";
	case TBPROC_TF::C_z: return "C_z";
	case TBPROC_TF::D: return "D";
	case TBPROC_TF::D_exp_1: return "D_exp_1";
	case TBPROC_TF::D_exp_2: return "D_exp_2";
	case TBPROC_TF::D_var: return "D_var";
	case TBPROC_TF::D_z: return "D_z";
	case TBPROC_TF::Dmin_Unconstrained: return "Dmin_Unconstrained";
	case TBPROC_TF::Dmin_Planar: return "Dmin_Planar";
	case TBPROC_TF::Dmin_Projective: return "Dmin_Projective";
	case TBPROC_TF::max_flux_weight: return "max_flux_weight";
	case TBPROC_TF::mean_flux_weight: return "mean_flux_weight";
	case TBPROC_TF::min_flux_weight: return "min_flux_weight";
	case TBPROC_TF::max_left_span: return "max_left_span";
	case TBPROC_TF::mean_left_span: return "mean_left_span";
	case TBPROC_TF::min_left_span: return "min_left_span";
	case TBPROC_TF::max_right_span: return "max_right_span";
	case TBPROC_TF::mean_right_span: return "mean_right_span";
	case TBPROC_TF::min_right_span: return "min_right_span";
	case TBPROC_TF::max_RL_ratio: return "max_RL_ratio";
	case TBPROC_TF::mean_RL_ratio: return "mean_RL_ratio";
	case TBPROC_TF::min_RL_ratio: return "min_RL_ratio";
	case TBPROC_TF::max_WS_ratio: return "max_WS_ratio";
	case TBPROC_TF::mean_WS_ratio: return "mean_WS_ratio";
	case TBPROC_TF::min_WS_ratio: return "min_WS_ratio";
	case TBPROC_TF::max_size: return "max_size";
	case TBPROC_TF::mean_size: return "mean_size";
	case TBPROC_TF::min_size: return "min_size";
	case TBPROC_TF::__last_value: return "__last_value";
	}
	// should never happen
	return "???";
}

#define index_of(_X_) static_cast<size_t>(TBPROC_TF::_X_)
#define n_idx index_of(n)
#define k2_idx index_of(k2)
#define k3_idx index_of(k3)
#define size_Q_idx index_of(size_Q)
#define headedness_idx index_of(headedness)
#define mean_hierarchical_distance_idx index_of(mean_hierarchical_distance)
#define mean_dependency_distance_idx index_of(mean_dependency_distance)
#define C_idx index_of(C)
#define C_exp1_idx index_of(C_exp_1)
#define C_exp2_idx index_of(C_exp_2)
#define C_var_idx index_of(C_var)
#define C_z_idx index_of(C_z)
#define D_idx index_of(D)
#define D_exp1_idx index_of(D_exp_1)
#define D_exp2_idx index_of(D_exp_2)
#define D_var_idx index_of(D_var)
#define D_z_idx index_of(D_z)
#define Dmin_Unconstrained_idx index_of(Dmin_Unconstrained)
#define Dmin_Planar_idx index_of(Dmin_Planar)
#define Dmin_Projective_idx index_of(Dmin_Projective)
#define max_flux_weight_idx index_of(max_flux_weight)
#define mean_flux_weight_idx index_of(mean_flux_weight)
#define min_flux_weight_idx index_of(min_flux_weight)
#define max_left_span_idx index_of(max_left_span)
#define mean_left_span_idx index_of(mean_left_span)
#define min_left_span_idx index_of(min_left_span)
#define max_right_span_idx index_of(max_right_span)
#define mean_right_span_idx index_of(mean_right_span)
#define min_right_span_idx index_of(min_right_span)
#define max_RL_ratio_idx index_of(max_RL_ratio)
#define mean_RL_ratio_idx index_of(mean_RL_ratio)
#define min_RL_ratio_idx index_of(min_RL_ratio)
#define max_WS_ratio_idx index_of(max_WS_ratio)
#define mean_WS_ratio_idx index_of(mean_WS_ratio)
#define min_WS_ratio_idx index_of(min_WS_ratio)
#define max_size_idx index_of(max_size)
#define mean_size_idx index_of(mean_size)
#define min_size_idx index_of(min_size)

// CLASS METHODS

treebank_dataset_processor::processor_error treebank_dataset_processor::init
(const string& file, const string& odir, bool all_fs) noexcept
{
	m_main_list = file;
	m_out_dir = odir;
	std::fill(m_what_fs.begin(), m_what_fs.end(), all_fs);

	// check that the input file and the output directory exist
	dataset_error dserr = m_treebank_dataset_reader.init(m_main_list);

	if (dserr == dataset_error::main_file_does_not_exist)
	{ return processor_error::main_file_does_not_exist; }

	// make sure output directory exists
	if (m_out_dir != "." and not filesystem::exists(m_out_dir))
	{ return processor_error::output_directory_does_not_exist; }

	return processor_error::none;
}

treebank_dataset_processor::processor_error treebank_dataset_processor::process() noexcept
{
	// -- this function assumes that init did not return any error -- //

	// check that there is something to be computed
	if (std::all_of(m_what_fs.begin(),m_what_fs.end(),[](bool x){return not x;}))
	{ return processor_error::no_features; }

	// process dataset using treebank_dataset class
	while (m_treebank_dataset_reader.has_treebank()) {
		const dataset_error dserr = m_treebank_dataset_reader.next_treebank();

		if (dserr == dataset_error::treebank_could_not_be_opened)
		{ return processor_error::treebank_file_could_not_be_opened; }

		// iterate to next language
		treebank_reader& tbread = m_treebank_dataset_reader.get_treebank_reader();

		const string lang = tbread.get_identifier();
		if (m_be_verbose) {
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

		// output header to the file
		if (m_output_header) {
			if (m_what_fs[0]) {
				out_lang_file << tree_feature_string(static_cast<tree_feature>(0));
			}
			for (size_t i = 1; i < m_what_fs.size(); ++i) {
				if (m_what_fs[i]) {
					out_lang_file
						<< m_sep
						<< tree_feature_string(static_cast<tree_feature>(i));
				}
			}
			out_lang_file << endl;
		}

		// process the current treebank
		rooted_tree rT;
		while (tbread.has_tree()) {
			dataset_error err = tbread.next_tree();
			if (err == dataset_error::empty_line) {
				// empty line, skip...
			}
			else {
				rT = tbread.get_tree();
				process_tree<rooted_tree, ofstream>(rT, out_lang_file);
			}
		}

		if (m_be_verbose) {
			cout << "    processed "
				 << tbread.get_num_trees() << " trees" << endl;
		}
	}

	return processor_error::none;
}

// PRIVATE

template<class TREE, class OUT_STREAM>
void treebank_dataset_processor::process_tree
(const TREE& rT, OUT_STREAM& out_lang_file)
const
{
	const free_tree fT = rT.to_undirected();
	const uint32_t n = fT.num_nodes();

	// -----------------------------------------------------------
	// compute features in a way that does not repeat computations
	double props[NUM_TREE_FEATURES]{0.0};
	bool prop_set[NUM_TREE_FEATURES]{false};

	const auto set_prop =
	[&](size_t idx, double val) -> void {
		props[idx] = val;
		prop_set[idx] = true;
	};

	// number of nodes
	if (m_what_fs[n_idx]) {
		set_prop(n_idx, n);
	}
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
		set_prop(mean_hierarchical_distance_idx,
				 properties::mean_hierarchical_distance(rT));
	}
	if (m_what_fs[mean_dependency_distance_idx]) {
		set_prop(mean_dependency_distance_idx,
				 linarr::mean_dependency_distance(rT));
	}

	// -----------------------------------------------------------------
	// C

	if (m_what_fs[C_idx]) {

		// choose a suitable algorithm depending on the value of 'n'
		const auto algo_C =
		[](uint32_t N) -> linarr::algorithms_C {
			if (N <= 8) {
				return linarr::algorithms_C::ladder;
			}
			if (N == 9) {
				return linarr::algorithms_C::dynamic_programming;
			}
			if (N <= 100) {
				return linarr::algorithms_C::ladder;
			}
			return linarr::algorithms_C::stack_based;
		}(rT.num_nodes());

		set_prop(C_idx, linarr::n_crossings(fT, {}, algo_C));
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
		set_prop(Dmin_Unconstrained_idx,
				 linarr::Dmin(fT, linarr::algorithms_Dmin::Unconstrained_YS).first);
	}
	if (m_what_fs[Dmin_Planar_idx]) {
		set_prop(Dmin_Planar_idx,
				 linarr::Dmin(fT, linarr::algorithms_Dmin::Planar).first);
	}
	if (m_what_fs[Dmin_Projective_idx]) {
		set_prop(Dmin_Projective_idx,
				 linarr::Dmin(rT, linarr::algorithms_Dmin::Projective).first);
	}

	// -----------------
	// flux computation
	const bool compute_any_of_flux = std::any_of(
		m_what_fs.begin() + max_flux_weight_idx - 1,
		m_what_fs.begin() + min_size_idx + 1,
		[](const bool& b) -> bool { return b; }
	);
	if (compute_any_of_flux) {
		const auto F = linarr::compute_flux(fT);
		// since these values are cheap to calculate, compute every all of them
		// and output whatever is needed later

#define DFMEM &linarr::dependency_flux
		// compute the means
		set_average_of(F, mean_flux_weight_idx, DFMEM::get_weight, props);
		set_average_of(F, mean_left_span_idx, DFMEM::get_left_span, props);
		set_average_of(F, mean_right_span_idx, DFMEM::get_right_span, props);
		set_average_of(F, mean_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_average_of(F, mean_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_average_of(F, mean_size_idx, DFMEM::get_size, props);

		// compute the maxs
		set_maximum_of(F, max_flux_weight_idx, DFMEM::get_weight, props);
		set_maximum_of(F, max_left_span_idx, DFMEM::get_left_span, props);
		set_maximum_of(F, max_right_span_idx, DFMEM::get_right_span, props);
		set_maximum_of(F, max_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_maximum_of(F, max_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_maximum_of(F, max_size_idx, DFMEM::get_size, props);

		// compute the mins
		set_minimum_of(F, min_flux_weight_idx, DFMEM::get_weight, props);
		set_minimum_of(F, min_left_span_idx, DFMEM::get_left_span, props);
		set_minimum_of(F, min_right_span_idx, DFMEM::get_right_span, props);
		set_minimum_of(F, min_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_minimum_of(F, min_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_minimum_of(F, min_size_idx, DFMEM::get_size, props);
#undef DFMEM
	}

	// ---------------
	// output features

	if (m_what_fs[0]) {
		out_lang_file << rT.num_nodes();
	}
	for (size_t i = 1; i < m_what_fs.size(); ++i) {
		out_lang_file << m_sep;
		if (m_what_fs[i]) {
			out_lang_file << props[i];
		}
	}
	out_lang_file << "\n";
}

} // -- namespace io
} // -- namespace lal
