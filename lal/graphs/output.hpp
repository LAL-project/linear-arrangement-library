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
#include <format>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/utilities/decorator.hpp>

#if not defined LAL_PYTHON

/// Specialization of std::formatter
template <>
struct std::formatter<lal::utilities::decorator<lal::graphs::undirected_graph>>
	: std::formatter<std::string> {
	auto format(
		const lal::utilities::decorator<lal::graphs::undirected_graph>& dec,
		std::format_context& ctx
	) const
	{
		auto out = ctx.out();
		std::string_view prefix = dec.prefix;
		const auto& g = dec.value;

		const uint64_t n = g.get_num_nodes();
		for (lal::node u = 0; u < n; ++u) {
			std::format_to(out, "{}{}:", prefix, u);
			for (const lal::node v : g.get_neighbors(u)) {
				std::format_to(out, " {}", v);
			}
			if (u < n - 1) {
				std::format_to(out, "\n");
			}
		}
		return out;
	}
};

#endif

/**
 * @brief Standard output operator for undirected graphs and free trees.
 *
 * Usable by @ref lal::graphs::undirected_graph and @ref lal::graphs::free_tree.
 *
 * This operator writes a graph from a @ref lal::utilities::decorator object:
 * every new line starts with adds a prefix string. The suffix string is not
 * used in this operator.
 *
 * Usage example:
@code
lal::graphs::undirected_graph g = ...;
std::cout << "|   " + g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input decorated graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (
	std::ostream& os,
	const lal::utilities::decorator<lal::graphs::undirected_graph>& dec
) noexcept
{
	std::format_to(std::ostreambuf_iterator<char>(os), "{}", dec);
	return os;
}

/**
 * @brief Standard output operator for undirected graphs.
 *
 * Usable by @ref lal::graphs::undirected_graph and @ref lal::graphs::free_tree.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const lal::graphs::undirected_graph& g) noexcept
{
	os << lal::utilities::decorator{.prefix = "", .value = g, .suffix = ""};
	return os;
}

#if not defined LAL_PYTHON

/**
 * @brief Standard output operator for directed graphs.
 *
 * This operator writes a graph from a @ref lal::utilities::decorator object:
 * every new line starts with adds a prefix string. The suffix string is not
 * used in this operator.
 *
 * Usage example:
@code
lal::graphs::directed_graph g = ...;
std::cout << "|   " + g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input decorated graph.
 * @returns The output stream.
 */
template <>
struct std::formatter<lal::utilities::decorator<lal::graphs::directed_graph>>
	: std::formatter<std::string> {
	auto format(
		const lal::utilities::decorator<lal::graphs::directed_graph>& dec,
		std::format_context& ctx
	) const
	{
		auto out = ctx.out();
		std::string_view prefix = dec.prefix;
		const auto& g = dec.value;

		const uint64_t n = g.get_num_nodes();
		std::format_to(out, "{}out:\n", prefix);
		for (lal::node u = 0; u < n; ++u) {
			std::format_to(out, "{}{}:", prefix, u);
			for (const lal::node v : g.get_out_neighbors(u)) {
				std::format_to(out, " {}", v);
			}
			if (u < n - 1) {
				std::format_to(out, "\n");
			}
		}
		std::format_to(out, "\n{}in:\n", prefix);
		for (lal::node u = 0; u < n; ++u) {
			std::format_to(out, "{}{}:", prefix, u);
			for (const lal::node v : g.get_in_neighbors(u)) {
				std::format_to(out, " {}", v);
			}
			if (u < n - 1) {
				std::format_to(out, "\n");
			}
		}
		return out;
	}
};

#endif

/**
 * @brief Standard output operator for directed graphs.
 *
 * This operator writes a graph from a @ref lal::utilities::decorator object:
 * every new line starts with adds a prefix string. The suffix string is not
 * used in this operator.
 *
 * Usage example:
@code
lal::graphs::directed_graph g = ...;
std::cout << "|   " + g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input decorated graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (
	std::ostream& os,
	const lal::utilities::decorator<lal::graphs::directed_graph>& dec
) noexcept
{
	std::format_to(std::ostreambuf_iterator<char>(os), "{}", dec);
	return os;
}

/**
 * @brief Standard output operator for directed graphs.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const lal::graphs::directed_graph& g) noexcept
{
	os << lal::utilities::decorator{.prefix = "", .value = g, .suffix = ""};
	return os;
}

#if not defined LAL_PYTHON

/**
 * @brief Standard output operator for rooted trees.
 *
 * This operator writes a graph from a @ref lal::utilities::decorator object:
 * every new line starts with adds a prefix string. The suffix string is not
 * used in this operator.
 *
 * Usage example:
@code
lal::graphs::rooted_tree g = ...;
std::cout << "|   " + g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input decorated graph.
 * @returns The output stream.
 */
template <>
struct std::formatter<lal::utilities::decorator<lal::graphs::rooted_tree>>
	: std::formatter<std::string> {
	auto format(
		const lal::utilities::decorator<lal::graphs::rooted_tree>& dec,
		std::format_context& ctx
	) const
	{
		auto out = ctx.out();
		std::string_view prefix = dec.prefix;
		const auto& g = dec.value;

		const uint64_t n = g.get_num_nodes();
		const std::string_view pad =
			(g.has_root() ? std::string_view{" "} : std::string_view{""});
		std::format_to(out, "{}out:\n", prefix);
		for (lal::node u = 0; u < n; ++u) {
			std::format_to(
				out,
				"{}{}{}:",
				prefix,
				(g.has_root() and u == g.get_root() ? "*" : pad),
				u
			);

			for (const lal::node v : g.get_out_neighbors(u)) {
				std::format_to(out, " {}", v);
			}
			if (u < n - 1) {
				std::format_to(out, "\n");
			}
		}
		std::format_to(out, "\n{}in:\n", prefix);
		for (lal::node u = 0; u < n; ++u) {
			std::format_to(
				out,
				"{}{}{}:",
				prefix,
				(g.has_root() and u == g.get_root() ? "*" : pad),
				u
			);
			for (const lal::node v : g.get_in_neighbors(u)) {
				std::format_to(out, " {}", v);
			}
			if (u < n - 1) {
				std::format_to(out, "\n");
			}
		}
		return out;
	}
};

#endif

/**
 * @brief Standard output operator for rooted trees.
 *
 * This operator writes a graph from a @ref lal::utilities::decorator object:
 * every new line starts with adds a prefix string. The suffix string is not
 * used in this operator.
 *
 * Usage example:
@code
lal::graphs::roted_tree g = ...;
std::cout << "|   " + g << '\n';
@endcode
 *
 * @param os ostream C++ object.
 * @param g Input decorated graph.
 * @returns The output stream.
 */
inline std::ostream& operator<< (
	std::ostream& os,
	const lal::utilities::decorator<lal::graphs::rooted_tree>& dec
) noexcept
{
	std::format_to(std::ostreambuf_iterator<char>(os), "{}", dec);
	return os;
}

/**
 * @brief Standard output operator for rooted trees.
 * @param os ostream C++ object.
 * @param g Input graph.
 * @returns The output stream.
 */
inline std::ostream&
operator<< (std::ostream& os, const lal::graphs::rooted_tree& g) noexcept
{
	os << lal::utilities::decorator{.prefix = "", .value = g, .suffix = ""};
	return os;
}
