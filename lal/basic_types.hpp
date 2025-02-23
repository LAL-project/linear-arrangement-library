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
#include <cstdint>
#include <vector>

namespace lal {

/// Node type. See @ref LAL_concepts__node page for further details.
typedef uint64_t node;
/// Node's position type.
typedef uint64_t position;

/// See @ref LAL_concepts__edge page for further details.
typedef std::pair<node, node> edge;
/// See @ref LAL_concepts__head_vector page for further details.
typedef std::vector<uint64_t> head_vector;
/// See @ref LAL_concepts__edge_list page for further details.
typedef std::vector<edge> edge_list;
/// Edge pair type.
typedef std::pair<edge, edge> edge_pair;
/// List of nodes.
typedef std::vector<node> neighbourhood;

/// Typesafe @ref node type.
struct node_t {
	uint64_t value;

	constexpr node_t() noexcept = default;

	template <typename T>
	constexpr node_t(const T& _v) noexcept
		: value(_v)
	{
		static_assert(std::is_integral_v<T>);
	}

	/* ASSIGNMENT OPERATORS */

	template <typename T>
	node_t& operator= (const T& _v) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value = _v;
		return *this;
	}

	/* COMPARISON OPERATORS */

	[[nodiscard]] constexpr std::strong_ordering operator<=> (const node_t& u
	) const noexcept
	{
		return value <=> u.value;
	}
	[[nodiscard]] constexpr bool operator== (const node_t& u) const noexcept
	{
		return value == u.value;
	}

	template <typename T>
	[[nodiscard]] constexpr std::strong_ordering operator<=> (const T& t
	) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return value <=> t;
	}
	template <typename T>
	[[nodiscard]] constexpr bool operator== (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return value == t;
	}

	template <typename T>
	[[nodiscard]] constexpr friend std::strong_ordering
	operator<=> (const T& t, const node_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return t <=> u.value;
	}
	template <typename T>
	[[nodiscard]] constexpr friend bool
	operator== (const T& t, const node_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return u.value == t;
	}

	/* ARITHMETIC OPERATORS */

	template <typename T>
	[[nodiscard]] constexpr node_t operator+ (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return node_t{value + t};
	}
	[[nodiscard]] constexpr node_t operator+ (const node_t& u) const noexcept
	{
		return node_t{value + u.value};
	}
	template <typename T>
	[[nodiscard]] constexpr friend node_t
	operator+ (const T& t, const node_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return node_t{u.value + t};
	}

	template <typename T>
	node_t& operator+= (const T& t) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value += t;
		return *this;
	}
	node_t& operator+= (const node_t& u) noexcept
	{
		value += u.value;
		return *this;
	}

	template <typename T>
	[[nodiscard]] constexpr node_t operator- (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return node_t{value - t};
	}
	[[nodiscard]] constexpr node_t operator- (const node_t& u) const noexcept
	{
		return node_t{value - u.value};
	}
	template <typename T>
	[[nodiscard]] constexpr friend node_t
	operator- (const T& t, const node_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return node_t{t - u.value};
	}

	template <typename T>
	node_t& operator-= (const T& t) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value -= t;
		return *this;
	}
	node_t& operator-= (const node_t& u) noexcept
	{
		value -= u.value;
		return *this;
	}

	node_t& operator++ () noexcept
	{
		++value;
		return *this;
	}
	node_t& operator-- () noexcept
	{
		--value;
		return *this;
	}

	/* INPUT/OUTPUT OPERATORS */

	template <class stream_t>
	friend stream_t& operator>> (stream_t& s, node_t& p) noexcept
	{
		s >> p.value;
		return s;
	}

	template <class stream_t>
	friend stream_t& operator<< (stream_t& s, const node_t& p) noexcept
	{
		s << p.value;
		return s;
	}

	[[nodiscard]] constexpr uint64_t operator* () const noexcept
	{
		return value;
	}
};

static_assert(std::is_trivial_v<node_t>);

static_assert(std::is_trivially_copyable_v<node_t>);
static_assert(std::is_trivially_copy_assignable_v<node_t>);
static_assert(std::is_copy_assignable_v<node_t>);
static_assert(std::is_trivially_copy_constructible_v<node_t>);
static_assert(std::is_copy_constructible_v<node_t>);
static_assert(std::is_trivially_move_constructible_v<node_t>);
static_assert(std::is_move_constructible_v<node_t>);
static_assert(std::is_trivially_destructible_v<node_t>);
static_assert(std::is_destructible_v<node_t>);

static_assert(std::is_trivially_constructible_v<node_t, const node_t&>);
static_assert(std::is_trivially_constructible_v<node_t, const node_t>);
static_assert(std::is_trivially_constructible_v<node_t, node_t>);
static_assert(std::is_trivially_constructible_v<node_t>);
static_assert(not std::is_trivially_constructible_v<node_t, node>);
static_assert(not std::is_trivially_constructible_v<node_t, uint64_t>);
static_assert(not std::is_trivially_constructible_v<node_t, int64_t>);
static_assert(not std::is_trivially_constructible_v<node_t, uint32_t>);
static_assert(not std::is_trivially_constructible_v<node_t, int32_t>);
static_assert(not std::is_trivially_constructible_v<node_t, uint16_t>);
static_assert(not std::is_trivially_constructible_v<node_t, int16_t>);
static_assert(not std::is_trivially_constructible_v<node_t, uint8_t>);
static_assert(not std::is_trivially_constructible_v<node_t, int8_t>);

static_assert(std::is_constructible_v<node_t, const node_t&>);
static_assert(std::is_constructible_v<node_t, const node_t>);
static_assert(std::is_constructible_v<node_t, node_t>);
static_assert(std::is_constructible_v<node_t>);
static_assert(std::is_constructible_v<node_t, node_t>);
static_assert(std::is_constructible_v<node_t, node>);
static_assert(std::is_constructible_v<node_t, uint64_t>);
static_assert(std::is_constructible_v<node_t, int64_t>);
static_assert(std::is_constructible_v<node_t, uint32_t>);
static_assert(std::is_constructible_v<node_t, int32_t>);
static_assert(std::is_constructible_v<node_t, uint16_t>);
static_assert(std::is_constructible_v<node_t, int16_t>);
static_assert(std::is_constructible_v<node_t, uint8_t>);
static_assert(std::is_constructible_v<node_t, int8_t>);

static_assert(std::is_trivially_assignable_v<node_t, const node_t&>);
static_assert(std::is_trivially_assignable_v<node_t, const node_t>);
static_assert(std::is_trivially_assignable_v<node_t, node_t>);
static_assert(not std::is_trivially_assignable_v<node_t, node>);
static_assert(not std::is_trivially_assignable_v<node_t, uint64_t>);
static_assert(not std::is_trivially_assignable_v<node_t, int64_t>);
static_assert(not std::is_trivially_assignable_v<node_t, uint32_t>);
static_assert(not std::is_trivially_assignable_v<node_t, int32_t>);
static_assert(not std::is_trivially_assignable_v<node_t, uint16_t>);
static_assert(not std::is_trivially_assignable_v<node_t, int16_t>);
static_assert(not std::is_trivially_assignable_v<node_t, uint8_t>);
static_assert(not std::is_trivially_assignable_v<node_t, int8_t>);

static_assert(std::is_assignable_v<node_t, const node_t&>);
static_assert(std::is_assignable_v<node_t, const node_t>);
static_assert(std::is_assignable_v<node_t, node_t>);
static_assert(std::is_assignable_v<node_t, node_t>);
static_assert(std::is_assignable_v<node_t, node>);
static_assert(std::is_assignable_v<node_t, uint64_t>);
static_assert(std::is_assignable_v<node_t, int64_t>);
static_assert(std::is_assignable_v<node_t, uint32_t>);
static_assert(std::is_assignable_v<node_t, int32_t>);
static_assert(std::is_assignable_v<node_t, uint16_t>);
static_assert(std::is_assignable_v<node_t, int16_t>);
static_assert(std::is_assignable_v<node_t, uint8_t>);
static_assert(std::is_assignable_v<node_t, int8_t>);

static_assert(node_t{0ull} == node_t{0ull});
static_assert(not(node_t{0ull} == node_t{1ull}));
static_assert(0ull == node_t{0ull});
static_assert(node_t{0ull} == 0ull);
static_assert(not(0ull == node_t{1ull}));
static_assert(not(node_t{0ull} == 1ull));

static_assert(node_t{0ull} != node_t{1ull});
static_assert(not(node_t{0ull} != node_t{0ull}));
static_assert(0ull != node_t{1ull});
static_assert(node_t{0ull} != 1ull);
static_assert(not(0ull != node_t{0ull}));
static_assert(not(node_t{0ull} != 0ull));

static_assert(node_t{0ull} < node_t{1ull});
static_assert(not(node_t{4ull} < node_t{0ull}));
static_assert(0ull < node_t{1ull});
static_assert(node_t{0ull} < 1ull);
static_assert(not(4ull < node_t{0ull}));
static_assert(not(node_t{4ull} < 0ull));

static_assert(node_t{0ull} <= node_t{1ull});
static_assert(node_t{0ull} <= node_t{0ull});
static_assert(not(node_t{4ull} <= node_t{0ull}));
static_assert(0ull <= node_t{1ull});
static_assert(node_t{0ull} <= 1ull);
static_assert(0ull <= node_t{0ull});
static_assert(node_t{0ull} <= 0ull);
static_assert(not(4ull <= node_t{0ull}));
static_assert(not(node_t{4ull} <= 0ull));

static_assert(node_t{1ull} > node_t{0ull});
static_assert(not(node_t{4ull} > node_t{7ull}));
static_assert(1ull > node_t{0ull});
static_assert(node_t{1ull} > 0ull);
static_assert(not(4ull > node_t{7ull}));
static_assert(not(node_t{4ull} > 7ull));

static_assert(node_t{1ull} >= node_t{0ull});
static_assert(node_t{0ull} >= node_t{0ull});
static_assert(not(node_t{4ull} >= node_t{6ull}));
static_assert(1ull >= node_t{0ull});
static_assert(node_t{1ull} >= 0ull);
static_assert(0ull >= node_t{0ull});
static_assert(node_t{0ull} >= 0ull);
static_assert(not(4ull >= node_t{6ull}));
static_assert(not(node_t{4ull} >= 6ull));

/// Similar to @ref edge.
typedef std::pair<node_t, node_t> edge_t;
/// Similar to @ref edge_pair.
typedef std::pair<edge_t, edge_t> edge_pair_t;

/// Typesafe @ref position type.
struct position_t {
	uint64_t value;

	constexpr position_t() noexcept = default;

	template <typename T>
	constexpr position_t(const T& _v) noexcept
		: value(_v)
	{
		static_assert(std::is_integral_v<T>);
	}

	/* ASSIGNMENT OPERATORS */

	template <typename T>
	position_t& operator= (const T& _v) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value = _v;
		return *this;
	}

	/* COMPARISON OPERATORS */

	[[nodiscard]] constexpr std::strong_ordering
	operator<=> (const position_t& p) const noexcept
	{
		return value <=> p.value;
	}
	[[nodiscard]] constexpr bool operator== (const position_t& u) const noexcept
	{
		return value == u.value;
	}

	template <typename T>
	[[nodiscard]] constexpr std::strong_ordering operator<=> (const T& t
	) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return value <=> t;
	}
	template <typename T>
	[[nodiscard]] constexpr bool operator== (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return value == t;
	}

	template <typename T>
	[[nodiscard]] constexpr friend std::strong_ordering
	operator<=> (const T& t, const position_t& p) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return t <=> p.value;
	}
	template <typename T>
	[[nodiscard]] constexpr friend bool
	operator== (const T& t, const position_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return u.value == t;
	}

	/* ARITHMETIC OPERATORS */

	template <typename T>
	[[nodiscard]] constexpr position_t operator+ (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return position_t{value + t};
	}
	[[nodiscard]] constexpr position_t operator+ (const position_t& u
	) const noexcept
	{
		return position_t{value + u.value};
	}
	template <typename T>
	[[nodiscard]] constexpr friend position_t
	operator+ (const T& t, const position_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return position_t{u.value + t};
	}

	template <typename T>
	position_t& operator+= (const T& t) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value += t;
		return *this;
	}
	position_t& operator+= (const position_t& u) noexcept
	{
		value += u.value;
		return *this;
	}

	template <typename T>
	[[nodiscard]] constexpr position_t operator- (const T& t) const noexcept
	{
		static_assert(std::is_integral_v<T>);
		return position_t{value - t};
	}
	[[nodiscard]] constexpr position_t operator- (const position_t& u
	) const noexcept
	{
		return position_t{value - u.value};
	}
	template <typename T>
	[[nodiscard]] constexpr friend position_t
	operator- (const T& t, const position_t& u) noexcept
	{
		static_assert(std::is_integral_v<T>);
		return position_t{t - u.value};
	}

	template <typename T>
	position_t& operator-= (const T& t) noexcept
	{
		static_assert(std::is_integral_v<T>);
		value -= t;
		return *this;
	}
	position_t& operator-= (const position_t& u) noexcept
	{
		value -= u.value;
		return *this;
	}

	position_t& operator++ () noexcept
	{
		++value;
		return *this;
	}
	position_t& operator-- () noexcept
	{
		--value;
		return *this;
	}

	/* INPUT/OUTPUT OPERATORS */

	template <class stream_t>
	friend stream_t& operator>> (stream_t& s, position_t& p) noexcept
	{
		s >> p.value;
		return s;
	}

	template <class stream>
	friend stream& operator<< (stream& s, const position_t& p) noexcept
	{
		s << p.value;
		return s;
	}

	[[nodiscard]] constexpr uint64_t operator* () const noexcept
	{
		return value;
	}
};

static_assert(std::is_trivial_v<position_t>);

static_assert(std::is_trivially_copyable_v<position_t>);
static_assert(std::is_trivially_copy_assignable_v<position_t>);
static_assert(std::is_copy_assignable_v<position_t>);
static_assert(std::is_trivially_copy_constructible_v<position_t>);
static_assert(std::is_copy_constructible_v<position_t>);
static_assert(std::is_trivially_move_constructible_v<position_t>);
static_assert(std::is_move_constructible_v<position_t>);
static_assert(std::is_trivially_destructible_v<position_t>);
static_assert(std::is_destructible_v<position_t>);

static_assert(std::is_trivially_constructible_v<position_t, const position_t&>);
static_assert(std::is_trivially_constructible_v<position_t, const position_t>);
static_assert(std::is_trivially_constructible_v<position_t, position_t>);
static_assert(std::is_trivially_constructible_v<position_t>);
static_assert(not std::is_trivially_constructible_v<position_t, node>);
static_assert(not std::is_trivially_constructible_v<position_t, uint64_t>);
static_assert(not std::is_trivially_constructible_v<position_t, int64_t>);
static_assert(not std::is_trivially_constructible_v<position_t, uint32_t>);
static_assert(not std::is_trivially_constructible_v<position_t, int32_t>);
static_assert(not std::is_trivially_constructible_v<position_t, uint16_t>);
static_assert(not std::is_trivially_constructible_v<position_t, int16_t>);
static_assert(not std::is_trivially_constructible_v<position_t, uint8_t>);
static_assert(not std::is_trivially_constructible_v<position_t, int8_t>);

static_assert(std::is_constructible_v<position_t, const position_t&>);
static_assert(std::is_constructible_v<position_t, const position_t>);
static_assert(std::is_constructible_v<position_t, position_t>);
static_assert(std::is_constructible_v<position_t>);
static_assert(std::is_constructible_v<position_t, position_t>);
static_assert(std::is_constructible_v<position_t, node>);
static_assert(std::is_constructible_v<position_t, uint64_t>);
static_assert(std::is_constructible_v<position_t, int64_t>);
static_assert(std::is_constructible_v<position_t, uint32_t>);
static_assert(std::is_constructible_v<position_t, int32_t>);
static_assert(std::is_constructible_v<position_t, uint16_t>);
static_assert(std::is_constructible_v<position_t, int16_t>);
static_assert(std::is_constructible_v<position_t, uint8_t>);
static_assert(std::is_constructible_v<position_t, int8_t>);

static_assert(std::is_trivially_assignable_v<position_t, const position_t&>);
static_assert(std::is_trivially_assignable_v<position_t, const position_t>);
static_assert(std::is_trivially_assignable_v<position_t, position_t>);
static_assert(not std::is_trivially_assignable_v<position_t, node>);
static_assert(not std::is_trivially_assignable_v<position_t, uint64_t>);
static_assert(not std::is_trivially_assignable_v<position_t, int64_t>);
static_assert(not std::is_trivially_assignable_v<position_t, uint32_t>);
static_assert(not std::is_trivially_assignable_v<position_t, int32_t>);
static_assert(not std::is_trivially_assignable_v<position_t, uint16_t>);
static_assert(not std::is_trivially_assignable_v<position_t, int16_t>);
static_assert(not std::is_trivially_assignable_v<position_t, uint8_t>);
static_assert(not std::is_trivially_assignable_v<position_t, int8_t>);

static_assert(std::is_assignable_v<position_t, const position_t&>);
static_assert(std::is_assignable_v<position_t, const position_t>);
static_assert(std::is_assignable_v<position_t, position_t>);
static_assert(std::is_assignable_v<position_t, position_t>);
static_assert(std::is_assignable_v<position_t, node>);
static_assert(std::is_assignable_v<position_t, uint64_t>);
static_assert(std::is_assignable_v<position_t, int64_t>);
static_assert(std::is_assignable_v<position_t, uint32_t>);
static_assert(std::is_assignable_v<position_t, int32_t>);
static_assert(std::is_assignable_v<position_t, uint16_t>);
static_assert(std::is_assignable_v<position_t, int16_t>);
static_assert(std::is_assignable_v<position_t, uint8_t>);
static_assert(std::is_assignable_v<position_t, int8_t>);

static_assert(position_t{0ull} == position_t{0ull});
static_assert(not(position_t{0ull} == position_t{1ull}));
static_assert(0ull == position_t{0ull});
static_assert(position_t{0ull} == 0ull);
static_assert(not(0ull == position_t{1ull}));
static_assert(not(position_t{0ull} == 1ull));

static_assert(position_t{0ull} != position_t{1ull});
static_assert(not(position_t{0ull} != position_t{0ull}));
static_assert(0ull != position_t{1ull});
static_assert(position_t{0ull} != 1ull);
static_assert(not(0ull != position_t{0ull}));
static_assert(not(position_t{0ull} != 0ull));

static_assert(position_t{0ull} < position_t{1ull});
static_assert(not(position_t{4ull} < position_t{0ull}));
static_assert(0ull < position_t{1ull});
static_assert(position_t{0ull} < 1ull);
static_assert(not(4ull < position_t{0ull}));
static_assert(not(position_t{4ull} < 0ull));

static_assert(position_t{0ull} <= position_t{1ull});
static_assert(position_t{0ull} <= position_t{0ull});
static_assert(not(position_t{4ull} <= position_t{0ull}));
static_assert(0ull <= position_t{1ull});
static_assert(position_t{0ull} <= 1ull);
static_assert(0ull <= position_t{0ull});
static_assert(position_t{0ull} <= 0ull);
static_assert(not(4ull <= position_t{0ull}));
static_assert(not(position_t{4ull} <= 0ull));

static_assert(position_t{1ull} > position_t{0ull});
static_assert(not(position_t{4ull} > position_t{7ull}));
static_assert(1ull > position_t{0ull});
static_assert(position_t{1ull} > 0ull);
static_assert(not(4ull > position_t{7ull}));
static_assert(not(position_t{4ull} > 7ull));

static_assert(position_t{1ull} >= position_t{0ull});
static_assert(position_t{0ull} >= position_t{0ull});
static_assert(not(position_t{4ull} >= position_t{6ull}));
static_assert(1ull >= position_t{0ull});
static_assert(position_t{1ull} >= 0ull);
static_assert(0ull >= position_t{0ull});
static_assert(position_t{0ull} >= 0ull);
static_assert(not(4ull >= position_t{6ull}));
static_assert(not(position_t{4ull} >= 6ull));

} // namespace lal
