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

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
#include <cassert>
using namespace std;

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

typedef vector<pair<uint32_t,uint32_t> > edge_vector;
// Vector with all edges

void size_of_all_subtrees_from_one_node(adjacency_matrix & g,vector<uint32_t> &v, uint32_t id_root);

void reachable_nodes(adjacency_matrix & g, vector<uint32_t> &v, uint32_t root);

void size_of_all_subtrees_from_all_nodes(adjacency_matrix & g, size_matrix & s, const vector<uint32_t> & v);

bool order_pair(const pair<uint32_t,uint32_t> &p1, const pair<uint32_t,uint32_t> & p2);

void order_subtrees_by_size(const adjacency_matrix & g, vector<uint32_t> & s, uint32_t root, ordering & ord);

void convert_tree_to_matrix(uint32_t n,const list<pair<uint32_t,uint32_t> > & t, adjacency_matrix & g);

uint32_t calculate_D_min_Shiloach(uint32_t n, list<pair<uint32_t,uint32_t> >&tree);

// Added to check minor changes, the code was in cpp
uint32_t evaluate_arrangement(const vector<uint32_t> & arr,const edge_vector & mt);


