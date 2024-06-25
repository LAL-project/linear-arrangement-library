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
#include <lal/detail/graphs/conversions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace graphs {

// -----------------------------------------------------------------------------
// head vector -> tree

undirected_graph from_head_vector_to_undirected_graph
(const head_vector& hv, bool normalize, bool check)
noexcept
{
	return detail::from_head_vector_to_graph<undirected_graph>(hv, normalize, check);
}

directed_graph from_head_vector_to_directed_graph
(const head_vector& hv, bool normalize, bool check)
noexcept
{
	return detail::from_head_vector_to_graph<directed_graph>(hv, normalize, check);
}

std::pair<free_tree,node> from_head_vector_to_free_tree
(const head_vector& hv, bool normalize, bool check)
noexcept
{
	return detail::from_head_vector_to_tree<free_tree>(hv, normalize, check);
}

rooted_tree from_head_vector_to_rooted_tree
(const head_vector& hv, bool normalize, bool check)
noexcept
{
	return detail::from_head_vector_to_tree<rooted_tree>(hv, normalize, check);
}

// -----------------------------------------------------------------------------
// edge list -> graph

rooted_tree from_edge_list_to_rooted_tree
(const edge_list& el, bool normalize = true, bool check = true)
noexcept
{
	return detail::from_edge_list_to_graph<rooted_tree>
		(el, normalize, check);
}

free_tree from_edge_list_to_free_tree
(const edge_list& el, bool normalize = true, bool check = true)
noexcept
{
	return detail::from_edge_list_to_graph<free_tree>
		(el, normalize, check);
}

directed_graph from_edge_list_to_directed_graph
(const edge_list& el, bool normalize = true, bool check = true)
noexcept
{
	return detail::from_edge_list_to_graph<directed_graph>
		(el, normalize, check);
}

undirected_graph from_edge_list_to_undirected_graph
(const edge_list& el, bool normalize = true, bool check = true)
noexcept
{
	return detail::from_edge_list_to_graph<undirected_graph>
		(el, normalize, check);
}

} // -- namespace graphs
} // -- namespace lal
