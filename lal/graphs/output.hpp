/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
#include <ostream>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief A singleton class to enrich output of graphs.
 *
 * This class is a helper to output graphs with a tabulator string at the beginning
 * of each line.
 */
class formatted_output {
public:

	/// The string that is written at the beginning of every line.
	std::string tabulator_string;
	/// Returns the only instance of this class.
	static formatted_output& get_instance() noexcept
	{
		static formatted_output i;
		return i;
	}

private:

	/// Default constructor.
	formatted_output() noexcept = default;
};

/**
 * @brief Standard output operator for undirected graphs.
 *
 * Usable by @ref lal::graphs::undirected_graph and @ref lal::graphs::free_tree.
 *
 * This operator adds a tabulator string at the beginning of each line. See
 * @ref lal::graphs::formatted_output, and @ref.
 *
 * Example:
@code
lal::graphs::undirected_graph g = ...;
std::cout << "|   "_tab << g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const undirected_graph& g) noexcept
{
	formatted_output& f = formatted_output::get_instance();
	const uint64_t n = g.get_num_nodes();
	for (node u = 0; u < n; ++u) {
		os << f.tabulator_string << u << ':';
		for (const node v : g.get_neighbors(u)) {
			os << " " << v;
		}
		if (u < n - 1) {
			os << '\n';
		}
	}
	f.tabulator_string.resize(0);
	return os;
}

/**
 * @brief Standard output operator for directed graphs.
 *
 * Usable only by @ref lal::graphs::directed_graph.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const directed_graph& g) noexcept
{
	formatted_output& f = formatted_output::get_instance();
	const uint64_t n = g.get_num_nodes();
	os << f.tabulator_string << "out:\n";
	for (node u = 0; u < n; ++u) {
		os << f.tabulator_string << u << ':';
		for (const node v : g.get_out_neighbors(u)) {
			os << " " << v;
		}
		if (u < n - 1) {
			os << '\n';
		}
	}
	os << f.tabulator_string << "\nin:\n";
	for (node u = 0; u < n; ++u) {
		os << f.tabulator_string << u << ':';
		for (const node v : g.get_in_neighbors(u)) {
			os << " " << v;
		}
		if (u < n - 1) {
			os << '\n';
		}
	}
	f.tabulator_string.resize(0);
	return os;
}

/**
 * @brief Standard output operator for directed rooted trees.
 *
 * Usable by @ref lal::graphs::rooted_tree.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const rooted_tree& g) noexcept
{
	formatted_output& f = formatted_output::get_instance();
	const uint64_t n = g.get_num_nodes();
	const std::string_view pad =
		(g.has_root() ? std::string_view{" "} : std::string_view{""});
	os << f.tabulator_string << "out:\n";
	for (node u = 0; u < n; ++u) {
		os << f.tabulator_string
		   << (g.has_root() and u == g.get_root() ? "*" : pad) << u << ':';
		for (const node v : g.get_out_neighbors(u)) {
			os << " " << v;
		}
		if (u < n - 1) {
			os << '\n';
		}
	}
	os << f.tabulator_string << "\nin:\n";
	for (node u = 0; u < n; ++u) {
		os << f.tabulator_string
		   << (g.has_root() and u == g.get_root() ? "*" : pad) << u << ':';
		for (const node v : g.get_in_neighbors(u)) {
			os << " " << v;
		}
		if (u < n - 1) {
			os << '\n';
		}
	}
	f.tabulator_string.resize(0);
	return os;
}

/**
 * @brief Tabulator for output of graphs.
 *
 * This is simply a wrapper over std::string_view. This object, when output-ed
 * via some std::ostream object, modifies @ref lal::graphs::formatted_output
 * so that the operator<< for the various lal::graphs::* objects can use it.
 *
 * It is best to use the user-defined literal operator defined below to instantiate
 * this class.
@code
lal::graphs::undirected_graph g = ...;

// option 1
std::cout << lal::graphs::tabulator{"|   "} << g << '\n';

// option 2
std::cout << "|   "_tab << g << '\n';
@endcode
 */
struct tabulator {
	/// The string that will become the tabulator string.
	const std::string_view tabulator_string;
	tabulator(const std::string_view str) noexcept
		: tabulator_string(str)
	{ }
};

/**
 * @brief Operator << for @ref lal::graphs::tabulator.
 *
 * This simply modifies the tabulator string in the class @ref lal::graphs::formatted_output.
 * @param os std::ostream C++ object.
 * @param t Tabulator object.
 * @returns std::ostream object.
 */
inline std::ostream& operator<< (std::ostream& os, const tabulator& t) noexcept
{
	formatted_output& ti = formatted_output::get_instance();
	ti.tabulator_string = std::string(t.tabulator_string);
	return os;
}

} // namespace graphs
} // namespace lal

/**
 * @brief Operator to aid in output tabulation of graphs.
 *
 * Example usage:
 * @code
lal::graphs::undirected_graph g = ...;
std::cout << "|   "_tab << g << '\n';
@endcode
 * @param str The tabulator string. Can be any string.
 * @param s Size.
 * @returns An instance of @ref lal::graphs::tabulator.
 */
[[nodiscard]] inline lal::graphs::tabulator
operator""_tab (const char *str, const std::size_t s) noexcept
{
	return lal::graphs::tabulator{std::string_view{str, s}};
}
