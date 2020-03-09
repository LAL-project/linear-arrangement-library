/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <functional>
#include <vector>
#include <queue>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs.hpp>

namespace lal {
namespace utils {

/* This class implements an abstract BFS traversal.
 *
 * Users of this class can control the traversal by setting custom control-flow
 * functions. The user can set:
 * - a function used for early termination of the traversal
 * (see @ref set_terminate),
 * - a function that processes the current vertex in the traversal
 * (see @ref set_process_current),
 * - a function that processes the current edge in the traversal
 * (see @ref set_process_neighbour),
 * - a function that can decide when to add another vertex to the queue of the
 * traversal (see @ref set_vertex_add).
 *
 * This bfs traversal can also use "reversed edges" when doing traversals on
 * directed graphs. A back edge in a directed graph of a vertex u is a vertex
 * that points to u, namely, the directed edge is of the form (v,u), for another
 * vertex v of the graph. This can be set via the @ref set_use_back_edges method.
 */
template<class G, typename node = typename lal::node>
class BFS {
	public:
		typedef std::function<void (const BFS<G>& bfs, node s)> bfs_process_one;

		typedef
		std::function<void (const BFS<G>& bfs, node s, node t, bool)>
		bfs_process_two;

		typedef std::function<bool (const BFS<G>& bfs, node s)> bfs_bool_function;

	public:
		// Constructor
		BFS(const G& g) : m_G(g) {
			m_vis = std::vector<bool>(g.n_nodes());
			reset();
		}
		// Destructor
		virtual ~BFS() { }

		// Set the BFS to its initial state.
		void reset() {
			reset_visited();
			clear_queue();

			set_terminate_default();
			set_process_current_default();
			set_process_neighbour_default();
			set_vertex_add_default();
		}

		void start_at(node source) {
			m_Q.push(source);
			m_vis[source] = true;
			do_traversal();
		}

		void start_at(const std::vector<node>& sources) {
			for (const node& u : sources) {
				m_Q.push(u);
				m_vis[u] = true;
			}
			do_traversal();
		}

		/* SETTERS */

		// set whether the traversal can use back edges
		void set_use_rev_edges(bool use) {
			m_use_rev_edges = use;
		}

		// see @ref m_term
		void set_terminate_default()
		{ m_term = [](const BFS<G>&, const node) -> bool { return false; }; }
		void set_terminate(bfs_bool_function f)
		{ m_term = f; }

		// see @ref m_proc_cur
		void set_process_current_default()
		{ m_proc_cur = [](const BFS<G>&, const node) -> void { }; }
		void set_process_current(bfs_process_one f)
		{ m_proc_cur = f; }

		// see @ref m_proc_neigh
		void set_process_neighbour_default()
		{ m_proc_out_neigh = [](const BFS<G>&, const node, const node, bool) -> void { }; }
		void set_process_neighbour(bfs_process_two f)
		{ m_proc_out_neigh = f; }

		// see @ref m_add_vertex
		void set_vertex_add_default()
		{ m_add_vertex = [](const BFS<G>&, const node) -> bool { return true; }; }
		void set_vertex_add(bfs_bool_function f)
		{ m_add_vertex = f; }

		/*
		 * @brief Should the algorithm call the neighbour processing function
		 * for already visited neighbours?
		 * @param v Either true or false.
		 */
		void process_visited_neighbours(bool v) {
			m_proc_vis_neighs = v;
		}

		// Sets all nodes to not visited.
		void reset_visited() {
			std::fill(m_vis.begin(), m_vis.end(), false);
		}

		// Empties the queue used for the traversal.
		void clear_queue() {
			while (not m_Q.empty()) { m_Q.pop(); }
		}

		// Set vertex @e u as visited.
		void set_visited(node u, bool vis = true) {
			m_vis[u] = vis;
		}

		/* GETTERS */

		// Returns the set of visited nodes.
		bool node_was_visited(node u) const { return m_vis[u]; }

		// have all vertices been visited?
		bool all_visited() const {
			return find(m_vis.begin(), m_vis.end(), false) == m_vis.end();
		}

		// Return visited nodes information
		const std::vector<bool>& get_visited() const { return m_vis; }

	protected:

		void deal_with_neighbour(node s, node t, bool dir) {
			// Process the neighbour found.
			// This is always called: even if it has been
			// visited before.
			if ((m_vis[t] and m_proc_vis_neighs) or not m_vis[t]) {
				m_proc_out_neigh(*this, s, t, dir);
			}

			if (not m_vis[t] and m_add_vertex(*this, t)) {
				m_Q.push(t);
				m_vis[t] = true;
			}
		}

		// process neighbours when the graph is an undirected graph
		template<class GG = G,
			typename std::enable_if<
				std::is_base_of<graphs::ugraph, GG>::value, int
			>::type = 0
		>
		void process_neighbours(node s) {
			for (const node& t : m_G.get_neighbours(s)) {
				deal_with_neighbour(s, t, true);
			}
		}

		// process neighbours when the graph is a directed graph
		template<class GG = G,
			typename std::enable_if<
				std::is_base_of<graphs::dgraph, GG>::value, int
			>::type = 0
		>
		void process_neighbours(node s) {
			for (const node& t : m_G.get_neighbours(s)) {
				deal_with_neighbour(s, t, true);
			}
			// process in-neighbours whenever appropriate
			if (m_use_rev_edges) {
				for (const node& t : m_G.get_in_neighbours(s)) {
					deal_with_neighbour(s, t, false);
				}
			}
		}

		/* The BFS traversal is implemented as follows:
		 *
		 * <pre>
		 * ProcessNeighbourhood(graph, u, Nv):
		 *	 1. for each w in Nv do
		 *   2.		if w has not been visited before, or it has been
		 *	 3.			but already visited vertices have to be processed
		 *	 4.		then
		 *	 5.			proc_neigh(u,w)
		 *	 6.		if w not visited before and vertex_add(w) then
		 *	 7.			push w into Q
		 *	 8.			mark w as visited in vis
		 *	 9.		endif
		 *	10.	endfor
		 * </pre>
		 *
		 * <pre>
		 * BFS(graph, source):
		 *	 1.	vis = {false}	// set of |V(graph)| bits set to false
		 *	 2.	Q = {source}	// queue of the traversal,
		 *	 3.					// initialised with the source
		 *	 4.	while Q is not empty do
		 *	 5.		v = Q.front
		 *	 6.		remove Q's front
		 *	 7.		proc_curr(v)
		 *	 8.		if terminate(v) then Finish traversal
		 *	 9.		else
		 *	10.			Nv = out-neighbourhood of v
		 *	11.			ProcessNeighbourhood(graph, v, Nv)
		 *	12.			If graph is directed and process reverse edges then
		 *	13.				Nv = in-neighbourhood of v
		 *	14.				ProcessNeighbourhood(graph, v, Nv)
		 *	15.			endif
		 *	16.		endif
		 *	17.	endwhile
		 * </pre>
		 *
		 * Note that lines (...) extend the neighbourhood of a vertex "Nv"
		 * depending of the type of graph. If the graph is directed and
		 * the user wants to process "reversed edges", then the neighbourhood
		 * is not limited to "out-neighbours", but also to "in-neighbours".
		 */
		void do_traversal() {
			while (not m_Q.empty()) {
				const node s = m_Q.front();
				m_Q.pop();

				// process current vertex
				m_proc_cur(*this, s);

				// check user-defined early termination condition
				if (m_term(*this, s)) { break; }

				process_neighbours(s);
			}
		}

	protected:
		// Constant reference to the graph.
		const G& m_G;
		// The queue of the traversal.
		std::queue<node> m_Q;
		// The set of visited nodes.
		std::vector<bool> m_vis;
		// Should we process already visitied neighbours?
		bool m_proc_vis_neighs = false;
		// Use back edges in directed graphs.
		bool m_use_rev_edges = false;

	protected:
		/*
		 * @brief BFS early terminating function.
		 *
		 * Returns true if the @ref BFS algorithm should terminate.
		 *
		 * For more details on when this function is called see @ref do_traversal.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The vertex at the front of the queue of the algorithm.
		 */
		bfs_bool_function m_term;

		/*
		 * @brief BFS vertex processing function.
		 *
		 * Processes the current vertex visited.
		 *
		 * For more details on when this function is called see @ref do_traversal.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The vertex at the front of the queue of the algorithm.
		 */
		bfs_process_one m_proc_cur;

		/*
		 * @brief BFS out-neighbour vertex processing function.
		 *
		 * Processes the next visited vertex. The direction of the vertices
		 * visited passed as parameters is given the boolean parameter. If
		 * it is true then the edge is s -> t. If it is false then the edge is
		 * t -> s.
		 *
		 * For more details on when this function is called see @ref do_traversal.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The vertex at the front of the queue of the algorithm.
		 * @param t The vertex neighbour of @e u visited by the algorithm.
		 */
		bfs_process_two m_proc_out_neigh;

		/*
		 * @brief BFS node addition function.
		 *
		 * Determines whether a vertex @e s should be added to the queue or not.
		 *
		 * For more details on when this function is called see @ref do_traversal.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The vertex candidate for addition.
		 */
		bfs_bool_function m_add_vertex;
};

} // -- namespace utils
} // -- namespace lal
