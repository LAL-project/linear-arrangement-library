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
#include <lal/properties/Q.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/io/treebank_collection_reader.hpp>
#include <lal/io/treebank_reader.hpp>
#include <lal/internal/io/treebank_feature_string.hpp>

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

#define index_of(_X_) static_cast<size_t>(treebank_feature::_X_)
#define n_idx index_of(num_nodes)
#define k2_idx index_of(k2)
#define k3_idx index_of(k3)
#define size_Q_idx index_of(num_pairs_independent_edges)
#define headedness_idx index_of(headedness)
#define mean_hierarchical_distance_idx index_of(mean_hierarchical_distance)
#define mean_dependency_distance_idx index_of(mean_dependency_distance)
#define C_idx index_of(C)
#define C_expected_idx index_of(C_expected)
#define C_variance_idx index_of(C_variance)
#define C_z_score_idx index_of(C_z_score)
#define D_idx index_of(D)
#define D_expected_idx index_of(D_expected)
#define D_variance_idx index_of(D_variance)
#define D_z_score_idx index_of(D_z_score)
#define Dmin_Unconstrained_idx index_of(Dmin)
#define Dmin_Planar_idx index_of(Dmin_Planar)
#define Dmin_Projective_idx index_of(Dmin_Projective)
#define flux_max_weight_idx index_of(flux_max_weight)
#define flux_mean_weight_idx index_of(flux_mean_weight)
#define flux_min_weight_idx index_of(flux_min_weight)
#define flux_max_left_span_idx index_of(flux_max_left_span)
#define flux_mean_left_span_idx index_of(flux_mean_left_span)
#define flux_min_left_span_idx index_of(flux_min_left_span)
#define flux_max_right_span_idx index_of(flux_max_right_span)
#define flux_mean_right_span_idx index_of(flux_mean_right_span)
#define flux_min_right_span_idx index_of(flux_min_right_span)
#define flux_max_RL_ratio_idx index_of(flux_max_RL_ratio)
#define flux_mean_RL_ratio_idx index_of(flux_mean_RL_ratio)
#define flux_min_RL_ratio_idx index_of(flux_min_RL_ratio)
#define flux_max_WS_ratio_idx index_of(flux_max_WS_ratio)
#define flux_mean_WS_ratio_idx index_of(flux_mean_WS_ratio)
#define flux_min_WS_ratio_idx index_of(flux_min_WS_ratio)
#define flux_max_size_idx index_of(flux_max_size)
#define flux_mean_size_idx index_of(flux_mean_size)
#define flux_min_size_idx index_of(flux_min_size)

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

	// make sure that the treebank file exists
	if (not filesystem::exists(m_treebank_filename)) {
		return treebank_error::treebank_file_does_not_exist;
	}
	return treebank_error::no_error;
}

treebank_error treebank_processor::process() noexcept {
	// check that there is something to be computed
	if (std::all_of(m_what_fs.begin(),m_what_fs.end(),[](bool x){return not x;}))
	{ return treebank_error::no_features; }

	// iterate to next language
	treebank_reader tbread;
	{
	const auto err = tbread.init(m_treebank_filename, m_treebank_id);
	if (err != treebank_error::no_error) {
		if (m_be_verbose >= 2) {
			cerr << "Processing treebank '" << m_treebank_filename << "' failed" << endl;
		}
		return err;
	}
	}

	// output file stream:
	// since the output directory exists there is no need to check for is_open()
	ofstream out_lang_file(m_output_file);
	if (not out_lang_file.is_open()) {
		return treebank_error::output_file_could_not_be_opened;
	}

	// output header to the file
	if (m_output_header) {
		size_t i = 0;

		// find the first feature
		while (not m_what_fs[i]) { ++i; }

		out_lang_file << internal::treebank_feature_string(
							 static_cast<treebank_feature>(i)
						 );
		++i;

		for (; i < m_what_fs.size(); ++i) {
			if (m_what_fs[i]) {
				out_lang_file
					<< m_separator
					<< internal::treebank_feature_string(
						   static_cast<treebank_feature>(i)
					   );
			}
		}
		out_lang_file << endl;
	}

	const auto start = std::chrono::system_clock::now();

	// process the current treebank
	rooted_tree rT;
	while (tbread.has_tree()) {
		tbread.next_tree();
		rT = tbread.get_tree();
		process_tree<rooted_tree, ofstream>(rT, out_lang_file);
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

template<class TREE, class OUT_STREAM>
void treebank_processor::process_tree
(const TREE& rT, OUT_STREAM& out_lang_file)
const
{
	const free_tree fT = rT.to_undirected();
	const uint32_t n = fT.get_num_nodes();

	// -----------------------------------------------------------
	// compute features in a way that does not repeat computations
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

	if (m_what_fs[0]) {
		out_lang_file << rT.get_num_nodes();
	}
	for (size_t i = 1; i < m_what_fs.size(); ++i) {
		if (m_what_fs[i]) {
			out_lang_file << m_separator;
			out_lang_file << props[i];
		}
	}
	out_lang_file << "\n";
}

} // -- namespace io
} // -- namespace lal
