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
		void printAdj();
	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		int** vetexCotain; // store the vertex already in multicast tree
};

Problem1::Problem1(Graph G) {
	
	adjList = new vector<edgeList>[G.V.size()];
	vetexCotain = new int*[G.V.size()];
	size = G.V.size();
	for (int i = 0; i < G.E.size(); i++) {
		edgeList t = {i, G.E[i].vertex[1], G.E[i].ce};
		edgeList t2 = {i, G.E[i].vertex[0], G.E[i].ce};
		adjList[G.E[i].vertex[0] - 1].push_back(t);
		adjList[G.E[i].vertex[1] - 1].push_back(t2);
	}

	for (int i = 0; i < G.V.size(); i++) {
		vetexCotain[i] = new int[G.V.size()];

		for (int j = 0; j < G.V.size(); j++) {
			vetexCotain[i][j] = 0;
			if (i == j) {
				vetexCotain[i][j] = 1;
			}
		}
	}

	//printAdj();
}

Problem1::~Problem1() {
	/* Write your code here. */
	for (int i = 0; i < size; i++) {
		adjList[i].clear();
		delete [] vetexCotain[i];
	}

	delete [] adjList;
	delete [] vetexCotain;
}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {

	/* Store your output graph and multicast tree into G and MTid */
	Tree temp;
	temp.s = s;
	temp.id = id;
	temp.ct = 0;

	//copy set into MTid Vetex set
	temp.V = D.destinationVertices;

	int n = D.size - 1; // how many the number of edge needed
	s--;
	// using prime's algorithm
	// initialize distance table
	priority_queue<edgeList, vector<edgeList>, cmp> pq;

	for (auto it = adjList[s].begin(); it < adjList[s].end(); it++) {
		if (G.E[it->index].b >= t) {
			pq.push(*it);
		}
	}

	while (n && !pq.empty()) {
		edgeList e = pq.top(); pq.pop();
		int i = e.dest-1;

		if (vetexCotain[s][i]) continue;
		
		// select edge
		vetexCotain[s][i] = 1;
		treeEdge te = {{G.E[e.index].vertex[0], G.E[e.index].vertex[1]}};
		temp.E.push_back(te);
		temp.ct += e.cost;

		// update bandwith
		G.E[e.index].b -= t;
		n--;

		// adjoint edge check
		vector<edgeList>::iterator it = adjList[i].begin();
		for (; it < adjList[i].end(); it++) {
			if (!vetexCotain[s][it->dest-1] && G.E[it->index].b >= t) {
				pq.push(*it);
			}
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

void Problem1::printAdj() {
	for (int i = 0; i < size; i++) {
		vector<edgeList>::iterator it;
		for (it = adjList[i].begin(); it < adjList[i].end(); it++) {
			std::cout << i+1 << " " << it->dest << " " << it->cost << std::endl;
		}
	}

	std::cout << std::endl;
}
