/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/head_initial.hpp>
#include <lal/linarr/flux.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/linarr/classify_syntactic_dependency_structure.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/hierarchical_distance.hpp>
#include <lal/properties/tree_centre.hpp>
#include <lal/properties/tree_centroid.hpp>
#include <lal/properties/tree_diameter.hpp>
#include <lal/io/treebank_collection_reader.hpp>
#include <lal/io/treebank_reader.hpp>

#include <lal/detail/graphs/tree_type.hpp>
#include <lal/detail/io/check_correctness.hpp>
#include <lal/detail/linarr/syntactic_dependency_structure.hpp>

#include <lal/detail/linarr/Dmin_Unconstrained_YS.hpp>
#include <lal/detail/linarr/Dmin_utils.hpp>
#include <lal/detail/properties/tree_centroid.hpp>

#include <lal/detail/macros/integer_convert.hpp>

#define square(x) ((x)*(x))

namespace lal {
namespace io {

template<typename T>
void set_average_of(
	const std::vector<linarr::dependency_flux>& F,
	const std::size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
noexcept
{
	if (F.size() == 0) {
		props[idx] = nan("");
		return;
	}

	const double cumul =
	[&]() -> double {
		double v =
		std::accumulate(
			F.begin(), F.end(),
			0.0, [&](double x, const linarr::dependency_flux& f) {
				return x + detail::to_double( (f.*FUNC)() );
			}
		);
		return v;
	}();
	props[idx] = cumul/detail::to_double(F.size());
}
template<typename T>
void set_maximum_of(
	const std::vector<linarr::dependency_flux>& F,
	const std::size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
noexcept
{
	if (F.size() == 0) {
		props[idx] = nan("");
		return;
	}

	const double value =
	[&]() -> double {
		double v = 0.0;
		std::for_each(
			F.begin(), F.end(),
			[&](const linarr::dependency_flux& f) {
				v = std::max(v, detail::to_double( (f.*FUNC)() ));
			}
		);
		return v;
	}();
	props[idx] = value;
}
template<typename T>
void set_minimum_of(
	const std::vector<linarr::dependency_flux>& F,
	const std::size_t idx,
	T (linarr::dependency_flux::*FUNC)() const,
	double *props
)
noexcept
{
	if (F.size() == 0) {
		props[idx] = nan("");
		return;
	}

	const double value =
	[&]() -> double {
		double v = 9999999.9;
		std::for_each(
			F.begin(), F.end(),
			[&](const linarr::dependency_flux& f) {
				v = std::min(v, detail::to_double( (f.*FUNC)() ));
			}
		);
		return v;
	}();
	props[idx] = value;
}

// -----------------------------------------------------------------------------
// CLASS METHODS

treebank_error treebank_processor::init
(
	const std::string& treebank_file,
	const std::string& output_file,
	const std::string& treebank_id
)
noexcept
{
	// keep data
	m_treebank_filename = treebank_file;
	m_output_file = output_file;
	m_treebank_id = treebank_id;

	// initalise features vector
	std::fill(m_what_fs.begin(), m_what_fs.end(), true);

	// initialise column names
	initialise_column_names();

	// If the linear arrangement cache is empty, fill it up
	// We should not expect sentences longer than 511 words
	if (m_id_linarrs.size() < 512) {
		m_id_linarrs.reserve(1024); // just in case
		m_id_linarrs.resize(512);
		// initialize only the most frequent lengths
		for (std::size_t i = 0; i < m_id_linarrs.size()/2; ++i) {
			m_id_linarrs[i].identity(i);
		}
	}

	// make sure that the treebank file exists
	if (not std::filesystem::exists(m_treebank_filename)) {
		return treebank_error(
			"Treebank file '" + m_treebank_filename + "' does not exist.",
			treebank_error_type::treebank_file_does_not_exist
		);
	}
	return treebank_error("", treebank_error_type::no_error);
}

treebank_error treebank_processor::process() noexcept {
	if (m_check_before_process) {
		const bool err =
		detail::check_correctness_treebank<true>(m_treebank_filename);

		if (err) {
			return treebank_error(
				"The treebank '" + m_treebank_filename + "' contains errors.",
				treebank_error_type::malformed_treebank_file
			);
		}
	}

	// check that there is something to be computed
	if (std::all_of(m_what_fs.begin(),m_what_fs.end(),[](bool x){return not x;}))
	{
		return treebank_error(
			"No features to be computed. Nothing to do.",
			treebank_error_type::no_features
		);
	}

	// output file stream:
	// since the output directory exists there is no need to check for is_open()
	std::ofstream out_treebank_file(m_output_file);
	if (not out_treebank_file.is_open()) {
		return treebank_error(
			"Output file '" + m_output_file + "' could not be opened.",
			treebank_error_type::output_file_could_not_be_opened
		);
	}

	// Construct treebank reader. Do this here so as to check for errors as
	// early as we can.
	treebank_reader tbread;
	{
	const auto err = tbread.init(m_treebank_filename, m_treebank_id);
	if (err != treebank_error_type::no_error) {
		if (m_be_verbose >= 2) {
			#pragma omp critical
			std::cerr
				<< "Processing treebank '" << m_treebank_filename << "' failed"
				<< '\n';
		}
		return err;
	}
	}

	// output header to the file
	if (m_output_header) {
		bool first = true;
		for (std::size_t i = 0; i < __treebank_feature_size; ++i) {
			if (m_what_fs[i]) {

				if (first) {
					first = false;
				}
				else {
					out_treebank_file << m_separator;
				}

				const auto tf = index_to_treebank_feature(i);
				switch (tf) {
				case treebank_feature::num_nodes:
				case treebank_feature::second_moment_degree:
				case treebank_feature::second_moment_degree_out:
				case treebank_feature::third_moment_degree:
				case treebank_feature::third_moment_degree_out:
				case treebank_feature::sum_squared_degrees:
				case treebank_feature::sum_squared_out_degrees:
				case treebank_feature::sum_cubed_degrees:
				case treebank_feature::sum_cubed_out_degrees:
				case treebank_feature::num_pairs_independent_edges:
				case treebank_feature::head_initial:
				case treebank_feature::hubiness:
				case treebank_feature::sum_hierarchical_distances:
				case treebank_feature::mean_hierarchical_distance:
				case treebank_feature::tree_diameter:
				case treebank_feature::num_crossings:
				case treebank_feature::predicted_num_crossings:
				case treebank_feature::exp_num_crossings:
				case treebank_feature::var_num_crossings:
				case treebank_feature::z_score_num_crossings:
				case treebank_feature::sum_edge_lengths:
				case treebank_feature::exp_sum_edge_lengths:
				case treebank_feature::exp_sum_edge_lengths_projective:
				case treebank_feature::exp_sum_edge_lengths_planar:
				case treebank_feature::var_sum_edge_lengths:
				case treebank_feature::z_score_sum_edge_lengths:
				case treebank_feature::min_sum_edge_lengths:
				case treebank_feature::min_sum_edge_lengths_planar:
				case treebank_feature::min_sum_edge_lengths_projective:
				case treebank_feature::mean_dependency_distance:
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
					out_treebank_file << m_column_names[i];
					break;

				case treebank_feature::tree_centre:
				case treebank_feature::tree_centroid:
					out_treebank_file << m_column_names[i] << '1';
					out_treebank_file << m_separator;
					out_treebank_file << m_column_names[i] << "2";
					break;

				case treebank_feature::tree_type:
					output_tree_type_header(out_treebank_file);
					break;

				case treebank_feature::syntactic_dependency_structure_class:
					output_syndepstruct_type_header(out_treebank_file);
					break;

				case treebank_feature::__last_value:
					break;
				}
			}
		}

		out_treebank_file << '\n';
	}

	const auto start = std::chrono::system_clock::now();

	detail::data_array<double> props(__treebank_feature_size, 0.0);
	detail::data_array<char> prop_set(__treebank_feature_size, 0);

	// process the current treebank
	graphs::rooted_tree rT;
	while (not tbread.end()) {
		rT = tbread.get_tree();

		process_tree<graphs::rooted_tree, std::ofstream>
			(rT, props.begin(), prop_set.begin(), out_treebank_file);

		props.fill(0.0);
		prop_set.fill(0);

		tbread.next_tree();
	}

	const auto end = std::chrono::system_clock::now();
	// this constructs a duration object using milliseconds
	const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

	if (m_be_verbose >= 1) {
		#pragma omp critical
		std::cout
			 << "    processed "
			 << tbread.get_num_trees()
			 << " trees in treebank '" << m_treebank_filename << "' in "
			 << elapsed.count() << " seconds."
			 << '\n';
	}

	return treebank_error("", treebank_error_type::no_error);
}

// PRIVATE

template<class OUT_STREAM>
void treebank_processor::output_tree_type_header(OUT_STREAM& out_treebank_file)
const noexcept
{
	out_treebank_file
	<< detail::tree_type_string(detail::array_of_tree_types[0]);

	for (std::size_t j = 1; j < graphs::__tree_type_size; ++j) {
		out_treebank_file
			<< m_separator
			<< detail::tree_type_string(detail::array_of_tree_types[j]);
	}
}

template<class OUT_STREAM>
void treebank_processor::output_syndepstruct_type_header(OUT_STREAM& out_treebank_file)
const noexcept
{
	out_treebank_file
	<< detail::syntactic_dependency_structure_to_string(
		   detail::array_of_syntactic_dependency_structures[0]
	   );

	for (std::size_t j = 1; j < linarr::__syntactic_dependency_structure_size; ++j) {
		out_treebank_file
			<< m_separator
			<< detail::syntactic_dependency_structure_to_string(
				   detail::array_of_syntactic_dependency_structures[j]
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
		out_treebank_file << (fT.is_of_tree_type(tt) ? '1' : '0');
	};

	if (not fT.is_tree_type_valid()) {
		fT.calculate_tree_type();
	}

	output_tt(detail::array_of_tree_types[0]);
	for (std::size_t j = 1; j < graphs::__tree_type_size; ++j) {
		out_treebank_file << m_separator;
		output_tt(detail::array_of_tree_types[j]);
	}
}

template<class TREE_TYPE, class OUT_STREAM>
void treebank_processor::output_syndepstruct_type_values(
	const TREE_TYPE& rT,
	uint64_t C,
	OUT_STREAM& out_treebank_file
)
const noexcept
{
	const auto v = linarr::syntactic_dependency_structure_class(rT, C);

	const auto output_sdst =
	[&](const linarr::syntactic_dependency_structure& sdst) {
		const std::size_t idx_tt = static_cast<std::size_t>(sdst);
		out_treebank_file << (v[idx_tt] ? '1' : '0');
	};

	output_sdst(detail::array_of_syntactic_dependency_structures[0]);
	for (std::size_t j = 1; j < linarr::__syntactic_dependency_structure_size; ++j) {
		out_treebank_file << m_separator;
		output_sdst(detail::array_of_syntactic_dependency_structures[j]);
	}
}

template<class TREE, class OUT_STREAM>
void treebank_processor::process_tree
(const TREE& rT, double *props, char *prop_set, OUT_STREAM& out_treebank_file)
noexcept
{
	graphs::free_tree fT = rT.to_undirected();
	const uint64_t n = fT.get_num_nodes();
	// choose a suitable algorithm depending on the value of 'n'
	const auto algo_C =
	[](uint64_t N) -> linarr::algorithms_C {
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

	if (m_id_linarrs.size() <= n) {
		for (std::size_t i = m_id_linarrs.size(); i <= n; ++i) {
			m_id_linarrs.emplace_back(linear_arrangement());
		}
	}
	if (m_id_linarrs[n].size() != n) { m_id_linarrs[n].identity(n); }
	const linear_arrangement& id_arr = m_id_linarrs[n];

	// -------------------------------------------------------------------
	// compute numeric features in a way that does not repeat computations

	const auto set_prop =
	[&](std::size_t idx, double val) -> void {
		props[idx] = val;
		prop_set[idx] = 1;
	};

	// number of nodes
	if (m_what_fs[n_idx]) {
		set_prop(n_idx, detail::to_double(n));
	}

	// <k^2>
	if (m_what_fs[k2_idx]) {
		set_prop(k2_idx, properties::moment_degree(fT, 2));
	}
	if (m_what_fs[k2_out_idx]) {
		set_prop(k2_out_idx, properties::moment_out_degree(rT, 2));
	}
	// <k^3>
	if (m_what_fs[k3_idx]) {
		set_prop(k3_idx, properties::moment_degree(fT, 3));
	}
	if (m_what_fs[k3_out_idx]) {
		set_prop(k3_out_idx, properties::moment_out_degree(rT, 3));
	}

	// K^2
	if (m_what_fs[SK2_idx]) {
		set_prop(SK2_idx, detail::to_double(properties::sum_powers_degrees(fT, 2)));
	}
	if (m_what_fs[SK2_out_idx]) {
		set_prop(SK2_out_idx, detail::to_double(properties::sum_powers_out_degrees(rT, 2)));
	}
	// K^3
	if (m_what_fs[SK3_idx]) {
		set_prop(SK3_idx, detail::to_double(properties::sum_powers_degrees(fT, 3)));
	}
	if (m_what_fs[SK3_out_idx]) {
		set_prop(SK3_out_idx, detail::to_double(properties::sum_powers_out_degrees(rT, 3)));
	}


	// |Q|
	if (m_what_fs[num_pairs_independent_edges_idx]) {
		set_prop(num_pairs_independent_edges_idx,
				 detail::to_double(properties::num_pairs_independent_edges(fT)));
	}
	// head initial
	if (m_what_fs[head_initial_idx]) {
		if (n > 1) {
			set_prop(head_initial_idx, linarr::head_initial(rT, id_arr));
		}
		else {
			set_prop(head_initial_idx, nan(""));
		}
	}
	// hubiness
	if (m_what_fs[hubiness_idx]) {
		if (n > 3) {
			set_prop(hubiness_idx, properties::hubiness(fT));
		}
		else {
			set_prop(hubiness_idx, nan(""));
		}
	}

	// SHD
	if (m_what_fs[sum_hierarchical_distance_idx]) {
		if (n > 1) {
			set_prop(sum_hierarchical_distance_idx,
					 detail::to_double(properties::sum_hierarchical_distances(rT)));
		}
		else {
			set_prop(mean_hierarchical_distance_idx, nan(""));
		}
	}
	// MHD
	if (m_what_fs[mean_hierarchical_distance_idx]) {
		if (n > 1) {
			if (m_what_fs[sum_hierarchical_distance_idx]) {
				// sum of hierarchical distances was calculated in the previous
				// 'if' statement: resuse it!
				set_prop(mean_hierarchical_distance_idx,
						 props[sum_hierarchical_distance_idx]/(detail::to_double(n) - 1));
			}
			else {
				set_prop(mean_hierarchical_distance_idx,
						 properties::mean_hierarchical_distance(rT));
			}
		}
		else {
			set_prop(mean_hierarchical_distance_idx, nan(""));
		}
	}
	// MDD
	if (m_what_fs[mean_dependency_distance_idx]) {
		if (n > 1) {
			set_prop(mean_dependency_distance_idx,
					 linarr::mean_dependency_distance(rT, id_arr));
		}
		else {
			set_prop(mean_dependency_distance_idx, nan(""));
		}
	}
	// diameter
	if (m_what_fs[tree_diameter_idx]) {
		set_prop(tree_diameter_idx, detail::to_double(properties::tree_diameter(rT)));
	}

	// -----------------------------------------------------------------
	// C

	if (m_what_fs[C_idx]) {
		set_prop(C_idx, detail::to_double(linarr::num_crossings(fT, id_arr, algo_C)));
	}
	if (m_what_fs[C_predicted_idx]) {
		set_prop(C_predicted_idx, linarr::predicted_num_crossings(fT, id_arr));
	}
	if (m_what_fs[C_expected_idx]) {
		set_prop(C_expected_idx, properties::exp_num_crossings(fT));
	}
	if (m_what_fs[C_variance_idx]) {
		set_prop(C_variance_idx, properties::var_num_crossings_tree(fT));
	}

	// z-score of C
	if (m_what_fs[C_z_score_idx]) {
		// we need C
		if (not m_what_fs[C_idx]) {
			set_prop(C_idx, detail::to_double(linarr::num_crossings(fT, id_arr, algo_C)));
		}
		// we need E[C]
		if (not m_what_fs[C_expected_idx]) {
			set_prop(C_expected_idx, properties::exp_num_crossings(fT));
		}
		// we need V[C]
		if (not m_what_fs[C_variance_idx]) {
			set_prop(C_variance_idx, properties::var_num_crossings_tree(fT));
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
		set_prop(D_idx, detail::to_double(linarr::sum_edge_lengths(fT, id_arr)));
	}
	if (m_what_fs[D_expected_idx]) {
		set_prop(D_expected_idx, properties::exp_sum_edge_lengths(fT));
	}
	if (m_what_fs[D_expected_projective_idx]) {
		set_prop(D_expected_projective_idx, properties::exp_sum_edge_lengths_projective(rT));
	}
	if (m_what_fs[D_expected_planar_idx]) {
		set_prop(D_expected_planar_idx, properties::exp_sum_edge_lengths_planar(fT));
	}
	if (m_what_fs[D_variance_idx]) {
		set_prop(D_variance_idx, properties::var_sum_edge_lengths(fT));
	}

	// z-score of D
	if (m_what_fs[D_z_score_idx]) {
		// we need D
		if (not m_what_fs[D_idx]) {
			set_prop(D_idx, detail::to_double(linarr::sum_edge_lengths(fT, id_arr)));
		}
		// we need E[D]
		if (not m_what_fs[D_expected_idx]) {
			set_prop(D_expected_idx, properties::exp_sum_edge_lengths(fT));
		}
		// we need V[D]
		if (not m_what_fs[D_variance_idx]) {
			set_prop(D_variance_idx, properties::var_sum_edge_lengths(fT));
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

	// initialized to 0 so that compiler does not cry.
	uint64_t Dmin_projective = 0;
	if (m_what_fs[Dmin_Projective_idx]) {

		// rooted adjacency list for the rooted tree
		std::vector<std::vector<detail::node_size>> L(n);
		detail::Dmin_utils::make_sorted_adjacency_list_rooted
			<detail::countingsort::non_increasing_t>
			(rT, L);

		Dmin_projective =
			detail::Dmin_utils::arrange_projective(n, L, rT.get_root());

		set_prop(Dmin_Projective_idx, detail::to_double(Dmin_projective));
	}

	if (m_what_fs[Dmin_Planar_idx]) {
		std::vector<std::vector<detail::node_size>> L;
		const auto centroid = detail::retrieve_centroid(rT, L);

		const bool centroid_contains_root =
			(centroid.first == rT.get_root() or
			 (centroid.second < n and centroid.second == rT.get_root()));

		if (m_what_fs[Dmin_Projective_idx] and centroid_contains_root) {
			// if 'Dmin_projective' has been set and the centroid contains the
			// root, do nothing
			set_prop(Dmin_Planar_idx, detail::to_double(Dmin_projective));
		}
		else {
			// Dmin_projective was not calculated, or the centroid does
			// not contain the root

			detail::Dmin_utils::root_adjacency_list
				(fT, centroid.first, centroid.first, L);

			const uint64_t Dmin_planar =
				detail::Dmin_utils::arrange_projective(n, L, centroid.first);

			set_prop(Dmin_Planar_idx, detail::to_double(Dmin_planar));
		}
	}

	if (m_what_fs[Dmin_Unconstrained_idx]) {
		const auto Dmin = detail::Dmin::unconstrained::YossiShiloach<false>(fT);
		set_prop(Dmin_Unconstrained_idx, detail::to_double(Dmin));
	}

	// -----------------
	// flux computation
	const bool compute_any_of_flux = std::any_of(
		m_what_fs.begin() + flux_max_weight_idx - 1,
		m_what_fs.begin() + flux_min_size_idx + 1,
		[](const bool& b) -> bool { return b; }
	);
	if (compute_any_of_flux) {
		const auto F = linarr::compute_flux(fT, id_arr);
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

	const auto centre_of_tree =
		(m_what_fs[tree_centre_idx] ?
			 properties::tree_centre(fT) :
			 std::make_pair(n+1,n+1));

	const auto centroid_of_tree =
		(m_what_fs[tree_centroid_idx] ?
			 properties::tree_centroid(fT) :
			 std::make_pair(n+1,n+1));

	// ---------------
	// output features

	bool first = true;
	for (std::size_t i = 0; i < __treebank_feature_size; ++i) {
		if (m_what_fs[i]) {
			if (first) {
				first = false;
			}
			else {
				out_treebank_file << m_separator;
			}

			const auto tf = index_to_treebank_feature(i);
			switch (tf) {
			case treebank_feature::num_nodes:
			case treebank_feature::second_moment_degree:
			case treebank_feature::second_moment_degree_out:
			case treebank_feature::third_moment_degree:
			case treebank_feature::third_moment_degree_out:
			case treebank_feature::sum_squared_degrees:
			case treebank_feature::sum_squared_out_degrees:
			case treebank_feature::sum_cubed_degrees:
			case treebank_feature::sum_cubed_out_degrees:
			case treebank_feature::num_pairs_independent_edges:
			case treebank_feature::head_initial:
			case treebank_feature::hubiness:
			case treebank_feature::sum_hierarchical_distances:
			case treebank_feature::mean_hierarchical_distance:
			case treebank_feature::tree_diameter:
			case treebank_feature::num_crossings:
			case treebank_feature::predicted_num_crossings:
			case treebank_feature::exp_num_crossings:
			case treebank_feature::var_num_crossings:
			case treebank_feature::z_score_num_crossings:
			case treebank_feature::sum_edge_lengths:
			case treebank_feature::exp_sum_edge_lengths:
			case treebank_feature::exp_sum_edge_lengths_projective:
			case treebank_feature::exp_sum_edge_lengths_planar:
			case treebank_feature::var_sum_edge_lengths:
			case treebank_feature::z_score_sum_edge_lengths:
			case treebank_feature::min_sum_edge_lengths:
			case treebank_feature::min_sum_edge_lengths_planar:
			case treebank_feature::min_sum_edge_lengths_projective:
			case treebank_feature::mean_dependency_distance:
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

			case treebank_feature::tree_centre:
				out_treebank_file
					<< centre_of_tree.first << m_separator
					<< centre_of_tree.second;
				break;

			case treebank_feature::tree_centroid:
				out_treebank_file
					<< centroid_of_tree.first << m_separator
					<< centroid_of_tree.second;
				break;

			case treebank_feature::tree_type:
				output_tree_type_values(fT, out_treebank_file);
				break;

			case treebank_feature::syntactic_dependency_structure_class:
				uint64_t C;
				if (m_what_fs[C_idx]) {
					C = static_cast<uint64_t>(props[C_idx]);
				}
				else {
					C = linarr::num_crossings(fT, id_arr, algo_C);
				}
				output_syndepstruct_type_values(rT, C, out_treebank_file);
				break;

			case treebank_feature::__last_value:
				break;
			}
		}
	}

	out_treebank_file << '\n';
}

} // -- namespace io
} // -- namespace lal
