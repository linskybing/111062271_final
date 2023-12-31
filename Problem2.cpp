#include "basicDS.h"
#include "Problem2.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define bpair pair<int, int>

class Problem2 {
	public:
		Problem2(Graph G);  //constructor
		~Problem2();        //destructor
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);
		int getBandwith(const int& s, const int& id);
		int getContainIndex(const int& s, const int& id);
		void printTree(int id, Forest MTidForest);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(Forest F); // print all of the MTid
		void printBandwid();
	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		int** vetexCotain; // store the vertex already in multicast tree
		vector<vector<bpair>> bandwidth;
};

Problem2::Problem2(Graph G) {
	adjList = new vector<edgeList>[G.V.size()];
	size = G.V.size();
	for (int i = 0; i < G.E.size(); i++) {
		// undirected graph
		edgeList t = {i, G.E[i].vertex[1], G.E[i].ce};
		edgeList t2 = {i, G.E[i].vertex[0], G.E[i].ce};
		adjList[G.E[i].vertex[0] - 1].push_back(t);
		adjList[G.E[i].vertex[1] - 1].push_back(t2);
	}

	for (int i = 0; i < G.V.size(); i++) {
		// sort the adjlist edge by cost
		sort(adjList[i].begin(), adjList[i].end(), Compare);
	}
	contain.reserve(size);
	bandwidth.reserve(size);
}

Problem2::~Problem2() {
	for (int i = 0; i < size; i++) {
		adjList[i].clear();
		bandwidth[i].clear();
		contain[i].clear();
	}

	delete [] adjList;

	contain.clear();
	bandwidth.clear();

}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */


	/* You should return true or false according the insertion result */
	return true;
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */
	
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
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
	std::cout << "===   [" << id << "]    ===\n";
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
	std::cout << "=======End======\n";
	return;
}

void Problem1::printAdj() {
	std::cout << "====AdjList====\n";
	for (int i = 0; i < size; i++) {
		vector<edgeList>::iterator it;
		for (it = adjList[i].begin(); it < adjList[i].end(); it++) {
			std::cout << i+1 << " " << it->dest << " " << it->cost << std::endl;
		}
	}
	std::cout << "================\n";
	std::cout << std::endl;
}

void Problem1::printGraph(Graph G) {
	std::cout << "===Graph Edge===\n";
	for (auto it = G.E.begin(); it < G.E.end(); it++) {
		std::cout << it->vertex[0] << " " << it->vertex[1] << ": " << it->b << "/" << it->ce << "\n";
	}
	std::cout << "=======End======\n";
}

void Problem1::printForest(Forest F) {
	for (int i = 0; i < F.size; i++) {
		printTree(F.trees[i].id, F);
	}
}

void Problem1::printBandwid() {
	for (int i = 0; i < size; i++) {
		std::cout << "vertex " << i+1 << ":\n";
		for (auto it = bandwidth[i].begin(); it < bandwidth[i].end(); it++) {
			std::cout << it->first << " " << it->second << "\n";
		}
	}
}