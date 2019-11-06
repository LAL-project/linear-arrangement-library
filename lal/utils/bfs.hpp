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

namespace lal {
namespace utils {

template<class G, typename node>
class BFS {
	public:
		/*
		 * @brief Terminating function.
		 *
		 * Returns true if the @ref BFS algorithm should terminate.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param g The graph being traversed.
		 * @param s The node at the front of the queue of the algorithm.
		 * @param vis The set of visited nodes.
		 * @param Q The queue of the traversal.
		 */
		typedef std::function<
		bool (const G& g, node s, const std::vector<bool>& vis,
			  const std::queue<node>& Q)
		> bfs_terminate;

		/*
		 * @brief Node processing function.
		 *
		 * Processes the current node visited.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param g The graph being traversed.
		 * @param s The node at the front of the queue of the algorithm.
		 * @param vis The set of visited nodes.
		 * @param Q The queue of the traversal.
		 */
		typedef std::function<
		void (const G& g, node s, const std::vector<bool>& vis,
			const std::queue<node>& Q)
		> bfs_process_current;

		/*
		 * @brief Node processing function.
		 *
		 * Processes the next visited node. The direction of the vertices
		 * visited is always s -> t.
		 *
		 * For more details on when this function is called see @ref start_at.
		 * @param g The graph being traversed.
		 * @param s The node at the front of the queue of the algorithm.
		 * @param t The node neighbour of @e u visited by the algorithm.
		 * @param vis The set of visited nodes.
		 * @param Q The queue of the traversal.
		 */
		typedef std::function<
			void (
				const G& g, node s, node t,
				const std::vector<bool>& vis, const std::queue<node>& Q
			)
		> bfs_process_neighbour;

	public:
		// Constructor
		BFS(const G& g) : m_G(g) { }
		// Destructor
		~BFS() { }

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
		 *	 7.		proc_curr(G,v,vis)
		 *	 8.		if terminate(G,v,vis) then Finish traversal
		 *	 9.		else
		 *	10.			Nv = neighbourhood of v
		 *	11.			for each w in Nv do
		 *  12.				if w has not been visited before, or it has been
		 *	13.					and we must process all previously visited nodes
		 *	14.				then
		 *	15.					proc_neigh(G,u,w,vis)
		 *	16.				if w not visited before then
		 *	17.					push w into Q
		 *	18.					mark w as visited in vis
		 *	19.				endif
		 *	20.			endfor
		 *	21.		endif
		 *	22.	endwhile
		 * </pre>
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
		void start_at(
			node source,
			bfs_terminate terminate,
			bfs_process_current proc_curr,
			bfs_process_neighbour proc_neig
		)
		{
			const uint32_t n = m_G.n_nodes();
			m_vis = std::vector<bool>(n);
			m_Q.push(source);
			m_vis[source] = true;
			bool term = false;

			while (not m_Q.empty() and not term) {
				const node s = m_Q.front();
				m_Q.pop();

				// process current vertex
				proc_curr(m_G, s, m_vis, m_Q);

				// check user-defined termination condition
				term = terminate(m_G, s, m_vis, m_Q);

				if (not term) {
					for (const node& t : m_G.get_neighbours(s)) {
						// Process the neighbour found.
						// This is always called: even if it has been
						// visited before.
						if ((m_vis[t] and m_proc_vis_neighs) or not m_vis[t]) {
							proc_neig(m_G, s, t, m_vis, m_Q);
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

		/*
		 * @brief Should the algorithm call the neighbour processing function
		 * for already visited neighbours?
		 * @param v Either true or false.
		 */
		void process_visited_neighbours(bool v) {
			m_proc_vis_neighs = v;
		}

	private:
		// Constant reference to the graph.
		const G& m_G;
		// The queue of the traversal.
		std::queue<node> m_Q;
		// The set of visited nodes.
		std::vector<bool> m_vis;
		// Should we process already visitied neighbours?
		bool m_proc_vis_neighs = false;
};

} // -- namespace utils
} // -- namespace lal
