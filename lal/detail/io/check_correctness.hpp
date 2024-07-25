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

// C includes
#include <omp.h>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <filesystem>
#include <charconv>
#include <fstream>
#include <sstream>
#include <string>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/io/head_vector_error.hpp>
#include <lal/io/treebank_file_error.hpp>
#include <lal/io/treebank_file_report.hpp>
#include <lal/io/treebank_collection_report.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/detail/graphs/cycles.hpp>
#include <lal/detail/graphs/conversions.hpp>

namespace lal {
namespace detail {

#define file_does_not_exist(F) \
"Error: Treebank '" + F + "' does not exist."

#define file_could_not_be_opened(F) \
"Error: Treebank '" + F + "' could not be opened."

#define invalid_integer(i, chunk) \
"Error: Value at position '" + std::to_string(i) + "' (value: '" + chunk + "') \
is not a valid non-negative integer number."

#define head_out_of_bounds(i) \
"Error: Head index at position '" + std::to_string(i) + "' (value: \
" + std::to_string(hv[i]) + ") is out of bounds."

#define wrong_num_roots(r) \
"Error: Wrong number of roots: " + std::to_string(n_roots) + "."

#define wrong_num_edges(n, m) \
"Error: Wrong number of edges. Number of vertices is '" + std::to_string(n) + \
	"'. Number of edges is '" + std::to_string(m) + "'; " + \
	"should be '" + std::to_string(n-1) + "'."

#define graph_has_cycles_msg \
"Error: The graph described is not a tree, i.e., it has cycles."

#define isolated_vertex(u) \
"Error: Vertex '" + std::to_string(u) + "' is isolated."

#define self_loop(pos) \
"Error: found a self-loop at position '" + std::to_string(pos) + "'."

/**
 * @brief Find errors in a head vector.
 *
 * The head vector may correspond to the contents of a line in a treebank file.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param hv Input head vector.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
[[nodiscard]] std::conditional_t<decide, bool, std::vector<io::head_vector_error>>
find_errors(const head_vector& hv)
noexcept
{
	std::vector<io::head_vector_error> error_list;

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
				error_list.emplace_back(
					head_out_of_bounds(i),
					io::head_vector_error_type::head_out_bounds
				);
				can_make_graph = false;
			}
		}
		// self loop
		else if (hv[i] == i + 1) {
			if constexpr (decide) { return true; }
			else {
				error_list.emplace_back(
					self_loop(i + 1),
					io::head_vector_error_type::self_loop
				);
				can_make_graph = false;
			}
		}
	}

	// check there is exactly one root
	if (n_roots != 1) {
		if constexpr (decide) { return true; }
		else {
			error_list.emplace_back(
				wrong_num_roots(n_roots),
				io::head_vector_error_type::wrong_number_of_roots
			);
		}
	}

	if (can_make_graph) {
		// ignore singleton graphs
		if (n == 1) {
			if constexpr (decide) { return false; }
			else { return error_list; }
		}

		// make a directed graph with the values
		const auto dgraph = from_head_vector_to_graph<graphs::directed_graph>(hv,false,false);
		{
		const bool has_cycles = detail::has_undirected_cycles(dgraph);
		if (has_cycles) {
			if constexpr (decide) { return true; }
			else {
				error_list.emplace_back(
					graph_has_cycles_msg,
					io::head_vector_error_type::graph_has_cycles
				);
			}
		}
		}

		// find isolated vertices
		for (node u = 0; u < dgraph.get_num_nodes(); ++u) {
			if (dgraph.get_degree(u) == 0) {
				if constexpr (decide) { return true; }
				else {
					error_list.emplace_back(
						isolated_vertex(u),
						io::head_vector_error_type::isolated_vertex
					);
				}
			}
		}

		// check the number of edges is correct
		if (dgraph.get_num_edges() != dgraph.get_num_nodes() - 1) {
			if constexpr (decide) { return true; }
			else {
				error_list.emplace_back(
					wrong_num_edges(dgraph.get_num_nodes(), dgraph.get_num_edges()),
					io::head_vector_error_type::wrong_number_of_edges
				);
			}
		}
	}

	if constexpr (decide) { return false; }
	else { return error_list;}
}

/**
 * @brief Find errors in a line of a treebank.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param current_line The line being analyzed.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
[[nodiscard]] std::conditional_t<decide, bool, std::vector<io::head_vector_error>>
find_errors(const std::string& current_line)
noexcept
{
	std::vector<io::head_vector_error> error_list;

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
				error_list.emplace_back(
					invalid_integer(i, chunk),
					io::head_vector_error_type::invalid_integer
				);
				non_numeric_characters = true;
			}
		}
		else {
			hv.push_back(value);
		}

		++i;
	}
	}

	// if the current line contains non-numeric characters then the appropriate
	// error messages have been stored, and so we can skip to the next line
	if (non_numeric_characters) {
		if constexpr (decide) { return true; }
		else { return error_list; }
	}

	// if the program reaches this point, the vector 'treebank_err_list' is empty.
#if defined DEBUG
	assert(error_list.size() == 0);
#endif
	return find_errors<decide>(hv);
}

/**
 * @brief Find errors in a treebank file.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param treebank_filename Name of the treebank file.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
[[nodiscard]] std::conditional_t<decide, bool, io::treebank_file_report>
check_correctness_treebank(const std::string& treebank_filename)
noexcept
{
	if (not std::filesystem::exists(treebank_filename)) {
		if constexpr (decide) { return true; }
		else {
			return io::treebank_file_report({
				file_does_not_exist(treebank_filename),
				io::treebank_file_error_type::treebank_file_does_not_exist
			});
		}
	}

	std::ifstream fin(treebank_filename);
	if (not fin.is_open()) {
		if constexpr (decide) { return true; }
		else {
			return io::treebank_file_report({
				file_could_not_be_opened(treebank_filename),
				io::treebank_file_error_type::treebank_result_file_could_not_be_opened
			});
		}
	}

	io::treebank_file_report report;
	report.set_treebank_error({"", io::treebank_file_error_type::no_error});

	std::string current_line;

	std::size_t line = 1;
	while (getline(fin, current_line)) {
		if (current_line == "") {
			// do nothing
		}
		else {
			auto r = find_errors<decide>(current_line);
			if constexpr (decide) {
				// if there are errors, return
				if (r) { return true; }
			}
			else {
				// append errors to the list
				for (io::head_vector_error& err : r) {
					report.add_error(line, std::move(err));
				}
			}
		}

		++line;
	}

	if constexpr (decide) { return false; }
	else { return report; }
}

/**
 * @brief Find errors in a treebank collection.
 * @tparam decide When true, return a value as soon as an error is found.
 * @param main_file_name Name of the collection's main file.
 * @param n_threads Number of threads that this function can use.
 * @returns A Boolean if @e decide is true, a list of errors if otherwise.
 */
template <bool decide>
[[nodiscard]] std::conditional_t<decide, bool, io::treebank_collection_report>
check_correctness_treebank_collection
(const std::string& main_file_name, std::size_t n_threads)
noexcept
{
	if (not std::filesystem::exists(main_file_name)) {
		if constexpr (decide) { return true; }
		else {
			return io::treebank_collection_report({
				file_does_not_exist(main_file_name),
				io::treebank_file_error_type::main_file_does_not_exist
			});
		}
	}
	std::ifstream fin_main_file(main_file_name);
	if (not fin_main_file.is_open()) {
		if constexpr (decide) { return true; }
		else {
			return io::treebank_collection_report({
				file_could_not_be_opened(main_file_name),
				io::treebank_file_error_type::main_file_could_not_be_opened
			});
		}
	}

	io::treebank_collection_report report;
	report.set_treebank_error({"", io::treebank_file_error_type::no_error});
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
			auto r = check_correctness_treebank<decide>(full_path_as_string);

			if constexpr (decide) {
				if (r) {
					#pragma omp critical
					errors_found = 1;
				}
			}
			else {
				// append errors found in the treebank to
				// the list of errors of this dataset
				if (r.get_num_errors() > 0) {
					#pragma omp critical
					{
						report.add_report(
							main_file_line,
							std::move(treebankname),
							std::move(id),
							std::move(r)
						);
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
	else { return report; }
}

} // -- namespace detail
} // -- namespace lal
