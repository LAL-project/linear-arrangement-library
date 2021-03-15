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

// C++ includes
#include <filesystem>
#include <algorithm>
#include <charconv>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/io/report_correctness.hpp>
#include <lal/internal/graphs/cycles.hpp>

namespace lal {
using namespace graphs;

namespace io {

inline directed_graph head_vector_to_directed_graph(const head_vector& hv) noexcept
{
	const uint32_t n = static_cast<uint32_t>(hv.size());
	directed_graph t(n);
	for (uint32_t i = 0; i < n; ++i) {
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

#define file_does_not_exist(F) \
"Error: Treebank '" + F + "' does not exist."

#define file_could_not_be_opened(F) \
"Error: Treebank '" + F + "' could not be opened."

#define invalid_integer(i, chunk) \
"Error: Value at position '" + std::to_string(i) + "' (value: '" + chunk + "') is not a valid integer number."

#define number_out_of_bounds(i) \
"Error: Number at position '" + std::to_string(i) + "' (value: " + std::to_string(hv[i]) + ") is out of bounds."

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

#define empty_line \
"Warning: Empty line found"

// line, what
vector<report_treebank_file>
check_correctness_treebank(const string& treebank_filename)
noexcept
{
	if (not filesystem::exists(treebank_filename)) {
		return {{0, file_does_not_exist(treebank_filename)}};
	}

	ifstream fin(treebank_filename);
	if (not fin.is_open()) {
		return {{0, file_could_not_be_opened(treebank_filename)}};
	}

	vector<report_treebank_file> treebank_err_list;
	string current_line;

	size_t line = 1;
	while (getline(fin, current_line)) {
		if (current_line == "") {
			treebank_err_list.emplace_back(line, empty_line);
		}
		else {
			bool non_numeric_characters = false;
			head_vector hv;

			// ensure there are only numeric characters
			{
			size_t i = 1;
			stringstream ss(current_line);
			string chunk;
			while (ss >> chunk) {

				uint32_t value;
				const auto result = std::from_chars
				(&chunk[0], (&chunk[chunk.size() - 1]) + 1, value);

				if (result.ec == std::errc::invalid_argument) {
					treebank_err_list.emplace_back
					(line, invalid_integer(i, chunk));
					non_numeric_characters = true;
				}
				else {
					hv.push_back(value);
				}

				++i;
			}
			}

			if (not non_numeric_characters) {
				uint32_t n_roots = 0;
				bool can_make_graph = true;

				// inspect the head vector
				for (size_t i = 0; i < hv.size(); ++i) {
					if (hv[i] == 0) { ++n_roots; }
					else {
						// ensure that the number is within bounds
						if (hv[i] > hv.size()) {
							treebank_err_list.emplace_back
							(line, number_out_of_bounds(i));
							can_make_graph = false;
						}
						else if (hv[i] == i + 1) {
							treebank_err_list.emplace_back
							(line, self_loop(i + 1));
							can_make_graph = false;
						}
					}
				}

				// check there is exactly one root
				if (n_roots != 1) {
					treebank_err_list.emplace_back
					(line, wrong_num_roots(n_roots));
				}

				if (can_make_graph) {
					// make a directed graph with the values
					const auto dgraph = head_vector_to_directed_graph(hv);
					if (internal::has_undirected_cycles(dgraph)) {
						treebank_err_list.emplace_back
						(line, graph_has_cycles);
					}

					// find isolated vertices
					for (node u = 0; u < dgraph.num_nodes(); ++u) {
						if (dgraph.degree(u) == 0) {
							treebank_err_list.emplace_back
							(line, isolated_vertex(u));
						}
					}

					if (dgraph.num_edges() != dgraph.num_nodes() - 1) {
						treebank_err_list.emplace_back
						(line, wrong_num_edges(dgraph.num_nodes(), dgraph.num_edges()));
					}
				}
			}
		}

		++line;
	}

	return treebank_err_list;
}

// file, line, what
vector<report_treebank_dataset>
check_correctness_treebank_dataset(const string& main_file_name)
noexcept
{
	if (not filesystem::exists(main_file_name)) {
		return {{"-", 0, 0, file_does_not_exist(main_file_name)}};
	}
	ifstream fin_main_file(main_file_name);
	if (not fin_main_file.is_open()) {
		return {{"-", 0, 0, file_could_not_be_opened(main_file_name)}};
	}

	vector<report_treebank_dataset> dataset_err_list;

	size_t main_file_line = 1;
	string id, treebankname;
	while (fin_main_file >> id >> treebankname) {
		// build path to the treebank file

		filesystem::path treebank_full_path(main_file_name);
		treebank_full_path.replace_filename(treebankname);

		// make full path to the treebank
		const auto treebank_err_list =
			check_correctness_treebank(treebank_full_path);

		// append errors found in the treebank to
		// the list of errors of this dataset
		for (const auto& report_treebank : treebank_err_list) {
			if (report_treebank.get_line_number() > 0) {
				dataset_err_list.emplace_back(
					treebank_full_path,
					main_file_line,
					report_treebank.get_line_number(),
					report_treebank.get_error_message()
				);
			}
			else {
				const auto& err_msg = report_treebank.get_error_message();
				string new_err_msg;
				if (err_msg.find("exist") != std::string::npos) {
					new_err_msg = "Treebank file does not exist";
				}
				else if (err_msg.find("opened") != std::string::npos) {
					new_err_msg = "Treebank file could not be opened";
				}

				dataset_err_list.emplace_back(
					treebank_full_path,
					main_file_line,
					report_treebank.get_line_number(),
					new_err_msg
				);
			}
		}

		++main_file_line;
	}

	return dataset_err_list;
}

} // -- namespace io
} // -- namespace lal
