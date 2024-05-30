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

// lal includes
#include <lal/linarr/chunking/algorithms.hpp>
#include <lal/linarr/chunking/chunking.hpp>
#include <lal/detail/linarr/chunking/Anderson.hpp>
#include <lal/detail/linarr/chunking/Macutek.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace linarr {

#define __ident(arr) detail::identity_arr(arr)
#define __nonident(arr) detail::nonidentity_arr(arr)

graphs::rooted_tree make_tree_from_chunk_sequence(const chunk_sequence& seq)
noexcept
{
	graphs::rooted_tree t(seq.size());

	for (node chunk_idx = 0; chunk_idx < seq.size(); ++chunk_idx) {
		const auto& c = seq[chunk_idx];

		if (c.has_parent_node()) {
			const std::size_t parent_chunk_idx = seq.get_chunk_index(c.get_parent_node());
			t.add_edge_bulk( parent_chunk_idx, chunk_idx );
		}
		else {
			t.set_root(chunk_idx);
		}
	}

	t.finish_bulk_add();
	return t;
}

/*
void print_chunk_list(
	const graphs::rooted_tree& rt,
	const chunk_sequence& seq
)
noexcept
{

	std::cout << "=================================\n";
	std::cout << "Chunks: " << seq.size() << '\n';
	for (const auto& c : seq.get_chunks()) {
		std::cout << "---------------------------------\n";
		std::cout << "    Size: " << c.get_nodes().size() << '\n';
		std::cout << "    Chunk parent: " << (c.has_parent_node() ? std::to_string(c.get_parent_node()) : "*") << '\n';
		std::cout << "    Nodes:";
		for (auto u : c.get_nodes()) {
			std::cout << ' ' << u;
		}
		std::cout << '\n';
	}

	std::cout << "=================================\n";
	std::cout << "Mapping:\n";
	for (lal::node u = 0; u < rt.get_num_nodes(); ++u) {
		std::cout << "    map[" << u << "]= " << seq.get_chunk_index(u) << '\n';
	}
}
*/

// -----------------------------------------------------------------------------

graphs::rooted_tree chunk_syntactic_dependency_tree
(const graphs::rooted_tree& rt, const algorithms_chunking& algo)
noexcept
{
	linear_arrangement arr;
	const auto __arr = __ident(arr);
	
	if (algo == algorithms_chunking::Anderson) {
		detail::chunks_Anderson C(rt, __arr);
		C.chunk_input_tree();
		return make_tree_from_chunk_sequence(C.m_sequence);
	}
	else if (algo == algorithms_chunking::Macutek) {
		detail::chunks_Macutek C(rt, __arr);
		C.chunk_input_tree();
		return make_tree_from_chunk_sequence(C.m_sequence);
	}
	else {
#if defined DEBUG
		assert(false);
#endif
	}

	return graphs::rooted_tree();
}

graphs::rooted_tree chunk_syntactic_dependency_tree(
	const graphs::rooted_tree& rt,
	const linear_arrangement& arr,
	const algorithms_chunking& algo
)
noexcept
{
	const auto __arr = __nonident(arr);
	
	if (algo == algorithms_chunking::Anderson) {
		detail::chunks_Anderson C(rt, __arr);
		C.chunk_input_tree();
		return make_tree_from_chunk_sequence(C.m_sequence);
	}
	else if (algo == algorithms_chunking::Macutek) {
		detail::chunks_Macutek C(rt, __arr);
		C.chunk_input_tree();
		return make_tree_from_chunk_sequence(C.m_sequence);
	}
	else {
#if defined DEBUG
		assert(false);
#endif
	}

	return graphs::rooted_tree();
}

// -----------------------------------------------------------------------------

chunk_sequence chunk_syntactic_dependency_tree_as_sequence
(const graphs::rooted_tree& rt, const algorithms_chunking& algo)
noexcept
{
	linear_arrangement arr;
	const auto __arr = __ident(arr);
	
	if (algo == algorithms_chunking::Anderson) {
		detail::chunks_Anderson C(rt, __arr);
		C.chunk_input_tree();
		return std::move(C.m_sequence);
	}
	else if (algo == algorithms_chunking::Macutek) {
		detail::chunks_Macutek C(rt, __arr);
		C.chunk_input_tree();
		return std::move(C.m_sequence);
	}
	else {
#if defined DEBUG
		assert(false);
#endif
	}

	return chunk_sequence();
}

chunk_sequence chunk_syntactic_dependency_tree_as_sequence(
	const graphs::rooted_tree& rt,
	const linear_arrangement& arr,
	const algorithms_chunking& algo
) 
noexcept
{
	const auto __arr = __nonident(arr);
	
	if (algo == algorithms_chunking::Anderson) {
		detail::chunks_Anderson C(rt, __arr);
		C.chunk_input_tree();
		return std::move(C.m_sequence);
	}
	else if (algo == algorithms_chunking::Macutek) {
		detail::chunks_Macutek C(rt, __arr);
		C.chunk_input_tree();
		return std::move(C.m_sequence);
	}
	else {
#if defined DEBUG
		assert(false);
#endif
	}

	return chunk_sequence();
}

} // -- namespace linarr
} // -- namespace lal
