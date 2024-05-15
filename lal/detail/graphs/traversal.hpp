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

#pragma once

// C++ includes
#include <functional>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/queue_array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Abstract graph Breadth-First Search traversal.
 *
 * The traversal can be controlled by setting custom control-flow functions:
 * - a function used for early termination of the traversal
 * (see @ref set_terminate),
 * - a function that processes the current node in the traversal
 * (see @ref set_process_current),
 * - a function that processes the current edge in the traversal
 * (see @ref set_process_neighbour),
 * - a function that controls when a node is to be added to the queue of the
 * traversal (see @ref set_node_add).
 *
 * This graph_traversal traversal can also use "reversed edges" when doing traversals on
 * directed graphs. A back edge in a directed graph of a node u is a node
 * that points to u, namely, the directed edge is of the form (v,u), for another
 * node v of the graph. This can be set via the @ref set_use_rev_edges method.
 *
 * An example of usage is as follows:
 @code
 detail::BFS<graphs::undirected_graph> bfs(g); // 'g' is an undirected graph
 bfs.set_terminate( ... ); // assign a function to decide when to terminate the search.
 bfs.set_process_neighbour( ... ); // assign a function to process neighbours
 bfs.start_at(0); // start the traversal now at node 0.
 @endcode
 *
 * @tparam graph_t Type of graph.
 */
template <
	class graph_t,
	std::enable_if_t< std::is_base_of_v<graphs::graph, graph_t>, bool > = true
>
class BFS {
public:
	/// Single node processing function.
	typedef std::function<void (const BFS<graph_t>&, node)> BFS_process_one;
	/// Two nodes processing function.
	typedef std::function<void (const BFS<graph_t>&, node, node, bool)> BFS_process_two;
	/// One node decision function.
	typedef std::function<bool (const BFS<graph_t>&, node)> BFS_bool_one;
	/// Two nodes decision function.
	typedef std::function<bool (const BFS<graph_t>&, node, node, bool)> BFS_bool_two;

public:

	/**
	 * @brief Is the graph used to initiliaze the object directed?
	 *
	 * A graph is directed if it is a base class of @ref lal::graphs::directed_graph.
	 */
	static constexpr bool is_graph_directed =
		std::is_base_of_v<graphs::directed_graph, graph_t>;
	// ! Note that this member attribute is public, and so it should not have
	//   a leading 'm_' like the other private/protected members.

public:
	/// Constructor
	BFS(const graph_t& g) noexcept :
		m_G(g),
		m_vis(m_G.get_num_nodes())
	{
		reset();
	}
	/// Destructor
	~BFS() noexcept = default;

	/**
	 * @brief Set the graph traversal to its default state.
	 *
	 * This includes the node processing functions:
	 * - @ref m_process_current,
	 * - @ref m_process_neighbour,
	 * - @ref m_add_node,
	 *
	 * the termination function @ref m_terminate, and the attributes:
	 * - @ref m_use_rev_edges,
	 * - @ref m_process_visited_neighbours.
	 */
	void reset() noexcept {
		clear_visited();
		m_queue.init(m_G.get_num_nodes());

		set_use_rev_edges(false);
		set_process_visited_neighbours(false);

		set_terminate_default();
		set_process_current_default();
		set_process_neighbour_default();
		set_node_add_default();
	}

	/**
	 * @brief Start traversal at a given node.
	 * @param source Node.
	 */
	void start_at(node source) noexcept {
		m_queue.push(source);
		m_vis[source] = 1;
		do_traversal();
	}

	/**
	 * @brief Start the traversal at every given node.
	 * @param sources List of node.
	 */
	void start_at(const std::vector<node>& sources) noexcept {
		for (const node& u : sources) {
			m_queue.push(u);
			m_vis[u] = 1;
		}
		do_traversal();
	}

	/* SETTERS */

	/// Set whether the traversal can use reversed edges
	void set_use_rev_edges(bool use) noexcept { m_use_rev_edges = use; }

	/// Set the default value of @ref m_terminate.
	void set_terminate_default() noexcept {
		m_terminate = [](const BFS<graph_t>&, node) -> bool { return false; };
		m_is_terminate_set = false;
	}
	/// Set the function that controls the termination of the loop.
	void set_terminate(const BFS_bool_one& f) noexcept {
		m_terminate = f;
		m_is_terminate_set = true;
	}

	/// Set the default value of @ref m_process_current.
	void set_process_current_default() noexcept {
		m_process_current = [](const BFS<graph_t>&, node) -> void { };
		m_is_process_current_set = false;
	}
	/// Set the function that controls the processing of the current node.
	void set_process_current(const BFS_process_one& f) noexcept {
		m_process_current = f;
		m_is_process_current_set = true;
	}

	/// Set the default value of @ref m_process_neighbour.
	void set_process_neighbour_default() noexcept {
		m_process_neighbour = [](const BFS<graph_t>&, node, node, bool) -> void { };
		m_is_process_neighbour_set = false;
	}
	/// Set the function that controls the processing of the current neighbour.
	void set_process_neighbour(const BFS_process_two& f) noexcept {
		m_process_neighbour = f;
		m_is_process_neighbour_set = true;
	}

	/// Set the default value of @ref m_add_node.
	void set_node_add_default() noexcept {
		m_add_node = [](const BFS<graph_t>&, node, node, bool) -> bool { return true; };
		m_is_add_node_set = false;
	}
	/// Set the function that controls when a node is to be added to the queue.
	void set_node_add(const BFS_bool_two& f) noexcept {
		m_add_node = f;
		m_is_add_node_set = true;
	}

	/**
	 * @brief Should the algorithm call the neighbour processing function
	 * for already visited neighbours?
	 * @param v Either true or false.
	 */
	void set_process_visited_neighbours(bool v) noexcept
	{ m_process_visited_neighbours = v; }

	/**
	 * @brief Sets all nodes to not visited.
	 *
	 * When using this function, users might also want to call @ref clear_queue.
	 */
	void clear_visited() noexcept { m_vis.fill(0); }

	/**
	 * @brief Clear the memory allocated for this structure.
	 *
	 * When using this function, users might also want to call @ref clear_visited.
	 */
	void clear_queue() noexcept {
		m_queue.reset();
	}

	/**
	 * @brief Set node @e u as visited or not.
	 * @param u Node to set as visitied.
	 * @param vis A 0-1 value.
	 */
	void set_visited(node u, char vis) noexcept {
#if defined DEBUG
		assert(vis == 0 or vis == 1);
#endif
		m_vis[u] = vis;
	}

	/* GETTERS */

	/// Returns whether or not node @e u has been visited.
	bool node_was_visited(node u) const noexcept { return m_vis[u] == 1; }

	/// Have all nodes been visited?
	bool all_visited() const noexcept {
		return std::all_of(m_vis.begin(), m_vis.end(), [](auto x){return x == 1;});
	}

	/// Returns a constant reference to the graph
	const graph_t& get_graph() const noexcept { return m_G; }

	/// Return visited nodes information
	const array<char>& get_visited() const noexcept { return m_vis; }

protected:
	/**
	 * @brief Deal with a neighbour of an input node.
	 *
	 * Processes the neighbour and pushes it into the queue.
	 *
	 * The neighbour is processed if it has not been visited before. In case the
	 * node was visited in a previous iteration, it is processed only if
	 * @ref m_process_visited_neighbours has been set via method @ref set_process_visited_neighbours.
	 *
	 * Node @e t is pushed into the queue only if it has not been visited before
	 * and the user function @ref m_add_node allows it.
	 * @param s Input node.
	 * @param t The neighbour of the input node.
	 * @param ltr Left-to-right orientation of the edge \f$\{s,t\}\f$. If @e ltr
	 * is true then the edge has orientation from @e s to @e t. If @e ltr is
	 * false, the edge has orientation from @e t to @e s.
	 */
	void deal_with_neighbour(node s, node t, bool ltr) noexcept {
		// Process the neighbour 't' of 's'.
		const bool t_vis = node_was_visited(t);

		if ((not t_vis) or (t_vis and m_process_visited_neighbours)) {
			if (m_is_process_neighbour_set) {
				m_process_neighbour(*this, s, t, ltr);
			}
		}

		if (not t_vis) {
			const bool add_node =
				m_is_add_node_set ? m_add_node(*this, s, t, ltr) : true;

			if (add_node) {
				m_queue.push(t);
				// set node as visited
				m_vis[t] = 1;
			}
		}
	}

	/// Process the neighbours of node @e s
	void process_neighbours(node s) noexcept {
		if constexpr (not is_graph_directed) {
			// for undirected graphs

			for (const node& t : m_G.get_neighbours(s)) {
				// Edges are processed in the direction "s -> t".
				// This is also the 'natural' orientation of the edge,
				// so this explains the 'true'.
				deal_with_neighbour(s, t, true);
			}
		}
		else {
			// for directed graphs

			for (const node& t : m_G.get_out_neighbours(s)) {
				// Edges are processed in the direction "s -> t".
				// This is also the 'natural' orientation of the edge,
				// hence the 'true'.
				deal_with_neighbour(s, t, true);
			}
			// process in-neighbours whenever appropriate
			if (m_use_rev_edges) {
				for (const node& t : m_G.get_in_neighbours(s)) {
					// Edges are processed in the direction "s -> t".
					// However, the 'natural' orientation of the edge
					// is "t -> s", hence the 'false'.
					deal_with_neighbour(s, t, false);
				}
			}
		}
	}

	/**
	 * @brief Traversal through the graph's vertices.
	 *
	 * The graph_traversal traversal is implemented as follows:
	 *
	 * <pre>
	 * ProcessNeighbourhood(graph, Q, v, Nv):
	 *	 1. for each w in Nv do
	 *   2.		if w has not been visited before, or it has been but
	 *	 3.			already-visited nodes have to be processed
	 *	 4.		then
	 *	 5.			proc_neigh(v,w)
	 *	 6.		endif
	 *	 7.
	 *	 8.		if w not visited before and node_add(v,w) then
	 *	 9.			push w into Q
	 *	10.			mark w as visited in vis
	 *	11.		endif
	 *	12.	endfor
	 * </pre>
	 *
	 * <pre>
	 * graph_traversal(graph, source):
	 *    .	// set of |V(graph)| bits set to false
	 *	 1.	vis = {false}
	 *    .	// structure of the traversal, initialised with the source, a queue.
	 *	 2.	Q = {source}
	 *	 3.	while Q is not empty do
	 *	 4.		v = Q.front
	 *	 5.		remove Q's front
	 *	 6.		proc_curr(v)
	 *	 7.		if terminate(v) then Finish traversal
	 *	 8.		else
	 *	 9.			Nv = out-neighbourhood of v
	 *	10.			ProcessNeighbourhood(graph, Q, v, Nv)
	 *	11.			If graph is directed and process reverse edges then
	 *	12.				Nv = in-neighbourhood of v
	 *	13.				ProcessNeighbourhood(graph, Q, v, Nv)
	 *	14.			endif
	 *	15.		endif
	 *	16.	endwhile
	 * </pre>
	 *
	 * Note that lines (...) extend the neighbourhood of a node "Nv"
	 * depending of the type of graph. If the graph is directed and
	 * the user wants to process "reversed edges", then the neighbourhood
	 * is not limited to "out-neighbours", but also to "in-neighbours".
	 */
	void do_traversal() noexcept {
		while (m_queue.size() > 0) {
			const node s = m_queue.pop();

			// process current node
			if (m_is_process_current_set) {
				m_process_current(*this, s);
			}

			// check user-defined early termination condition
			if (m_is_terminate_set) {
				if (m_terminate(*this, s)) { break; }
			}

			process_neighbours(s);
		}
	}

protected:
	/// Constant reference to the graph.
	const graph_t& m_G;

	/// The structure of the traversal.
	queue_array<node> m_queue;

	/// The set of visited nodes.
	array<char> m_vis;
	/// Should the traversal process previously-visitied neighbours?
	bool m_process_visited_neighbours = false;
	/**
	 * @brief In directed graphs, traverse edges in the reverse direction.
	 *
	 * Besides reaching neighbours following out-edges, reach neighbours
	 * following in-neighbours. If vertex @e s has out-neighbours @e {1,2,3}
	 * and in-neighbours @e {4,5}, this attribute controls whether vertices
	 * @e {4,5} should also be included in the traversal.
	 */
	bool m_use_rev_edges = false;

protected:
	/**
	 * @brief Early terminating function.
	 *
	 * Returns true if the graph_traversal algorithm should terminate.
	 *
	 * For more details on when this function is called see @ref do_traversal.
	 */
	BFS_bool_one m_terminate;
	/// Is function @ref m_terminate set?
	bool m_is_terminate_set;

	/**
	 * @brief Node processing function.
	 *
	 * Processes the current node visited.
	 *
	 * For more details on when this function is called see @ref do_traversal.
	 */
	BFS_process_one m_process_current;
	/// Is function @ref m_process_current set?
	bool m_is_process_current_set;

	/**
	 * @brief Node processing function.
	 *
	 * Processes the next visited node. The direction of the nodes
	 * visited passed as parameters is given by the boolean parameter. If
	 * it is true then the edge is s -> t. If it is false then the edge is
	 * t -> s.
	 *
	 * For more details on when this function is called see @ref do_traversal.
	 */
	BFS_process_two m_process_neighbour;
	/// Is function @ref m_process_neighbour set?
	bool m_is_process_neighbour_set;

	/**
	 * @brief Node addition function.
	 *
	 * Determines whether a node @e s should be added to the queue or not.
	 *
	 * For more details on when this function is called see @ref do_traversal.
	 */
	BFS_bool_two m_add_node;
	/// Is function @ref m_add_node set?
	bool m_is_add_node_set;
};

} // -- namespace detail
} // -- namespace lal
