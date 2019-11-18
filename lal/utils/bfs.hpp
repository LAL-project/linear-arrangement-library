/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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

namespace lal {
namespace utils {

template<class G, typename node = typename lal::node>
class BFS {
	public:
		/*
		 * @brief BFS early terminating function.
		 *
		 * Returns true if the @ref BFS algorithm should terminate.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The node at the front of the queue of the algorithm.
		 */
		typedef
		std::function<bool (const BFS<G>& bfs, const node s)>
		bfs_terminate;

		/*
		 * @brief BFS node processing function.
		 *
		 * Processes the current node visited.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The node at the front of the queue of the algorithm.
		 */
		typedef
		std::function<void (const BFS<G>& bfs, const node s)>
		bfs_process_current;

		/*
		 * @brief BFS node processing function.
		 *
		 * Processes the next visited node. The direction of the vertices
		 * visited passed as parameter is always s -> t.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param BFS The object containing the traversal. This also contains
		 * several attributes that might be useful to guide the traversal.
		 * @param s The node at the front of the queue of the algorithm.
		 * @param t The node neighbour of @e u visited by the algorithm.
		 */
		typedef
		std::function<void (const BFS<G>& bfs, const node s, const node t)>
		bfs_process_neighbour;

	public:
		// Constructor
		BFS(const G& g) : m_G(g) {
			reset();
		}
		// Destructor
		~BFS() { }

		// Set the BFS to its initial state.
		void reset() {
			const uint64_t n = m_G.n_nodes();
			m_vis = std::vector<bool>(n, false);

			set_terminate_default();
			set_process_current_default();
			set_process_neighbour_default();
		}

		/*
		 * @brief Generic Breadth-First search algorithm.
		 *
		 * The BFS traversal is implemented as follows:
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
		 *	10.			Nv = neighbourhood of v
		 *	11.			for each w in Nv do
		 *  12.				if w has not been visited before, or it has been
		 *	13.					and we must process all previously visited nodes
		 *	14.				then
		 *	15.					proc_neigh(u,w)
		 *	16.				if w not visited before then
		 *	17.					push w into Q
		 *	18.					mark w as visited in vis
		 *	19.				endif
		 *	20.			endfor
		 *	21.		endif
		 *	22.	endwhile
		 * </pre>
		 *
		 * However, the queue of this object is not cleared before or after
		 * calling this function. The user of this class has call method
		 * @ref clear_queue if they want the queue to be empty at the beginning
		 * of every traversal.
		 *
		 * @param source The node where the algorithm starts at.
		 * @param terminate The terminating function. It is used as a termination
		 * condition in line 8.
		 * @param proc_curr The function to process the currently visited node.
		 * It is called in line 7 used to perform some operation on the current
		 * node of the traversal.
		 * @param proc_neigh It is called in line 15 used to perform some
		 * operation on each of the neighbours.
		 */
		void start_at(const node source) {
			m_Q.push(source);
			m_vis[source] = true;
			bool term = false;

			while (not m_Q.empty() and not term) {
				const node s = m_Q.front();
				m_Q.pop();

				// process current vertex
				m_proc_cur(*this, s);

				// check user-defined termination condition
				term = m_term(*this, s);

				if (not term) {
					for (const node& t : m_G.get_neighbours(s)) {
						// Process the neighbour found.
						// This is always called: even if it has been
						// visited before.
						if ((m_vis[t] and m_proc_vis_neighs) or not m_vis[t]) {
							m_proc_neigh(*this, s, t);
						}
						if (not m_vis[t]) {
							m_Q.push(t);
							m_vis[t] = true;
						}
					}
				}
			}
		}

		/* SETTERS */

		void set_terminate_default()
		{ m_term = [](const BFS<G>&, const node) -> bool { return false; }; }
		void set_terminate(bfs_terminate f)
		{ m_term = f; }

		void set_process_current_default()
		{ m_proc_cur = [](const BFS<G>&, const node) -> void { }; }
		void set_process_current(bfs_process_current f)
		{ m_proc_cur = f; }

		void set_process_neighbour_default()
		{ m_proc_neigh = [](const BFS<G>&, const node, const node) -> void { }; }
		void set_process_neighbour(bfs_process_neighbour f)
		{ m_proc_neigh = f; }

		/*
		 * @brief Should the algorithm call the neighbour processing function
		 * for already visited neighbours?
		 * @param v Either true or false.
		 */
		void process_visited_neighbours(bool v) {
			m_proc_vis_neighs = v;
		}

		// Sets all nodes to not visited
		void reset_visited() {
			std::fill(m_vis.begin(), m_vis.end(), false);
		}

		// Empties the queue used for the traversal
		void clear_queue() {
			while (not m_Q.empty()) { m_Q.pop(); }
		}

		/* GETTERS */

		// Returns the set of visited nodes.
		bool node_was_visited(node u) const { return m_vis[u]; }

		bool all_visited() const {
			return find(m_vis.begin(), m_vis.end(), false) == m_vis.end();
		}

		// Return visited nodes information
		const std::vector<bool> get_visited() const { return m_vis; }

	private:
		// Constant reference to the graph.
		const G& m_G;
		// The queue of the traversal.
		std::queue<node> m_Q;
		// The set of visited nodes.
		std::vector<bool> m_vis;
		// Should we process already visitied neighbours?
		bool m_proc_vis_neighs = false;

	private:
		// Terminating function.
		bfs_terminate m_term;
		// Process current vertex function
		bfs_process_current m_proc_cur;
		// Process neighbour vertex function
		bfs_process_neighbour m_proc_neigh;
};

} // -- namespace utils
} // -- namespace lal
