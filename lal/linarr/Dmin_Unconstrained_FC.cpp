/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          LOGPROG: Logics and Programming Research Group
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 ********************************************************************/

#include <lal/linarr/Dmin.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
#include <vector>
#include <list>
using namespace std;

#include <lal/utils/Dmin/common.hpp>

#define LEFT_ANCHOR -1
#define RIGHT_ANCHOR 1
#define NO_ANCHOR 0
#define ANCHOR 1 // It is used for parameter anchored in calculate_p in Shiloach's algorithm
#define TO_THE_RIGHT 1 // Not used
#define TO_THE_LEFT -1 // Not used

typedef vector<vector<uint32_t> > adjacency_matrix;
// Neither row 0 nor column 0 is used.

typedef vector<vector<uint32_t> > size_matrix;
// Neither row 0 nor column 0 is used
// It's the same as adjacency_matrix but is
// used to keep the sizes of all subtrees
// using all nodes as roots.
// elem[i][j] is the size of the subtree
// rooted in j in the tree rooted in i
// In one matrix there can be several
// unconnected trees.

typedef vector<pair<uint32_t,uint32_t> > ordering;
// Elements are size and root of subtrees ordered by size

namespace lal {
using namespace graphs;

namespace linarr {

uint32_t get_center_for_chung(adjacency_matrix & g, const size_matrix & s, const vector<uint32_t> & list_of_nodes)
{
	uint32_t n=g.size()-1;
	uint32_t number_of_nodes=list_of_nodes.size();
	uint32_t h=number_of_nodes/2;
	uint32_t v_star=0;
	uint32_t i=0;
	bool v_star_found=false;
	while(i<number_of_nodes and not v_star_found){
		uint32_t j=1;
		bool tree_too_big=false;
		while (not tree_too_big and j<=n){
			uint32_t ith_root=list_of_nodes[i];
			if (g[ith_root][j]==1){
				if (s[ith_root][j]>h) tree_too_big=true;
				else j++;
			}
			else j++;
		}
		if (not tree_too_big) {
			v_star=list_of_nodes[i];
			v_star_found=true;
		}
		if (not v_star_found) ++i;
	}
	return v_star;
}

int calculate_q(const ordering & ord)
{
	uint32_t n=1; // Counting root
	for (uint32_t i=0;i<ord.size();++i) n+=ord[i].first; // n is number of nodes
	uint32_t k=ord.size(); // number of subtrees
	--k; // T_0, T_1, ..., T_k
	uint32_t t_0= ord[0].first;
	int q= k/2;  // Maximum possible p_alpha
	uint32_t sum=0;
	for (int i=0;i<=2*q;++i) sum+=ord[i].first;
	uint32_t z=n-sum;
	uint32_t tricky_formula=(t_0+2)/2+(z+2)/2;
	// t_0 >= t_1 >= ... >= t_k
	uint32_t t_2q=ord[2*q].first;
	while(q>=0 and t_2q <= tricky_formula){
		z+=ord[2*q].first;
		if(q>0) z+=ord[2*q-1].first;
		--q;
		tricky_formula=(t_0+2)/2+(z+2)/2;
		if (q>=0) t_2q=ord[2*q].first;
	}
	return q;
}

int calculate_p(const ordering & ord)
{
	if (ord.size()<2) {
		uint32_t p=-1;
		return p;
	}
	uint32_t n=1; // Counting root
	for (uint32_t i=0;i<ord.size();++i) n+=ord[i].first; // n is number of nodes
	uint32_t k=ord.size(); // number of subtrees
	--k; // T_0, T_1, ..., T_k
	uint32_t t_0= ord[0].first;
	int  p=(k-1)/2;
	uint32_t sum=0;
	for (uint32_t i=0;i<=2*p+1;++i) sum+=ord[i].first;
	uint32_t y=n-sum;
	uint32_t tricky_formula=(t_0+2)/2+(y+2)/2;
	uint32_t t_2p_plus_1=ord[2*p+1].first;
	while(p>=0 and t_2p_plus_1 <=tricky_formula){
		y=y+ord[2*p+1].first+ord[2*p].first;
		--p;
		tricky_formula=(t_0+2)/2+(y+2)/2;
		if (p>=0) t_2p_plus_1=ord[2*p+1].first;
	}
	return p;

}

void get_P(vector<uint32_t> & v, uint32_t p, uint32_t i)
{
	v=vector<uint32_t> (2*p+1+1,-1);
	uint32_t j=0;
	uint32_t pos=v.size()-1;
	uint32_t pos_der=pos;
	uint32_t pos_izq=1;
	while (j<=2*p+1){
		if (j==i) ++j;
		else {
			v[pos]=j;
			if (pos>pos_izq) {--pos_der;pos=pos_izq;}
			else {++pos_izq; pos=pos_der;}
			++j;
		}
	}
}

void get_Q(vector<uint32_t> & v, uint32_t q, uint32_t i)
{
	v=vector<uint32_t> (2*q+1,-1);
	uint32_t j=0;
	uint32_t pos=v.size()-1;
	uint32_t pos_der=pos;
	uint32_t pos_izq=1;
	while (j<=2*q){
		if (j==i) ++j;
		else {
			v[pos]=j;
			if (pos>pos_izq) {--pos_der;pos=pos_izq;}
			else {++pos_izq; pos=pos_der;}
			++j;
		}
	}
}
void calculate_mla_chung(adjacency_matrix & g, int root, uint32_t one_node, uint32_t start, vector<uint32_t> & arr, uint32_t & cost)
{
	uint32_t n=g.size()-1;
	vector<uint32_t> node_vector;
	reachable_nodes(g,node_vector,one_node);
	uint32_t size_tree=node_vector.size(); // Size of the tree

	assert(size_tree>0);
	assert(root==NO_ANCHOR or root==RIGHT_ANCHOR or root==LEFT_ANCHOR);

	// Base case
	if (size_tree==1) {
		assert(one_node==node_vector[0]);
		assert(start<=n);
		arr[one_node-1]=start;
		cost=0;
		return;
	}

	size_matrix s;
	size_of_all_subtrees_from_all_nodes(g,s,node_vector);

	if (root==NO_ANCHOR){
		uint32_t u=get_center_for_chung(g,s,node_vector);
		one_node=u; // Probably not needed
		ordering ord;
		order_subtrees_by_size(g,s[u],u,ord);
		int q=calculate_q(ord);
		if (q==-1){
			uint32_t t_0=ord[0].second;
			uint32_t n_0=ord[0].first;
			assert(u!=t_0);
			g[u][t_0]=g[t_0][u]=0;
			uint32_t c1,c2;
			calculate_mla_chung(g,RIGHT_ANCHOR,t_0,start,arr,c1);
			calculate_mla_chung(g,LEFT_ANCHOR,u,start+n_0,arr,c2);
			cost=c1+c2+1;
			g[u][t_0]=g[t_0][u]=1;
		}
		else{
			cost=n*n; // More than the maximum
			for(uint32_t i=0;i<=2*q;++i){
				assert(u!=ord[i].second);
				g[u][ord[i].second]=g[ord[i].second][u]=0;
			} // Now g is Y

			// Central tree size
			uint32_t size_rest_of_trees= 0;
			for (uint32_t i=2*q+1;i < ord.size() ;++i) size_rest_of_trees+=ord[i].first;


			for(uint32_t i=0;i<=2*q;++i){
				vector<uint32_t> Q_i;
				get_Q(Q_i,q,i);
				g[u][ord[i].second]=g[ord[i].second][u]=1;

				uint32_t c_i=0;
				vector<uint32_t> arr_aux;
				arr_aux = arr;
				uint32_t start_aux=start;

				// Left part of the arrangement
				for (uint32_t j=1;j<=q;++j){
					uint32_t c_i_j;
					uint32_t node_position_in_ordering=Q_i[j];
					calculate_mla_chung(g,RIGHT_ANCHOR,ord[node_position_in_ordering].second,start_aux,arr_aux,c_i_j);
					start_aux+=ord[node_position_in_ordering].first;
					c_i+=c_i_j;
				}

				// Central part of the arrangement
				uint32_t c_i_j;
				calculate_mla_chung(g,NO_ANCHOR,u,start_aux,arr_aux,c_i_j);
				c_i+=c_i_j;

				// Right part of the arrangement
				start_aux+=ord[i].first+1+size_rest_of_trees;
				for (uint32_t j=q+1;j<=2*q;++j){
					uint32_t c_i_j;
					uint32_t node_position_in_ordering=Q_i[j];
					calculate_mla_chung(g,LEFT_ANCHOR,ord[node_position_in_ordering].second,start_aux,arr_aux,c_i_j);
					start_aux+=ord[node_position_in_ordering].first;
					c_i+=c_i_j;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i+=size_tree*q;
				uint32_t subs=0;
				for (uint32_t j=1;j<=q;++j) {
					subs+=(q-j+1)*(ord[Q_i[j]].first+ ord[Q_i[2*q-j+1]].first);
				}
				c_i-=subs;
				c_i+=q; // NOT IN CHUNG'S PAPER
				if (c_i<cost) {
					cost=c_i;
					arr=arr_aux;
				}
				assert(u!=ord[i].second);
				g[u][ord[i].second]=g[ord[i].second][u]=0;
			}
			for(uint32_t i=0;i<=2*q;++i){
				g[u][ord[i].second]=g[ord[i].second][u]=1;
			} // Now g is again g
		}
	}
	// Rooted tree
	else{
		ordering ord;
		order_subtrees_by_size(g,s[one_node],one_node,ord);
		int p=calculate_p(ord);
		if (p==-1){
			uint32_t t_0=ord[0].second;
			uint32_t n_0=ord[0].first;
			assert(one_node!=t_0);
			g[one_node][t_0]=g[t_0][one_node]=0;
			uint32_t c1,c2;
			calculate_mla_chung(g,RIGHT_ANCHOR,t_0,start,arr,c1);
			calculate_mla_chung(g,NO_ANCHOR,one_node,start+n_0,arr,c2);
			cost=c1+c2+size_tree-ord[0].first;
			g[one_node][t_0]=g[t_0][one_node]=1;
		}
		else{
			cost=n*n; // More than the maximum
			for(uint32_t i=0;i<=2*p+1;++i){
				assert(one_node!=ord[i].second);
				g[one_node][ord[i].second]=g[ord[i].second][one_node]=0;
			} // Now g is Y

			// Central tree size
			uint32_t size_rest_of_trees= 0;
			for (uint32_t i=2*p+2;i < ord.size() ;++i) size_rest_of_trees+=ord[i].first;


			for(uint32_t i=0;i<=2*p+1;++i){
				vector<uint32_t> P_i;
				get_P(P_i,p,i);
				g[one_node][ord[i].second]=g[ord[i].second][one_node]=1;

				uint32_t c_i=0;
				vector<uint32_t> arr_aux;
				arr_aux = arr;
				uint32_t start_aux=start;

				// Left part of the arrangement
				for (uint32_t j=1;j<=p;++j){
					uint32_t c_i_j;
					uint32_t node_position_in_ordering=P_i[j];
					calculate_mla_chung(g,RIGHT_ANCHOR,ord[node_position_in_ordering].second,start_aux,arr_aux,c_i_j);
					start_aux+=ord[node_position_in_ordering].first;
					c_i+=c_i_j;
				}

				// Central part of the arrangement
				uint32_t c_i_j;
				calculate_mla_chung(g,NO_ANCHOR,one_node,start_aux,arr_aux,c_i_j);
				start_aux+=ord[i].first+1+size_rest_of_trees;
				c_i+=c_i_j;

				// Right part of the arrangement
				for (uint32_t j=p+1;j<=2*p+1;++j){
					uint32_t c_i_j;
					uint32_t node_position_in_ordering=P_i[j];
					calculate_mla_chung(g,LEFT_ANCHOR,ord[node_position_in_ordering].second,start_aux,arr_aux,c_i_j);
					start_aux+=ord[node_position_in_ordering].first;
					c_i+=c_i_j;
				}

				// Adding parts of the anchors over trees nearer to the central tree
				c_i+=size_tree*(p+1);
				c_i-=(p+1)*ord[P_i[P_i.size()-1]].first;
				uint32_t subs=0;
				for (uint32_t j=1;j<=p;++j) {
					subs+=(p-j+1)*(ord[P_i[j]].first+ ord[P_i[2*p-j+1]].first);
				}
				c_i-=subs;
				c_i+=p; // NOT IN CHUNG'S PAPER
				if (c_i<cost) {
					cost=c_i;
					arr=arr_aux;
				}
				assert(one_node!=ord[i].second);
				g[one_node][ord[i].second]=g[ord[i].second][one_node]=0;
			}

			for(uint32_t i=0;i<=2*p+1;++i){
				g[one_node][ord[i].second]=g[ord[i].second][one_node]=1;
			} // Now g is again g
		}
	}

	// Flipping arrangement if needed
	if(root==RIGHT_ANCHOR) {
		if (2*arr[one_node-1]-2*start<size_tree-1){
			// Right anchor and the root is too much to the left
			for(uint32_t i=0;i<size_tree;++i){
				uint32_t aux = start+size_tree-1-arr[node_vector[i]-1]+start;
				arr[node_vector[i]-1]=aux;
			}
		}
	}
	else if (root==LEFT_ANCHOR) {
		if (2*arr[one_node-1]-2*start>size_tree-1){
			// Left anchor and the root is too much to the right
			for(uint32_t i=0;i<size_tree;++i){
				uint32_t aux = start+size_tree-1-arr[node_vector[i]-1]+start;
				arr[node_vector[i]-1]=aux;
			}
		}
	}
}

pair<uint32_t, linearrgmnt> compute_Dmin_Unconstrained_FC(const ftree& t) {
	assert(t.is_tree());

	const uint32_t n = t.n_nodes();

	adjacency_matrix g;
	g = adjacency_matrix (n+1,vector<uint32_t> (n+1, 0));
	for (node u = 0; u < t.n_nodes(); ++u) {
		for (const node v : t.get_neighbours(u)) {
			g[u + 1][v + 1] = g[v + 1][u + 1] = 1;
		}
	}

	vector<uint32_t> arrangement(n,0);

	uint32_t c;
	uint32_t alpha=0;
	//calculate_mla(g,alpha,1,1,arrangement,c);
	calculate_mla_chung(g,alpha,1,0,arrangement,c);

	return make_pair(c, arrangement);
}

} // -- namespace linarr
} // -- namespace lal
