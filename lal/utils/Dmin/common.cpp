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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          LOGPROG: Logics and Programming Research Group
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 ********************************************************************/

#include <lal/utils/Dmin/common.hpp>

// Implementation of Shiloach's algorithm for Minimal Linear Arrangement
// for undirected trees.

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
using namespace std;

void size_of_all_subtrees_from_one_node(adjacency_matrix & g,vector<uint32_t> &v, uint32_t id_root)
// v[i] is the size of the subtree rooted in i inside the tree
// rooted in id_root, if there is a path from id_root to i.
// There can be several nodes unconnected to id_root
// after some recursive calls.
{
	uint32_t n=v.size()-1;
	v[id_root]=1;
	for (uint32_t i=1;i<=n;++i){
		if (g[id_root][i]==1){
			g[i][id_root]=0;
			size_of_all_subtrees_from_one_node(g,v,i);
			v[id_root]+=v[i];
			g[i][id_root]=1;
		}
	}
}

void reachable_nodes(adjacency_matrix & g, vector<uint32_t> &v, uint32_t root)
// We get all nodes reachable from node root
{
	v.push_back(root);
	uint32_t n=g.size()-1;
	for (uint32_t i=1;i<=n;++i){
		if (g[root][i]==1){
			g[i][root]=0;
			reachable_nodes(g,v,i);
			g[i][root]=1;
		}
	}
}

void size_of_all_subtrees_from_all_nodes(adjacency_matrix & g, size_matrix & s, const vector<uint32_t> & v)
// elements of v are all the nodes connected among them in g
{
	uint32_t n=g.size()-1;
	s = size_matrix(n+1,vector<uint32_t> (n+1,0));
	for (uint32_t i=0;i<v.size();++i) {
		size_of_all_subtrees_from_one_node(g,s[v[i]],v[i]);
	}
}

bool order_pair(const pair<uint32_t,uint32_t> &p1, const pair<uint32_t,uint32_t> & p2)
{
	if (p1.first==p2.first) return p1.second<p2.second;
	else return p1.first > p2.first;
}

void order_subtrees_by_size(const adjacency_matrix & g, vector<uint32_t> & s, uint32_t root, ordering & ord)
{
	uint32_t n=g.size()-1;
	assert(ord.size()==0); // Ordering should be empty
	pair<uint32_t,uint32_t> elem;
	for (uint32_t i=1;i<=n;++i){
		if (g[root][i]==1) {
			elem.first=s[i];
			elem.second=i;
			ord.push_back(elem);
		}
	}
	sort(ord.begin(),ord.end(),order_pair);
}

uint32_t evaluate_arrangement(const vector<uint32_t> & arr,const edge_vector & mt)
// arrangement uses position 0 of the vector
// arr[i]=p means that node i+1 is in possition p in the arrangement
// for example arr[2]=5 means that node labelled 3 is fifth in the arrangement
{
	uint32_t res=0;
	for (uint32_t i=0;i<mt.size();++i){
		uint32_t x,a,b;
		a = arr[mt[i].first-1];
		b = arr[mt[i].second-1];
		if (a>b) x=a-b; else x=b-a;
		res+=x;
	}

	return res;
}

void convert_tree_to_matrix(uint32_t n,const list<pair<uint32_t,uint32_t> > & t, adjacency_matrix & g)
{
	g = adjacency_matrix (n+1,vector<uint32_t> (n+1,0));
	list<pair<uint32_t,uint32_t> >::const_iterator it;
	for (it=t.begin();it!=t.end();++it){
		uint32_t x,y;
		x=(*it).first;
		y=(*it).second;
		assert(g[x][y]==0); // If it is 1, the node is repeated
		g[x][y]=g[y][x]=1;
	}
}

