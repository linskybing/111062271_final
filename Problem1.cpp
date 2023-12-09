#include "basicDS.h"
#include "Problem1.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

class Problem1 {
	public:
		Problem1(Graph G);  //constructor
		~Problem1();        //destructor
		void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);
		void printTree(int id, Forest MTidForest);
	private:
		vector<sortEdge> edges; // sort the graph edge with bandwithcost;
		vector<vector<int>> tree_contain; // store the vertex already in multicast tree
};

Problem1::Problem1(Graph G) {
	/* Write your code here. */
	for (int i = 0; i < G.E.size(); i++) {
		sortEdge t = {G.E[i], i};
		edges.push_back(t);
	}

	std::sort(edges.begin(), edges.end(), Compare);
}

Problem1::~Problem1() {
	/* Write your code here. */
	edges.clear();
}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {

	/* Store your output graph and multicast tree into G and MTid */
	Tree temp;
	temp.s = s;
	temp.id = id;
	temp.ct = 0;

	// disjoint set
	DSU dsu(D.size);

	//copy set into MTid Vetex set
	temp.V = D.destinationVertices;

	int edge_need_num = D.size - 1;

	for (int i = 0; i < edges.size() && edge_need_num > 0; i++) {
		std::cout << G.E[edges[i].index].vertex[0] << " " 
				  << G.E[edges[i].index].vertex[1] << " " 
				  << G.E[edges[i].index].b << "\n";
		if (!dsu.same(edges[i].e.vertex[0], edges[i].e.vertex[1]) && G.E[edges[i].index].b >= t) {
				// union
			    dsu.uni(edges[i].e.vertex[0], edges[i].e.vertex[1]);

				// add treeEdge into multicast tree
				treeEdge te = {{edges[i].e.vertex[0], edges[i].e.vertex[1]}};
				temp.E.push_back(te);
                temp.ct += edges[i].e.ce;
				// update Graph information
				G.E[edges[i].index].b -= t;
                edge_need_num--;
		}
	}

	MTid = temp;
	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */

	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	
	return;
}

void Problem1::printTree(int id, Forest MTidForest) {
	// find the tree in MTidForest by id
	int index = 0;
	for (; index < MTidForest.size && MTidForest.trees[index].id != id; index++);
	if (index >= MTidForest.size) return;

	// print the tree information
	Tree t = MTidForest.trees[index];
	std::cout << "source: " << t.s << "\n";
	std::cout << "Vertex: { ";
	for (int it : t.V) {
		std::cout << it << " ";
	}
	std::cout << "}" << "\n";
	std::cout << "edge: " << "\n";
	for (treeEdge edge : t.E) {
		std::cout << edge.vertex[0] << " " << edge.vertex[1] << "\n";
	}
	std::cout << "total cost: " << t.ct << "\n\n";

	return;
}
