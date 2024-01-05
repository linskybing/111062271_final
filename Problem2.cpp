#include "basicDS.h"
#include "Problem2.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define bpair pair<int, int>
#define btuple tuple<int, int, int>
#define INF 10000001
class Problem2 {
	public:
		Problem2(Graph G);  //constructor
		~Problem2();        //destructor

		// main function
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);

		// sub function
		void dijkstra(Tree &MTid, Metric& metric , int s, const int& t); // metric closure
		bool steiner(Tree &MTid, Metric& metric, const int& t);
		void addPath(Tree &MTid, Metric& metric, bool* contain, const int& s, const int& d, const int& t, const int& cid); // from mst add path to MTid
		bool MST(Tree &MTid, Metric& metric, const int& t); // find MST from metric closure
		void release(Tree& MTid, const int& bandw);
		// get member variable
		int getBandwith(const int& s, const int& id);
		int getUsageIndex(const int s, const int& id);

		// output
		void printTree(int id);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(); // print all of the MTid
		void printBandwid();
		void printShortest(Tree &MTid, Metric& metric);

	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		vector<vector<bpair>> bandwidth;
		vector<vector<Contain>> usageEdge;
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
	usageEdge.reserve(size);
	t_G = G;
}

Problem2::~Problem2() {
	for (int i = 0; i < size; i++) {
		vector<edgeList>().swap(adjList[i]);
		vector<bpair>().swap(bandwidth[i]);
		vector<Contain>().swap(usageEdge[i]);
	}
	vector<vector<bpair>>().swap(bandwidth);
	vector<vector<Contain>>().swap(usageEdge);
	delete [] adjList;

}

int Problem2::getBandwith(const int& s,const int& id) {
	for (int i = 0; i < bandwidth[s].size(); i++) {
		if (bandwidth[s][i].first == id) return i;
	}
	return 0;
}

int Problem2::getUsageIndex(const int s, const int& id) {
	int i;
	for (i = 0; i < usageEdge[s].size() && usageEdge[s][i].id != id; i++);
	return i;
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
			if (u != v && dist_u + edge.cost < metric.distance[s][v] && t_G.E[edge.index].b >= t) {
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

	priority_queue<btuple, vector<btuple>, greater<btuple>> pq;

	// initilize
	for (int i = 0; i < size; i++) {
		contain[i] = false;
	}

	for (int i = 0; i < t_G.E.size(); i++) {
		contain_path[i] = false;
	}

	int s = MTid.s-1;
	int cid = getUsageIndex(s, MTid.id);

	contain[s] = true;
	int need = MTid.V.size() - 1;

	// prime's algorithm

	for (auto v : MTid.V) {
		v--;
		if (metric.distance[s][v] != INF && v != s)
			pq.push({metric.distance[s][v], v, s});
	}

	while (need && !pq.empty()) {
		int v = get<1>(pq.top());
		int v1 = get<2>(pq.top());
		pq.pop();

		if (contain[v]) continue;

		contain[v] = true;
		need--;

		addPath(MTid, metric, contain_path, v1, v, t, cid);

		for (auto u : MTid.V) {
			u--;
			if (!contain[u] && metric.distance[v][u] != INF) {
				pq.push({metric.distance[v][u], u, v});
			}
		}
	}

	// free memonry
	delete [] contain;
	delete [] contain_path;

	if (need){
		MTid.E.clear();
		MTid.ct = 0;
		for (auto it : usageEdge[s][cid].edges) {
			t_G.E[it].b += t;
		}
		usageEdge[s][cid].edges.clear();
		return false;
	};

	return true; 
}

void Problem2::addPath(Tree &MTid, Metric& metric, bool* contain, const int& s, const int& d, const int& t, const int& cid) {
	int parent = d;
	edgeList e;
	vector<treeEdge> temp;

	do {
		e = metric.edges[s][parent];

		// find the parent node
		if (t_G.E[e.index].vertex[0] != e.dest) parent = t_G.E[e.index].vertex[0]-1;
		else parent = t_G.E[e.index].vertex[1]-1;

		if (!contain[e.index]) {
			temp.push_back({parent+1, e.dest});

			// update graph
			t_G.E[e.index].b -= t;
			MTid.ct += t_G.E[e.index].ce * t;

			// record edge index
			usageEdge[MTid.s-1][cid].edges.push_back(e.index);

			contain[e.index] = true;
		}
	} while(parent != s);

	for (auto it = temp.end()-1; it >= temp.begin(); it--) {
		MTid.E.push_back(*it);
	}

	return;
}

bool Problem2::steiner(Tree &MTid, Metric& metric, const int& t) {

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

void Problem2::printShortest(Tree &MTid, Metric& metric) {

	for (auto i : MTid.V) {
		i--;
		std::cout << "===From [" << i+1 << "] ===\n";
		for (auto j : t_G.V) {
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

	Contain t_c;
	t_c.id = id;
	usageEdge[s].push_back(t_c);

	// build
	bool result = steiner(t_MTid, metric, t);
	t_F.trees.push_back(t_MTid);
	t_F.size++;

	MTid = t_MTid;
	G = t_G;

	/* You should return true or false according the insertion result */
	return result;
}

void Problem2::release(Tree& MTid, const int& bandw) {
	int index = getUsageIndex(MTid.s-1, MTid.id);
	for (int i = 0; i < usageEdge[MTid.s-1][index].edges.size(); i++) {
		// release bandwith
		t_G.E[usageEdge[MTid.s-1][index].edges[i]].b += bandw;
	}

	MTid.ct = 0;
	MTid.E.clear();
	usageEdge[MTid.s-1][index].edges.clear();
	return;
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	// find the multicast tree index
	int i = 0;
	for (; i < t_F.size && t_F.trees[i].id != id; i++);
	
	if (i == t_F.size) return;

	// release resource
	int s = t_F.trees[i].s-1;
	int index = getBandwith(s, id);
	release(t_F.trees[i], bandwidth[s][index].second);
	t_F.trees[i].V.clear();

	// remove tree
	bandwidth[s].erase(bandwidth[s].begin() + index); // clean the multicast bandwith
	usageEdge[s].erase(usageEdge[s].begin() + getUsageIndex(s, id));
	t_F.trees.erase(t_F.trees.begin() + i);
	t_F.size--;

	// find the most expensive cost that MTid has not yet been satisfied 
	priority_queue<bpair, vector<bpair>, less<bpair>> pq;

	for (int i = 0; i < t_F.size; i++) {
		if (t_F.trees[i].E.size() == 0) {
			int j = getBandwith(s, id);
			pq.push({bandwidth[t_F.trees[i].s-1][j].second , i});
		}
	}

	// allocate resource
	Metric metric;
	while(!pq.empty()) {
		int index = pq.top().second;
		int t = pq.top().first;
		pq.pop();
		steiner(t_F.trees[index], metric, t);
	}

	MTidForest = t_F;
	G = t_G;
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	// release all resouce
	int s, id, t, n;
	for (int i = 0; i < t_F.size; i++) {
		s = t_F.trees[i].s-1;
		id = t_F.trees[i].id;
		t = bandwidth[s][getBandwith(s, id)].second;
		release(t_F.trees[i], t);
	}

	// find the most expensive cost that MTid has not yet been satisfied 
	priority_queue<bpair, vector<bpair>, less<bpair>> pq;

	for (int i = 0; i < t_F.size; i++) {
		if (t_F.trees[i].E.size() == 0) {
			int j = getBandwith(s, id);
			pq.push({bandwidth[t_F.trees[i].s-1][j].second , i});
		}
	}

	// allocate resource
	Metric metric;
	while(!pq.empty()) {
		int index = pq.top().second;
		int t = pq.top().first;
		pq.pop();
		steiner(t_F.trees[index], metric, t);
	}

	G = t_G;
	MTidForest = t_F;
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