#include "basicDS.h"
#include "Problem2.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define bpair pair<int, int>
#define INF 1000
class Problem2 {
	public:
		Problem2(Graph G);  //constructor
		~Problem2();        //destructor
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);
		void dijkstra(Graph &G, Tree &MTid, Metric& metric , int s, const int& t);
		void shortestPath(Graph &G, Tree &MTid, Metric& metric, const int& t); // metric closure
		int getBandwith(const int& s, const int& id);
		int getContainIndex(const int& s, const int& id);
		void printTree(int id, Forest MTidForest);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(Forest F); // print all of the MTid
		void printBandwid();
		void printShortest(Graph& G, Tree &MTid, Metric& metric);
	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		vector<vector<Contain>> contain; // key: id value: 1D array
		vector<vector<bpair>> bandwidth;
		Graph t_G;
		Forest t_F;
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

	t_G = G;
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

int Problem2::getBandwith(const int& s,const int& id) {
	for (int i = 0; i < bandwidth[s].size(); i++) {
		if (bandwidth[s][i].first == id) return i;
	}
	return -1;
}

int Problem2::getContainIndex(const int& source, const int& id) {
	int i;
	for (i = 0; i < contain[source].size() && contain[source][i].id != id; i++);
	return i;
}


void Problem2::dijkstra(Graph &G, Tree &MTid, Metric& metric , int s, const int& t) {
	priority_queue<bpair, vector<bpair>, greater<bpair>> pq;
	s--;
	for (int i = 0; i < size; i++) {
		metric.distance[s][i] = INF;
	}

	metric.distance[s][s] = 0;

	pq.push({0, s});

	while(!pq.empty()) {
		int u = pq.top().second;
		int dist_u = pq.top().first;
		pq.pop();

		for (auto edge : adjList[u]) {
			int v = edge.dest-1;

			if (dist_u + edge.cost < metric.distance[s][v]) {
				metric.distance[s][v] = dist_u + edge.cost;
				metric.edges[s][v] = edge;
				pq.push({metric.distance[s][v], v});
			}
		}

	}
}

void Problem2::shortestPath(Graph &G, Tree &MTid, Metric& metric, const int& t) {

	// initilize
	metric.distance = new int*[size];
	metric.edges = new edgeList*[size];

	for (int i = 0; i < G.V.size(); i++) {
		metric.distance[i] = new int[size];
		metric.edges[i] = new edgeList[size];
	}

	// find all shortestPath

	for (auto i : MTid.V) {
		dijkstra(G, MTid, metric, i, t);
	}

	printShortest(G, MTid, metric);
}

void Problem2::printShortest(Graph& G, Tree &MTid, Metric& metric) {

	for (auto i : MTid.V) {
		i--;
		std::cout << "===From [" << i+1 << "] ===\n";
		for (int j = 0; j < G.V.size(); j++) {
			std::cout << i+1 << " " << j+1 <<  " : " << metric.distance[i][j] << "\n";
		}
		std::cout << "=======End======\n";
	}
}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */
	Tree t_MTid;
	Metric metric;
	
	// initial vertex
	for (int i = 0; i < D.size; i++) {
		t_MTid.V.push_back(D.destinationVertices[i]);
	}

	t_MTid.s = s;
	t_MTid.id = id;
	t_MTid.ct = 0;

	// store multicast tree cost
	bandwidth[s].push_back(make_pair(id, t));

	shortestPath(t_G, t_MTid, metric, t);
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

void Problem2::printTree(int id, Forest MTidForest) {
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

void Problem2::printAdj() {
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

void Problem2::printGraph(Graph G) {
	std::cout << "===Graph Edge===\n";
	for (auto it = G.E.begin(); it < G.E.end(); it++) {
		std::cout << it->vertex[0] << " " << it->vertex[1] << ": " << it->b << "/" << it->ce << "\n";
	}
	std::cout << "=======End======\n";
}

void Problem2::printForest(Forest F) {
	for (int i = 0; i < F.size; i++) {
		printTree(F.trees[i].id, F);
	}
}

void Problem2::printBandwid() {
	for (int i = 0; i < size; i++) {
		std::cout << "vertex " << i+1 << ":\n";
		for (auto it = bandwidth[i].begin(); it < bandwidth[i].end(); it++) {
			std::cout << it->first << " " << it->second << "\n";
		}
	}
}