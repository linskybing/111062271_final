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

		// main function
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);

		// sub function
		void dijkstra(Tree &MTid, Metric& metric , int s, const int& t);
		bool shortestPath(Graph &G, Tree &MTid, Metric& metric, const int& t); // metric closure
		void addPath(Tree &MTid, Metric& metric, bool* contain, const int& d, const int& t); // from mst add path to MTid
		bool MST(Tree &MTid, Metric& metric, const int& t); // find MST from metric closure

		// get member variable
		int getBandwith(const int& s, const int& id);

		// output
		void printTree(int id);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(); // print all of the MTid
		void printBandwid();
		void printShortest(Graph& G, Tree &MTid, Metric& metric);

	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
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
	bandwidth.reserve(size);
	t_G = G;
}

Problem2::~Problem2() {
	for (int i = 0; i < size; i++) {
		vector<edgeList>().swap(adjList[i]);
		vector<bpair>().swap(bandwidth[i]);
	}
	vector<vector<bpair>>().swap(bandwidth);
	delete [] adjList;

}

int Problem2::getBandwith(const int& s,const int& id) {
	for (int i = 0; i < bandwidth[s].size(); i++) {
		if (bandwidth[s][i].first == id) return i;
	}
	return 0;
}

void Problem2::dijkstra(Tree &MTid, Metric& metric , int s, const int& t) {
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
			if (dist_u + edge.cost < metric.distance[s][v] && t_G.E[edge.index].b >= t) {
				metric.distance[s][v] = dist_u + edge.cost;
				metric.edges[s][v] = edge;
				pq.push({metric.distance[s][v], v});
			}
		}
	}
}

bool Problem2::MST(Tree &MTid, Metric& metric, const int& t) {
	// using prime's algorithm
	bool* contain = new bool[size];
	bool* contain_path = new bool[t_G.E.size()];
	priority_queue<bpair, vector<bpair>, greater<bpair>> pq;

	// initilize
	for (int i = 0; i < size; i++) {
		contain[i] = false;
	}

	for (int i = 0; i < t_G.E.size(); i++) {
		contain[i] = false;
	}

	int s = MTid.s-1;
	contain[s] = true;
	int need = MTid.V.size() - 1;

	// prime's algorithm

	for (auto v : MTid.V) {
		if (metric.distance[s][v-1] != INF)
			pq.push({metric.distance[s][v-1], v-1});
	}

	while (need && !pq.empty()) {
		int v = pq.top().second; pq.pop();

		if (contain[v]) continue;

		contain[v] = true;
		need--;

		addPath(MTid, metric, contain_path, v, t);

		for (auto u : MTid.V) {
			u--;
			if (!contain[u-1] && metric.distance[v][u] != INF) {
				pq.push({metric.distance[v][u], u});
			}
		}
	}

	// free memonry
	delete [] contain;
	delete [] contain_path;

	if (need){
		MTid.E.clear();
		MTid.ct = 0;
		return false;
	};

	return true; 
}

void Problem2::addPath(Tree &MTid, Metric& metric, bool* contain, const int& d, const int& t) {
	int parent = d;
	int s = MTid.s-1;

	edgeList e;
	vector<treeEdge> temp;
	do {
		e = metric.edges[s][parent];

		if (t_G.E[e.index].vertex[0] != e.dest) parent = t_G.E[e.index].vertex[0]-1;
		else parent = t_G.E[e.index].vertex[1]-1;
		if (!contain[e.index]) {
			temp.push_back({parent+1, e.dest});
			t_G.E[e.index].b -= t;
			MTid.ct += t_G.E[e.index].ce * t;
			contain[e.index] = true;
		}
	} while(parent != s);

	for (auto it = temp.end()-1; it >= temp.begin(); it--) {
		MTid.E.push_back(*it);
	}

	return;
}

bool Problem2::shortestPath(Graph &G, Tree &MTid, Metric& metric, const int& t) {

	// initilize
	metric.distance = new int*[size];
	metric.edges = new edgeList*[size];

	for (int i = 0; i < size; i++) {
		metric.distance[i] = new int[size];
		metric.edges[i] = new edgeList[size];
	}

	// find all shortestPath
	for (auto i : MTid.V) {
		dijkstra(MTid, metric, i, t);
	}
	// using the MST to construct path
	bool result =  MST(MTid, metric, t);

	// free memonry
	for (int i = 0; i < size; i++) {
		delete [] metric.distance[i];
		delete [] metric.edges[i];
	}

	delete [] metric.distance;
	delete [] metric.edges;

	return result;
}

void Problem2::printShortest(Graph& G, Tree &MTid, Metric& metric) {

	for (auto i : MTid.V) {
		i--;
		std::cout << "===From [" << i+1 << "] ===\n";
		for (auto j : G.V) {
			edgeList t = metric.edges[i][j];
			std::cout << i+1 << " "
					  << j+1 <<  " : "
					  << metric.distance[i][j] << "\n";
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
	s--;
	
	// store multicast tree cost
	bandwidth[s].push_back(make_pair(id, t));
	
	bool result = shortestPath(t_G, t_MTid, metric, t);
	t_F.trees.push_back(t_MTid);
	t_F.size++;
	printTree(id);
	printGraph(t_G);

	MTid = t_MTid;
	G = t_G;

	/* You should return true or false according the insertion result */
	return result;
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

void Problem2::printTree(int id) {
	// find the tree in MTidForest by id
	int index = 0;
	for (; index < t_F.size && t_F.trees[index].id != id; index++);
	if (index >= t_F.size) return;
	std::cout << "===   [" << id << "]    ===\n";

	// print the tree information
	Tree t = t_F.trees[index];
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

	std::cout << "total cost: " << t.ct << "\n";
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

void Problem2::printForest() {
	for (int i = 0; i < t_F.size; i++) {
		printTree(t_F.trees[i].id);
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