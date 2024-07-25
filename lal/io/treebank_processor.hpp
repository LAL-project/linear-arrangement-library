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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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
#include <string>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/io/treebank_file_error.hpp>
#include <lal/io/treebank_processor_base.hpp>

namespace lal {
namespace io {

/**
 * @brief Automatic processing of treebank files.
 *
 * This class, the objects of which will be referred to as the "processors",
 * has the goal to ease the processing a whole treebank collection and produce
 * data for a fixed set of features available in the library. See the enumeration
 * @ref lal::io::treebank_feature for details on the features available.
 *
 * This class is meant to process a single treebank file only (see
 * @ref LAL_concepts__treebank for further details on treebank files).
 *
 * Every processor must be initialized prior to processing the treebank file.
 * This is done via method @ref init, which requires the path to the treebank file
 * and the name of the output file (the treebank result) where the results are
 * going to be stored. It also requires a Boolean value indicating whether all
 * (or none) of the features should be used. Processing a treebank file with this
 * class will produce a single file, with as many columns as features added to
 * the processor. The columns are separated with a separating character (see
 * method @ref set_separator); this file will contain a header only if method
 * @ref set_output_header has been called with true). Progress and error messages
 * can be controlled via method @ref set_verbosity.
 *
 * When initialized, features can be added to or removed from the processor: when
 * the number of features to calculate is low, it can be initialized with no
 * features, and then be added some via method @ref add_feature. Conversely, if
 * the number of features is high, but not all features are needed, a processer
 * can be initialized with all features, and then be removed some of them via
 * method @ref remove_feature.
 *
 * Finally, the treebank file is processed via method @ref process. This method
 * returns a value of the enumeration @ref treebank_error.
 *
 * The usage of this class is a lot simpler than that of class
 * @ref treebank_collection_reader. For example:
 * @code
 *		treebank_processor tbproc;
 *		// initialize the processor without features (remmeber to check for errors)
 *		tbproc.init(treebank_input_file, result_filename, "Book_1");
 *		tbproc.add_feature(lal::io::treebank_feature::num_crossings);
 *		tbproc.add_feature(lal::io::treebank_feature::var_num_crossings);
 *		tbproc.process();
 *		// it is advisable to check for errors
 * @endcode
 */
class treebank_processor : public _treebank_processor_base {
public:
	// PROCESS THE TREEBANK collection

	/**
	 * @brief Initialize the processor with a new collection.
	 * @param treebank_input_file File listing all the treebanks.
	 * @param output_file File where the results are to be stored.
	 * @param treebank_id A nickname for this treebank (for example, an ISO code).
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_file_error_type::treebank_file_does_not_exist
	 */
	treebank_file_error init
	(
		const std::string& treebank_input_file,
		const std::string& output_file,
		const std::string& treebank_id = ""
	)
	noexcept;

	/**
	 * @brief Process the treebank file.
	 *
	 * This method produces the information as explained in this class'
	 * description. However, it may fail to do so. In this case it will return
	 * a value different from @ref lal::io::treebank_file_error_type::no_error.
	 *
	 * This function uses attributes @ref m_separator, @ref m_output_header to
	 * format the output data. It also outputs the current progress if
	 * @ref m_be_verbose is set to true.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_file_error_type::no_features
	 * - @ref lal::io::treebank_file_error_type::output_file_could_not_be_opened
	 * - @ref lal::io::treebank_file_error_type::treebank_file_could_not_be_opened
	 * @pre Initialisation did not return any errors.
	 */
	[[nodiscard]] treebank_file_error process() noexcept;

private:
	/// Process a single tree in a treebank.
	template <class TREE, class OUT_STREAM>
	void process_tree
	(const TREE& rT, double *props, char *prop_set, OUT_STREAM& out_lab_file)
	noexcept;

	// HEADER

	/// Output the header for the tree types
	template <class OUT_STREAM>
	void output_tree_type_header
	(OUT_STREAM& out_lab_file)
	const noexcept;

	/// Output the header for the tree types
	template <class OUT_STREAM>
	void output_syndepstruct_type_header
	(OUT_STREAM& out_lab_file)
	const noexcept;

	// VALUES

	/// Output the values for the tree types
	template <class TREE_TYPE, class OUT_STREAM>
	void output_tree_type_values
	(TREE_TYPE& t, OUT_STREAM& out_lab_file)
	const noexcept;

	/// Output the values for the syntactic dependency tree types
	template <class TREE_TYPE, class OUT_STREAM>
	void output_syndepstruct_type_values
	(const TREE_TYPE& t, uint64_t C, OUT_STREAM& out_lab_file)
	const noexcept;

private:
	/// File containing the list of languages and their treebanks.
	std::string m_treebank_filename = "none";
	/// Output directory.
	std::string m_output_file = "none";
	/// Treebank identifier.
	std::string m_treebank_id = "";

	static constexpr std::size_t n_idx = treebank_feature_to_index(treebank_feature::num_nodes);
	static constexpr std::size_t k2_idx = treebank_feature_to_index(treebank_feature::second_moment_degree);
	static constexpr std::size_t k2_out_idx = treebank_feature_to_index(treebank_feature::second_moment_degree_out);
	static constexpr std::size_t k3_idx = treebank_feature_to_index(treebank_feature::third_moment_degree);
	static constexpr std::size_t k3_out_idx = treebank_feature_to_index(treebank_feature::third_moment_degree_out);
	static constexpr std::size_t SK2_idx = treebank_feature_to_index(treebank_feature::sum_squared_degrees);
	static constexpr std::size_t SK2_out_idx = treebank_feature_to_index(treebank_feature::sum_squared_out_degrees);
	static constexpr std::size_t SK3_idx = treebank_feature_to_index(treebank_feature::sum_cubed_degrees);
	static constexpr std::size_t SK3_out_idx = treebank_feature_to_index(treebank_feature::sum_cubed_out_degrees);
	static constexpr std::size_t num_pairs_independent_edges_idx = treebank_feature_to_index(treebank_feature::num_pairs_independent_edges);
	static constexpr std::size_t head_initial_idx = treebank_feature_to_index(treebank_feature::head_initial);
	static constexpr std::size_t hubiness_idx = treebank_feature_to_index(treebank_feature::hubiness);
	static constexpr std::size_t sum_hierarchical_distance_idx = treebank_feature_to_index(treebank_feature::sum_hierarchical_distances);
	static constexpr std::size_t mean_hierarchical_distance_idx = treebank_feature_to_index(treebank_feature::mean_hierarchical_distance);
	static constexpr std::size_t tree_centre_idx = treebank_feature_to_index(treebank_feature::tree_centre);
	static constexpr std::size_t tree_centroid_idx = treebank_feature_to_index(treebank_feature::tree_centroid);
	static constexpr std::size_t tree_diameter_idx = treebank_feature_to_index(treebank_feature::tree_diameter);
	static constexpr std::size_t tree_caterpillar_distance_idx = treebank_feature_to_index(treebank_feature::tree_caterpillar_distance);
	static constexpr std::size_t C_idx = treebank_feature_to_index(treebank_feature::num_crossings);
	static constexpr std::size_t C_predicted_idx = treebank_feature_to_index(treebank_feature::predicted_num_crossings);
	static constexpr std::size_t C_expected_idx = treebank_feature_to_index(treebank_feature::exp_num_crossings);
	static constexpr std::size_t C_variance_idx = treebank_feature_to_index(treebank_feature::var_num_crossings);
	static constexpr std::size_t C_z_score_idx = treebank_feature_to_index(treebank_feature::z_score_num_crossings);
	static constexpr std::size_t D_idx = treebank_feature_to_index(treebank_feature::sum_edge_lengths);
	static constexpr std::size_t D_expected_idx = treebank_feature_to_index(treebank_feature::exp_sum_edge_lengths);
	static constexpr std::size_t D_expected_bipartite_idx = treebank_feature_to_index(treebank_feature::exp_sum_edge_lengths_bipartite);
	static constexpr std::size_t D_expected_projective_idx = treebank_feature_to_index(treebank_feature::exp_sum_edge_lengths_projective);
	static constexpr std::size_t D_expected_planar_idx = treebank_feature_to_index(treebank_feature::exp_sum_edge_lengths_planar);
	static constexpr std::size_t D_variance_idx = treebank_feature_to_index(treebank_feature::var_sum_edge_lengths);
	static constexpr std::size_t D_z_score_idx = treebank_feature_to_index(treebank_feature::z_score_sum_edge_lengths);
	static constexpr std::size_t Dmin_Unconstrained_idx = treebank_feature_to_index(treebank_feature::min_sum_edge_lengths);
	static constexpr std::size_t Dmin_Bipartite_idx = treebank_feature_to_index(treebank_feature::min_sum_edge_lengths_bipartite);
	static constexpr std::size_t Dmin_Planar_idx = treebank_feature_to_index(treebank_feature::min_sum_edge_lengths_planar);
	static constexpr std::size_t Dmin_Projective_idx = treebank_feature_to_index(treebank_feature::min_sum_edge_lengths_projective);
	static constexpr std::size_t DMax_Unconstrained_idx = treebank_feature_to_index(treebank_feature::max_sum_edge_lengths);
	static constexpr std::size_t DMax_1_thistle_idx = treebank_feature_to_index(treebank_feature::max_sum_edge_lengths_1_thistle);
	static constexpr std::size_t DMax_Bipartite_idx = treebank_feature_to_index(treebank_feature::max_sum_edge_lengths_bipartite);
	static constexpr std::size_t DMax_Planar_idx = treebank_feature_to_index(treebank_feature::max_sum_edge_lengths_planar);
	static constexpr std::size_t DMax_Projective_idx = treebank_feature_to_index(treebank_feature::max_sum_edge_lengths_projective);
	static constexpr std::size_t mean_dependency_distance_idx = treebank_feature_to_index(treebank_feature::mean_dependency_distance);
	static constexpr std::size_t flux_max_weight_idx = treebank_feature_to_index(treebank_feature::flux_max_weight);
	static constexpr std::size_t flux_mean_weight_idx = treebank_feature_to_index(treebank_feature::flux_mean_weight);
	static constexpr std::size_t flux_min_weight_idx = treebank_feature_to_index(treebank_feature::flux_min_weight);
	static constexpr std::size_t flux_max_left_span_idx = treebank_feature_to_index(treebank_feature::flux_max_left_span);
	static constexpr std::size_t flux_mean_left_span_idx = treebank_feature_to_index(treebank_feature::flux_mean_left_span);
	static constexpr std::size_t flux_min_left_span_idx = treebank_feature_to_index(treebank_feature::flux_min_left_span);
	static constexpr std::size_t flux_max_right_span_idx = treebank_feature_to_index(treebank_feature::flux_max_right_span);
	static constexpr std::size_t flux_mean_right_span_idx = treebank_feature_to_index(treebank_feature::flux_mean_right_span);
	static constexpr std::size_t flux_min_right_span_idx = treebank_feature_to_index(treebank_feature::flux_min_right_span);
	static constexpr std::size_t flux_max_RL_ratio_idx = treebank_feature_to_index(treebank_feature::flux_max_RL_ratio);
	static constexpr std::size_t flux_mean_RL_ratio_idx = treebank_feature_to_index(treebank_feature::flux_mean_RL_ratio);
	static constexpr std::size_t flux_min_RL_ratio_idx = treebank_feature_to_index(treebank_feature::flux_min_RL_ratio);
	static constexpr std::size_t flux_max_WS_ratio_idx = treebank_feature_to_index(treebank_feature::flux_max_WS_ratio);
	static constexpr std::size_t flux_mean_WS_ratio_idx = treebank_feature_to_index(treebank_feature::flux_mean_WS_ratio);
	static constexpr std::size_t flux_min_WS_ratio_idx = treebank_feature_to_index(treebank_feature::flux_min_WS_ratio);
	static constexpr std::size_t flux_max_size_idx = treebank_feature_to_index(treebank_feature::flux_max_size);
	static constexpr std::size_t flux_mean_size_idx = treebank_feature_to_index(treebank_feature::flux_mean_size);
	static constexpr std::size_t flux_min_size_idx = treebank_feature_to_index(treebank_feature::flux_min_size);
};

/**
 * @brief Automatically process a treebank.
 *
 * This function is an utility to process easily a single treebank file.
 * This function uses the class @ref lal::io::treebank_processor in order to
 * process such a file. The default values of the processor are used, i.e.,
 * all features available in @ref lal::io::treebank_feature are computed.
 * @param treebank_file The treebank file name.
 * @param output_file The output file name.
 * @returns A treebank error (see @ref lal::io::treebank_error) if any.
 */
[[nodiscard]] inline treebank_file_error process_treebank
(const std::string& treebank_file, const std::string& output_file)
noexcept
{
	treebank_processor tbproc;
	auto err = tbproc.init(treebank_file, output_file);
	if (not err.is_error()) { return err; }
	return tbproc.process();
}

} // -- namespace io
} // -- namespace lal
