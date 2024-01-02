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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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

// C includes
#include <omp.h>

// C++ includes
#include <filesystem>
#include <charconv>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/io/report_correctness.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/detail/graphs/cycles.hpp>

namespace lal {
namespace detail {

#define file_does_not_exist(F) \
"Error: Treebank '" + F + "' does not exist."

#define file_could_not_be_opened(F) \
"Error: Treebank '" + F + "' could not be opened."

#define invalid_integer(i, chunk) \
"Error: Value at position '" + std::to_string(i) + "' (value: '" + chunk + "') \
is not a valid non-negative integer number."

#define number_out_of_bounds(i) \
"Error: Number at position '" + std::to_string(i) + "' (value: \
" + std::to_string(hv[i]) + ") is out of bounds."

#define wrong_num_roots(r) \
"Error: Wrong number of roots: " + std::to_string(n_roots) + "."

#define wrong_num_edges(n, m) \
"Error: Wrong number of edges. Number of vertices is '" + std::to_string(n) + \
	"'. Number of edges is '" + std::to_string(m) + "'; " + \
	"should be '" + std::to_string(n-1) + "'."

#define graph_has_cycles \
"Error: The graph described is not a tree, i.e., it has cycles."

#define isolated_vertex(u) \
"Error: Vertex '" + std::to_string(u) + "' is isolated."

#define self_loop(pos) \
"Error: found a self-loop at position '" + std::to_string(pos) + "'."

/// Transforms a head vector in a directed graph
inline graphs::directed_graph head_vector_to_directed_graph(const head_vector& hv)
noexcept
{
	const uint64_t n = hv.size();
	graphs::directed_graph t(n);
	for (uint64_t i = 0; i < n; ++i) {
		if (hv[i] != 0) {
			// add the edge:
			// * i ranges in [0,n-1]
			// * L[i] ranges in [1,n]
			t.add_edge_bulk(i, hv[i] - 1);
		}
	}
	t.finish_bulk_add(true);
	return t;
}

/**
 * @brief Find errors in a head vector.
 *
 * The head vector may correspond to the contents of a line in a treebank file.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param hv Input head vector.
 * @param line Line number of the treebank, if appropriate.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
std::conditional_t<decide, bool, std::vector<io::report_treebank_file>>
find_errors(const head_vector& hv, const std::size_t line)
noexcept
{
	std::vector<io::report_treebank_file> treebank_err_list;

	// number of nodes of the graph
	const uint64_t n = hv.size();

	uint64_t n_roots = 0;
	bool can_make_graph = true;

	// inspect the head vector
	for (std::size_t i = 0; i < hv.size(); ++i) {
		if (hv[i] == 0) {
			++n_roots;
			continue;
		}

		// ensure that the number is within bounds
		if (hv[i] > hv.size()) {
			if constexpr (decide) { return true; }
			else {
				treebank_err_list.emplace_back(line, number_out_of_bounds(i));
				can_make_graph = false;
			}
		}
		// self loop
		else if (hv[i] == i + 1) {
			if constexpr (decide) { return true; }
			else {
				treebank_err_list.emplace_back(line, self_loop(i + 1));
				can_make_graph = false;
			}
		}
	}

	// check there is exactly one root
	if (n_roots != 1) {
		if constexpr (decide) { return true; }
		else {
			treebank_err_list.emplace_back(line, wrong_num_roots(n_roots));
		}
	}

	if (can_make_graph) {
		// ignore singleton graphs
		if (n == 1) {
			if constexpr (decide) { return false; }
			else { return treebank_err_list; }
		}

		// make a directed graph with the values
		const auto dgraph = head_vector_to_directed_graph(hv);
		const bool has_cycles = detail::has_undirected_cycles(dgraph);
		if (has_cycles) {
			if constexpr (decide) { return true; }
			else {
				treebank_err_list.emplace_back(line, graph_has_cycles);
			}
		}

		// find isolated vertices
		for (node u = 0; u < dgraph.get_num_nodes(); ++u) {
			if (dgraph.get_degree(u) == 0) {
				if constexpr (decide) { return true; }
				else {
					treebank_err_list.emplace_back(line, isolated_vertex(u));
				}
			}
		}

		// check the number of edges is correct
		if (dgraph.get_num_edges() != dgraph.get_num_nodes() - 1) {
			if constexpr (decide) { return true; }
			else {
				treebank_err_list.emplace_back
				(line, wrong_num_edges(dgraph.get_num_nodes(), dgraph.get_num_edges()));
			}
		}
	}

	if constexpr (decide) { return false; }
	else { return treebank_err_list;}
}

/**
 * @brief Find errors in a line of a treebank.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param current_line The line being analyzed.
 * @param line Line number of the treebank.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
std::conditional_t<decide, bool, std::vector<io::report_treebank_file>>
find_errors(const std::string& current_line, const std::size_t line)
noexcept
{
	std::vector<io::report_treebank_file> treebank_err_list;

	bool non_numeric_characters = false;
	head_vector hv;

	// ensure there are only numeric characters
	{
	std::size_t i = 1;
	std::stringstream ss(current_line);
	std::string chunk;
	while (ss >> chunk) {

		uint64_t value;
		const auto result = std::from_chars
		(&chunk[0], (&chunk[chunk.size() - 1]) + 1, value);

		if (result.ec == std::errc::invalid_argument) {
			if constexpr (decide) { return true; }
			else {
				treebank_err_list.emplace_back(line, invalid_integer(i, chunk));
				non_numeric_characters = true;
			}
		}
		else {
			hv.push_back(value);
		}

		++i;
	}
	}

	// if the current line contains non-numeric characters then the
	// appropriate error messages have been stored, and so we can skip
	// to the next line
	if (non_numeric_characters) {
		if constexpr (decide) { return true; }
		else { return treebank_err_list; }
	}

	if constexpr (decide) {
		return find_errors<decide>(hv, line);
	}
	else {
		auto errors = find_errors<decide>(hv, line);
		for (std::size_t i = 0; i < errors.size(); ++i) {
			treebank_err_list.emplace_back( std::move(errors[i]) );
		}
		return treebank_err_list;
	}
}

/**
 * @brief Find errors in a treebank file.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param treebank_filename Name of the treebank file.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
std::conditional_t<decide, bool, std::vector<io::report_treebank_file>>
check_correctness_treebank(const std::string& treebank_filename)
noexcept
{
	if (not std::filesystem::exists(treebank_filename)) {
		if constexpr (decide) { return true; }
		else { return {{0, file_does_not_exist(treebank_filename)}}; }
	}

	std::ifstream fin(treebank_filename);
	if (not fin.is_open()) {
		if constexpr (decide) { return true; }
		else { return {{0, file_could_not_be_opened(treebank_filename)}}; }
	}

	std::vector<io::report_treebank_file> treebank_err_list;
	std::string current_line;

	std::size_t line = 1;
	while (getline(fin, current_line)) {
		if (current_line == "") {
			// do nothing
		}
		else {
			const auto r = find_errors<decide>(current_line, line);
			if constexpr (decide) {
				// if there are errors, return
				if (r) { return true; }
			}
			else {
				// append errors to the list
				treebank_err_list.insert(
					treebank_err_list.end(), r.begin(), r.end()
				);
			}
		}

		++line;
	}

	if constexpr (decide) { return false; }
	else { return treebank_err_list; }
}

/**
 * @brief Find errors in a treebank collection.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param main_file_name Name of the collection's main file.
 * @param n_threads Number of threads that this function can use.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
std::conditional_t<decide, bool, std::vector<io::report_treebank_collection>>
check_correctness_treebank_collection
(const std::string& main_file_name, std::size_t n_threads)
noexcept
{
	if (not std::filesystem::exists(main_file_name)) {
		if constexpr (decide) { return true; }
		else { return {{"-", 0, 0, file_does_not_exist(main_file_name)}}; }
	}
	std::ifstream fin_main_file(main_file_name);
	if (not fin_main_file.is_open()) {
		if constexpr (decide) { return true; }
		else { return {{"-", 0, 0, file_could_not_be_opened(main_file_name)}}; }
	}

	std::vector<io::report_treebank_collection> dataset_err_list;
	char errors_found = 0;

	#pragma omp parallel num_threads(n_threads) shared(errors_found)
	{

	const int tid = omp_get_thread_num();
	if (tid == 0) {

	std::size_t main_file_line = 1;
	std::string id, treebankname;

	while (fin_main_file >> id >> treebankname and errors_found == 0) {
		// make full path to the treebank
		std::filesystem::path treebank_full_path(main_file_name);
		treebank_full_path.replace_filename(treebankname);
		const std::string full_path_as_string = treebank_full_path.string();

		#pragma omp task
		{

		if (errors_found == 0) {
			
			// check correctess of treebank
			const auto r =
				check_correctness_treebank<decide>(full_path_as_string);

			if constexpr (decide) {
				if (r) {
					errors_found = 1;
				}
			}
			else {
				// append errors found in the treebank to
				// the list of errors of this dataset
				#pragma omp critical
				{
				for (const auto& report_treebank : r) {
					if (report_treebank.get_line_number() > 0) {
						dataset_err_list.emplace_back(
							full_path_as_string,
							main_file_line,
							report_treebank.get_line_number(),
							report_treebank.get_error_message()
						);
					}
					else {
						const auto& err_msg = report_treebank.get_error_message();
						std::string new_err_msg;
						if (err_msg.find("exist") != std::string::npos) {
							new_err_msg = "Treebank file does not exist";
						}
						else if (err_msg.find("opened") != std::string::npos) {
							new_err_msg = "Treebank file could not be opened";
						}

						dataset_err_list.emplace_back(
							full_path_as_string,
							main_file_line,
							report_treebank.get_line_number(),
							new_err_msg
						);
					}
				}
				}
			}
		}
		}
		++main_file_line;
	}

	}
	}

	if constexpr (decide) {
		return (errors_found == 0 ? false : true);
	}
	else { return dataset_err_list; }
}

} // -- namespace detail
} // -- namespace lal
