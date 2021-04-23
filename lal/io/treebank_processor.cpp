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

#include <lal/io/treebank_processor.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <chrono>
#include <cmath>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/headedness.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/linarr/flux.hpp>
#include <lal/linarr/classify_syntactic_dependency_structure.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/io/treebank_collection_reader.hpp>
#include <lal/io/treebank_reader.hpp>

#include <lal/internal/graphs/tree_type.hpp>
#include <lal/internal/io/treebank_feature.hpp>
#include <lal/internal/io/check_correctness.hpp>
#include <lal/internal/linarr/syntactic_dependency_structure_type.hpp>

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

#define treebank_feature_to_index(_X_) \
	internal::treebank_feature_to_index(treebank_feature::_X_)

#define n_idx treebank_feature_to_index(num_nodes)
#define k2_idx treebank_feature_to_index(k2)
#define k3_idx treebank_feature_to_index(k3)
#define size_Q_idx treebank_feature_to_index(num_pairs_independent_edges)
#define headedness_idx treebank_feature_to_index(headedness)
#define hubiness_idx treebank_feature_to_index(hubiness)
#define mean_hierarchical_distance_idx treebank_feature_to_index(mean_hierarchical_distance)
#define mean_dependency_distance_idx treebank_feature_to_index(mean_dependency_distance)
#define C_idx treebank_feature_to_index(C)
#define C_expected_idx treebank_feature_to_index(C_expected)
#define C_variance_idx treebank_feature_to_index(C_variance)
#define C_z_score_idx treebank_feature_to_index(C_z_score)
#define D_idx treebank_feature_to_index(D)
#define D_expected_idx treebank_feature_to_index(D_expected)
#define D_variance_idx treebank_feature_to_index(D_variance)
#define D_z_score_idx treebank_feature_to_index(D_z_score)
#define Dmin_Unconstrained_idx treebank_feature_to_index(Dmin)
#define Dmin_Planar_idx treebank_feature_to_index(Dmin_Planar)
#define Dmin_Projective_idx treebank_feature_to_index(Dmin_Projective)
#define flux_max_weight_idx treebank_feature_to_index(flux_max_weight)
#define flux_mean_weight_idx treebank_feature_to_index(flux_mean_weight)
#define flux_min_weight_idx treebank_feature_to_index(flux_min_weight)
#define flux_max_left_span_idx treebank_feature_to_index(flux_max_left_span)
#define flux_mean_left_span_idx treebank_feature_to_index(flux_mean_left_span)
#define flux_min_left_span_idx treebank_feature_to_index(flux_min_left_span)
#define flux_max_right_span_idx treebank_feature_to_index(flux_max_right_span)
#define flux_mean_right_span_idx treebank_feature_to_index(flux_mean_right_span)
#define flux_min_right_span_idx treebank_feature_to_index(flux_min_right_span)
#define flux_max_RL_ratio_idx treebank_feature_to_index(flux_max_RL_ratio)
#define flux_mean_RL_ratio_idx treebank_feature_to_index(flux_mean_RL_ratio)
#define flux_min_RL_ratio_idx treebank_feature_to_index(flux_min_RL_ratio)
#define flux_max_WS_ratio_idx treebank_feature_to_index(flux_max_WS_ratio)
#define flux_mean_WS_ratio_idx treebank_feature_to_index(flux_mean_WS_ratio)
#define flux_min_WS_ratio_idx treebank_feature_to_index(flux_min_WS_ratio)
#define flux_max_size_idx treebank_feature_to_index(flux_max_size)
#define flux_mean_size_idx treebank_feature_to_index(flux_mean_size)
#define flux_min_size_idx treebank_feature_to_index(flux_min_size)
#define tree_type_idx treebank_feature_to_index(tree_type)
#define sdst_idx treebank_feature_to_index(syntactic_dependency_structure_type)

// -----------------------------------------------------------------------------
// CLASS METHODS

treebank_error treebank_processor::init(
	const string& treebank_file, const string& output_file,
	const string& treebank_id
) noexcept
{
	// keep data
	m_treebank_filename = treebank_file;
	m_output_file = output_file;
	m_treebank_id = treebank_id;

	// initalise features vector
	std::fill(m_what_fs.begin(), m_what_fs.end(), true);

	// make sure that the treebank file exists
	if (not filesystem::exists(m_treebank_filename)) {
		return treebank_error::treebank_file_does_not_exist;
	}
	return treebank_error::no_error;
}

treebank_error treebank_processor::process() noexcept {
	if (m_check_before_process) {
		const bool err =
		internal::check_correctness_treebank<true>(m_treebank_filename);

		if (err) {
			return treebank_error::malformed_treebank_file;
		}
	}

	// check that there is something to be computed
	if (std::all_of(m_what_fs.begin(),m_what_fs.end(),[](bool x){return not x;}))
	{ return treebank_error::no_features; }

	// iterate to next language
	treebank_reader tbread;
	{
	const auto err = tbread.init(m_treebank_filename, m_treebank_id);
	if (err != treebank_error::no_error) {
		if (m_be_verbose >= 2) {
			cerr << "Processing treebank '" << m_treebank_filename << "' failed"
				 << endl;
		}
		return err;
	}
	}

	// output file stream:
	// since the output directory exists there is no need to check for is_open()
	ofstream out_treebank_file(m_output_file);
	if (not out_treebank_file.is_open()) {
		return treebank_error::output_file_could_not_be_opened;
	}

	// output header to the file
	if (m_output_header) {
		bool first = true;

		for (size_t i = 0; i < __treebank_feature_size; ++i) {
			if (m_what_fs[i]) {
				if (first) {
					first = false;
				}
				else {
					out_treebank_file << m_separator;
				}

				const auto tf = internal::index_to_treebank_feature(i);
				switch (tf) {
				case treebank_feature::num_nodes:
				case treebank_feature::k2:
				case treebank_feature::k3:
				case treebank_feature::num_pairs_independent_edges:
				case treebank_feature::headedness:
				case treebank_feature::hubiness:
				case treebank_feature::mean_hierarchical_distance:
				case treebank_feature::mean_dependency_distance:
				case treebank_feature::C:
				case treebank_feature::C_expected:
				case treebank_feature::C_variance:
				case treebank_feature::C_z_score:
				case treebank_feature::D:
				case treebank_feature::D_expected:
				case treebank_feature::D_variance:
				case treebank_feature::D_z_score:
				case treebank_feature::Dmin:
				case treebank_feature::Dmin_Planar:
				case treebank_feature::Dmin_Projective:
				case treebank_feature::flux_max_weight:
				case treebank_feature::flux_mean_weight:
				case treebank_feature::flux_min_weight:
				case treebank_feature::flux_max_left_span:
				case treebank_feature::flux_mean_left_span:
				case treebank_feature::flux_min_left_span:
				case treebank_feature::flux_max_right_span:
				case treebank_feature::flux_mean_right_span:
				case treebank_feature::flux_min_right_span:
				case treebank_feature::flux_max_RL_ratio:
				case treebank_feature::flux_mean_RL_ratio:
				case treebank_feature::flux_min_RL_ratio:
				case treebank_feature::flux_max_WS_ratio:
				case treebank_feature::flux_mean_WS_ratio:
				case treebank_feature::flux_min_WS_ratio:
				case treebank_feature::flux_max_size:
				case treebank_feature::flux_mean_size:
				case treebank_feature::flux_min_size:
					out_treebank_file << internal::treebank_feature_string(tf);
					break;

				case treebank_feature::tree_type:
					output_tree_type_header(out_treebank_file);
					break;

				case treebank_feature::syntactic_dependency_structure_type:
					output_syndepstruct_type_header(out_treebank_file);
					break;

				case treebank_feature::__last_value:
					break;
				}
			}
		}

		out_treebank_file << "\n";
	}

	const auto start = std::chrono::system_clock::now();

	// process the current treebank
	rooted_tree rT;
	while (tbread.has_tree()) {
		const auto err = tbread.next_tree();
		if (err == treebank_error::no_error) {
			rT = tbread.get_tree();
			process_tree<rooted_tree, ofstream>(rT, out_treebank_file);
		}
	}

	const auto end = std::chrono::system_clock::now();
	// this constructs a duration object using milliseconds
	const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

	if (m_be_verbose >= 1) {
		cout << "    processed "
			 << tbread.get_num_trees()
			 << " trees in treebank '" << m_treebank_filename << "' in "
			 << elapsed.count() << " seconds."
			 << endl;
	}

	return treebank_error::no_error;
}

// PRIVATE

template<class OUT_STREAM>
void treebank_processor::output_tree_type_header(OUT_STREAM& out_treebank_file)
const noexcept
{
	out_treebank_file
	<< internal::tree_type_string(internal::array_of_tree_types[0]);

	for (size_t j = 1; j < graphs::__tree_type_size; ++j) {
		out_treebank_file
			<< m_separator
			<< internal::tree_type_string(internal::array_of_tree_types[j]);
	}
}

template<class OUT_STREAM>
void treebank_processor::output_syndepstruct_type_header(OUT_STREAM& out_treebank_file)
const noexcept
{
	out_treebank_file
	<< internal::syntactic_dependency_structure_type_to_string(
		   internal::array_of_syntactic_dependency_structure_types[0]
	   );

	for (size_t j = 1; j < linarr::__tree_structure_type_size; ++j) {
		out_treebank_file
			<< m_separator
			<< internal::syntactic_dependency_structure_type_to_string(
				   internal::array_of_syntactic_dependency_structure_types[j]
			   );
	}
}

// output the tree type
template<class TREE_TYPE, class OUT_STREAM>
void treebank_processor::output_tree_type_values(
	TREE_TYPE& fT,
	OUT_STREAM& out_treebank_file
)
const noexcept
{
	const auto output_tt =
	[&](const graphs::tree_type& tt) {
		if (fT.is_of_tree_type(tt)) {
			out_treebank_file << "1";
		}
		else {
			out_treebank_file << "0";
		}
	};

	if (not fT.is_tree_type_valid()) {
		fT.calculate_tree_type();
	}

	output_tt(internal::array_of_tree_types[0]);
	for (size_t j = 1; j < graphs::__tree_type_size; ++j) {
		out_treebank_file << m_separator;
		output_tt(internal::array_of_tree_types[j]);
	}
}

template<class TREE_TYPE, class OUT_STREAM>
void treebank_processor::output_syndepstruct_type_values(
	const TREE_TYPE& rT,
	OUT_STREAM& out_treebank_file
)
const noexcept
{
	const auto v = linarr::classify_syntactic_dependency_structure(rT);

	const auto output_sdst =
	[&](const linarr::syntactic_dependency_structure_type& sdst) {
		const size_t idx_tt = static_cast<size_t>(sdst);
		if (v[idx_tt]) {
			out_treebank_file << "1";
		}
		else {
			out_treebank_file << "0";
		}
	};

	output_sdst(internal::array_of_syntactic_dependency_structure_types[0]);
	for (size_t j = 1; j < linarr::__tree_structure_type_size; ++j) {
		out_treebank_file << m_separator;
		output_sdst(internal::array_of_syntactic_dependency_structure_types[j]);
	}
}

template<class TREE, class OUT_STREAM>
void treebank_processor::process_tree
(const TREE& rT, OUT_STREAM& out_treebank_file)
const noexcept
{
	free_tree fT = rT.to_undirected();
	const uint32_t n = fT.get_num_nodes();

	// -------------------------------------------------------------------
	// compute numeric features in a way that does not repeat computations
	double props[__treebank_feature_size]{0.0};
	bool prop_set[__treebank_feature_size]{false};

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
		set_prop(k2_idx, properties::moment_degree(fT, 2));
	}
	if (m_what_fs[k3_idx]) {
		set_prop(k3_idx, properties::moment_degree(fT, 3));
	}
	if (m_what_fs[size_Q_idx]) {
		set_prop(size_Q_idx, static_cast<double>(properties::size_Q(fT)));
	}
	if (m_what_fs[headedness_idx]) {
		set_prop(headedness_idx, linarr::headedness(rT));
	}
	if (m_what_fs[hubiness_idx]) {
		set_prop(hubiness_idx, properties::hubiness(fT));
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
		}(rT.get_num_nodes());

		set_prop(C_idx, linarr::number_of_crossings(fT, algo_C));
	}
	if (m_what_fs[C_variance_idx]) {
		set_prop(C_variance_idx, properties::variance_C_tree(fT));
	}
	if (m_what_fs[C_expected_idx]) {
		set_prop(C_expected_idx, properties::expectation_C(fT));
	}

	// z-score of C
	if (m_what_fs[C_z_score_idx]) {
		// we need C
		if (not m_what_fs[C_idx]) {
			set_prop(C_idx, linarr::number_of_crossings(fT));
		}
		// we need E[C]
		if (not m_what_fs[C_expected_idx]) {
			set_prop(C_expected_idx, properties::expectation_C(fT));
		}
		// we need V[C]
		if (not m_what_fs[C_variance_idx]) {
			set_prop(C_variance_idx, properties::variance_C_tree(fT));
		}

#if defined DEBUG
		assert(prop_set[C_idx]);
		assert(prop_set[C_variance_idx]);
		assert(prop_set[C_expected_idx]);
#endif
		set_prop(C_z_score_idx,
			(props[C_idx] - props[C_expected_idx])/std::sqrt(props[C_variance_idx])
		);
	}

	// -----------------------------------------------------------------
	// D

	if (m_what_fs[D_idx]) {
		set_prop(D_idx, linarr::sum_length_edges(fT));
	}
	if (m_what_fs[D_variance_idx]) {
		set_prop(D_variance_idx, properties::variance_D(fT));
	}
	if (m_what_fs[D_expected_idx]) {
		set_prop(D_expected_idx, properties::expectation_D(fT));
	}

	// z-score of D
	if (m_what_fs[D_z_score_idx]) {
		// we need D
		if (not m_what_fs[D_idx]) {
			set_prop(D_idx, linarr::sum_length_edges(fT));
		}
		// we need E[D]
		if (not m_what_fs[D_expected_idx]) {
			set_prop(D_expected_idx, properties::expectation_D(fT));
		}
		// we need V[D]
		if (not m_what_fs[D_variance_idx]) {
			set_prop(D_variance_idx, properties::variance_D(fT));
		}

#if defined DEBUG
		assert(prop_set[D_idx]);
		assert(prop_set[D_variance_idx]);
		assert(prop_set[D_expected_idx]);
#endif
		set_prop(D_z_score_idx,
			(props[D_idx] - props[D_expected_idx])/std::sqrt(props[D_variance_idx])
		);
	}

	// -----------------
	// Optimisation of D

	if (m_what_fs[Dmin_Unconstrained_idx]) {
		set_prop(Dmin_Unconstrained_idx, linarr::Dmin(fT).first);
	}
	if (m_what_fs[Dmin_Planar_idx]) {
		set_prop(Dmin_Planar_idx, linarr::Dmin_Planar(fT).first);
	}
	if (m_what_fs[Dmin_Projective_idx]) {
		set_prop(Dmin_Projective_idx, linarr::Dmin_Projective(rT).first);
	}

	// -----------------
	// flux computation
	const bool compute_any_of_flux = std::any_of(
		m_what_fs.begin() + flux_max_weight_idx - 1,
		m_what_fs.begin() + flux_min_size_idx + 1,
		[](const bool& b) -> bool { return b; }
	);
	if (compute_any_of_flux) {
		const auto F = linarr::compute_flux(fT);
		// since these values are cheap to calculate, compute every all of them
		// and output whatever is needed later

#define DFMEM &linarr::dependency_flux
		// compute the means
		set_average_of(F, flux_mean_weight_idx, DFMEM::get_weight, props);
		set_average_of(F, flux_mean_left_span_idx, DFMEM::get_left_span, props);
		set_average_of(F, flux_mean_right_span_idx, DFMEM::get_right_span, props);
		set_average_of(F, flux_mean_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_average_of(F, flux_mean_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_average_of(F, flux_mean_size_idx, DFMEM::get_size, props);

		// compute the maxs
		set_maximum_of(F, flux_max_weight_idx, DFMEM::get_weight, props);
		set_maximum_of(F, flux_max_left_span_idx, DFMEM::get_left_span, props);
		set_maximum_of(F, flux_max_right_span_idx, DFMEM::get_right_span, props);
		set_maximum_of(F, flux_max_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_maximum_of(F, flux_max_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_maximum_of(F, flux_max_size_idx, DFMEM::get_size, props);

		// compute the mins
		set_minimum_of(F, flux_min_weight_idx, DFMEM::get_weight, props);
		set_minimum_of(F, flux_min_left_span_idx, DFMEM::get_left_span, props);
		set_minimum_of(F, flux_min_right_span_idx, DFMEM::get_right_span, props);
		set_minimum_of(F, flux_min_RL_ratio_idx, DFMEM::get_RL_ratio, props);
		set_minimum_of(F, flux_min_WS_ratio_idx, DFMEM::get_WS_ratio, props);
		set_minimum_of(F, flux_min_size_idx, DFMEM::get_size, props);
#undef DFMEM
	}

	// ---------------
	// output features

	bool first = true;
	for (size_t i = 0; i < __treebank_feature_size; ++i) {
		if (m_what_fs[i]) {
			if (first) {
				first = false;
			}
			else {
				out_treebank_file << m_separator;
			}

			const auto tf = internal::index_to_treebank_feature(i);
			switch (tf) {
			case treebank_feature::num_nodes:
			case treebank_feature::k2:
			case treebank_feature::k3:
			case treebank_feature::num_pairs_independent_edges:
			case treebank_feature::headedness:
			case treebank_feature::hubiness:
			case treebank_feature::mean_hierarchical_distance:
			case treebank_feature::mean_dependency_distance:
			case treebank_feature::C:
			case treebank_feature::C_expected:
			case treebank_feature::C_variance:
			case treebank_feature::C_z_score:
			case treebank_feature::D:
			case treebank_feature::D_expected:
			case treebank_feature::D_variance:
			case treebank_feature::D_z_score:
			case treebank_feature::Dmin:
			case treebank_feature::Dmin_Planar:
			case treebank_feature::Dmin_Projective:
			case treebank_feature::flux_max_weight:
			case treebank_feature::flux_mean_weight:
			case treebank_feature::flux_min_weight:
			case treebank_feature::flux_max_left_span:
			case treebank_feature::flux_mean_left_span:
			case treebank_feature::flux_min_left_span:
			case treebank_feature::flux_max_right_span:
			case treebank_feature::flux_mean_right_span:
			case treebank_feature::flux_min_right_span:
			case treebank_feature::flux_max_RL_ratio:
			case treebank_feature::flux_mean_RL_ratio:
			case treebank_feature::flux_min_RL_ratio:
			case treebank_feature::flux_max_WS_ratio:
			case treebank_feature::flux_mean_WS_ratio:
			case treebank_feature::flux_min_WS_ratio:
			case treebank_feature::flux_max_size:
			case treebank_feature::flux_mean_size:
			case treebank_feature::flux_min_size:
				out_treebank_file << props[i];
				break;

			case treebank_feature::tree_type:
				output_tree_type_values(fT, out_treebank_file);
				break;

			case treebank_feature::syntactic_dependency_structure_type:
				output_syndepstruct_type_values(rT, out_treebank_file);
				break;

			case treebank_feature::__last_value:
				break;
			}
		}
	}

	out_treebank_file << "\n";
}

} // -- namespace io
} // -- namespace lal
