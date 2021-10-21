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

#include <lal/io/treebank_collection_processor.hpp>

// C includes
#include <omp.h>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>

// lal includes
#include <lal/io/treebank_collection_reader.hpp>
#include <lal/io/treebank_processor.hpp>
#include <lal/io/treebank_reader.hpp>
#include <lal/detail/io/check_correctness.hpp>

#define feature_to_str(i) detail::treebank_feature_string(static_cast<treebank_feature>(i))

inline
std::string make_result_file_name(const std::string& treebank_name) noexcept
{
	return treebank_name + ".csv";
}

inline
std::string name_of_file_without_path_extension(const std::string& file_name)
noexcept
{
	std::filesystem::path p(file_name);
	p.replace_extension("");
	return p.filename().string();
}

namespace lal {
namespace io {

// CLASS METHODS

treebank_error treebank_collection_processor::init
(const std::string& file, const std::string& odir)
noexcept
{
	// initialise data
	m_all_individual_treebank_ids.clear();
	m_main_file = file;
	m_out_dir = odir;

	// initalise features vector
	std::fill(m_what_fs.begin(), m_what_fs.end(), true);

	// initialise column names
	initialise_column_names();

	// make sure main file exists
	if (not std::filesystem::exists(m_main_file)) {
		return treebank_error(
			"Treebank collection main file '" + m_main_file + "' does not exist.",
			treebank_error_type::main_file_does_not_exist
		);
	}
	// check whether output directory exists or not
	if (m_out_dir != "." and not std::filesystem::exists(m_out_dir)) {
		// if it does not exist, create it (...?)
		const auto r = std::filesystem::create_directory(m_out_dir);
		if (not r) {
			return treebank_error(
				"Output directory '" + m_out_dir + "' could not be created.",
				treebank_error_type::output_directory_could_not_be_created
			);
		}
	}
	return treebank_error("", treebank_error_type::no_error);
}

treebank_error treebank_collection_processor::process() noexcept
{
	m_errors_from_processing.clear();

	if (m_check_before_process) {
		const bool err =
		detail::check_correctness_treebank_collection<true>
		(m_main_file, m_num_threads);

		if (err) {
			return treebank_error(
				"The treebank collection '" + m_main_file + "' contains errors.",
				treebank_error_type::malformed_treebank_collection
			);
		}
	}

	// -- this function assumes that init did not return any error -- //

	// check that there is something to be computed
	if (std::all_of(m_what_fs.begin(),m_what_fs.end(),[](bool x){return not x;})) {
		return treebank_error(
			"No features to be computed. Nothing to do.",
			treebank_error_type::no_features
		);
	}

	// Stream object to read the main file.
	std::ifstream main_file_reader(m_main_file);
	if (not main_file_reader.is_open()) {
		return treebank_error(
			"Main file '" + m_main_file + "' could not be opened.",
			treebank_error_type::main_file_could_not_be_opened
		);
	}

	// process dataset using treebank_dataset class
	#pragma omp parallel num_threads(m_num_threads)
	{
	const int tid = omp_get_thread_num();

	if (tid == 0) {
		std::string treebank_id, treebank_filename;

		// this is executed only by the main thread
		while (main_file_reader >> treebank_id >> treebank_filename) {

			// full path to the treebank file
			std::filesystem::path treebank_file_full_path(m_main_file);
			treebank_file_full_path.replace_filename(treebank_filename);

			// full path to the output file corresponding to this treebank
			std::filesystem::path output_file_full_path(m_out_dir);
			output_file_full_path /= make_result_file_name(treebank_id);

			// store the name of the treebank so that we can join the files later
			m_all_individual_treebank_ids.push_back(treebank_id);

			// Launch a task consisting of processing a treebank, catching
			// error, and storing it into 'm_errors_from_processing'.
			#pragma omp task
			{
				// declare and initialise treebank processor
				treebank_processor tbproc;
				tbproc.set_check_before_process(false);
				tbproc.init(
					treebank_file_full_path.string(),
					output_file_full_path.string(),
					treebank_id
				);
				tbproc.clear_features();
				tbproc.set_output_header(m_output_header);
				tbproc.set_separator(m_separator);
				tbproc.set_verbosity(m_be_verbose);

				// add features in this treebank collection processor
				for (size_t i = 0; i < __treebank_feature_size; ++i) {
					if (m_what_fs[i]) {
						tbproc.add_feature(index_to_treebank_feature(i));
					}
					tbproc.set_column_name
					(index_to_treebank_feature(i), m_column_names[i]);
				}

				// process the treebank file
				const auto err = tbproc.process();
				if (err != treebank_error_type::no_error) {
					#pragma omp critical
					{
					m_errors_from_processing.push_back(make_tuple(
						err,
						treebank_file_full_path.string(),
						treebank_id
					));
					}
				}
			}
		}
	}
	}

	if (m_join_files) {
		const auto err = join_all_files();
		if (err != treebank_error_type::no_error) {
			m_errors_from_processing.push_back(make_tuple(
				err,
				m_main_file,
				"treebank collection main file"
			));
		}
	}

	return
	(m_errors_from_processing.size() > 0 ?
		treebank_error(
			"There were errors in processing the treebank collection '" + m_main_file + "'.",
			treebank_error_type::some_treebank_file_failed
		)
		:
		treebank_error("", treebank_error_type::no_error)
	);
}

treebank_error treebank_collection_processor::join_all_files() const noexcept
{
	// use the filesystem namespace to create the path properly
	std::filesystem::path p;
	if (m_join_to_file == "") {
		p = std::filesystem::path(m_out_dir);
		p /= name_of_file_without_path_extension(m_main_file) + "_full.csv";
	}
	else {
		p = std::filesystem::path(m_join_to_file);
	}

	if (m_be_verbose >= 1) {
		std::cout << "Gather all results into file: " << p.string() << '\n';
	}

	// the file where the contents of all the individual files are dumped to
	std::ofstream output_together(p.string());
	if (not output_together.is_open()) {
		return treebank_error(
			"Output join file '" + p.string() + "' could not be opened.",
			treebank_error_type::output_join_file_could_not_be_opened
		);
	}

	bool first_time_encounter_header = true;

	// read all files and dump their contents into
	for (size_t i = 0; i < m_all_individual_treebank_ids.size(); ++i) {
		const std::string& name_of_treebank = m_all_individual_treebank_ids[i];

		std::filesystem::path path_to_treebank_result(m_out_dir);
		path_to_treebank_result /= make_result_file_name(name_of_treebank);

		if (m_be_verbose >= 1) {
			std::cout << "    "
				 << path_to_treebank_result.string()
				 << '\n';
		}

		std::ifstream fin(path_to_treebank_result);
		if (not fin.is_open()) {
			return treebank_error(
				"Treebank result file '" + path_to_treebank_result.string() + "' could not be opened.",
				treebank_error_type::treebank_result_file_could_not_be_opened
			);
		}

		std::string line;
		bool first_line = true;

		while (getline(fin, line)) {
			// ignore the first line
			if (first_line) {
				first_line = false;

				if (first_time_encounter_header) {
					if (m_output_header) {
						output_together
							<< m_treebank_column_name << m_separator
							<< line
							<< '\n';
					}
					first_time_encounter_header = false;
				}

				continue;
			}

			output_together
				<< name_of_treebank << m_separator << line
				<< '\n';
		}

		fin.close();

		const bool success = std::filesystem::remove(path_to_treebank_result);
		if (not success and m_be_verbose >= 2) {
			std::cerr << "Treebank result file '"
				 << path_to_treebank_result
				 << "' could not be removed."
				 << '\n';
		}
	}

	output_together.close();

	return treebank_error("", treebank_error_type::no_error);
}

} // -- namespace io
} // -- namespace lal
