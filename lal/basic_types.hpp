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

#pragma once

// C++ includes
#include <cinttypes>
#include <vector>

namespace lal {

/// Node type. See @ref LAL_concepts__node page for further details.
typedef uint64_t node;
/// Node's position type.
typedef uint64_t position;

/// See @ref LAL_concepts__edge page for further details.
typedef std::pair<node, node> edge;
/// Edge pair type.
typedef std::pair<edge,edge> edge_pair;
/// List of nodes.
typedef std::vector<node> neighbourhood;
/// See @ref LAL_concepts__head_vector page for further details.
typedef std::vector<uint64_t> head_vector;

/// Typesafe @ref node type.
struct node_t {
	uint64_t value;

	// CONSTRUCTORS

	node_t() = default;

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t(const T& _v) noexcept : value(_v) { }

	// -- ASSIGNMENT OPERATORS --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t& operator= (const T& _v) noexcept { value = _v; return *this; }

	// -- COMPARISONS --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator== (const T& t) const noexcept { return value == t; }
	bool operator== (const node_t& u) const noexcept { return value == u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator== (const T& t, const node_t& u) noexcept { return u.value == t; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator!= (const T& t) const noexcept { return value != t; }
	bool operator!= (const node_t& u) const noexcept { return value != u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator!= (const T& t, const node_t& u) noexcept { return u.value != t; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator< (const T& t) const noexcept { return value < t; }
	bool operator< (const node_t& u) const noexcept { return value < u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator< (const T& t, const node_t& u) noexcept { return t < u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator<= (const T& t) const noexcept { return value <= t; }
	bool operator<= (const node_t& u) const noexcept { return value <= u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator<= (const T& t, const node_t& u) noexcept { return t <= u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator> (const T& t) const noexcept { return value > t; }
	bool operator> (const node_t& u) const noexcept { return value > u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator> (const T& t, const node_t& u) noexcept { return t > u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator>= (const T& t) const noexcept { return value >= t; }
	bool operator>= (const node_t& u) const noexcept { return value >= u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator>= (const T& t, const node_t& u) noexcept { return t >= u.value; }

	// -- ARITHMETIC --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t operator+ (const T& t) const noexcept { return node_t{value + t}; }
	node_t operator+ (const node_t& u) const noexcept { return node_t{value + u.value}; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend node_t operator+ (const T& t, const node_t& u) noexcept { return node_t{u.value + t}; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t& operator+= (const T& t) noexcept { value += t; return *this; }
	node_t& operator+= (const node_t& u) noexcept { value += u.value; return *this; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t operator- (const T& t) const noexcept { return node_t{value - t}; }
	node_t operator- (const node_t& u) const noexcept { return node_t{value - u.value}; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend node_t operator- (const T& t, const node_t& u) noexcept { return node_t{t - u.value}; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	node_t& operator-= (const T& t) noexcept { value -= t; return *this; }
	node_t& operator-= (const node_t& u) noexcept { value -= u.value; return *this; }

	node_t& operator++() noexcept { ++value; return *this; }
	node_t& operator--() noexcept { --value; return *this; }

	// input/output operators

	template<class stream>
	friend stream& operator>> (stream& s, node_t& p) noexcept {
		s >> p.value;
		return s;
	}

	template<class stream>
	friend stream& operator<< (stream& s, const node_t& p) noexcept {
		s << p.value;
		return s;
	}
};

/// Similar to @ref edge.
typedef std::pair<node_t, node_t> edge_t;
/// Similar to @ref edge_pair.
typedef std::pair<edge_t, edge_t> edge_pair_t;

/// Typesafe @ref position type.
struct position_t {
	uint64_t value;

	// CONSTRUCTORS

	position_t() = default;

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t(const T& _v) noexcept : value(_v) { }

	// -- ASSIGNMENT OPERATORS --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t& operator= (const T& _v) noexcept { value = _v; return *this; }

	// -- COMPARISONS --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator== (const T& t) const noexcept { return value == t; }
	bool operator== (const position_t& u) const noexcept { return value == u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator== (const T& t, const position_t& u) noexcept { return u.value == t; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator!= (const T& t) const noexcept { return value != t; }
	bool operator!= (const position_t& u) const noexcept { return value != u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator!= (const T& t, const position_t& u) noexcept { return u.value != t; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator< (const T& t) const noexcept { return value < t; }
	bool operator< (const position_t& u) const noexcept { return value < u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator< (const T& t, const position_t& u) noexcept { return t < u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator<= (const T& t) const noexcept { return value <= t; }
	bool operator<= (const position_t& u) const noexcept { return value <= u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator<= (const T& t, const position_t& u) noexcept { return t <= u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator> (const T& t) const noexcept { return value > t; }
	bool operator> (const position_t& u) const noexcept { return value > u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator> (const T& t, const position_t& u) noexcept { return t > u.value; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool operator>= (const T& t) const noexcept { return value >= t; }
	bool operator>= (const position_t& u) const noexcept { return value >= u.value; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend bool operator>= (const T& t, const position_t& u) noexcept { return t >= u.value; }

	// -- ARITHMETIC --

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t operator+ (const T& t) const noexcept { return position_t{value + t}; }
	position_t operator+ (const position_t& u) const noexcept { return position_t{value + u.value}; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend position_t operator+ (const T& t, const position_t& u) noexcept { return position_t{u.value + t}; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t& operator+= (const T& t) noexcept { value += t; return *this; }
	position_t& operator+= (const position_t& u) noexcept { value += u.value; return *this; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t operator- (const T& t) const noexcept { return position_t{value - t}; }
	position_t operator- (const position_t& u) const noexcept { return position_t{value - u.value}; }
	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	friend position_t operator- (const T& t, const position_t& u) noexcept { return position_t{t - u.value}; }

	template<typename T,std::enable_if_t<std::is_integral_v<T>, bool> = true>
	position_t& operator-= (const T& t) noexcept { value -= t; return *this; }
	position_t& operator-= (const position_t& u) noexcept { value -= u.value; return *this; }

	position_t& operator++() noexcept { ++value; return *this; }
	position_t& operator--() noexcept { --value; return *this; }

	// input/output operators

	template<class stream>
	friend stream& operator>> (stream& s, position_t& p) noexcept {
		s >> p.value;
		return s;
	}

	template<class stream>
	friend stream& operator<< (stream& s, const position_t& p) noexcept {
		s << p.value;
		return s;
	}
};

static_assert(    std::is_trivial_v<node_t>);
static_assert(    std::is_trivially_constructible_v<node_t>);
static_assert(    std::is_trivially_copy_assignable_v<node_t>);
static_assert(    std::is_trivially_copy_constructible_v<node_t>);
static_assert(    std::is_trivially_copyable_v<node_t>);
static_assert(    std::is_trivially_move_constructible_v<node_t>);
static_assert(    std::is_trivially_move_assignable_v<node_t>);
static_assert(    std::is_constructible_v<node_t, node_t>);
static_assert(    std::is_constructible_v<node_t, node>);
static_assert(    std::is_constructible_v<node_t, int>);
static_assert(    std::is_trivially_assignable_v<node_t, node_t>);
static_assert(not std::is_trivially_assignable_v<node_t, node>);
static_assert(not std::is_trivially_assignable_v<node_t, int>);
static_assert(    std::is_trivially_destructible_v<node_t>);

static_assert(    std::is_trivial_v<position_t>);
static_assert(    std::is_trivially_constructible_v<position_t>);
static_assert(    std::is_trivially_copy_assignable_v<position_t>);
static_assert(    std::is_trivially_copy_constructible_v<position_t>);
static_assert(    std::is_trivially_copyable_v<position_t>);
static_assert(    std::is_trivially_move_constructible_v<position_t>);
static_assert(    std::is_trivially_move_assignable_v<position_t>);
static_assert(    std::is_constructible_v<position_t, position_t>);
static_assert(    std::is_constructible_v<position_t, position>);
static_assert(    std::is_constructible_v<position_t, int>);
static_assert(    std::is_trivially_assignable_v<position_t, position_t>);
static_assert(not std::is_trivially_assignable_v<position_t, position>);
static_assert(not std::is_trivially_assignable_v<position_t, int>);
static_assert(    std::is_trivially_destructible_v<position_t>);

} // -- namespace lal
